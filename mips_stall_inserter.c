#include <stdio.h>
#include <string.h>

#define MAX_INSTRUCTIONS 100
#define MAX_LINE_LENGTH 256

// Function to determine if an instruction causes a hazard
int causes_hazard(const char *prev, const char *current) {
    // Check for load-use hazard
    if (strstr(current, "ADD.D") && strstr(prev, "L.D")) {
        return 1; // Load-use hazard
    }
    // Check for data hazard between ADD.D and S.D
    if (strstr(current, "S.D") && strstr(prev, "ADD.D")) {
        return 1; // Result from ADD.D is needed by S.D
    }
    return 0;
}

// Function to insert NOPs in the assembly code
void insert_stalls(char instructions[MAX_INSTRUCTIONS][MAX_LINE_LENGTH], int count) {
    printf("%s\n", instructions[0]); // Print first instruction directly
    for (int i = 1; i < count; i++) {
        if (causes_hazard(instructions[i - 1], instructions[i])) {
            printf("    NOP\n"); // Insert a stall
        }
        printf("%s\n", instructions[i]);
    }
}

int main() {
    char instructions[MAX_INSTRUCTIONS][MAX_LINE_LENGTH] = {
        "L.D F0, 0(R1)",
        "ADD.D F4, F0, F2",
        "S.D F4, 0(R1)",
        "DADDUI R1, R1, #-8",
        "BNE R1, R2, Loop"
    };
    
    int count = 5; // Number of instructions

    printf("Processed MIPS Code with Stalls:\n");
    insert_stalls(instructions, count);

    return 0;
}

