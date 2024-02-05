#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
//#include "queue.c"

// Create global variables
int W;
int T;
char *SA;
int currW = -1;
pid_t *pids;
int doneCount = 0;

void signalHandler(int sig, siginfo_t *info){
        if (sig == SIGUSR1) {
                // Handle request signal
                for (int i = 0; i < W; i++){
                        if (pids[i] == NULL){
                                pids[i] = info->si_pid;
                                break;
                        }
                }

        } else if (sig == SIGUSR2) {
                // Handle finish signal
                for (int i = 0; i < W; i++){
                        if (pids[i] == (info->si_pid)){
                                pids[i] = -1;
                                doneCount++;
                                break;
                        }
                }
        }
}

int hasSeven(int pid){
        int num = pid % 10000;
        while (num > 0) {
                if (num % 10 == 7) {
                        return 1;
                }
                num /= 10;
    }
    return 0;
}

void roundRobin(){
        while (1) {
                // Select worker
                currW = (currW + 1) % W;
                pid_t currWPID = pids[currW];

                // Check if worker is already finished
                if ((int)currWPID != -1){
                        // Send signal
                        kill(currWPID, SIGUSR1);
                        kill(currWPID, SIGSTOP);
                        kill(currWPID, SIGCONT);

                        // Wait for 'T' seconds
                        if (strcmp(SA, "L7") == 0 && hasSeven((int)currWPID)){
                                sleep(T*2);
                        } else {
                                sleep(T);
                        }

                        // Suspend current worker
                        kill(currWPID, SIGSTOP);
                }

                if (doneCount == W) {
                        break;
                }
        }
}

int main(int argc, char *argv[]){
        // Check number of arguments
        if (argc != 4) {
                fprintf(stderr, "Usage: %s <int: workers> <int: timeslice> <String: algorithm>\n", argv[0]);
                exit(1);
        }

        // Set arguments to variables
        W = atoi(argv[1]);
        T = atoi(argv[2]);
        SA = argv[3];
        pids = malloc(sizeof(pid_t) * W);

        // Create and setup sigactioni
        struct sigaction sa;
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction = &signalHandler;

        if (sigaction(SIGUSR1, &sa, NULL) == -1) {
                fprintf(stderr, "sigaction");
                exit(1);
        }

        if (sigaction(SIGUSR2, &sa, NULL) == -1) {
                fprintf(stderr, "sigaction");
                exit(1);
        }

        // Wait for all signals to be received
        for (int i = 0; i < W; i++) {
                pause();
        }

        // Start Scheduling Algorithms
        if (strcmp(SA, "RR") == 0 || strcmp(SA, "L7") == 0){
                roundRobin();
        } else {
                fprintf(stderr, "Algorithm can only be RR or L7\n");
        }

        // Ending
        printf("All processes complete\n");

        return 0;
}
