#!/usr/bin/env python3

import socket
import sys
import time
import os

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 2525        # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT)) # parameter is a tuple that contains hostname and port number
    s.listen()
    while True:
        conn, addr = s.accept()
        child_pid = os.fork()
        #print('Connected by', addr)
        if child_pid == 0:
            hostname = socket.gethostname() # it returns a string
            conn.sendall(f'Welcome from {hostname}'.encode('ascii'))
            # socket must be closed by client! sleep for 1 second to wait for the client
            sys.exit()