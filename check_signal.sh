#!/bin/bash
PROJECT_DIR="."

echo "Checking for signal-related commands..."
grep -r -E "trap|kill|killall" "$PROJECT_DIR"

echo "Checking trap commands..."
grep -r "trap" "$PROJECT_DIR" | while read -r line; do
    if [[ $line =~ SIGUSR1|SIGUSR2 ]]; then
        echo "Found communication signal: $line"
    else
        echo "Warning: Non-communication signal found: $line"
    fi
done

echo "Checking PID usage..."
grep -r "pid" "$PROJECT_DIR"