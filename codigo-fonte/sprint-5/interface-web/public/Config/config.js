
// Esta função faz uma requisição para salvar os novos parâmetros no servidor
// quando o botão de salvar é pressionado
$('#salvar').on('click', function () {
    // Cria JSON com base nos inputs
    let json = {
        tempMin: Number.parseFloat($("#tempMin").val()),
        tempMax: Number.parseFloat($("#tempMax").val()),
        humidityMin: Number.parseFloat($("#humidityMin").val()),
        humidityMax: Number.parseFloat($("#humidityMax").val()),
    }
    // Faz o POST com o json
    fetch("https://greener-g6it.onrender.com/insert_parameters", {
        method: "POST",
        body: JSON.stringify(json),
        headers: { "Content-type": "application/json; charset=UTF-8" }
    })
});