#!/usr/bin/env python3

import socket
import sys

HOST=sys.argv[1]
PORT=2525

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT)) # if HOST is not an IP address it is converted into it
    hostname = socket.gethostname() # it returns a string
    s.sendall(f'Welcome from {hostname}'.encode('ascii'))
    