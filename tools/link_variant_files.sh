#!/bin/zsh

#
# this script symlinks all `variant` files from the local variant folder to the STM32duino variant folder. this 
# script only needs to be run when files are added or deleted. this script is for development purposes only.
#
# e.g `./link_variant_files.sh ../variants/KLST_PANDA/variant $STM32DUINO_HOME/variants/STM32H7xx/H723Z\(E-G\)T_H730ZBT_H733ZGT`
#

# Check if correct number of arguments is passed
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 source_directory destination_directory"
    exit 1
fi

# Assign command line arguments to variables
source_dir=$1
destination_dir=$2

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

# moved to extra script
# # Remove old symbolic links in the destination directory
# find "$destination_dir" -type l -exec rm {} \;

# Create new symbolic links from the source directory to the destination directory
for file in "$source_dir"/*; do
    echo "linking file   : '$(basename "$file")'"
    ln -s "$(realpath "$file")" "$destination_dir"/$(basename "$file")
done
