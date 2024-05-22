#!/usr/bin/env python3

import socket
import os
import sys
import time

HOST = '127.0.0.1' # Standard loopback interface address (localhost)
PORT = 2525 # Port to connect to

def expect_response(s: str, exp_code: str):
    data = s.splitlines()
    for l in data:
        print(l)
    # data[last_line] -> response
    rv = data[-1][0:3]
    if rv != exp_code:
        print('received: "%s" instead of "%s"' %(rv, exp_code))
        sys.exit()

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    # Receive welcome message
    expect_response(s.recv(1024).decode('ascii'), '220')
    # send HELO message
    s.sendall('HELO localhost\r\n'.encode('ascii')) # EHLO: Extended HELLO (part of the Extended SMTP which is used nowadays)
    expect_response(s.recv(1024).decode('ascii'), '250')
    # send MAIL envelope
    s.sendall('MAIL FROM: <saverio@localhost>\r\n'.encode('ascii'))
    expect_response(s.recv(1024).decode('ascii'), '250')
    s.sendall('RCPT TO: <saverio@localhost>\r\n'.encode('ascii'))
    expect_response(s.recv(1024).decode('ascii'), '250')
    # send MAIL message (RFC822)
    s.sendall('DATA\r\n'.encode('ascii'))
    expect_response(s.recv(1024).decode('ascii'), '354')
    s.sendall('From: saverio <saverio@localhost>\r\n'\
            'To: saverio <saverio@localhost>\r\n'\
            'Subject: prova\r\n\r\n'\
            'Messaggio di prova\r\n.\r\n'.encode('ascii'))
    expect_response(s.recv(1024).decode('ascii'), '250')
    # Submit message
    s.sendall('QUIT\r\n'.encode('ascii'))
    expect_response(s.recv(1024).decode('ascii'), '221')
