#!/bin/zsh

#
# this script symlinks all libraries the local library or middleware folder to the arduino library folder. this 
# script only needs to be run when libraries are added or deleted. this script is for development purposes only.
#
# e.g `./link_libraries.sh ../KLST_PANDA/firmware/KLST_PANDA-libraries.txt ../KLST_PANDA/firmware $HOME/Documents/Arduino/libraries`
#

# Check if correct number of arguments is passed
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 file_list.txt source_directory destination_directory"
    exit 1
fi

# Assign command line arguments to variables
file_list=$1
source_dir=$2
destination_dir=$3

# Check if file list exists
if [ ! -f "$file_list" ]; then
    echo "Error: File list '$file_list' does not exist."
    exit 1
fi

# Check if source directory exists
if [ ! -d "$source_dir" ]; then
    echo "Error: Source directory '$source_dir' does not exist."
    exit 1
fi

# Check if destination directory exists
if [ ! -d "$destination_dir" ]; then
    echo "Error: Destination directory '$destination_dir' does not exist."
    exit 1
fi

while IFS= read -r file; do
    # Construct full path for source file
    full_path="$source_dir/$file"
    full_path=$(realpath "$full_path")
    
    # Check if the file exists before copying
    if [ -d "$full_path" ]; then
        full_dst_path="$destination_dir"/$(basename "$file")
        echo "linking library: '$(basename "$file")'"
        if [ -d "$full_dst_path" ]; then
            rm "$full_dst_path"
        fi
        ln -s "$full_path" "$full_dst_path"
    else
        echo "Warning: Folder '$full_path' not found, skipping..."
    fi
done < "$file_list"
