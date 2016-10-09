Zach Martin

README:
LinearSim is a one dimensional simulation similar to CFD. LinearSim Utilizes fork, exec, and pipe system calls. LinearSim has a line of cells (no grid or 3-D arrangement). At each end of the line there is a cell with a fixed value. All the interior cells (those not on the ends of the line) will start with a value of 0. LinearSim loops through some number of time iterations, and on each iteration all interior cells will get the temperature from its two neighboring cells for time period n, and average it to produce a new cell temperature for time period n+1.

For this project there will be a different process for each cell plus a main process to set up all of the cells with the necessary pipes between neighboring cells. The main process is one program and the cell processes is a different program. The main program is the parent process and the cell program is its child processes.

The Cell Program:
Cell takes commandline arguments of the following forms, where N represents a non-negative integer value and X represents a double value. Note that there is no space between the command's letter and its number.

SN: Simulate from time 0 to time N, inclusive.
ON: Transmit state to another program, via a pipe on file descriptor N.
IN: Accept state from another program, via a pipe on file descriptor N.
VX: Assume a fixed value of X (do not average neighboring values).
DN: Use N as your unique ID number when reporting state on pipes.
For example, here is one possible Cell command for the leftmost interior cell base on the assumption that out pipes to the next cell and to the parent are on fds 3 and 4, and in pipes are on fds 5 and 6.
Cell S5 O3 O4 I5 I6 D1

LinearSim: The Parent Program
This is the program that sets up the line of cells. It takes commandline arguments of the following forms, where N represents a non-negative integer value and X represents a double value. Note that there is no space between the command's letter and its number.

CN: Use N cells in the simulation (will be between 1 and 50, inclusive).
SN: Simulate from time 0 to time N, inclusive.
LX: Use X as the fixed value of the left-most cell.
RX: Use X as the fixed value of the right-most cell (when there is one).

The LinearSim program assumes the arguments are well formed and simply ignores any that do not follow the formats described above. If there are extra arguments it uses the value of the first one encountered. Here are a few of examples of well-formed invocations of LinearSim. 
LinearSim C7 S11 L43.5 R-76.3421
LinearSim R99.9 C6 L21.1 S50
LinearSim R7 L3 S1 C2
LinearSim R7.11 L3.25 S7 C4 C3 C2 S6 X4 Z1 XYZ TUV R32.1 L11.1
LinearSim C5 S3
