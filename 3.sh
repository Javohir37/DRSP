#!/bin/bash
# Read the size of the list
echo "Enter the number of elements:"
read n

# Declare an array
declare -a list

# Read the elements into the array
echo "Enter $n numbers:"
for ((i=0; i<n; i++)); do
    read list[i]
done

# Initialize max and min
max=${list[0]}
min=${list[0]}

# Find max and min
for num in "${list[@]}"; do
    if (( num > max )); then
        max=$num
    fi
    if (( num < min )); then
        min=$num
    fi
done

echo "Maximum Element: $max"
echo "Minimum Element: $min"

# Search for a key
echo "Enter the key to search for:"
read key
found=0

for i in "${!list[@]}"; do
    if [ "${list[$i]}" == "$key" ]; then
        echo "KEY FOUND at position $i"
        found=1
        break
    fi
done

if [ $found -eq 0 ]; then
    echo "NOT FOUND"
fi

