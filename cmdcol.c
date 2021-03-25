#include "commando.h"
// cmdcol.c: functions related to cmdcol_t collections of commands.
int count=0;
void cmdcol_add(cmdcol_t *col, cmd_t *cmd)
{
    if(count>=MAX_CMDS)
    {
        printf("Array Full\n");
        exit(1);
    }
    col->cmd[count]=cmd;
    count++;
    col->size=count;
    
}
// Add the given cmd to the col structure. Update the cmd[] array and
// size field. Report an error if adding would cause size to exceed
// MAX_CMDS, the maximum number commands supported.

void cmdcol_print(cmdcol_t *col)
{
    printf("JOB  #PID      STAT   STR_STAT OUTB COMMAND\n");
    int i=0;
    for(i=0; i<col->size; i++)
    {
        printf("%d    #%d     %4d    %5s %4d ", i, col->cmd[i]->pid, col->cmd[i]->status, col->cmd[i]->str_status, col->cmd[i]->output_size);
        int x=0;
        while(col->cmd[i]->argv[x]!=NULL)
        {
		if ( strncmp(col->cmd[i]->argv[x],"-std=gnu99",10) ){ 
            		printf("%s ", col->cmd[i]->argv[x]);
		} // Don't show the -std=gnu99. My operating system require the use of -std=gnu99 during tests.
            x++;
        }
        printf("\n");
    }
}
// Print all cmd elements in the given col structure.  The format of
// the table is
//
// JOB  #PID      STAT   STR_STAT OUTB COMMAND
// 0    #17434       0    EXIT(0) 2239 ls -l -a -F 
// 1    #17435       0    EXIT(0) 3936 gcc --help 
// 2    #17436      -1        RUN   -1 sleep 2 
// 3    #17437       0    EXIT(0)  921 cat Makefile 
// 
// Widths of the fields and justification are as follows
// 
// JOB  #PID      STAT   STR_STAT OUTB COMMAND
// 1234 #12345678 1234 1234567890 1234 Remaining
// left  left    right      right rigt left
// int   int       int     string  int string
//
// The final field should be the contents of cmd->argv[] with a space
// between each element of the array.

void cmdcol_update_state(cmdcol_t *col, int block)
{
    int i;
    for(i=0; i<col->size; i++)
    {
        cmd_update_state(col->cmd[i],block);
    }
}
// Update each cmd in col by calling cmd_update_state() which is also
// passed the block argument (either NOBLOCK or DOBLOCK) 

void cmdcol_freeall(cmdcol_t *col)
{
    int i;
    for(i=0; i<col->size; i++)
    {
        cmd_free(col->cmd[i]);
    }
    col->size=0;
    count=0;
}
// Call cmd_free() on all of the constituent cmd_t's.
