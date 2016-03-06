#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_ARGS 40

void printArray(char *args[], char len) {
	printf("printArray > ARRAY LEN: %d\n", len);	
	for (char i = 0; i < len; i++) {
		printf("[%d] %s\n", i, args[i]);			
	}
}

void emptyPointerArray(char *args[], char len) {
	for (char i = 0; i < len; i++) {
		free(args[i]);
		args[i] = NULL;
	}
}

void setup(char inputBuffer[], char *args[], int *background) {
	emptyPointerArray(args, MAX_ARGS);
	int lenght;
	lenght = read(STDIN_FILENO, inputBuffer, MAX_LINE);	
	char argIndex = 0;
	char size = 0;	
	for (char i = 0 ; i < lenght - 1; i++) {
		if (inputBuffer[i] == ' ') {		
			if (size > 0) {	
				args[argIndex] = (char *) malloc ((size + 1) * sizeof(char));
				for (char j = 0; j < size; j++) {
					*(args[argIndex] + j) = inputBuffer[i - size + j];						
				}
				*(args[argIndex++] + size) = '\0';
				size = 0;				
			}
		} else {		
			size++;
		}
	}	
	if (size > 0) {
		args[argIndex] = (char *) malloc ((size + 1) * sizeof(char));
		for (char j = 0; j < size; j++) {
			*(args[argIndex] + j) = inputBuffer[lenght - 1 - size + j];	
		}
		*(args[argIndex] + size) = '\0';
		
		if (*(args[argIndex]) == '&') {
			args[argIndex] = NULL;
			*background = 1;
		}	
	}
}		

int main() {
	printf("Hello, that is the Xell V.1\n");
	printf("Galileo University, 2016.\n");
	printf("===========================\n");

	char inputBuffer[MAX_LINE];
	int background;
	char *args[MAX_ARGS] = {NULL};
	while(1) {
		background = 0;
		printf("%s","baquiax@baquiax:$ \n");
		setup(inputBuffer, args, &background);		
		pid_t pid = fork();
		if (pid < 0) {
			printf("%s\n", "An error has been happened!");
		} else if (pid == 0) {
			execvp(args[0], args);
		} else {
			if (!background) {			
				wait(NULL);								
			}
		}
	}
}

