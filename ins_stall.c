#include <stdio.h>

#define CYCLES 9

// Function to print the execution timeline with stalls
void print_execution_timeline() {
    const char *instructions[CYCLES] = {
        "L.D F0, 0(R1)",   // Cycle 1
        "stall",           // Cycle 2
        "ADD.D F4, F0, F2",// Cycle 3
        "stall",           // Cycle 4
        "stall",           // Cycle 5
        "S.D F4, 0(R1)",   // Cycle 6
        "DADDUI R1, R1, #-8", // Cycle 7
        "stall",           // Cycle 8
        "BNE R1, R2, Loop" // Cycle 9
    };

    printf("Execution of the Loop\n");
    printf("The loop will execute as follows, taking a total of %d cycles:\n", CYCLES);
    printf("| Clock Cycle | Instruction        | Action                   |\n");
    printf("|-------------|--------------------|--------------------------|\n");

    for (int cycle = 0; cycle < CYCLES; cycle++) {
        printf("| %-11d | %-18s | ", cycle + 1, instructions[cycle]);
        if (instructions[cycle][0] == 's') {
            printf("Stall for hazard        |\n");
        } else {
            printf("Issue instruction        |\n");
        }
    }

    printf("\nTotal Clock Cycles: %d cycles\n", CYCLES);
}

int main() {
    print_execution_timeline();
    return 0;
}

