
// CÓDIGO DO SERVIDOR DE GREENER: serve interface gráfica, recebe informações do ESP-32 e salva
// no banco de dados

// importa bibliotecas necessárias para criar servidor (express) e se comunicar com database (sqlite3)
const express = require('express');
const sqlite3 = require('sqlite3').verbose();
var moment = require('moment')

// cria servidor 
const app = express();

// define host e porta do servidor
const port = process.env.PORT || 1234;

// cria objetos para interpretar dados passados em requisições HTTP
const bodyParser = require('body-parser');
const urlencodedParser = bodyParser.urlencoded({ extended: false });

// define que pasta "public" deve ser utilizada como source para página principal (index.html)
app.use(express.static("public"));
// define que utilizaremos json em nossas comunicações
app.use(express.json());

// evita erros de CORS nas requisições de banco de dados
const cors = require("cors");
const corsOptions = {
    origin: '*',
    credentials: true,
    optionSuccessStatus: 200
}
app.use(cors(corsOptions))

// caminho do banco de dados
const DBPATH = 'db.db'

// salva últimas leituras recebidas do ESP-32
let currentReadings = [0, 0, 0, 0]
// salva parâmetros recebidos do frontend para o ESP-32
let parameters = [0, 0, 0, 0]
// salva estado de erro atual
let error = "0"

/* DEFINIÇÃO DOS ENDPOINTS */

// Retorna todos os dados salvos no banco de dados
app.get('/all_readings', (req, res) => {
    // Define header e status de sucesso
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');

    // Inicia comunicação com database
    const db = new sqlite3.Database(DBPATH);
    var sql = 'SELECT * FROM sensor ORDER BY datetime'; // comando SQL
    // Executa comando e devolve resultados em json
    db.all(sql, [], (err, rows) => {
        if (err) {
            throw err;
        }
        res.json(rows);
    });
    db.close(); // Fecha banco
});

// Devolve todas as medições para uma certa estufa
app.get('/greenhouse_readings', (req, res) => {
    const { greenhouse } = req.query; // pega número da estufa
    // Define header e status de sucesso
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');

    // Inicia comunicação com database
    const db = new sqlite3.Database(DBPATH);
    var sql = 'SELECT * FROM sensor WHERE greenhouse = ' + greenhouse + ' ORDER BY datetime'; // comando SQL
    // Executa comando e devolve resultados em json
    db.all(sql, [], (err, rows) => {
        if (err) {
            throw err;
        }
        res.json(rows);
    });
    db.close(); // Fecha banco
});

// Retorna linhas do banco de dados entre certas datas e oriundas de certa estufa, segundo query HTTP
app.get('/filtered_readings', (req, res) => {
    // Define header e status de sucesso
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');
    // Inicia comunicação com database
    const db = new sqlite3.Database(DBPATH);
    const { datetime_start, datetime_end, greenhouse } = req.query; // Desmembra query em constantes
    var sql = 'SELECT * FROM sensor WHERE (datetime BETWEEN "' + moment().format() + '" AND "' +
        moment(datetime_end).format() + '") AND greenhouse == ' + Number.parseInt(greenhouse); // Filtra por data e estufa
    db.all(sql, [], (err, rows) => {
        if (err) {
            throw err;
        }
        res.json(rows); // Retorna resultados em json
    });
    db.close();
});

// retorna temperatura mínima atual
app.get('/tempMin', (req, res) => {
    // Define header e status de sucesso
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');
    // Inicia comunicação com database
    const db = new sqlite3.Database(DBPATH);
    var sql = 'SELECT * FROM parameter ORDER BY id DESC LIMIT 1'; // Pega última linha adicionada
    db.all(sql, [], (err, rows) => {
        if (err) {
            throw err;
        }
        res.json(rows[0].tempMin); // Retorna resultados em json
    });
    db.close();
});

// retorna temperatura máxima atual
app.get('/tempMax', (req, res) => {
    // Define header e status de sucesso
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');
    // Inicia comunicação com database
    const db = new sqlite3.Database(DBPATH);
    var sql = 'SELECT * FROM parameter ORDER BY id DESC LIMIT 1'; // Retorna linha mais recente da tabela
    db.all(sql, [], (err, rows) => {
        if (err) {
            throw err;
        }
        res.json(rows[0].tempMax); // Retorna resultados em json
    });
    db.close();
});

// Retorna umidade mínima atual
app.get('/humidityMin', (req, res) => {
    // Define header e status de sucesso
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');
    // Inicia comunicação com database
    const db = new sqlite3.Database(DBPATH);
    var sql = 'SELECT * FROM parameter ORDER BY id DESC LIMIT 1'; // Retorna linha mais recente da tabela
    db.all(sql, [], (err, rows) => {
        if (err) {
            throw err;
        }
        res.json(rows[0].humidityMin); // Retorna resultados em json
    });
    db.close();
});

// Retorna umidade máxima atual
app.get('/humidityMax', (req, res) => {
    // Define header e status de sucesso
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');
    // Inicia comunicação com database
    const db = new sqlite3.Database(DBPATH);
    var sql = 'SELECT * FROM parameter ORDER BY id DESC LIMIT 1'; // Retorna linha mais recente da tabela
    db.all(sql, [], (err, rows) => {
        if (err) {
            throw err;
        }
        res.json(rows[0].humidityMax); // Retorna resultados em json
    });
    db.close();
});

// Endpoint para inserir novos parâmetros
app.post('/insert_parameters', (req, res) => {
    res.statusCode = 200;
    const db = new sqlite3.Database(DBPATH);
    let parameters = req.body // Salva dados da requisição em uma variável
    // Cria comando SQL para inserir campos do body no banco de dados
    sql = "INSERT INTO parameter (tempMin, tempMax, humidityMin, humidityMax) VALUES ('" +
        Number.parseFloat(parameters.tempMin) + "', " + Number.parseFloat(parameters.tempMax) + ", "
        + Number.parseFloat(parameters.humidityMin) + ", " + Number.parseFloat(parameters.humidityMax) + ")";
    db.run(sql, [], err => {
        if (err) {
            throw err;
        }
    });
    db.close();
    res.end();
});

// Retorna dados recebidos no último POST
app.get('/last_readings', (req, res) => {
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.json(currentReadings) // Transforma array de últimas leituras em json
});

// Insere dados recebidos em JSON no banco de dados e salva no array "currentReadings"
app.post('/insert_reading', urlencodedParser, (req, res) => {
    res.statusCode = 200;
    const db = new sqlite3.Database(DBPATH);
    let reading = req.body // Salva dados da requisição em uma variável
    // Cria comando SQL para inserir campos do body no banco de dados
    sql = "INSERT INTO sensor (datetime, temperature, humidity, greenhouse) VALUES ('" +
        moment(reading.datetime).format() + "', " + Number.parseFloat(reading.temperature) + ", "
        + Number.parseFloat(reading.humidity) + ", " + Number.parseInt(reading.greenhouse) + ")";

    // Salva dados no array currentReadings 
    // (no momento, hardocded para salvar apenas dados das estufa 1), 
    // pois recebemos apenas dados dessa estufa por enquanto
    currentReadings[reading.greenhouse - 1] = reading
    console.log(currentReadings)
    db.run(sql, [], err => {
        if (err) {
            throw err;
        }
    });
    db.close();
    res.end()
});

// Endpoint para inserir erro do sistema
app.post('/insert_error', urlencodedParser, (req, res) => {
    console.log(req.body)
    res.statusCode = 200;
    error = req.body // Salva erro atual em uma variável global
    error.datetime = moment() // Salva hora do erro
    res.end()
});

// Retorna último erro
app.get('/error', urlencodedParser, (req, res) => {
    res.statusCode = 200;
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.json(error)
});

// Inicia o servidor
app.listen(port, function () {
    console.log(`Listening on port ${port}`);
});

module.exports = app; // Exporta para funcionar na hospedagem remota