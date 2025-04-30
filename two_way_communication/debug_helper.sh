#!/bin/bash

if [ -z "$1" ]; then
  echo "Usage: $0 <pid>"
  exit 1
fi

echo "Signal Monitor for PID $1"
echo "Press Ctrl+C to exit"

while true; do
  ps -p $1 > /dev/null 2>&1
  if [ $? -ne 0 ]; then
    echo "Process $1 no longer exists"
    exit 1
  fi
  
  echo "Monitoring process $1..."
  sleep 2
done