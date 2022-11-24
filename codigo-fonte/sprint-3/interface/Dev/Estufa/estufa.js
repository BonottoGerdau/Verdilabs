/* 
const data = {
    labels: [],
    datasets: [{
        backgroundColor: 'rgb(255, 99, 132)',
        borderColor: 'rgb(255, 99, 132)',
        data: [],
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
); */

function getReadings() {
    fetch("http://10.254.18.116:1234/all_readings")
        .then(response => response.json()
            .then(data => {
                console.log(data);
            }));

}

function show_graph(data) {
    console.log(getReadings())
    /* myChart.data.labels = data['time_array']
    myChart.data.datasets[0].data = data['all_rats']
    myChart.update() */
}

show_graph();