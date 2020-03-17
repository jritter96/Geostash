#!/bin/sh
openssl genrsa -out key.pem
openssl req -new -config myserver.cnf -key key.pem -out csr.pem
openssl x509 -req -days 9999 -in csr.pem -signkey key.pem -out cert.pem -sha256 -extfile v3.ext
rm csr.pem