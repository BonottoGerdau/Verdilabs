# Este script Python alimenta o banco de dados com valores aleatórios para teste da interface

import time # Biblioteca para pausar programa quando necessário
import requests # Biblioteca para fazer requisições HTTP
import datetime # Biblioteca para trabalhar com timestamps
import random # Biblioteca para trabalhar com sorteios

def post(timestamp):
    for i in range(1, 5):
        body = {
            'datetime': timestamp, # Pega data e hora atuais
            'greenhouse': i, # Pega próxima estufa na sequência (1-4)
            'temperature': random.randrange(10, 51), # Sorteia valor para temperatura
            'humidity': random.randrange(40, 101) # Sorteia valor para umidade
        }
        resp = requests.post(
            'http://localhost:1234/insert_reading', data=body) # Faz requisição

start_time = datetime.datetime(year=2022, month=11, day=30, hour=13, minute=0, second=0)
for hour in range(24):
    for minute in range(60):
        print(start_time)
        post(start_time)
        start_time += datetime.timedelta(minutes=1)
        time.sleep(0.1)
        