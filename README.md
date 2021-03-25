## Quasi-Command Line Shell
The main goal of this quasi-command line shell is to allow users to effectively operate and control machine using simple, interactive methods by typing commands with a keyboard instead of controlling graphical user interfaces (GUIs).

## Table of contents
* [General Information](#general-information)
* [File Description](#file-description)
* [Screenshots](#screen-shots)
* [Code Examples](#code-examples)
* [Contact](#contact)

## General Information

The key systms programming topics covered in this project:

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

## Screen Shots
This screenshot is to show the output of server and several clients mid-chat.

The the server umn_server is started in the leftside large terminal and shows the output about current process.

There are 3 clients who log into the server and specfify their name to be Bruce, Mike and Clark.

![Example screenshot](./screenshot.png)

## Code Examples
To build the executable program bl_server and bl_client, run:
```bash
make
```
which will give the following results:
```bash
gcc -Wall -g  -c bl_server.c
gcc -Wall -g  -c server_funcs.c
gcc -Wall -g  -c util.c
gcc -Wall -g  -o bl_server  bl_server.o server_funcs.o  util.o
bl_server is ready
gcc -Wall -g  -c bl_client.c
gcc -Wall -g  -c simpio.c
gcc -Wall -g  -o bl_client bl_client.o simpio.o util.o -lpthread 
bl_client is ready
```
Run the command: 
```bash
./bl_server <server_name>
```
for example, 
```bash
./bl_server umn_server
```
The umn_server will start a chat room with following output
```bash
LOG: BEGIN: server_start()
LOG: END: server_start()
LOG: BEGIN: server_check_sources()
LOG: poll()'ing to check 1 input sources
```
Run the command: 
```bash
./bl_client <server_name> <client_name>
```
for example, 
```bash
./bl_client umn_server Bruce
```
The umn_client will join the chat room. The server side will give the following output
```bash
LOG: poll() completed with return value 1
LOG: join_ready = 1
LOG: END: server_check_sources()
LOG: BEGIN: server_handle_join()
LOG: join request for new client 'Bruce'
LOG: BEGIN: server_add_client()
LOG: END: server_add_client()
LOG: END: server_handle_join()
LOG: BEGIN: server_check_sources()
LOG: poll()'ing to check 2 input sources
```

The client side will give the following output
```bash
-- Bruce JOINED --
Bruce>>
```
Using the same method, more clients could join the same server. The client can type the intput on the keyboard and send to the server, and the server will broadcast the message to all the other clients.
 
## Contact
Created by Buyun Liang [liang664@umn.edu] (https://www.linkedin.com/in/buyun-liang/) and Rick Wang - feel free to contact me if you have any questions!
