#!/usr/local/bin/python3
# -*- coding: utf-8 -*-


import sys
from flask import Flask, request
from flask_restful import Resource, Api
from sklearn import svm
from pymongo import MongoClient


db = MongoClient('mongodb://localhost')
db = db['safe_desk']

app = Flask(__name__)
api = Api(app)


class Data(Resource):
    def get(self):
        data = [i for i in db['data'].find({})]
        return data;
    def post(self):
        data = request.form['data']
        return data
        #db['data'].insert_one(data)


api.add_resource(Data, '/data')

if __name__ == '__main__':
    app.run(debug=False)
