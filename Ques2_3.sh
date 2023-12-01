#!/bin/bash
create_files() {
  local num_files=$1
  local base_dir=$2

  for ((i=1; i<=num_files; i++)); do
    #echo "This is file $i" > "$base_dir/file$i.txt"
    dd if=/dev/zero of="$base_dir/file$i.txt" bs=1024 count=1 status=none
  done
}

create_directories() {
  local depth=$1
  local base_dir=$2
 
  if [ "$depth" -le 0 ]; then
    return
  fi

  for ((i=1; i<=depth; i++)); do
    current_dir="$base_dir/dir$i"
    mkdir -p "$current_dir"
    #echo "Created directory: $current_dir"
   
    # Call the function to create files within the current directory
    create_files 100 "$current_dir"
   
    # Recursive call to create subdirectories
    create_directories "$((depth-1))" "$current_dir"
  done
}
# Set the depth and base directory
depth=100
base_directory="main_directory"

# Record the start time
start_time=$(date +%s)

# Call the function to start creating directories
create_directories "$depth" "$base_directory"

# Record the end time
end_time=$(date +%s)

# Calculate the total time taken
total_time=$((end_time - start_time))
echo "Total time taken: $total_time seconds"
