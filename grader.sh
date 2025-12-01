# grader.sh - Christopher Moretti

# Execute the grader executable inside of gdb in a way that doesn't
# suffer the "Illegal instruction" issue with the A and A+ attacks
# that surfaced in the Fall 2025 semester.

# Run as:
# ./grader.sh dataA
# or
# ./grader.sh dataAplus

# (./grader.sh dataB is also valid, but not necessary, as the B
# attack works with the native grader executable outside gdb.)

# If no command line argument is given, just run the grader
# executable in its standard interactive form



if [ "$#" -eq 0 ]; then
   ./grader
elif [ "$#" -eq 1 ]; then
    gdb --batch-silent -ex 'b *0x400858' -ex "r < $1" -ex 'c' -ex 'q' ./grader
else
    echo "Usage: $0 [inputFile]"
    exit 1
fi
