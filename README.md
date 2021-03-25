## Quasi-Command Line Shell
The main goal of this quasi-command line shell is to allow users to effectively operate and control machine using simple, interactive methods by typing commands with a keyboard instead of controlling graphical user interfaces (GUIs).

## Table of contents
* [General Information](#general-information)
* [File Description](#file-description) 
* [Code Examples](#code-examples)
* [Contact](#contact)

## General Information

The key systems programming topics covered in this project:

1.Basic C Memory Discipline: Various strings and structs are allocated and de-allocated during execution which will require the utilization of memory tool Valgrind.

2.fork() and exec(): The shell will execute an command (external program, not built-in) by spawning a child process to execute the new program.

3.Pipes, dup2(), read(): The output from child process will not be immediately printed, instead it is redirected into pipes and then retrieved by using command.

4.wait() and waitpid(), blocking and nonblocking: The shell will check the status of child process occasionally, as child processes will take a while to complete.

## File Description
*Makefile* is used for building project and running tests;

*commando.c* is the main function for the interactive shell;

*cmd.c* contains the functions to deal with the cmd_t struct;

*cmdcol.c* contains the functions to deal with the cmdcol_t struct;

*commando.h* is the header file which contains required structs, defines, and prototypes;

*util.c* contains utility methods for creating argv[] arrays and pausing execution; 
 

## Code Examples
To build the executable program commando, run:
```bash
make
```
which will give the following results:
```bash
gcc -Wall -g   -c commando.c
gcc -Wall -g   -c cmd.c
gcc -Wall -g   -c cmdcol.c
gcc -Wall -g   -c util.c
gcc -Wall -g   -o commando  commando.o  cmd.o cmdcol.o util.o
commando is ready
```

Run the command to start the shell: 
```bash
./commando
```

The shell will give the propmt:
```bash
@>
```

Type the command in the shell: 
```bash
@> help
```
 
The shell will output available built-ins:
```bash
COMMANDO COMMANDS
help               : show this message
exit               : exit the program
list               : list all jobs that have been started giving information on each
pause nanos secs   : pause for the given number of nanseconds and seconds
output-for int     : print the output for given job number
output-all         : print output for all jobs
wait-for int       : wait until the given job number finishes
wait-all           : wait for all jobs to finish
command arg1 ...   : non-built-in is run as a job
```

Type the command in the shell to present the running job: 
```bash
@> list
```

Since there is no child process forked, the shell output empty job information:
```bash
JOB  #PID      STAT   STR_STAT OUTB COMMAND
```

Type the command in the shell to run ls on the test-data/ directory and present the running job: 
```bash
@> ls test-data/
@> list
```

The shell output job information of *ls test-data/*:
```bash
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    #990152       -1      RUN   -1 ls test-data/ 
@!!! ls[#990152]: EXIT(0)
```

list agin. The shell shows the exit status and output size:
```bash
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    #990152        0    EXIT(0)   89 ls test-data/ 
```
show output for job 0 (ls). The shell returns the actual output of ls.
```bash
@> output-for 0
@<<< Output for ls[#990152] (89 bytes):
----------------------------------------
3K.txt
gettysburg.txt
print_args
print_args.c
quote.txt
README
sleep_print
sleep_print.c
----------------------------------------
```

Run another child job and check the job status (JOB 0 is finshed while JOB 1 is running):
```bash
@> ls -l test-data/
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    #990152        0    EXIT(0)   89 ls test-data/ 
1    #991176       -1      RUN   -1 ls -l test-data/ 
@!!! ls[#991176]: EXIT(0)
```

Run list to show two completed jobs
```bash
@> list
JOB  #PID      STAT   STR_STAT OUTB COMMAND
0    #990152        0    EXIT(0)   89 ls test-data/ 
1    #991176        0    EXIT(0)  530 ls -l test-data/ 
```

Show output for JOB 1:
```bash
@> output-for 1
@<<< Output for ls[#991176] (530 bytes):
----------------------------------------
total 76
-rwx------ 1 liang664 CSEL-student 13893 Mar 25 11:11 3K.txt
-rwx------ 1 liang664 CSEL-student  1511 Mar 25 11:11 gettysburg.txt
-rwx------ 1 liang664 CSEL-student  8304 Mar 25 11:11 print_args
-rwx------ 1 liang664 CSEL-student   218 Mar 25 11:11 print_args.c
-rwx------ 1 liang664 CSEL-student   125 Mar 25 11:11 quote.txt
-rwx------ 1 liang664 CSEL-student   184 Mar 25 11:11 README
-rwx------ 1 liang664 CSEL-student  8496 Mar 25 11:11 sleep_print
-rwx------ 1 liang664 CSEL-student   346 Mar 25 11:11 sleep_print.c
----------------------------------------
```

Show output for JOB 0, which is permanently available:
```bash
@> output-for 0
@<<< Output for ls[#990152] (89 bytes):
----------------------------------------
3K.txt
gettysburg.txt
print_args
print_args.c
quote.txt
README
sleep_print
sleep_print.c
----------------------------------------
```

Exit the shell:
```bash
@> exit
```

 
## Contact
Created by Buyun Liang [liang664@umn.edu] (https://www.linkedin.com/in/buyun-liang/) and Rick Wang - feel free to contact me if you have any questions!
