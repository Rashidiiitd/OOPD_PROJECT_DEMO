#!/bin/bash

# Create directory for 100 files of 1GB each
#mkdir large_files
#for i in $(seq 1 100); do
  #  touch "large_files/file_$i.1gb"
 #   fallocate -l 512M "large_files/file_$i.1gb"
#done

# Time taken to create large_files directory
#time ls -l large_files

# Create directory for 10,000 files of 10MB each
mkdir medium_files
for i in $(seq 1 1000); do
    touch "medium_files/file_$i.10mb"
    fallocate -l 10M "medium_files/file_$i.10mb"
done

# Time taken to create medium_files directory
time ls -l medium_files

# Create directory structure with 10,000 10MB files

# Time taken to create nested_files directory
