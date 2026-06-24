/*******************************************************************
 * File: miniShell.c
 * Author: Tanika Goyal
 * Project: MiniShell
 * Description:
 * A Unix-like shell implemented in C using POSIX system calls.
 * Supports built-in commands, process creation, command execution,
 * and I/O redirection.
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUILTIN_COMMANDS 5

char PWD[1024];
char PATH[1024];

char * builtin[] = {"cd", "exit", "help", "pwd", "echo"};

int shell_cd(char ** args){
	if (args[1] == NULL){
		fprintf(stderr, "minsh: one argument required\n");
	}
	else if (chdir(args[1]) < 0){
		perror("msh");
	}
	getcwd(PWD, sizeof(PWD));	// Update present working directory
	return 1;
}

int shell_exit(char ** args){
	return 0;
}

int shell_help(char ** args){
	printf("\nMiniShell - A Unix-like shell written in C.\n");
	printf("\nCommands implemented: ");
	printf("\n\t- help");
	printf("\n\t- exit");
	printf("\n\t- cd dir");
	printf("\n\t- pwd");
	printf("\n\t- echo [string to echo]");
	printf("\n\t- clear");
	printf("\n\t- ls [-ail] [dir1 dir2 ...]");
	printf("\n\t- cp source target (or) cp file1 [file2 ...] dir");
	printf("\n\t- mv source target (or) mv file1 [file2 ...] dir");
	printf("\n\t- rm file1 [file2 ...]");
	printf("\n\t- mkdir dir1 [dir2 ...]");
	printf("\n\t- rmdir dir1 [dir2 ...]");
	printf("\n\t- ln [-s] source target");
	printf("\n\t- cat [file1 file2 ...]");
	printf("\n\n");
	printf("Other features : ");
	printf("\n\t* Input, Output and Error Redirection (<, <<, >, >>, 2>, 2>> respectively)  : ");
	printf("\n\t* Example: ls -i >> outfile 2> errfile [Space mandatory around redirection operators!]");
	printf("\n\n");
	return 1;
}

int shell_pwd(char ** args){
	printf("%s\n", PWD);
	return 1;
}

int shell_echo(char ** args){
	int i = 1;
	while (1){
		if (args[i] == NULL){
			break;
		}
		printf("%s ", args[i]);
		i++;
	}
	printf("\n");
	return 1;
}

int (* builtin_function[]) (char **) = {
	&shell_cd,
	&shell_exit,
	&shell_help,
	&shell_pwd,
	&shell_echo
};


char ** split_command_line(char * command){
        int position = 0;
        int no_of_tokens = 64;
        char ** tokens = malloc(sizeof(char *) * no_of_tokens);
        char delim[2] = " ";

        char * token = strtok(command, delim);
        while (token != NULL){
                tokens[position] = token;
                position++;
                token = strtok(NULL, delim);
        }
        tokens[position] = NULL;
        return tokens;
}

char * read_command_line(void){
        int position = 0;
        int buf_size = 1024;
        char * command = (char *)malloc(sizeof(char) * 1024);
        char c;

        c = getchar();
        while (c != EOF && c != '\n'){
                command[position] = c;

                // Reallocate buffer as and when needed
                if (position >= buf_size){
                        buf_size += 64;
                        command = realloc(command, buf_size);
                }

                position++;
                c = getchar();
        }
		command[position] = '\0';
        return command;
}

int start_process(char ** args){

        int status;
        pid_t pid, wpid;

        pid = fork();

        if (pid == 0){  // It's the child process
		
		// execvp searches PATH and replaces the child process image
		if (execvp(args[0], args) == -1){
			perror("msh");
	}

		exit(EXIT_FAILURE);
        }
        else if (pid < 0){      // Error in forking
		perror("msh");
        }
        else{           // It's the parent process
                do{
                        // Wait until the child exits or is terminated by a signal
                        wpid = waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }

        return 1;
}

int shell_execute(char ** args){

	if (args[0] == NULL){	// Empty command
		return 1;
	}

	// Copy the current Standard Input and Output file descriptors
	// so they can be restored after executing the current command
	int std_in, std_out, std_err;
	std_in = dup(0);
	std_out = dup(1);
	std_err = dup(2);

	// Check if redirection operators are present
	int i = 1;

	while ( args[i] != NULL ){
		if ( strcmp( args[i], "<" ) == 0 ){	// Input redirection
			int inp = open( args[i+1], O_RDONLY );
			if ( inp < 0 ){
				perror("msh");
				return 1;
			}

			if ( dup2(inp, 0) < 0 ){
				perror("msh");
				return 1;
			}
			close(inp);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;
		}
		else if ( strcmp( args[i], "<<" ) == 0 ){	// Input redirection
			int inp = open( args[i+1], O_RDONLY );
			if ( inp < 0 ){

				perror("msh");
				return 1;
			}

			if ( dup2(inp, 0) < 0 ){
				perror("msh");
				return 1;
			}
			close(inp);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;
		}
		else if( strcmp( args[i], ">") == 0 ){	// Output redirection

			int out = open( args[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0755 );
			if ( out < 0 ){
				perror("msh");
				return 1;
			}

			if ( dup2(out, 1) < 0 ){
				perror("msh");
				return 1;
			}
			close(out);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;
		}
		else if( strcmp( args[i], ">>") == 0 ){	// Output redirection (append)
			int out = open( args[i+1], O_WRONLY | O_APPEND | O_CREAT, 0755 );
			if ( out < 0 ){
				perror("msh");
				return 1;
			}

			if ( dup2(out, 1) < 0 ){
				perror("msh");
				return 1;

			}
			close(out);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;
		}
		else if( strcmp( args[i], "2>") == 0 ){	// Error redirection
			int err = open( args[i+1], O_WRONLY | O_CREAT, 0755 );
			if ( err < 0 ){
				perror("msh");
				return 1;
			}

			if ( dup2(err, 2) < 0 ){
				perror("msh");
				return 1;
			}
			close(err);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;
		}
		else if( strcmp( args[i], "2>>") == 0 ){	// Error redirection
			int err = open( args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0755 );

			if ( err < 0 ){
				perror("msh");
				return 1;
			}

			if ( dup2(err, 2) < 0 ){
				perror("msh");
				return 1;
			}
			close(err);
			args[i] = NULL;
			args[i+1] = NULL;
			i += 2;

		}
		else{
			i++;
		}
	}

	// If the command is a built-in command, execute that function
	for(i = 0 ; i < BUILTIN_COMMANDS ; i++){
		if ( strcmp(args[0], builtin[i]) == 0 ){
			int ret_status = (* builtin_function[i])(args);
			
			// Restore the Standard Input and Output file descriptors
			dup2(std_in, 0);
			dup2(std_out, 1);
			dup2(std_err, 2);

			return ret_status;
		}
	}

	// For other commands, execute a child process
	int ret_status = start_process(args);

	// Restore the Standard Input and Output file descriptors
	dup2(std_in, 0);
	dup2(std_out, 1);
	dup2(std_err, 2);

	return ret_status;
}

void shell_loop(void){

	int status = shell_help(NULL);

        char * command_line;
        char ** arguments;
	status = 1;

        while (status){
                printf("msh> ");
                command_line = read_command_line();
		if ( strcmp(command_line, "") == 0 ){
			continue;
		}
                arguments = split_command_line(command_line);
                status = shell_execute(arguments);
        }
}

int main(int argc, char ** argv){
	getcwd(PWD, sizeof(PWD));
	strcpy(PATH, PWD);
	strcat(PATH, "/cmds/");

        shell_loop();
		
        return 0;
}
