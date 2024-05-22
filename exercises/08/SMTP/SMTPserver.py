#!/usr/bin/env python3
import socket
import sys
import time
import re

HOST = '127.0.0.1' # Standard loopback interface (localhost)
PORT = 2525 # Port to listen on (non-privileged ports are > 1023)

def process_quit(conn, msg):
    print('submitting message %s', msg)
    send_response(conn, '221')

def add_msg_body(data, msg):
    data=data.decode('ascii')
    if 'rfc822' not in msg.keys(): msg['rfc822']=''
    msg['rfc822'] += data
    m=re.search('\r\n.\r\n', data)
    print('adding data: "%s", rv: %r' % (data, bool(m)))
    return bool(m)

def process_envelope(conn, msg, cmd, param): 
    if cmd == 'MAIL':
        m=re.search('FROM: <(.+@.+)>', param) # (): group I can refer to, .+: one or more characters 
        msg['from'] = m.group(1)
        print('add envelope sender %s' % msg['from'])
        send_response(conn, '250', msg='Sender OK')
    if cmd == 'RCPT':
        m=re.search('TO: <(.+)>', param)
        msg['to'] = m.group(1)
        print('add envelope recipient %s' % msg['to'])
        send_response(conn, '250', msg='Recipient OK')

def send_response(conn, code, msg=''):
    resp=f'{code} {msg}\r\n'
    print('response is "%s"' % resp.splitlines()[0])
    conn.sendall(resp.encode('ascii'))

def process_mail_command(conn, data, msg):
    if data is not None:
        cmd=data[0:4].decode('ascii')
        param=data[5:-1].decode('ascii').strip()
        print('Mail command: cmd="%s", param="%s"'%(cmd, param))
        if cmd == 'QUIT': process_quit(conn, msg, msg='Goodbye')
        if cmd == 'HELO': send_response(conn, '250')
        if cmd == 'MAIL': process_envelope(conn, msg, cmd, param)
        if cmd == 'RCPT': process_envelope(conn, msg, cmd, param)
        if cmd == 'DATA': send_response(conn, '354')
        return cmd

def process_mail_request(conn):
    # send welcome message
    send_response(conn, '220', msg='Welcome from tinySMTP')
    msg = {}
    rv = process_mail_command(conn, conn.recv(1024), msg)
    # process commands, end when receiving QUIT
    while rv != 'QUIT':
        rv = process_mail_command(conn, conn.recv(1024), msg)
        if rv == 'DATA':
            while not add_msg_body(conn.recv(1024), msg): pass
            send_response(conn, '250', msg='Data received')

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    process_mail_request(conn)
    time.sleep(1)