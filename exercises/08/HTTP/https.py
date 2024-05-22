import socket
import os
import sys
import time

# server HTTP in C is quite complex

HOST = '127.0.0.1'
PORT = 8080

HTDCOS = './htdocs' # root directory of Web documents mapped on the url /, needs to be created if not already present (mkdir)
MIMETYPES = [ ('.gif', 'image/gif'),
			  ('.png', 'image/png'), 
			  ('.jpeg', 'image/jpeg'), 
			  ('.html', 'text/html')] # based on extensions (matches them with types of files), list of tuples

def parse_request(conn): # need to retrieve request line (method, url, version)
	request = ''
	# there's no guarantee everything is received within one read
	while True:
		request += conn.recv(1024).decode('ascii') # decode because it's a byte array and a string is needed
		if request.find('\r\n\r\n'): # if blank line is found
			break
	reqline, headers = request.split('\r\n', 1) # limit number of splits to 1
	method, url, version = reqline.split(' ', 3) 
	return url
	
def send_response(conn, url):
	doc_root = HTDCOS
	fname = doc_root + url 
	if not os.path.exists(fname):
		print(f'file {fname} not found')
		send_404(conn, url)
	else:
		send_200(conn, fname)
	conn.close()

def send_404(conn, fname):
	body = f'<html><body><h1>Requested URL {fname} not found</h1></body></html>'
	resp = 'HTTP/1.0 404 Not Found\r\n' + \
	       'Connection: close\r\n' + \
	       'Content-Type: text/html\r\n' + \
	       'Content-Length: %d\r\n\r\n' % len(body) + body
	conn.sendall(resp.encode('ascii')) # encode because it's a string and a byte array is needed
	time.sleep(1) # to be sure client closes the connection first

def send_200(conn, fname): # retrieve the file
	resp = 'HTTP/1.0 404 Not Found\r\n' + \
	       'Connection: close\r\n' + \
	       'Content-Type: %s\r\n' % get_mime(fname) + \
	       'Content-Length: %d\r\n\r\n' % os.path.getsize(fname)
	conn.sendall(resp.encode('ascii'))
	with open(fname, 'rb') as f:
		l = f.read(1024)
		while (l):
			conn.send(l)
			l = f.read(1024)
	time.sleep(1)

def get_mime(fname):
	for ext, mimetype in MIMETYPES:
		if fname.endswith(ext):
			return mimetype
	return 'text/html' # default

def serve_request(conn):
	url=parse_request(conn)
	print(url)
	send_response(conn, url)
	conn.close()

# main
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
	s.bind((HOST, PORT))
	s.listen()
	while True: # each time a request is received, father creates a child to manage it
		conn, addr = s.accept() # create the socket (father waits for requests)
		child_pid = os.fork() # inefficient, better use a pre-fork approach (fixed-number of children created before the loop) or multi-threading (unstable)
		if child_pid == 0:
			print(f'connection from {addr}')
			serve_request(conn)
			sys.exit()
