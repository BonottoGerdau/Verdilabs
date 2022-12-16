# Este script Python alimenta o banco de dados com valores aleatórios para teste da interface

import time # Biblioteca para pausar programa quando necessário
import requests # Biblioteca para fazer requisições HTTP
import datetime # Biblioteca para trabalhar com timestamps
import random # Biblioteca para trabalhar com sorteios

# Função que faz post de leituras aleatórias para certo timestamp, para todas as estufas
def post(timestamp):
    for i in range(1, 5):
        body = {
            'datetime': timestamp, # Pega data e hora atuais
            'greenhouse': i, # Pega próxima estufa na sequência (1-4)
            'temperature': random.randrange(10, 51), # Sorteia valor para temperatura
            'humidity': random.randrange(40, 101) # Sorteia valor para umidade
        }
        resp = requests.post(
            'http://10.128.65.52:1234/insert_reading', data=body) # Faz requisição

# Alimenta banco continuamente de 10 em 10 minutos
#while True:
#    post(datetime.datetime.now()) # Faz post com timestamp atual
#    time.sleep(10) # Espera 10 segundos

# Alimenta banco a longo prazo, para simular leituras de mais de um dia (neste caso, para uma semana)
start_date = datetime.datetime(year=2022, month=12, day=16, hour=3, minute=14, second=0) # Data inicial
for day in range(7): # Sete dias
    for hour in range(24): # 24h por dia
        for minute in range(60): # 60m por hora
            post(start_date) # Faz o post
            start_date += datetime.timedelta(minutes=1) # Aumenta 1 minuto
            time.sleep(0.05) # Espera 0,05 segundos para não sobrecarregar o servidor