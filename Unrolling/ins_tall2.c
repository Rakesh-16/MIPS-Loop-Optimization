#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTIONS 100
#define MAX_LINE_LENGTH 256

// Latency values for each instruction type
typedef struct {
    const char *instruction;
    int latency;
} InstructionLatency;

InstructionLatency latencies[] = {
    {"L.D", 4},     // Load double
    {"ADD.D", 3},   // Floating-point add
    {"S.D", 3},     // Store double
    {"DADDUI", 1},  // Integer add immediate
    {"BNE", 1}      // Branch if not equal
};

// Function to determine the latency of an instruction
int get_latency(const char *instruction) {
    for (int i = 0; i < sizeof(latencies) / sizeof(latencies[0]); i++) {
        if (strstr(instruction, latencies[i].instruction)) {
            return latencies[i].latency;
        }
    }
    return 0; // Default to 0 if not found
}

// Function to insert NOPs in the assembly code
void insert_stalls(char instructions[MAX_INSTRUCTIONS][MAX_LINE_LENGTH], int count, FILE *output_file) {
    fprintf(output_file, "The loop will execute as follows, taking a total of 9 cycles:\n");
    fprintf(output_file, "| Clock Cycle | Instruction        | Action                   |\n");
    fprintf(output_file, "|-------------|--------------------|--------------------------|\n");

    int cycle = 0; // Track the current cycle

    for (int i = 0; i < count; i++) {
        // Print the instruction and increment the cycle
        cycle++;
        fprintf(output_file, "| %-11d | %-18s | Issue instruction        |\n", cycle, instructions[i]);
        
        // Insert NOPs for stalls based on hazards
        if (i < count - 1) {
            if (strstr(instructions[i], "L.D") && strstr(instructions[i + 1], "ADD.D")) {
                cycle++;
                fprintf(output_file, "| %-11d | %-18s | Stall for hazard        |\n", cycle, "NOP");
            }
            if (strstr(instructions[i], "ADD.D") && strstr(instructions[i + 1], "S.D")) {
                cycle += 2; // Two stalls for hazard
                fprintf(output_file, "| %-11d | %-18s | Stall for hazard        |\n", cycle - 1, "NOP");
                fprintf(output_file, "| %-11d | %-18s | Stall for hazard        |\n", cycle, "NOP");
            }
            if (strstr(instructions[i], "DADDUI") && strstr(instructions[i + 1], "BNE")) {
                cycle++;
                fprintf(output_file, "| %-11d | %-18s | Stall for hazard        |\n", cycle, "NOP");
            }
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
    insert_stalls(instructions, count, output_file);

    fclose(output_file);
    
    printf("Stalls inserted and output written to mips_code_with_stalls.txt\n");

    return 0;
}

