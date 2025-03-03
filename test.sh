#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <number_of_clients>"
    exit 1
fi

num_clients=$1

for ((i=0; i<num_clients; i++)); do
    (
      echo "liren"
      echo "hello"
    ) | ./client &
done

wait

echo "All clients have been launched."
