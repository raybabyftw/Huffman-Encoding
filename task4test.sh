# Codes for printing colour to the terminal
PURPLE='\033[95m\033[1m\033[4m';
GREEN='\033[92m\033[1m\033[4m';
BLUE='\033[94m\033[1m\033[4m'
ENDC='\033[0m';


if [ -z "$1" ] 
then
    echo "Usage: sh task4test.sh [repeat]";
    echo "For example: >> sh task4test.sh 5 << to run each program 5 times.";
    echo;
    exit
fi

# Clear all pre-existing outputs
rm task4/*.enc;

# Compile the files.
printf $BLUE;
echo Compiling;
printf $ENDC;
make;
echo;

# For each file in the task4 directory, except only once for each unique file name
# eg. only "wonderland" for the files "wonderland.enc", "wonderland.txt"
for file in `ls task4 | grep tree | cut -f 1 -d '.' ` 
do
    printf $BLUE;
    echo "ENCODING $file...";
    printf $ENDC;
    
    # Do 10 tries.
    for i in `seq $1`
    do
        # Time how long it takes to encode the file.
        /usr/bin/time -f "Encoding took %U seconds." ./encode task4/$file.txt task4/$file.tree task4/$file.enc;
    done

    # Print the diff result in purple
    printf $PURPLE;
    diff -sq task4/$file.enc task4/expected_encodings/$file.enc;
    printf $ENDC;
    echo;
    echo;
done

# Finished!
printf $GREEN;
echo Finished!;
printf $ENDC;
echo;
echo;