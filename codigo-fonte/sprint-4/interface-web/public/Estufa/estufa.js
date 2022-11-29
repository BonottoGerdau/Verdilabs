chart_labels = []
humidity_dataset = [];

const intervals = {
    "1_minute": 1,
    "5_minutes": 5,
}

let chosenInterval = "1_minute"


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

myChart = new Chart(
    document.getElementById('chart'),
    config
);

function getEveryNth(arr, nth) {
    const result = [];
  
    for (let i = 0; i < arr.length; i += nth) {
      result.push(arr[i]);
    }
  
    return result;
  }

function getReadings() {
    fetch("http://localhost:1234/greenhouse_readings?" + new URLSearchParams({
        greenhouse: 1
    }))
        .then(response => response.json()
            .then(data => {
                data.forEach(reading => {
                    chart_labels.push(reading['datetime'])
                    humidity_dataset.push(reading['humidity'])
                })
                myChart.data.labels = chart_labels
                myChart.data.data = humidity_dataset
                myChart.data.labels = getEveryNth(chart_labels, 5);
                myChart.data.data = getEveryNth(humidity_dataset, 5).slice(0, 49);
                console.log(getEveryNth(chart_labels, 5))
                myChart.update()
                console.log('ere')
            }));

}

