$('#salvar').on('click', function() {
    let json = {
        tempMin: Number.parseFloat($("#tempMin").val()),
        tempMax: Number.parseFloat($("#tempMax").val()),
        humidityMin: Number.parseFloat($("#humidityMin").val()),
        humidityMax: Number.parseFloat($("#humidityMax").val()),
    }
    console.log(json)
    fetch("https://greener-g6it.onrender.com/insert_parameters", {
        method: "POST",
        body: JSON.stringify(json),
        headers: {"Content-type": "application/json; charset=UTF-8"}
      })
  });