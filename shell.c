#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>



int get_tokens(char command_1[], char * args[], int * num_args) {
	
	int counter = 0;
	char * token = strtok(command_1, " ");
	while(token != NULL){
		args[counter] = token;
		token = strtok(NULL, " ");
		counter++;
	}
	* num_args = counter;

	return 0;
}


//________________________________________________________________________________________________________________________________________


void my_echo(char command[]) {
	int count = 5;
	for (count; count < strlen(command); count++)
		printf("%c", command[count]);

	printf("\n");

}

//________________________________________________________________________________________________________________________________________

int my_pwd(int num_args, char path[]) {
	if (num_args != 1){
		printf("invalid usage of pwd!\n");
		return -1;
	}

	getcwd(path, 200);
	if (getcwd(path, 200) == NULL) {
		printf("could not get the current working directory \n");
		return -1;
	}
	return 0;
}
//________________________________________________________________________________________________________________________________________

int my_cd(char second_arg[]){
	int ret_cd = chdir(second_arg);
	if (ret_cd != 0) {
		printf("could not reach the path\n");
		return -1;
	}
	return 0;
}
//________________________________________________________________________________________________________________________________________



int my_exit (int num_args) {
	if (num_args != 1){
		printf("invalid usage of exit!\n");
		return -1;
	}else
		return 0;
}


//________________________________________________________________________________________________________________________________________



int main(int argc, char *argv[]) {
	
	int num_args = 0;
	unsigned char path[200];
	char ** buf= (char**)malloc(20 * sizeof(char*));
	char * command;
	int result = 0, size_command = 0;
	size_t n = 0;
	if (buf == NULL)
		printf("memory allocation faild!\n");
	

	
	while(1) {
	
		my_pwd(1, path);
		printf("Demo@linux-<<new_shell>>:%s$ ", path);
		size_command = getline(&command, &n, stdin);
		command[size_command - 1] = '\0';
		char * cpy_command = strdup(command);
		result = get_tokens(cpy_command, buf, &num_args);
		
		
		
		if (strlen(command) == 0)
			continue;
//________________________________________________________________________________________________________________________________________
		
		if (strcmp(buf[0], "echo") == 0) {
			my_echo(command);
		}
//________________________________________________________________________________________________________________________________________
		
		else if(strcmp(buf[0], "pwd") == 0) {
			int ret_pwd = my_pwd(num_args, path);
			if(ret_pwd == -1)
				continue;
			printf("%s\n", path);
		}
//________________________________________________________________________________________________________________________________________
		
		else if(strcmp(buf[0], "cd") == 0) {
			my_cd(buf[1]);
		}
//________________________________________________________________________________________________________________________________________
		
		else if(strcmp(buf[0], "exit") == 0) {
			int ret_exit = my_exit(num_args);
			if (ret_exit == -1)
				continue;
			else
				break;
		}
//________________________________________________________________________________________________________________________________________

		else{
		
			int ret_pid = fork();
			if (ret_pid == -1)
				printf("fork does not seccedded!\n");
			else if (ret_pid > 0) {
				int status;
				wait(&status);
			}else{
				char * new_argv[] = {NULL};
		                execvp(command,new_argv);
		                printf("Exec failed!\n");
		                return -1;
			}
		}
	}
	free(buf);
	printf("Good bye!\n");
	return 0;	
}
	
	



