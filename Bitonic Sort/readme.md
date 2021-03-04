## Bitonic Sort

Before looking at Bitonic sort let us look at 
the comparator circuits which form the basics of
bitonic sort.

![alt text](comparators.png)

In the increasing comparator circuit 
(denoted by +) we provide two numbers. In 
the upper line we get the minimum of
the two numbers as output while in the lower
line we get the maximum number as output.

The decreasing comparator circuit (denoted
by -) provides the reverse output. The
upper line gets the maximum while the 
lower line gets the minimum value.

A bitonic sequence is one which is increasing
and then decreasing for example 2,4,6,8,10,9,7,5,3,1
is a bitonic sequence. Now given a bitonic
sequence it can be easily converted into
a sorted sequence using the increasing
comparator circuit.

![alt text](BitonicToSorted.png)

Now to convert a given sequence to bitonic
we need the increasing as well as the 
decreasing comparator circuits.

![alt text](RandomToBitonic.png)

Now we can easily combine the two to get 
a sorted array 

## Implementation

## Results
![alt text](timevsdata.png)


time v/s number of data elements; colors
represent the number of processes


![alt text](barchart.png)

bar chart of time v/s number of data elements; 
colors represent the number of processes

## Running the Program

Prerequisites MPI for C should be configured in the path

To compile the program

 mpicc -o bitonic bitonic.c
  
 Some machines give error for the math library so compile using
 
 mpicc -o bitonic bitonic.c -lm

To run the program 

mpirun -np \<number of processes> ./bitonic \<size of data>  \<1 or 2>

where \<number of processes> is the number of processes to be created

\<size of data> is the size of the array to be generated

<1 or 2> if 1 then the process will print the sorted array, if 2 will not print
Both the number of processes and the size of data should be a power of 2 else the program will fail

e.g. 

mpirun -np 16 ./bitonic 1048576 2
 
mpirun -np 8 ./bitonic 1024 2