//commando.c:  loops over input provided by the user either interactively or via standard input as is done in the
//automated tests.
//tie the basic low-level pieces from cmd.c and cmdcol.c into a usable application

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include "commando.h"


int main(int argc, char *argv[]) {
	
	char *echo_env = getenv("COMMANDO_ECHO");	// check the environment variable for echo
	int echo_env_flag=1;                          // random non-zero value for default ENV_ECHO 
	
	if(echo_env != NULL){
		echo_env_flag=0;			// set the flag to be zero for any ECHO environment variable                   
	}

	int echo_flag=1;                          // random non-zero value for default strcmp (--echo)

	if(argc > 1){                                                 // check input arguments (--echo)
		echo_flag = strncmp(argv[1],"--echo",10);   		
	}	

	cmdcol_t *cmdcol;
	cmdcol = (cmdcol_t *)calloc(1,sizeof(cmdcol_t));               // initialize the cmdcol

	setvbuf(stdout, NULL, _IONBF, 0); // Turn off output buffering

	while(1){

		printf("@> ");                    // print prompt
		char buffer[1024];
		char *input = fgets(buffer,MAX_LINE,stdin);         // read args from command line

		if( input == NULL  ){				// Press ctrl +D
			printf(" \n");
			printf("End of input\n");
			cmdcol_freeall(cmdcol);
		    	free(cmdcol);				// free all dynamically allocated memory.
			exit(0);
		}

		else if( *input == '\n' ){			// Press Enter
			printf("\n");
		}

		else {						// Press other things
			int ntok=-1;
			char *tokens[20];			// initialize ntok and tokens
			if(!echo_flag||!echo_env_flag){                            
				printf("%s",input);   		// echo
			}
		

			parse_into_tokens(input,tokens,&ntok);	// get argv from command line
	
			if( !strncmp(tokens[0],"exit",10) ){      //exit program
			cmdcol_freeall(cmdcol);			// free all dynamically allocated memory.
			free(cmdcol);
			exit(0);
			}

			else if( !strncmp(tokens[0],"help",10)  ){   //print built-in functions
				printf("COMMANDO COMMANDS\n");
				printf("help               : show this message\n");
				printf("exit               : exit the program\n");
				printf("list               : list all jobs that have been started giving information on each\n");
				printf("pause nanos secs   : pause for the given number of nanseconds and seconds\n");
				printf("output-for int     : print the output for given job number\n");
				printf("output-all         : print output for all jobs\n");
				printf("wait-for int       : wait until the given job number finishes\n");
				printf("wait-all           : wait for all jobs to finish\n");
				printf("command arg1 ...   : non-built-in is run as a job\n");
			}

			else if( !strncmp(tokens[0],"list",10) ){      //list all jobs that have been started giving information on each
				
				if(cmdcol!=NULL){
					cmdcol_print(cmdcol);	
				}	

				else{
					printf("memory error\n");      
					exit(1);
				}	
			}
			else if( !strncmp(tokens[0],"pause",10)  ){      //pause for the given number of nanseconds and seconds
				int nano = atoi(tokens[1]);
				int sec = atoi(tokens[2]);
				pause_for(nano,sec);
			}

			else if( !strncmp(tokens[0],"output-for",20)  ){      //print the output for given job number          
				int idx = atoi(tokens[1]);
				printf("@<<< Output for %s[#%d] (%d bytes):\n",cmdcol->cmd[idx]->name,cmdcol->cmd[idx]->pid,cmdcol->cmd[idx]->output_size );
				printf("----------------------------------------\n");
				cmd_print_output(cmdcol->cmd[ idx ]);
				printf("----------------------------------------\n");
			}

			else if( !strncmp(tokens[0],"output-all",20)  ){      //print output for all jobs
				int idx;
				for(idx=0;idx< cmdcol->size;idx++){
					printf("@<<< Output for %s[#%d] (%d bytes):\n",cmdcol->cmd[idx]->name,cmdcol->cmd[idx]->pid,cmdcol->cmd[idx]->output_size );
					printf("----------------------------------------\n");
					cmd_print_output(cmdcol->cmd[idx]);
					printf("----------------------------------------\n");
				}
			}
			else if( !strncmp(tokens[0],"wait-for",20)  ){      // wait until the given job number finishes                        
				int idx = atoi(tokens[1]);
				cmd_update_state(cmdcol->cmd[idx],DOBLOCK);
			}

			else if( !strncmp(tokens[0],"wait-all",20)   ){      // wait for all jobs to finish
				cmdcol_update_state(cmdcol, DOBLOCK);
			}

			else{	
				if( !strncmp(tokens[0],"gcc",20)   ){ 
					char ** ptr = tokens;
					ptr[ntok] = "-std=gnu99";
					ptr[ntok+1] = NULL;
				} 						// My operating system require the use of -std=gnu99 for gcc during tests.
	
				cmd_t *cmd = cmd_new(tokens);			// put the command line argv into cmd
				cmdcol_add(cmdcol, cmd );			// add new cmd into cmdcol
				cmd_start(cmd);					// execute the new command
					
			}

			
			cmdcol_update_state(cmdcol, NOBLOCK);			// update the status of all jobs at the end of each loop
		}
	}
}

