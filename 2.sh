#!/bin/bash
# Read the filename from user input
echo "Enter the filename:"
read filename

# Check if file exists
if [ -e "$filename" ]; then
    echo "$filename exists."
    
    # Check if regular file
    if [ -f "$filename" ]; then
        echo "$filename is a regular file."
    fi
    
    # Check if directory
    if [ -d "$filename" ]; then
        echo "$filename is a directory."
    fi
    
    # Check if symbolic link
    if [ -L "$filename" ]; then
        echo "$filename is a symbolic link."
    fi
    
    # Check if block or character device file
    if [ -b "$filename" ]; then
        echo "$filename is a block device file."
    fi
    
    if [ -c "$filename" ]; then
        echo "$filename is a character device file."
    fi
    
    # Check if file is readable, writable and executable
    [ -r "$filename" ] && echo "$filename is readable."
    [ -w "$filename" ] && echo "$filename is writable."
    [ -x "$filename" ] && echo "$filename is executable."
    
    # Check if file is empty
    if [ ! -s "$filename" ]; then
        echo "$filename is empty."
    fi
    
else
    echo "$filename does not exist."
fi

