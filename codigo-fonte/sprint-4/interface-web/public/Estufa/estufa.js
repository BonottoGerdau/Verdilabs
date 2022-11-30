let dates = []
let humidity_dataset = [];
let temperature_dataset = [];
let chart_labels = []
$('#dropdown a').on('click', function () {
    chosenInterval = ($(this).attr('id'));
    humidity_dataset = []
    temperature_dataset = []
    chart_labels = []
    dates = []
    if (chosenInterval === "10m") {
        getReadings10Min();
    } else if (chosenInterval === "hour") {
        getReadings1H();
    } else if (chosenInterval === "day") {
        getReadingsDay();
    } else if (chosenInterval === "week") {
        getReadingsWeek();
    } else if (chosenInterval === "month") {
        getReadingsMonth();
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
    getReadings10Min();
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
                humidity_dataset = humidity_dataset
                temperature_dataset = temperature_dataset
                dates.forEach(label => {
                    let date = moment(label);
                    chart_labels.push(date.format('kk:mm:ss'));
                })
                myChart.data.labels = chart_labels;
                myChart.data.datasets[0].data = humidity_dataset;
                myChart.data.datasets[1].data = temperature_dataset;
                myChart.update()

                let tableRows = "";
                for (let i = 0; i < chart_labels.length; i++) {
                    tableRows += `
                    <tr>
                            <th scope="row">${dates[i].toLocaleDateString()}</th>
                            <td>${dates[i].toLocaleTimeString()}</td>
                            <td>${temperature_dataset[i]} ºC</td>
                            <td>${humidity_dataset[i]}%</td>
                        </tr>`
                }
                $("tbody").html(tableRows);
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
                data.forEach(reading => {
                    dates.push(new Date(reading['datetime']))
                    humidity_dataset.push(reading['humidity'])
                    temperature_dataset.push(reading['temperature'])
                })
                chart_labels = []
                dates = getEveryNth(dates, 5);
                humidity_dataset = getEveryNth(humidity_dataset, 5)
                temperature_dataset = getEveryNth(temperature_dataset, 5)
                dates.forEach(label => {
                    let date = moment(label);
                    chart_labels.push(date.format('kk:mm:ss'));
                })
                myChart.data.labels = chart_labels;
                myChart.data.datasets[0].data = humidity_dataset;
                myChart.data.datasets[1].data = temperature_dataset;
                myChart.update()

                let tableRows = "";
                for (let i = 0; i < chart_labels.length; i++) {
                    tableRows += `
                    <tr>
                            <th scope="row">${dates[i].toLocaleDateString()}</th>
                            <td>${dates[i].toLocaleTimeString()}</td>
                            <td>${temperature_dataset[i]}</td>
                            <td>${humidity_dataset[i]}</td>
                        </tr>`
                }
                $("tbody").html(tableRows);
            }));
}

function getReadingsDay() {
    console.log("DIA")
    let start = moment().subtract(1, "day")
    let end = moment()
    fetch("http://localhost:1234/filtered_readings?" + new URLSearchParams({
        datetime_start: start.format("YYYY-MM-DD kk:mm:ss"),
        datetime_end: end.format("YYYY-MM-D kk:mm:ss"),
        greenhouse: 1
    }))
        .then(response => response.json()
            .then(data => {
                data.forEach(reading => {
                    dates.push(new Date(reading['datetime']))
                    humidity_dataset.push(reading['humidity'])
                    temperature_dataset.push(reading['temperature'])
                })
                dates = getEveryNth(dates, 60);
                chart_labels = []
                humidity_dataset = getEveryNth(humidity_dataset, 60)
                temperature_dataset = getEveryNth(temperature_dataset, 60)
                dates.forEach(label => {
                    let date = moment(label);
                    chart_labels.push(date.format('kk[h]'));
                })
                myChart.data.labels = chart_labels;
                myChart.data.datasets[0].data = humidity_dataset;
                myChart.data.datasets[1].data = temperature_dataset;
                myChart.update()
                let tableRows = ""
                for (let i = 0; i < chart_labels.length; i++) {
                    tableRows += `
                    <tr>
                            <th scope="row">${dates[i].toLocaleDateString()}</th>
                            <td>${dates[i].toLocaleTimeString()}</td>
                            <td>${temperature_dataset[i]}</td>
                            <td>${humidity_dataset[i]}</td>
                        </tr>`
                }
                $("tbody").html(tableRows);
            }));
}

function getReadingsWeek() {
    let start = moment().subtract(1, "week")
    let end = moment()
    fetch("http://localhost:1234/filtered_readings?" + new URLSearchParams({
        datetime_start: start.format("YYYY-MM-DD kk:mm:ss"),
        datetime_end: end.format("YYYY-MM-D kk:mm:ss"),
        greenhouse: 1
    }))
        .then(response => response.json()
            .then(data => {
                data.forEach(reading => {
                    dates.push(new Date(reading['datetime']))
                    humidity_dataset.push(reading['humidity'])
                    temperature_dataset.push(reading['temperature'])
                })
                dates = getEveryNth(dates, 5);
                chart_labels = []
                humidity_dataset = getEveryNth(humidity_dataset, 720)
                temperature_dataset = getEveryNth(temperature_dataset, 720)
                dates.forEach(label => {
                    let date = moment(label);
                    chart_labels.push(date.format('DD/MM kk[h]'));
                })
                myChart.data.labels = chart_labels;
                myChart.data.datasets[0].data = humidity_dataset;
                myChart.data.datasets[1].data = temperature_dataset;
                myChart.update()
            }));
}

function getReadingsMonth() {
    let start = moment().subtract(1, "month")
    let end = moment()
    console.log({ start, end })
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
                dates = getEveryNth(dates, 5);
                chart_labels = []
                humidity_dataset = getEveryNth(humidity_dataset, 4320);
                temperature_dataset = getEveryNth(temperature_dataset, 4320)
                dates.forEach(label => {
                    let date = moment(label);
                    chart_labels.push(date.format('DD/MM kk[h]'));
                })
                myChart.data.labels = chart_labels;
                myChart.data.datasets[0].data = humidity_dataset;
                myChart.data.datasets[1].data = temperature_dataset;
                myChart.update()
            }));
}