
const getReadings = function () {
    fetch("http://10.128.64.110:1234/last_readings")
        .then(response => response.json()
            .then(data => updateCards(data)))
}

function updateCards(readings) {
    console.log(readings)
    for (let i = 0; i < 4; i++) {
        console.log(readings[i])
        $('#temp-estufa' + (i + 1)).text(readings[i].temperature + " ºC")
        $('#hum-estufa' + (i + 1)).text(readings[i].humidity + "%")
    }
}

$(function() {
    $('.greenhouseLink').on('click', function(event) {
      alert(this.id);
    })
  });

getReadings()
setInterval(getReadings, 10000)