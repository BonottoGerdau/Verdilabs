/* JS para tela de estufa específica */


$(document).ready(async function () {
    let tempMin, tempMax, humidityMin, humidityMax;
    await fetch("https://greener-g6it.onrender.com/tempMin").then(response => response.json()
        .then(data => {
            tempMin = Number.parseInt(data);
        }))

    await fetch("https://greener-g6it.onrender.com/tempMax").then(response => response.json()
        .then(data => {
            tempMax = Number.parseInt(data);
        }))

    await fetch("https://greener-g6it.onrender.com/humidityMin").then(response => response.json()
        .then(data => {
            humidityMin = Number.parseInt(data);
        }))

    await fetch("https://greener-g6it.onrender.com/humidityMax").then(response => response.json()
        .then(data => {
            humidityMax = Number.parseInt(data);
        }))



    const getReadings = function () {
        fetch("https://greener-g6it.onrender.com/error")
            .then(response => response.json() // Transforma payload em json
                .then(data => {
                    if (data.error != "0") {
                        console.log(data.error)
                        //document.getElementById('status-image').src = "assets/Xcinza.png";
                        //document.getElementById("status-text").innerHTML = ("Erro de " + data.error);
                    } else {
                        fetch("https://greener-g6it.onrender.com/last_readings")
                            .then(response => response.json() // Transforma payload em json
                                .then(data => giveFeedback(data[0].temperature))) // Passa json para a função de atualizar cards de estufas
                    }
                })) // Passa json para a função de atualizar cards de estufas
    }

    function giveFeedback(temp) {
        if (temp < tempMin * 0.95) { // checa condições
            $(`#status-text`).css({"color": "rgb(203, 19, 19)", "font-size": "calc(1.3rem + .6vw)"}) // dá contorno vermelho ao card da estufa
            $(`#status-text`).text("Fechar janelas") // dá contorno vermelho ao card da estufa
        } else if (temp < tempMin) { // A lógica acima é aplicada para as outras faixas de medida, mudando o contorno e a legenda de acordo com a gravidade da situação
            $(`#status-text`).css({"color": "rgb(215, 180, 6)", "font-size": "calc(1.3rem + .6vw)"}) // dá contorno vermelho ao card da estufa
            $(`#status-text`).text("Atenção... Esfriando!") // dá contorno vermelho ao card da estufa
        } else if (temp > tempMax * 1.1) {
            $(`#status-text`).text("Abra todas\nas janelas") // dá contorno vermelho ao card da estufa
            $(`#status-text`).css("color", "rgba(203, 19, 19, 0.5)") // dá contorno vermelho ao card da estufa
        } else if (temp > tempMax * 1.05) {
            $(`#status-text`).text("Abra 100%\ndas laterais") // dá contorno vermelho ao card da estufa
            $(`#status-text`).css("color", "rgba(203, 19, 19, 0.5)") // dá contorno vermelho ao card da estufa
        } else if (temp > tempMax * 1.03) {
            $(`#status-text`).text("Abra 50%\ndas laterais") // dá contorno vermelho ao card da estufa
            $(`#status-text`).css("color", "rgba(203, 19, 19, 0.5)") // dá contorno vermelho ao card da estufa
        } else if (temp > tempMax) {
            $(`#status-text`).text("Abra 25%\ndas laterais") // dá contorno vermelho ao card da estufa
            $(`#status-text`).css("color", "rgba(203, 19, 19, 0.5)") // dá contorno vermelho ao card da estufa
        } else {
            $(`#status-text`).text("Tudo certo!") // dá contorno vermelho ao card da estufa
            $(`#status-text`).css("color", "rgb(86,144,103)") // dá contorno vermelho ao card da estufa
        }
    }

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
                    console.log(data)
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
                            <td>${dates[i].toLocaleDateString()}</th>
                            <td>${dates[i].toLocaleTimeString()}</td>
                            <td>${temperature_dataset[i]}</td>
                            <td>${humidity_dataset[i]}</td>
                        </tr>`
                    }
                    $("tbody").html(tableRows);
                }));
    }

    $(document).ready(function () {
        $('table').each(function () {
            var $table = $(this);
    
            var $button = $("<button type='button'>");
            $button.text("Baixar dados");
            $button.insertAfter($table);
    
            $button.click(function () {
                var csv = $table.table2CSV({
                    delivery: 'value'
                });
                window.location.href = 'data:text/csv;charset=UTF-8,' 
                + encodeURIComponent(csv);
            });
        });
    })
    // Recebe leituras assim que a página carrega
    getReadings()
    // Atualiza leituras a cada 10 segundos
    setInterval(getReadings, 10000)

    
});