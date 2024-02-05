#!/bin/bash

# Check if correct arguments were given
if [ $# -ne 2 ]; then
	echo "Usage: $0 <int: workers> <int: scheduler pid>"
	exit 1
fi

# Assign arguments to variables
W="$1"
sPID="$2"

# Run cleanup function when SIGINT is trapped
function cleanup() {
	echo -e "\nKilling Worker Processes:"
	# Delete all worker files and kill worker processes
	for ((i=1; i<=$W; i++)); do
		rm -f "worker${i}.txt"
	done
	kill $(jobs -p)
	exit 0
}
trap cleanup SIGINT

# Create worker files and processes
for ((i=1; i<=$W; i++)); do
        touch "worker${i}.txt"
	# Run processes in background for display to show
	./worker "worker${i}.txt" $sPID &
done

# Display worker files content, 1 file per second
while true; do
	clear
	for ((i=1; i<=$W; i++)); do
                echo "Printing worker${i}.txt:"
                cat "worker${i}.txt"
                echo ""
        done
	sleep 1
done
