import requests

#BASE = "http://127.0.0.1:5000/"
BASE = "http://192.168.1.7:5000/"


temperature = {"token": "222.3", "Temperature": 23}


"""
    Post Temperature
"""
def r_post_temperature():
    response = requests.post(BASE + "/api/post_temperature", json=temperature)
    if response.status_code == 200:
        print(response.json())
        print(response.status_code)
    else:
        print(response.status_code)


"""
    Get Temperature
"""
def r_get_temperature():
    response = requests.get(BASE + "/api/get_temperature")
    print(response.json())



r_post_temperature()
r_get_temperature()