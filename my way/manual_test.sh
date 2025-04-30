#!/bin/bash

echo "Starting server..."
./server &
server_pid=$!

echo "Waiting for server to initialize..."
sleep 2

echo "Server process ID is: $server_pid"
echo "The PID you should see above is what to use for the client."

echo -e "\nTesting with a short message:"
./client $server_pid "Hello World! This is a test message."

echo -e "\nTesting with a longer message (100 characters):"
./client $server_pid "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"

echo -e "\nPress Ctrl+C to stop the server when done."
wait $server_pid
