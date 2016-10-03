University of Victoria
Fall 2016
CSC 360 Assignment 1
Michael Reiter
V00831568

--------------------------------------------------------------------------

Instructions for Building and Running (for use in a Unix environment)

1. To compile my code, simply execute `make` from the terminal.
2. To run the executable, execute `./PMan` from the terminal.

--------------------------------------------------------------------------

Usage

This prompt supports 6 commands:

1. bg <cmd>: starts <cmd> running in the background
2. bgkill <pid>: sends the TERM signal to the process with process id <pid> (terminates the process)
3. bgstop <pid>: sends the STOP signal to the process with process id <pid> (stops the process)
4. bgstart <pid>: sends the CONT signal to the process with process id <pid> (restarts the process)
5. bglist: lists all processes that have been started through the prompt
6. pstat <pid>: lists comm, state, utime, stime, rss, voluntary_ctxt_switches and nonvoluntary_ctxt_switches for the process with process id <pid>
