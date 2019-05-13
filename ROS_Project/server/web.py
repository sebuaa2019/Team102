# -*- coding: UTF-8 -*-
from flask import Flask
from flask import render_template, redirect, url_for, send_from_directory, session
from flask import request
from flask import jsonify
import json
import os
import sqlite3
from functools import reduce
import time
import base64

app = Flask(__name__)

import socket

robot_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
robot_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
robot_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 4096)
robot_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 4096)
robot_server_socket.bind(("0.0.0.0", 13245))
robot_server_socket.listen(12)

recvsock = 0

def isConnected():
    print("hhhhh")
    try:
        recvsock.send(bytes('!', "ascii"), socket.MSG_OOB)
        return True
    except:
        return False

def reaccept():
    global recvsock
    conn,addr = robot_server_socket.accept()
    recvsock = conn


@app.route("/forward", methods = ['GET'])
def forward():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        recvsock.send(bytes("hhhhh", "ascii"))
    return "hahahaha"

@app.route("/right", methods = ['GET'])
def right():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        recvsock.send(bytes("qqqqq", "ascii"))
    return "hahahaha"
@app.route("/back", methods = ['GET'])
def back():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        recvsock.send(bytes("wwwww", "ascii"))
    return "hahahaha"
@app.route("/left", methods = ['GET'])
def left():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        recvsock.send(bytes("zzzzz", "ascii"))
    return "hahahaha"
def main():
    app.run()

if __name__ == "__main__":
    main()
