#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

// Check if a given string is a positive integer
int isInt(char *S){
        int len = strlen(S);

        // Check if first argument given is a digit
        if (len>0 && !isdigit(S[0])) return 0;

        // Check if all other chars are digits
        for (int i=1; i<len; i++){
                if(!isdigit(S[i])) return 0;
        }

        return 1;
}

// Print stars to given file
void printStars(FILE *file){
        for (int i = 0; i < 40; i++) {
                fprintf(file, "*");
                fflush(file);
                sleep(1);
        }
}

// Used to catch SIGUSR1 from scheduler
void signalHandler(int sig){
	// temp
}

int main(int argc, char *argv[]){
	pid_t sPID;
	
	// Check number of arguments
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <String: filename> <int: scheduler pid>\n", argv[0]);
		exit(1);
	}

	// Check if second argument is an integer
        if (isInt(argv[2])) {
                sPID = atoi(argv[2]);
        }
        else {
                fprintf(stderr, "Usage: %s <String: filename> <int: scheduler pid>\n", argv[0]);
                exit(1);
        }
	
	// Assign arguments to variables and open the file
	char *filename = argv[1];
	FILE *fp = fopen(filename, "w");
	
	// Send request to scheduler to be included in scheduling pool
	kill(sPID, SIGUSR1);

	// Wait for scheduler
	signal(SIGUSR1, signalHandler);
	pause();

	// Write 40 '*' characters to the file
	printStars(fp);

	// Send signal to scheduler that the process is finished
	kill(sPID, SIGUSR2);

	// Close the file and exit process
	fclose(fp);
	return 0;
}
