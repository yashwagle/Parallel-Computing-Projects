To compile the program on my machine I used the following command:
 mpicc -o bitonic bitonic.c
 but on kraken I got an error for the math library I have imported so I used the command
 mpicc -o bitonic bitonic.c -lm

To run the program I have used
mpirun -np <number of processes> ./bitonic <size of data> <1 or 2>
where <number of processes> is the number of processes to be created
<size of data> is the size of the array to be generated
<1 or 2> if 1 then the process will print the sorted array, if 2 will not print
Both the number of processes and the size of data should be a power of 2 else the program will fail
e.g. runs on kraken

mpirun -np 16 ./bitonic 1048576 2
 
mpirun -np 8 ./bitonic 1024 2