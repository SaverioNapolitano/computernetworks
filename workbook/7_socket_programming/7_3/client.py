#!/usr/bin/env python3

import socket
import sys
import json

HOST=sys.argv[1]
PORT=8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT)) # if HOST is not an IP address it is converted into it
    print('Enter the name of the file you want to download (ex. "file.txt")')
    filename = input()
    request = json.dumps({'filename': filename}, indent=4)
    print(f'Request to the server:\n{request}')
    s.sendall(request.encode('utf-8'))
    answer = s.recv(1024)
    print(f'Answer received from the server:\n{answer}')
    if str(answer).startswith('{'):
        content = str(answer).split('}', 1)
        print(f'Content of the file\n{content[1]}')
        with open(filename, mode='a') as f:
            f.write(content[1])
    
