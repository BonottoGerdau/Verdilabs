let dates = []
let humidity_dataset = [];
let temperature_dataset = [];
let chart_labels = []
$('#dropdown a').on('click', function () {
    chosenInterval = ($(this).attr('id'));
    if (chosenInterval === "10m") {
        getReadings10Min();
    } else if (chosenInterval === "hour") {
        getReadings1H();
    } else if (chosenInterval === "day") {
        getReadingsDay();
    }
});


const data = {
    labels: chart_labels,
    datasets: [
        {
            label: 'Umidade',
            data: humidity_dataset,
            fill: false,
            borderColor: 'rgb(75, 192, 192)',
            color: 'rgb(75, 192, 192)',
            backgroundColor: 'rgb(75, 192, 192)',
            yAxisID: 'y',
        },
        {
            label: 'Temperatura',
            data: temperature_dataset,
            fill: false,
            borderColor: 'rgb(245, 152, 66)',
            backgroundColor: 'rgb(245, 152, 66)',
            yAxisID: 'y1',
        }
    ]
}

const config = {
    type: 'line',
    data: data,
    options: {
        responsive: true,
        interaction: {
            mode: 'index',
            intersect: false,
        },
        stacked: false,
        plugins: {
            title: {
                display: false,
            }
        },
        scales: {
            y: {
                type: 'linear',
                display: true,
                position: 'left',
            },
            y1: {
                type: 'linear',
                display: true,
                position: 'right',

                // grid line settings
                grid: {
                    drawOnChartArea: false, // only want the grid lines for one axis to show up
                },
            },
        },

    },
};
let myChart = new Chart(
    document.getElementById('chart'),
    config
);

function onload() {
}

function getEveryNth(arr, nth) {
    const result = [];

    for (let i = 0; i < arr.length; i += nth) {
        result.push(arr[i]);
    }

    return result;
}

function getReadings10Min() {
    let start = moment().subtract(10, "minutes")
    let end = moment()
    fetch("http://localhost:1234/filtered_readings?" + new URLSearchParams({
        datetime_start: start.format("YYYY-MM-DD kk:mm:ss"),
        datetime_end: end.format("YYYY-MM-D kk:mm:ss"),
        greenhouse: 1
    }))
        .then(response => response.json()
            .then(data => {
                console.log(data)
                data.forEach(reading => {
                    dates.push(new Date(reading['datetime']))
                    humidity_dataset.push(reading['humidity'])
                    temperature_dataset.push(reading['temperature'])
                })
                chart_labels = []
                humidity_dataset = humidity_dataset.slice(0, 59);
                temperature_dataset = temperature_dataset.slice(0, 59)
                dates.forEach(label => {
                    let date = moment(label);
                    chart_labels.push(date.format('kk:mm:ss'));
                })
                chart_labels = chart_labels.slice(0, 59);
                myChart.data.labels = chart_labels;
                myChart.data.datasets[0].data = humidity_dataset;
                myChart.data.datasets[1].data = temperature_dataset;


                myChart.update()
            }));
}

function getReadings1H() {
    let start = moment().subtract(1, "hours")
    let end = moment()
    fetch("http://localhost:1234/filtered_readings?" + new URLSearchParams({
        datetime_start: start.format("YYYY-MM-DD kk:mm:ss"),
        datetime_end: end.format("YYYY-MM-D kk:mm:ss"),
        greenhouse: 1
    }))
        .then(response => response.json()
            .then(data => {
                console.log(data)
                data.forEach(reading => {
                    dates.push(new Date(reading['datetime']))
                    humidity_dataset.push(reading['humidity'])
                    temperature_dataset.push(reading['temperature'])
                })
                chart_labels = []
                humidity_dataset = getEveryNth(humidity_dataset, 5).slice(0, 59);
                temperature_dataset = getEveryNth(temperature_dataset, 5).slice(0, 59)
                dates.forEach(label => {
                    let date = moment(label);
                    chart_labels.push(date.format('kk:mm:ss'));
                })
                console.log(chart_labels)
                chart_labels = getEveryNth(chart_labels, 5).slice(0, 59);
                console.log(chart_labels)
                myChart.data.labels = chart_labels;
                myChart.data.datasets[0].data = humidity_dataset;
                myChart.data.datasets[1].data = temperature_dataset;
                myChart.update()
            }));
}

function getReadings() {
    fetch("http://localhost:1234/greenhouse_readings?" + new URLSearchParams({
        greenhouse: 1
    }))
        .then(response => response.json()
            .then(data => {
                data.forEach(reading => {
                    dates.push(new Date(reading['datetime']))
                    humidity_dataset.push(reading['humidity'])
                })
                chart_labels = []
                if (chosenInterval === "1min") {
                    myChart.data.data = humidity_dataset.slice(0, 59);
                    dates.forEach(label => {
                        let date = new Date(label);
                        chart_labels.push(`${date.getHours()}:${date.getMinutes()}:${date.getSeconds()}`);
                    })
                    myChart.data.labels = chart_labels.slice(0, 59);
                }
                if (chosenInterval === "1min") {
                    myChart.data.data = humidity_dataset.slice(0, 59);
                    dates.forEach(label => {
                        let date = new Date(label);
                        chart_labels.push(`${date.getHours()}:${date.getMinutes()}:${date.getSeconds()}`);
                    })
                    myChart.data.labels = chart_labels.slice(0, 59);
                }
                /* myChart.data.labels = getEveryNth(chart_labels, 5);
                myChart.data.data = getEveryNth(humidity_dataset, 5).slice(0, 49); */
                myChart.update()
            }));

}

