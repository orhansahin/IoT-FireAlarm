from flask import Flask
from flask_restful import Api, Resource
from flask import request, abort
import datetime
import time

app = Flask(__name__)
api = Api(app)


temperature = {}
"""
    Post Temperature To Server via ESP8266
"""
class TemperaturePost(Resource):
    def post(self):
        t1 = time.clock()
        global temperature
        temperature = request.json          

        t2 = time.clock()
        if t2 - t1 <= 1:
            print(t2-t1)
            print(temperature)
            print(request.json)
            return "Temperature value received", 200
        else:
            print(t2-t1)
            abort(400)

class TemperatureGet(Resource):
    def get(self):
        global temperature
        temperature["Temperature"] = float("{:.1f}".format(temperature["Temperature"]))
        return temperature, 200


api.add_resource(TemperaturePost, "/api/post_temperature")
api.add_resource(TemperatureGet, "/api/get_temperature")


if __name__ == "__main__":
    app.run(debug=True) #debug mode on