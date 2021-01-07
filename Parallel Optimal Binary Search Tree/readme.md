#Optimal Binary Search Trees
In this problem given a set of keys and their access frequencies, we have
to construct a BST such that the least overall access time is achieved. For
example suppose we have two keys [10,15] with frequencies as [10,30]. 
There can be two BSTs with one 10 as the root and the second with 15.


If we consider the first tree with 10 as the root then the overall
cost = 10 * 1 + 30 * 2 = 70

If we consider the second tree with 20 as the root then overall cost =
10 * 2 + 30 * 1 = 50

Hence the structure of the tree affects the retrieval performance.


## Dynamic Programming Approach
The  naive approach for solving this problem is to create all the 
possible trees and calculate their costs. Then select the tree with the 
minimum cost. This program will have a factorial time complexity and 
the same sub-problems will be recomputed. Hence we attempt to solve this 
program using dynamic programming.



## Parallelize Solution 





## Running the Program
To run parallel_obst


javac parallel_obst.java,


java parallel_obst \<number of keys> \<number of threads>


example


java parallel_obst 5000 12


5000 - keys, 12- threads


This program requires serial_obst.java as well since after running it in parallel it will run it in serial.
The program will calculate the cost in parallel and then calculate the cost in serial on the same frequencies
To run serial_obst.java


javac serial_obst.java


java serial_obst \<number of keys>


serial_obst will only print the total cost. 
