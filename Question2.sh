#!/bin/bash

# Function to create directory structure and measure time
createDirectory() {
    local dirName=$1
    local fileCount=$2
    local fileSize=$3
    local isRecursive=$4

    echo "Creating $dirName with $fileCount files of $fileSize each..."

    # Convert human-readable size to bytes
    case $fileSize in
        G) fileSizeBytes=$(( ${fileSize%G} * 1024*3 ));;
        M) fileSizeBytes=$(( ${fileSize%M} * 1024*2 ));;
        *K) fileSizeBytes=$(( ${fileSize%K} * 1024 ));;
        *)  fileSizeBytes=$fileSize;;
    esac

    if [ "$isRecursive" == true ]; then
        time (
            mkdir -p "$dirName"
            for ((i=1; i<=fileCount; i++)); do
                mkdir -p "$dirName/subdir$i"
                dd if=/dev/zero of="$dirName/subdir$i/file$i" bs=$fileSizeBytes count=1 status=none
            done
        )
    else
        time (
            mkdir -p "$dirName"
            for ((i=1; i<=fileCount; i++)); do
                dd if=/dev/zero of="$dirName/file$i" bs=$fileSizeBytes count=1 status=none
            done
        )
    fi
}

# Case 1: 100 files of 1GB each
# createDirectory "case1" 100 1G false

# Case 2: 10,000 files of 10MB each
# createDirectory "case2" 10000 10M false

# Case 3: 100 files of 10MB each, recursively creating subdirectories until a total of 10,000 files
createDirectory "case3" 100 1M true

echo "Directory creation completed."
