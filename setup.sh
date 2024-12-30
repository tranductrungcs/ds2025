#!/bin/bash

# Install required dependencies
sudo apt update
sudo apt install -y rpcbind gcc make

# Compile the RPC files
rpcgen rpc_ftp.x
gcc -o server server.c rpc_ftp_svc.c rpc_ftp_xdr.c -lnsl
gcc -o client client.c rpc_ftp_clnt.c rpc_ftp_xdr.c -lnsl

echo "Setup complete. Run ./server to start the server and ./client to use the client."
