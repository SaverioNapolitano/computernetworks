#!/usr/bin/env python3

import socket
import sys
import time
import os
import json

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 8080        # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT)) # parameter is a tuple that contains hostname and port number
    s.listen()
    conn, addr = s.accept()
    #print('Connected by', addr)
    request = conn.recv(1024).decode('utf-8')
    print('Request received from the client:\n')
    print('%s'% request)
    filename = json.loads(request)['filename']
    if os.path.exists(filename):
        with open(filename, 'rb') as f:
            answer = json.dumps({'filename': filename, 'filesize': os.path.getsize(filename)}).encode('utf-8') + '\n'.encode('utf-8') + f.read()
    else:
        answer = 'File not found\n'.encode('utf-8')
    print(f'Answer to the client\n{answer}')
    conn.sendall(answer)
    # socket must be closed by client! sleep for 1 second to wait for the client
    time.sleep(1)
    # otherwise socket goes to TIME_WAIT!
