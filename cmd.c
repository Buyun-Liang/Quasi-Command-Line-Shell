
// cmd.c: functions related the cmd_t struct abstracting a
// command. Most functions maninpulate cmd_t structs.
#include "commando.h"
cmd_t *cmd_new(char *argv[])
{
    int x=0;
    cmd_t *new;
    new=(cmd_t *)calloc(1,sizeof(cmd_t));
    if (new==NULL)
    {
        printf("memory error\n");
        exit(1);
    }
    while(argv[x]!=NULL)
    {
        char *p=strdup(argv[x]);
        new->argv[x]=p;
        x++;
    }
    new->argv[x+1]=NULL;
    strcpy(new->name,new->argv[0]);
    snprintf(new->str_status,5,"%s","INIT");
    new->finished=0;
    new->pid=-1;
    new->status=-1;
    new->output=NULL;
    new->output_size=-1;
    return new;
}
// Allocates a new cmd_t with the given argv[] array. Makes string
// copies of each of the strings contained within argv[] using
// strdup() as they likely come from a source that will be
// altered. Ensures that cmd->argv[] is ended with NULL. Sets the name
// field to be the argv[0]. Sets finished to 0 (not finished yet). Set
// str_status to be "INIT" using snprintf(). Initializes the remaining
// fields to obvious default values such as -1s, and NULLs.

void cmd_free(cmd_t *cmd)
{
    int x=0;
    while(cmd->argv[x]!=NULL)
    {
        free(cmd->argv[x]);
        x++;
    }
    if(cmd->output!=NULL)
    {
        free(cmd->output);
    }
    free(cmd);
}
// Deallocates a cmd structure. Deallocates the strings in the argv[]
// array. Also deallocats the output buffer if it is not
// NULL. Finally, deallocates cmd itself.

void cmd_start(cmd_t *cmd)
{
    snprintf(cmd->str_status,4,"%s","RUN");
    int result=pipe(cmd->out_pipe);
    if(result<0)
    {
        printf("pipe error\n");
        exit(1);
    }
    pid_t child=fork();
    if(child<0)
    {
        printf("fork error\n");
        exit(1);
    }
    if(child==0)
    {
        close(cmd->out_pipe[PREAD]);
        dup2(cmd->out_pipe[PWRITE], STDOUT_FILENO);
        execvp(cmd->name,cmd->argv);
    }
    else
    {
        close(cmd->out_pipe[PWRITE]);
        cmd->pid=child;
    }
}
// Forks a process and starts executes command in cmd in the process.
// Changes the str_status field to "RUN" using snprintf().  Creates a
// pipe for out_pipe to capture standard output.  In the parent
// process, ensures that the pid field is set to the child PID. In the
// child process, directs standard output to the pipe using the dup2()
// command. For both parent and child, ensures that unused file
// descriptors for the pipe are closed (write in the parent, read in
// the child).

void cmd_update_state(cmd_t *cmd, int block)
{
    if(cmd->finished!=1)
    {
        int result=waitpid(cmd->pid,&cmd->status,block);
        if(result==cmd->pid)
        {
            if(WIFEXITED(cmd->status)!=0)
            {
                cmd->status=WEXITSTATUS(cmd->status);
                snprintf(cmd->str_status,8,"EXIT(%d)",cmd->status);
                cmd->finished=1;
                cmd_fetch_output(cmd);
                printf("@!!! %s[#%d]: EXIT(%d)\n",cmd->name,cmd->pid,cmd->status);
            }
        }

    }
}
// If the finished flag is 1, does nothing. Otherwise, updates the
// state of cmd.  Uses waitpid() and the pid field of command to wait
// selectively for the given process. Passes block (one of DOBLOCK or
// NOBLOCK) to waitpid() to cause either non-blocking or blocking
// waits.  Uses the macro WIFEXITED to check the returned status for
// whether the command has exited. If so, sets the finished field to 1
// and sets the cmd->status field to the exit status of the cmd using
// the WEXITSTATUS macro. Calls cmd_fetch_output() to fill up the
// output buffer for later printing.
//
// When a command finishes (the first time), prints a status update
// message of the form
//
// @!!! ls[#17331]: EXIT(0)
//
// which includes the command name, PID, and exit status.

char *read_all(int fd, int *nread)
{
    int size=1024;
    int total=0;
    char *buff=malloc(sizeof(char)*size);
    while(1)
    {
        if(total>=size)
        {
            size = size * 2;                               // double in size
            buff = realloc(buff, size);
        }
        int c=read(fd,buff+total,size-total);       
        if(c==0)
        {
            break;
        }
        total+=c;
    }
    buff[total]='\0';
    *nread=total;
    return buff;
}
// Reads all input from the open file descriptor fd. Stores the
// results in a dynamically allocated buffer which may need to grow as
// more data is read.  Uses an efficient growth scheme such as
// doubling the size of the buffer when additional space is
// needed. Uses realloc() for resizing.  When no data is left in fd,
// sets the integer pointed to by nread to the number of bytes read
// and return a pointer to the allocated buffer. Ensures the return
// string is null-terminated. Does not call close() on the fd as this
// is done elsewhere.

void cmd_fetch_output(cmd_t *cmd)
{
    if(cmd->finished==0)
    {
        printf("%s[#%d] not finished yet\n",cmd->name,cmd->pid);
    }
    else
    {
        cmd->output=read_all(cmd->out_pipe[PREAD],&cmd->output_size);
        close(cmd->out_pipe[PREAD]);
    }
    
}
// If cmd->finished is zero, prints an error message with the format
// 
// ls[#12341] not finished yet
// 
// Otherwise retrieves output from the cmd->out_pipe and fills
// cmd->output setting cmd->output_size to number of bytes in
// output. Makes use of read_all() to efficiently capture
// output. Closes the pipe associated with the command after reading
// all input.

void cmd_print_output(cmd_t *cmd)
{
    if(cmd->output==NULL)
    {
        printf("%s[#%d] : output not ready\n",cmd->name,cmd->pid);
    }
    else
    {
        write(STDOUT_FILENO,cmd->output,cmd->output_size);
    }
    
}
// Prints the output of the cmd contained in the output field if it is
// non-null. Prints the error message
// 
// ls[#17251] : output not ready
//
// if output is NULL. The message includes the command name and PID.

///////////////////////////////////////////////////////////////////////////////////////////////////

