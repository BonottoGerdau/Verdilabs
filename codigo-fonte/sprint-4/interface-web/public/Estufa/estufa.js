
let dates = []
let humidity_dataset = [];
let chart_labels = []
let chosenInterval = "1min"
$('#dropdown a').on('click', function () {
    chosenInterval = ($(this).attr('id'));
    console.log(chosenInterval)
    getReadings1Min();
});

const data = {
    labels: chart_labels,
    datasets: [{
        label: 'Umidade',
        data: humidity_dataset,
        fill: false,
        borderColor: 'rgb(75, 192, 192)',
        tension: 0.1
    }]
};

const config = {
    type: 'line',
    data: data,
    options: {
        plugins: {
            legend: {
                display: false
            }
        }
    }
};
let myChart = new Chart(
    document.getElementById('chart'),
    config
);
function onload() {
    getReadings1Min()
}

function getEveryNth(arr, nth) {
    const result = [];

    for (let i = 0; i < arr.length; i += nth) {
        result.push(arr[i]);
    }

    return result;
}

function getReadings1Min() {
    //let start = moment().seconds(0).minutes(0)
    //let end = start.add(1, "hours")
    let start = moment("2022-11-30 13:00:00")
    console.log('HERE')
    fetch("http://localhost:1234/filtered_readings?" + new URLSearchParams({
        datetime_start: start.format("YYYY-MM-DD kk:mm:ss"),
        datetime_end: start.add(1, "hours").format("YYYY-MM-D kk:mm:ss"),
        greenhouse: 1
    }))
        .then(response => response.json()
            .then(data => {
                console.log(data)
                data.forEach(reading => {
                    dates.push(new Date(reading['datetime']))
                    humidity_dataset.push(reading['humidity'])
                })
                chart_labels = []
                if (chosenInterval === "1min") {
                    myChart.data.data = humidity_dataset.slice(0, 59);
                    dates.forEach(label => {
                        let date = moment(label);
                        chart_labels.push(date.format('kk:mm:ss'));
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

