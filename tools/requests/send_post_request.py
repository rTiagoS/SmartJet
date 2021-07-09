import requests

smartjet_url = 'http://192.168.4.1/user_credentials'

smartjet_data = {
    'tag'          : 'tiagoramos121@gmailcom',
    'ssid'         : 'LIVE TIM_3322_2G',
    'pass'         : 'KyxqxYSG',
    'idplayer'     : '2111212121212121',
    'nr_smartjets' : '200'
    }

response = requests.post(smartjet_url, json = smartjet_data)

print(response.text)