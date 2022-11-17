import time
import requests
import datetime
import random

while True:
    for i in range(1, 5):
        body = {
            'datetime': datetime.datetime.now(),
            'greenhouse': i,
            'temperature': random.randrange(10, 51),
            'humidity': random.randrange(40, 101)
        }
        resp = requests.post(
            'http://10.128.64.110:1234/insert_reading', data=body)
    time.sleep(10)
