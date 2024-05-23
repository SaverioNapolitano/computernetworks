#!/usr/bin/env python3

import socket
import sys

HOST=sys.argv[1]
PORT=2525

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT)) # if HOST is not an IP address it is converted into it
    data = s.recv(1024)
    print('%s'% data.decode('utf-8')) # 'ascii' is fine too in this case