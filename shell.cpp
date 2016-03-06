#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80
#define MAX_ARGS 40

void printArray(char *args[], char len) {
	printf("printArray > ARRAY LEN: %d\n", len);	
	for (char i = 0; i < len; i++) {
		printf("[%d] %s\n", i, args[i]);			
	}
}

void setup(char inputBuffer[], char *args[], int *background) {
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
		*(args[argIndex++] + size) = '\0';
	}
	if (background) {
		args[argIndex++] = "&";
	}
	printArray(args, MAX_ARGS);
	execvp(args[0], args);
}		

int main() {
	char inputBuffer[MAX_LINE];
	int background;
	char *args[MAX_ARGS] = {NULL};
	while(1) {			
		background = 1;
		printf("$-> ");
		setup(inputBuffer, args, &background);	
		printf("\n");
	}
}

