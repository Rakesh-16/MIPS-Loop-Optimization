#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTIONS 100
#define MAX_LINE_LENGTH 256

void unroll_loop(char instructions[MAX_INSTRUCTIONS][MAX_LINE_LENGTH], int count, int unroll_factor, FILE *output_file) {
    for (int i = 0; i < count; i++) {
        // Write the original instruction to the output file
        fprintf(output_file, "%s\n", instructions[i]);

        // Check for loop start
        if (strstr(instructions[i], "Loop:") != NULL) {
            int loop_start = i + 1;
            int loop_end = -1;

            // Find the end of the loop (branch instruction)
            while (loop_start < count) {
                if (strstr(instructions[loop_start], "BNE") || strstr(instructions[loop_start], "BEQ")) {
                    loop_end = loop_start;
                    break;
                }
                loop_start++;
            }

            // Check if loop was found
            if (loop_end != -1) {
                // Unroll the loop
                for (int j = 0; j < unroll_factor; j++) { // Repeat the loop body 'unroll_factor' times
                    for (int k = i + 1; k < loop_end; k++) {
                        fprintf(output_file, "%s\n", instructions[k]);
                    }
                }

                // Write the branch instruction after the unrolled body
                fprintf(output_file, "%s\n", instructions[loop_end]);

                // Update the index to skip over the loop
                i = loop_end;
            }
        }
    }
}

int main() {
    FILE *input_file = fopen("mips_code.txt", "r");
    FILE *output_file = fopen("unrolled_mips_code.txt", "w");
    
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

    int unroll_factor;

    // Ask user for unroll factor between 2 and 6
    do {
        printf("Enter unroll factor (2 to 6): ");
        scanf("%d", &unroll_factor);
    } while (unroll_factor < 2 || unroll_factor > 6);

    // Unroll the loop and write to the output file
    unroll_loop(instructions, count, unroll_factor, output_file);

    fclose(output_file);
    
    printf("Loop unrolled with output written to unrolled_mips_code.txt\n");

    return 0;
}

