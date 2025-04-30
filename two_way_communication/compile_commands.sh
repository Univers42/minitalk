#!/bin/bash
# Clean previous builds
rm -f server client

# Compile the server
gcc -Wall -Werror -Wextra -o server server.c

# Compile the client
gcc -Wall -Werror -Wextra -o client client.c

echo "Compilation complete."
echo "Run server: ./server"
echo "Run client: ./client <server_pid>"
