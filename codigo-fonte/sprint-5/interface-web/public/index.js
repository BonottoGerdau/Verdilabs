/* Este script coordena a atualização assíncrona da página principal da interface gráfica
de Greener de acordo com os dados recebidos do ESP e sua adequação às regras do negócio.

Este script prioriza a pior medida, ou seja, caso apenas um dos indicadores esteja fora
do intervalo permitido, a interface ficará vermelha e sinalizará urgência mesmo assim, 
independentemente de o outro indicador estar normal.
*/

// Quando a página carrega...
$(document).ready(async function () {

    let tempMin, tempMax, humidityMin, humidityMax; // Variáveis para guardar parâmetros de tolerância

    // Faz quatro fetches para pegar cada um dos parâmetros e salvá-los nas variáveis correspondentes.
    // O await é utilizado para que o código não continue até esses valores serem obtidos, evitando
    // erros de "undefined"

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

    // Variáveis que guardam estado do sistema, isto é, 
    // se é necessário mostrar mensagens de alerta e/ou urgência
    let cautionNeeded = false;
    let urgencyNeeded = false;

    // Função que atualiza data e hora no header e nos cards (no momento, apenas estufa 1)
    $(function () {
        // Bloco de código é executado a cada minuto
        setInterval(function () {
            moment.locale('pt-br'); // Localização para data e hora é setada para Brasil

            $('#time').html(moment().format('LT')); // Hora no header é exibida da forma brasileira
            $('#estufa1-time').html(moment().format('LT')); // Hora do card é exibida da forma brasileira
            $('#estufa2-time').html(moment().format('LT')); // Hora do card é exibida da forma brasileira
            $('#estufa3-time').html(moment().format('LT')); // Hora do card é exibida da forma brasileira
            $('#estufa4-time').html(moment().format('LT')); // Hora do card é exibida da forma brasileira
            $('#date').html(moment().format('LL')); // Data no header é exibida da forma brasileira
        }, 1000);
    })

    // Faz uma requisição GET para os últimos erros identificados
    const getReadings = function () {
        fetch("https://greener-g6it.onrender.com/error")
            .then(response => response.json() // Transforma payload em json
                .then(data => {
                    if (data.error != "0") { // Se tiver erro...
                        document.getElementById('status-image').src = "assets/Xcinza.png"; // Troca ícone por ícone de erro
                        document.getElementById("status-text").innerHTML = ("Erro de " + data.error); // Troca status pelo nome do erro
                    } else { // Se não tiver, faz o fetch normalmente para pegar as últimas medições
                        fetch("https://greener-g6it.onrender.com/last_readings")
                            .then(response => response.json() // Transforma payload em json
                                .then(data => updateCards(data))) // Passa json para a função de atualizar cards de estufas
                    }
                }))
    }

    // Checa medida de temperatura e exibe legenda correspondente, se necessário, 
    // e seta a flag de urgência e/ou alerta segundo regras do negócio
    function checkTemperature(temp, greenhouse) {
        if (temp < tempMin * 0.95) { // checa condições
            urgencyNeeded = true; // seta urgência como true
            $(`#estufa${greenhouse}-card`).css("box-shadow", "0 4px 4px 0 rgba(203, 19, 19, 0.5), 0 10px 20px 0 rgba(203, 19, 19, 0.4)") // dá contorno vermelho ao card da estufa
        } else if (temp < tempMin) { // A lógica acima é aplicada para as outras faixas de medida, mudando o contorno e a legenda de acordo com a gravidade da situação
            cautionNeeded = true;
            console.log(temp)
            $(`#estufa${greenhouse}-card`).css("box-shadow", "0 4px 4px 0 rgba(251, 203, 6, 0.5), 0 10px 20px 0 rgba(251, 203, 6, 0.4)")
        } else if (temp > tempMax * 1.1) {
            urgencyNeeded = true;
            $(`#estufa${greenhouse}-card`).css("box-shadow", "0 4px 4px 0 rgba(203, 19, 19, 0.5), 0 10px 20px 0 rgba(203, 19, 19, 0.4)")
        } else if (temp > tempMax * 1.05) {
            console.log(temp)
            urgencyNeeded = true;
            $(`#estufa${greenhouse}-card`).css("box-shadow", "0 4px 4px 0 rgba(203, 19, 19, 0.5), 0 10px 20px 0 rgba(203, 19, 19, 0.4)")
        } else if (temp > tempMax * 1.03) {
            urgencyNeeded = true;
            $(`#estufa${greenhouse}-card`).css("box-shadow", "0 4px 4px 0 rgba(203, 19, 19, 0.5), 0 10px 20px 0 rgba(203, 19, 19, 0.4)")
        } else if (temp > tempMax) {
            $(`#estufa${greenhouse}-card`).css("box-shadow", "0 4px 4px 0 rgba(251, 203, 6, 0.5), 0 10px 20px 0 rgba(251, 203, 6, 0.4)")
            cautionNeeded = true;
        }
    }

    // Checa medida de de umidade e altera legenda e flags de urgência/alerta conforme necessário, seguindo a lógica anterior
    function checkHumidity(humidity, greenhouse) {
        if (humidity < humidityMin * 0.95) {
            urgencyNeeded = true;
            $(`#estufa${greenhouse}-card`).css("box-shadow", "0 4px 4px 0 rgba(203, 19, 19, 0.5), 0 10px 20px 0 rgba(203, 19, 19, 0.4)")
        } else if (humidity < humidityMin) {
            $(`#estufa${greenhouse}-card`).css("box-shadow", "0 4px 4px 0 rgba(251, 203, 6, 0.5), 0 10px 20px 0 rgba(251, 203, 6, 0.4)")
            cautionNeeded = true;
        } else if (humidity > humidityMax) {
            $(`#estufa${greenhouse}-card`).css("box-shadow", "0 4px 4px 0 rgba(203, 19, 19, 0.5), 0 10px 20px 0 rgba(203, 19, 19, 0.4)")
            urgencyNeeded = true;
        } else {
            $(`#estufa${greenhouse}-card`).css("box-shadow", "")

        }
    }

    // Atualiza os cards e o status conforme medidas recebidas
    function updateCards(readings) {
        // Reseta flags
        urgencyNeeded = false
        cautionNeeded = false
        // Exibe medidas no card correspodente
        for (let i = 0; i < 4; i++) {
            $('#temp-estufa' + (i + 1)).text(readings[i].temperature + " ºC")
            $('#hum-estufa' + (i + 1)).text(readings[i].humidity + "%")
            checkHumidity(Number.parseFloat(readings[i].humidity), i + 1)
            checkTemperature(Number.parseFloat(readings[i].temperature), i + 1)
        }
        // Se alguma medida estiver na zona de tolerância, troca ícone pela exclamação amarela, 
        // exibe "Atenção" no status e adicionar sombra amarela atrás do card da estufa
        if (cautionNeeded) {
            $("#status-image").attr("src", "assets/yellow.png");
            $('#status-text').text("Atenção!")
            // Se alguma medida estiver fora do intervalo permitido, troca ícone pelo x vermelho, 
            // exibe "Perigo" no status e adicionar sombra vermelha atrás do card da estufa
        } if (urgencyNeeded) {
            $("#status-image").attr("src", "assets/red.png");
            $('#status-text').text("Perigo!")
            // Se estiver tudo dentro de desejado, troca o ícone pelo OK verde, exibe "Tudo Certo" e remove
            // sombras coloridas do card da estufa
        } else if (!urgencyNeeded && !cautionNeeded) {
            $("#status-image").attr("src", "assets/green.png");
            $('#status-text').text("Tudo certo!")
        }
    }

    // Recebe leituras assim que a página carrega
    getReadings()
    // Atualiza leituras a cada 10 segundos
    setInterval(getReadings, 10000)
});
