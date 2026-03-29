#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTIONS 100

void schedule_instructions(char *input[], int count) {
    char *loads[MAX_INSTRUCTIONS];
    char *adds[MAX_INSTRUCTIONS];
    char *stores[MAX_INSTRUCTIONS];
    char *control[MAX_INSTRUCTIONS]; // To hold control instructions
    int load_count = 0, add_count = 0, store_count = 0, control_count = 0;

    // Categorize instructions
    for (int i = 0; i < count; i++) {
        if (strncmp(input[i], "L.D", 3) == 0) {
            loads[load_count++] = input[i];
        } else if (strncmp(input[i], "ADD.D", 5) == 0) {
            adds[add_count++] = input[i];
        } else if (strncmp(input[i], "S.D", 3) == 0) {
            stores[store_count++] = input[i];
        } else {
            // Handle DADDUI and BNE
            control[control_count++] = input[i];
        }
    }

    // Output the scheduled instructions in the correct order
    for (int i = 0; i < load_count; i++) {
        printf("%s\n", loads[i]);
    }
    for (int i = 0; i < add_count; i++) {
        printf("%s\n", adds[i]);
    }
    for (int i = 0; i < store_count; i++) {
        printf("%s\n", stores[i]);
    }
    for (int i = 0; i < control_count; i++) {
        printf("%s\n", control[i]);
    }
}

int main() {
    FILE *file = fopen("mips_instructions.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file\n");
        return 1;
    }

    char *instructions[MAX_INSTRUCTIONS];
    char buffer[256];
    int instruction_count = 0;

    // Read instructions from file
    while (fgets(buffer, sizeof(buffer), file) != NULL && instruction_count < MAX_INSTRUCTIONS) {
        // Remove newline character
        buffer[strcspn(buffer, "\n")] = 0;
        instructions[instruction_count] = strdup(buffer);
        instruction_count++;
    }

    fclose(file);

    printf("Rescheduled Loop Instructions:\n");
    schedule_instructions(instructions, instruction_count);

    // Free allocated memory
    for (int i = 0; i < instruction_count; i++) {
        free(instructions[i]);
    }

    return 0;
}

