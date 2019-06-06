# -*- coding: UTF-8 -*-
from flask import Flask
from flask import render_template, redirect, url_for, send_from_directory, session
from flask import make_response
from flask import request
from flask import jsonify
import json
import os
import sqlite3
from functools import reduce
import time
import base64
import struct

app = Flask(__name__)

import socket

robot_daemon_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
robot_daemon_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
robot_daemon_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 4096)
robot_daemon_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 4096)
robot_daemon_server_socket.bind(("0.0.0.0", 13245))
robot_daemon_server_socket.listen(12)

robot_server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
robot_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
robot_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 4096)
robot_server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 4096)
robot_server_socket.bind(("0.0.0.0", 13246))
robot_server_socket.listen(12)

recvsock = 0

def protocolSend(sock, data):
    length = len(data)
    sock.send(struct.pack('II', 0xdeadbeef, length))
    sock.send(data)

def protocolRecv(sock):
    data = sock.recv(8, socket.MSG_WAITALL)
    header = struct.unpack('II', data)
    if header[0] != 0xdeadbeef:
       return None 
    return sock.recv(header[1], socket.MSG_WAITALL).decode('ascii')

def return_response(string):
    resp = make_response()
    resp.headers["Access-Control-Allow-Origin"] = "*"
    resp.response = string 
    return resp

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


@app.route("/mapScanning", methods = ['GET'])
def mapScanning():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        protocolSend(recvsock, bytes("mapScanning", "ascii"))
#        state = protocolRecv(recvsock)
    return return_response("MapSuccess")

@app.route("/appoint", methods = ['GET'])
def appoint():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        protocolSend(recvsock, bytes("appoint", "ascii"))
    return return_response("AppointSuccess")

@app.route("/kill", methods = ['GET'])
def kill():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        protocolSend(recvsock, bytes("kill", "ascii"))
    return return_response("killsuccess")


@app.route("/test", methods = ['GET'])
def test():
    return return_response("hahahaha")

@app.route("/forward", methods = ['GET'])
def forward():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        protocolSend(recvsock, bytes("hhhhh", "ascii"))
    return return_response("hahahaha")

@app.route("/right", methods = ['GET'])
def right():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        protocolSend(recvsock, bytes("qqqqq", "ascii"))
    return return_response("hahahaha")

@app.route("/back", methods = ['GET'])
def back():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        protocolSend(recvsock, bytes("wwwww", "ascii"))
    return return_response("hahahaha")

@app.route("/left", methods = ['GET'])
def left():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        protocolSend(recvsock, bytes("zzzzz", "ascii"))
    return return_response("hahahaha")

@app.route("/stop", methods = ['GET'])
def stop():
    global recvsock
    if request.method == 'GET':
        if recvsock == 0:
            reaccept()
        elif not isConnected():
            reaccept()
        protocolSend(recvsock, bytes("sssss", "ascii"))
    return return_response("StopSuccess")

@app.route("/getRobotServerPort", methods = ['GET'])
def getRobotServerPort():
    return str(robot_server_socket.getsockname()[1])

def main():
    app.run()

if __name__ == "__main__":
    main()
