// CÓDIGO DO SERVIDOR

// importa bibliotecas necessárias
const express = require('express');
const sqlite3 = require('sqlite3').verbose();

// cria servidor no endereço local e determina que a pasta frontend deve ser usada como source
const app = express();
const hostname = '10.128.64.110';
const port = 1234;
const bodyParser = require('body-parser');
const urlencodedParser = bodyParser.urlencoded({ extended: false });

app.use(express.static("../public/"));

app.use(express.json());
const cors = require("cors");
const corsOptions = {
    origin: '*',
    credentials: true,            //access-control-allow-credentials:true
    optionSuccessStatus: 200
}
app.use(cors(corsOptions)) //

// caminho do banco de dados
const DBPATH = 'db.db'
let currentReadings = [0, 0, 0, 0]


/* DEFINIÇÃO DOS ENDPOINTS */

app.get('/', function(request, response){
    response.sendFile('public/index.html', { root: '.' });
});

// NETWORKS - checar registros cadastros na tabela NETWORK
app.get('/all_readings', (req, res) => {
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');
    const db = new sqlite3.Database(DBPATH);
    var sql = 'SELECT * FROM sensor ORDER BY datetime'; // ordena por name
    db.all(sql, [], (err, rows) => {
        if (err) {
            throw err;
        }
        res.json(rows);
    });
    db.close();
});

app.get('/filtered_readings', (req, res) => {
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');
    const db = new sqlite3.Database(DBPATH);
    const { datetime_start, datetime_end, greenhouse } = req.query;
    var sql = 'SELECT * FROM sensor WHERE (datetime BETWEEN "' + datetime_start + '" AND "' +
        datetime_end + '") AND greenhouse == ' + Number.parseInt(greenhouse); // ordena por name
    db.all(sql, [], (err, rows) => {
        if (err) {
            throw err;
        }
        res.json(rows);
    });
    db.close();
});

app.get('/last_readings', (req, res) => {
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.json(currentReadings)
});

// NETWORKINSERT - inserir novos registros na tabela NETWORK
app.post('/insert_reading', urlencodedParser, (req, res) => {
    res.statusCode = 200;
    // insere valores de nome e tipo segundo a request enviada pelo cliente
    const db = new sqlite3.Database(DBPATH);
    let reading = req.body
    console.log(reading)
    sql = "INSERT INTO sensor (datetime, temperature, humidity, greenhouse) VALUES ('" +
        reading.datetime + "', " + Number.parseFloat(reading.temperature) + ", " 
        + Number.parseFloat(reading.humidity) + ", " + Number.parseInt(reading.greenhouse) + ")";
    currentReadings[reading.greenhouse - 1] = reading
    db.run(sql, [], err => {
        if (err) {
            throw err;
        }
    });
    db.close();
    res.end()
});

/* Inicia o servidor */
app.listen(port, hostname, () => {
    console.log(`Server running at http://${hostname}:${port}/`);
});