// Variáveis para guardar dados recebidos nos endpoints
let dates = [] // Guarda datas
let humidity_dataset = []; // Guarda valores de umidade
let temperature_dataset = []; // Guarda valores de temperatura
let chart_labels = [] // Guarda strings de eixo x para o gráfico

// Esta função roda toda vez que a seleção do dropdown muda
$('select').on('click', function () {
    chosenInterval = ($(this).val()); // Valor escolhido no dropdown
    // Esvazia arrays
    humidity_dataset = []
    temperature_dataset = []
    chart_labels = []
    dates = []
    // Chama função correspondente ao intervalo escolhido
    if (chosenInterval === "minute") {
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

// Define dados do gráfico
const data = {
    labels: chart_labels, // Eixo X
    datasets: [ // Define um dataset para temperatura e um para umidade
        {
            label: 'Umidade', // Título
            data: humidity_dataset, // Dados (array)
            fill: false,
            borderColor: 'rgb(75, 192, 192)',
            color: 'rgb(75, 192, 192)', // Cor da linha
            backgroundColor: 'rgb(75, 192, 192)',
            yAxisID: 'y', // Define um eixo para cada dataset, um de cada lado
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

// Configurações do gráfico
const config = {
    type: 'line',
    data: data, // Traz json de dados para o gráfico
    options: {
        responsive: true,
        interaction: {
            mode: 'index',
            intersect: false,
        },
        stacked: false,
        plugins: {
            title: { // Não mostra título
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
                grid: {
                    drawOnChartArea: false,
                },
            },
        },
    },
};

// Cria objeto de gráfico e posiciona no div desejado
let myChart = new Chart(
    document.getElementById('chart'),
    config
);

// Esta função é executada toda vez que a página é carregada
function onload() {
    getReadingsWeek();
}

// Devolve array dos enésimos elementos de um array
function getEveryNth(arr, nth) {
    const result = [];

    for (let i = 0; i < arr.length; i += nth) {

        result.push(arr[i]);
    }
    result.push(arr[arr.length - 1])
    return result;
}

// Função para intervalos de 10 minutos
function getReadings10Min() {
    let start = moment().subtract(10, "minutes") // Começa dez minutos antes do momento atual
    let end = moment() // Termina no momento atual
    // GET com filtro por data de início, data de fim e estufa
    fetch("https://greener-g6it.onrender.com/filtered_readings?" + new URLSearchParams({
        datetime_start: start.format(),
        datetime_end: end.format(),
        greenhouse: 1
    }))
        .then(response => response.json()
            .then(data => {
                // Desmembra body em arrays para datas, umidade e temperatura
                data.forEach(reading => {
                    dates.push(moment(reading['datetime']))
                    humidity_dataset.push(reading['humidity'])
                    temperature_dataset.push(reading['temperature'])
                })
                // Esvazia legendas
                chart_labels = []
                // Transforma datas em strings
                dates.forEach(label => {
                    let date = moment(label);
                    chart_labels.push(date.format('kk:mm:ss'));
                })
                // Atualiza valores no objeto de gráfico
                myChart.data.labels = chart_labels;
                myChart.data.datasets[0].data = humidity_dataset;
                myChart.data.datasets[1].data = temperature_dataset;
                myChart.update() // Executa atualizações

                // Cria string de html a ser adicionada
                let tableRows = "";
                // Cria linhas da tabela com valores atualizados
                for (let i = chart_labels.length - 1; i >= 0; i--) {
                    tableRows += `
                    <tr>
                            <th scope="row">${dates[i].format("DD/MM/YY")}</th>
                            <td>${dates[i].format("hh[h]mm")}</td>
                            <td>${temperature_dataset[i]} ºC</td>
                            <td>${humidity_dataset[i]}%</td>
                        </tr>`
                }
                $("tbody").html(tableRows); // Adiciona string ao html de fato
            }));
}

// Função para intervalos de 1 hora
function getReadings1H() {
    // Segue a mesma lógica da função anterior
    let start = moment().subtract(1, "hours")
    let end = moment()
    fetch("https://greener-g6it.onrender.com/filtered_readings?" + new URLSearchParams({
        datetime_start: start.format(),
        datetime_end: end.format(),
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
                // Pega dados de 5 em 5 minutos
                dates = getEveryNth(dates, 5);
                humidity_dataset = getEveryNth(humidity_dataset, 5)
                console.log(humidity_dataset)
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
                for (let i = chart_labels.length - 1; i >= 0; i--) {
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

// Função para intervalos de 1 hora
function getReadingsDay() {
    // Segue a mesma lógica da função anterior
    let start = moment().subtract(1, "day")
    let end = moment()
    fetch("https://greener-g6it.onrender.com/filtered_readings?" + new URLSearchParams({
        datetime_start: start.format(),
        datetime_end: end.format(),
        greenhouse: 1
    }))
        .then(response => response.json()
            .then(data => {
                data.forEach(reading => {
                    dates.push(new Date(reading['datetime']))
                    humidity_dataset.push(reading['humidity'])
                    temperature_dataset.push(reading['temperature'])
                })
                // Pega dados de 1 em 1 hora
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
                for (let i = chart_labels.length - 1; i >= 0; i--) {
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

// Função para intervalos de 1 semana
function getReadingsWeek() {
    let start = moment().subtract(1, "week")
    let end = moment()
    fetch("https://greener-g6it.onrender.com/filtered_readings?" + new URLSearchParams({
        datetime_start: start.format(),
        datetime_end: end.format(),
        greenhouse: 1
    }))
        .then(response => response.json()
            .then(data => {
                data.forEach(reading => {
                    dates.push(new Date(reading['datetime']))
                    humidity_dataset.push(reading['humidity'])
                    temperature_dataset.push(reading['temperature'])
                })
                // Pega dados de 1 em 1 dia
                dates = getEveryNth(dates, 1440);
                chart_labels = []
                humidity_dataset = getEveryNth(humidity_dataset, 1440)
                temperature_dataset = getEveryNth(temperature_dataset, 1440)
                dates.forEach(label => {
                    let date = moment(label);
                    chart_labels.push(date.format('DD/MM kk[h]'));
                })
                console.log(chart_labels)
                myChart.data.labels = chart_labels;
                myChart.data.datasets[0].data = humidity_dataset;
                myChart.data.datasets[1].data = temperature_dataset;
                myChart.update()
                let tableRows = ""
                for (let i = chart_labels.length - 1; i >= 0; i--) {
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

// Função para intervalos de 1 mês
function getReadingsMonth() {
    let start = moment().subtract(1, "month")
    let end = moment()
    console.log({ start, end })
    fetch("https://greener-g6it.onrender.com/filtered_readings?" + new URLSearchParams({
        datetime_start: start.format(),
        datetime_end: end.format(),
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
                dates = getEveryNth(dates, 1440);
                chart_labels = []
                // Pega dados de dia em dia
                humidity_dataset = getEveryNth(humidity_dataset, 1440);
                temperature_dataset = getEveryNth(temperature_dataset, 1440)
                dates.forEach(label => {
                    let date = moment(label);
                    chart_labels.push(date.format('DD/MM - kk[h]'));
                })
                myChart.data.labels = chart_labels;
                myChart.data.datasets[0].data = humidity_dataset;
                myChart.data.datasets[1].data = temperature_dataset;
                myChart.update()
                let tableRows = ""
                for (let i = chart_labels.length - 1; i >= 0; i--) {
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