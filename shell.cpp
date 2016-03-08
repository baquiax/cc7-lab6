#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_ARGS 40
#define MAX_HISTORY 10

char history[MAX_HISTORY][MAX_LINE] ;
char stackCounter; 
 
void pushToHistory (char string[]) {
	if (stackCounter >= MAX_HISTORY) {
		//Move the stack
		for(char i = 1; i < MAX_HISTORY; i++) {
			strcpy(history[i - 1], history[i]);
		}
		stackCounter = MAX_HISTORY - 1;
	}
	strcpy(history[stackCounter], string);    
	stackCounter++;
}

void printPointerArray(char *args[], char len) {
	printf("printArray > ARRAY LEN: %d\n", len);	
	for (char i = 0; i < len; i++) {
		printf("[%d] %s\n", i, args[i]);			
	}
}

void showHistory(int signum) {
    if (signum == SIGINT) {
        char message[] = "\n\nCurrent history: \n\n";
        write(STDOUT_FILENO, message, strlen(message));
        for (char i = 0; i < MAX_HISTORY; i++) {
            if (strlen(history[i])) {			
                char command[85];
                sprintf(command, "%d %s\n",i + 1, history[i]);
                write(STDOUT_FILENO, command, strlen(command));
            } else {
                break;
            }
        }   
    }
}

int findCommand(char initWith[]) {
    for (char i = stackCounter - 1; i >= 0; i--) {
        if (strlen(history[i])) {
            if(strncmp(history[i], initWith, strlen(initWith)) == 0) {
                return i;  
            }		
        } else {
            break;
        }
    }
    return -1;
}
void emptyPointerArray(char *args[], char len) {
	for (char i = 0; i < len; i++) {
		free(args[i]);
		args[i] = NULL;
	}
}

void splitStringBySpace(char *args[], char inputBuffer[], int *elements) {
	char currentIndex = -1;
	char size = 0;	
	char lenght = strlen(inputBuffer);    
	for (char i = 0 ; i < lenght ; i++) {
		if (inputBuffer[i] == ' ') {		
			if (size > 0) {	
				args[++currentIndex] = (char *) malloc ((size + 1) * sizeof(char));
				for (char j = 0; j < size; j++) {
					*(args[currentIndex] + j) = inputBuffer[i - size + j];						
				}
				*(args[currentIndex] + size) = '\0';
				size = 0;				
			}
		} else {		
			size++;
		}
	}	
	if (size > 0) {
		args[++currentIndex] = (char *) malloc ((size + 1) * sizeof(char));
		for (char j = 0; j < size; j++) {
			*(args[currentIndex] + j) = inputBuffer[lenght - size + j];	
		}
		*(args[currentIndex] + size) = '\0';
	}
	*(elements) = currentIndex + 1;
}

void setup(char inputBuffer[], char *args[], int *background) {    
	emptyPointerArray(args, MAX_ARGS);    
    int len = read(STDIN_FILENO, inputBuffer, MAX_LINE);
    //Remove EOL            
    inputBuffer[--len] = '\0';
    
	int nElements;
	splitStringBySpace(args, inputBuffer, &nElements);
	if (nElements > 0 && *(args[nElements - 1]) == '&') {
		args[nElements - 1] = NULL;
		*background = 1;
	}	

	if (nElements > 0) {
		if (strcmp(args[0],"exit") == 0) {
			exit(0);		
		} else if (strcmp(args[0],"r") == 0) {
			if (stackCounter > 0) {
				if (nElements > 1) {
					int i = findCommand(args[1]);                                        
                    printf("%s\n", history[i]);
                    emptyPointerArray(args, MAX_ARGS);
					splitStringBySpace(args, history[i], &nElements);
				} else {
                    printf("%s\n", history[stackCounter - 1]);						
					splitStringBySpace(args, history[stackCounter - 1], &nElements);
				}
			}
		}  else {
            pushToHistory(inputBuffer);
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
	
    struct sigaction handler;
    handler.sa_flags = 0;
	handler.sa_handler = &showHistory;
	sigaction(SIGINT, &handler, NULL);

	while(1) {
		background = 0;
		printf("%s","baquiax@baquiax:$ \n");                   
		setup(inputBuffer, args, &background);                
        if (strlen(inputBuffer) < 1){
            continue;
        }         
		pid_t pid = fork();
        
		if (pid < 0) {
			printf("%s\n", "An error has been happened!");
		} else if (pid == 0) {
			execvp(args[0], args);
			exit(0);
		} else {
			if (!background) {			
				wait(NULL);								
			}			
		}
	}
}

