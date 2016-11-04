University of Victoria
Fall 2016
CSC 360 Assignment 2
Michael Reiter
V00831568

--------------------------------------------------------------------------

Instructions for Building and Running (for use in a Unix environment)

1. To compile my code, simply execute `make` from the terminal.
2. To run the executable, execute `./MFS <flow text file>` from the terminal.

--------------------------------------------------------------------------

The flows text file must be in the following format.

The first line contains the number of flows.
For each flow:
1. The first character specifies the unique number of the flow.
2. A colon immediately follows the unique number of the flow.
3. Immediately following is an integer that indicates the arrival time of the flow.
4. A comma immediately follows the previous number.
5. Immediately following is an integer that indicates the transmission time of the flow. 6. A comma immediately follows the previous number.
7. Immediately following is an integer that indicates the priority of the flow.
8. A newline ends a line.

A sample input is included (flows.txt).
