To run parallel_obst
javac parallel_obst.java,
java parallel_obst <number of keys> <number of threads>
example
java parallel_obst 5000 12
5000 - keys, 12- threads
This program requires serial_obst.java as well since after running it in parallel it will run it in serial.
The program will calculate the cost in parallel and then calculate the cost in serial on the same frequencies
To run serial_obst.java
javac serial_obst.java
java serial_obst <number of keys>
serial_obst will only print the total cost. 
