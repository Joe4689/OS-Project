#include "kernel/types.h"
#include "user/user.h"

int main(void) {
    int pid_low, pid_high;
    printf("=== Aging Test ===\n");
    printf("Low-priority process should run even with CPU-hog high-priority process.\n\n");

    pid_low = fork();
    if (pid_low == 0) {
        // Low priority process – should starve without aging
        setpriority(3);
        for (int i = 0; i < 20; i++) {
            printf("[LOW] Running iteration %d\n", i);
            sleep(30);  // give up CPU so others can run
        }
        printf("[LOW] Finished.\n");
        exit(0);
    }

    pid_high = fork();
    if (pid_high == 0) {
        // High priority CPU hog – never sleeps
        setpriority(0);
        printf("[HIGH] Starting CPU hog...\n");
        while (1) {
            // Busy loop
            for (volatile int j = 0; j < 1000000; j++);
        }
        exit(0);
    }

    // Let the system run for a while
    sleep(200);  // about 2 seconds in xv6 tick time

    printf("Killing high-priority process...\n");
    kill(pid_high);
    wait(0);  // wait for high to die

    // Wait for low to finish
    wait(0);
    printf("\n=== Aging test finished ===\n");
    exit(0);
}