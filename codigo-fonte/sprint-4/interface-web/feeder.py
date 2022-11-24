# Este script Python alimenta o banco de dados com valores aleatórios para teste da interface

import time # Biblioteca para pausar programa quando necessário
import requests # Biblioteca para fazer requisições HTTP
import datetime # Biblioteca para trabalhar com timestamps
import random # Biblioteca para trabalhar com sorteios

# Loop do programa; roda até o programa ser terminado
while True:
    # Gera e envia dados aleatórios para cada uma das estufas, de 1 a 4
    for i in range(1, 5):
        body = {
            'datetime': datetime.datetime.now(), # Pega data e hora atuais
            'greenhouse': i, # Pega próxima estufa na sequência (1-4)
            'temperature': random.randrange(10, 51), # Sorteia valor para temperatura
            'humidity': random.randrange(40, 101) # Sorteia valor para umidade
        }
        resp = requests.post(
            'http://10.128.2.41:1234/insert_reading', data=body) # Faz requisição
    time.sleep(10) # Espera 10 segundos
