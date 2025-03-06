# CPU-Scheduling-Algorithms
Implemented many standard OS CPU scheduling algorithms in C++ using object oriented programming concepts and priority queue.

# Implementation Details 
- Created a `process` class 
- Storing the process in a priority queue or a vector according to scheduling algorithm
- Used operator overloading for sorting in the `ready_queue_compare` struct
- Used standard formulae for calculating completion time, turn around time, response time and waiting time in the `calc_attributes` function
- Using a seperate `calc_avg` function for calculating the averages for the attributes
- Printing the process table in the terminal using `print_table` function
- Also printing `Gantt chart` in the terminal using the `print_gantt_chart`
- Separate `vector<pair>` data structure for storing gantt chart
