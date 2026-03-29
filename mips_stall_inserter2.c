#include <stdio.h>
#include <stdlib.h>
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
    printf("%s", instructions[0]); // Print first instruction directly
    for (int i = 1; i < count; i++) {
        if (causes_hazard(instructions[i - 1], instructions[i])) {
            printf("\n    NOP"); // Insert a stall
        }
        printf("\n%s", instructions[i]);
    }
    printf("\n"); // Final newline for output
}

int main() {
    FILE *file = fopen("mips_code.txt", "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file.\n");
        return 1;
    }

    char instructions[MAX_INSTRUCTIONS][MAX_LINE_LENGTH];
    int count = 0;

    // Read instructions from file
    while (fgets(instructions[count], MAX_LINE_LENGTH, file) && count < MAX_INSTRUCTIONS) {
        // Remove newline character at the end
        instructions[count][strcspn(instructions[count], "\n")] = 0;
        count++;
    }
    
    fclose(file);

    // Process and insert stalls
    printf("Processed MIPS Code with Stalls:\n");
    insert_stalls(instructions, count);

    return 0;
}

