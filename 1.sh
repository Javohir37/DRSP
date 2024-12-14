#!/bin/bash
# Replace the 6th line with a new line
sed -i '6s/.*/Hi, this new line has been inserted here/' listdir
cat listdir
