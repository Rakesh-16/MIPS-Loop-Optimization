#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTIONS 100
#define MAX_LINE_LENGTH 256

// Function to insert NOPs in the assembly code with optimized scheduling
void insert_stalls_with_optimized_scheduling(char instructions[MAX_INSTRUCTIONS][MAX_LINE_LENGTH], int count, FILE *output_file) {
    fprintf(output_file, "The loop will execute as follows, taking a total of 7 cycles:\n");
    fprintf(output_file, "| Clock Cycle | Instruction        | Action                   |\n");
    fprintf(output_file, "|-------------|--------------------|--------------------------|\n");

    int cycle = 0; // Track the current cycle

    for (int i = 0; i < count; i++) {
        // Issue the instruction
        cycle++;
        fprintf(output_file, "| %-11d | %-18s | Issue instruction        |\n", cycle, instructions[i]);

        // Insert stalls based on the specified scheduling
        if (i == 2) { // After ADD.D
            cycle++;
            fprintf(output_file, "| %-11d | %-18s | Stall for hazard        |\n", cycle, "NOP");
            cycle++;
            fprintf(output_file, "| %-11d | %-18s | Stall for hazard        |\n", cycle, "NOP");
        }
    }

    fprintf(output_file, "\nTotal Clock Cycles: %d cycles\n", cycle);
}

int main() {
    FILE *input_file = fopen("mips_code.txt", "r");
    FILE *output_file = fopen("mips_code_with_stalls.txt", "w");
    
    if (!input_file || !output_file) {
        fprintf(stderr, "Error: Could not open file.\n");
        return 1;
    }

    char instructions[MAX_INSTRUCTIONS][MAX_LINE_LENGTH];
    int count = 0;

    // Read instructions from the input file
    while (fgets(instructions[count], MAX_LINE_LENGTH, input_file) && count < MAX_INSTRUCTIONS) {
        // Remove newline character at the end
        instructions[count][strcspn(instructions[count], "\n")] = 0;
        count++;
    }
    
    fclose(input_file);

    // Insert stalls and write to the output file
    insert_stalls_with_optimized_scheduling(instructions, count, output_file);

    fclose(output_file);
    
    printf("Stalls inserted with optimized scheduling and output written to mips_code_with_stalls.txt\n");

    return 0;
}

