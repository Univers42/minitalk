#!/bin/bash

# Simple test script for minitalk performance

# Compile the programs
echo "Compiling client and server..."
make

# Check if compilation was successful
if [ ! -f "./server" ] || [ ! -f "./client" ]; then
    echo "Error: Compilation failed or executables not found."
    exit 1
fi

# Kill any existing server process
pkill -f "./server" 2>/dev/null

# Remove previous output files if they exist
rm -f server_output.txt

# Start the server in background with unbuffered output
stdbuf -o0 ./server > server_output.txt 2>&1 &
server_pid=$!

# Verify the server process is running
if ! ps -p $server_pid > /dev/null; then
    echo "Error: Server failed to start."
    exit 1
fi

echo "Server started with process ID: $server_pid"
echo "Waiting for server to initialize..."
sleep 2  # Give it more time to start up

# Make sure the output file exists
if [ ! -f server_output.txt ]; then
    echo "Error: server_output.txt not created. Check permissions."
    kill $server_pid
    exit 1
fi

# Display the current server output for debugging
echo "Current server output:"
cat server_output.txt
echo "--------------------------------"

# Try to get the server PID from the output
SERVER_PID=$(grep "Server PID:" server_output.txt 2>/dev/null | awk '{print $3}')

if [ -z "$SERVER_PID" ]; then
    echo "Error: Could not get server PID from output."
    echo "Checking if server is still running..."
    
    if ps -p $server_pid > /dev/null; then
        echo "Server process is still running."
        echo "Will use the process ID directly: $server_pid"
        SERVER_PID=$server_pid
    else
        echo "Server process has died."
        exit 1
    fi
fi

echo "Using Server PID: $SERVER_PID for communications"

# Function to generate test strings of specified length
generate_string() {
    length=$1
    if command -v python3 &>/dev/null; then
        python3 -c "print('A' * $length)"
    else
        # Fallback for systems without Python
        perl -e "print 'A' x $length"
    fi
}

# Test with various message sizes
for size in 10 50 100; do
    echo "====== Testing with message size $size ======"
    test_message=$(generate_string $size)
    
    # Run client with the test message
    echo "Starting client with $size characters..."
    ./client $SERVER_PID "$test_message"
    
    # Wait for processing to complete
    sleep 1
    
    # Display updated server output
    echo "Server output after sending $size characters:"
    tail -5 server_output.txt
    echo "----------------------------------------"
done

# Clean up
echo "Stopping server (PID: $server_pid)..."
kill $server_pid
echo "Test completed."
echo "Server full output has been saved to server_output.txt"
