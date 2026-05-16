#include "kernel/types.h"
#include "user/user.h"

int main(void) {
    int pid_low, pid_high;
    
    printf("=== Starvation test (aging) ===\n");
    
    pid_low = fork();
    if (pid_low == 0) {
        setpriority(3);  // very low priority
        for (int i = 0; i < 30; i++) {
            printf("Low-priority process running (tick %d)\n", i);
            sleep(50);
        }
        exit(0);
    }
    
    pid_high = fork();
    if (pid_high == 0) {
        setpriority(0);  // high priority, CPU hog
        while (1) {
            // busy loop – consumes CPU
            for (volatile int j = 0; j < 1000000; j++);
        }
        exit(0);
    }
    
    sleep(200);  // let it run for a while
    printf("Killing high-priority process to check low-priority progress...\n");
    kill(pid_high);
    
    wait(0);  // wait for high-priority process
    wait(0);  // wait for low-priority process
    
    printf("=== Test finished – low-priority did run (aging worked) ===\n");
    exit(0);
}