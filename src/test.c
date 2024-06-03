#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void appendStrings(char **destination, const char *source) {
    if (destination == NULL || *destination == NULL || source == NULL) {
        fprintf(stderr, "Invalid input to appendStrings.\n");
        return;
    }

    size_t originalLength = strlen(*destination);
    size_t appendLength = strlen(source);
    size_t newLength = originalLength + appendLength + 1;  // +1 for the null terminator

    // Reallocate memory to accommodate the appended string
    char *newStr = realloc(*destination, newLength);
    if (newStr == NULL) {
        fprintf(stderr, "Failed to reallocate memory.\n");
        return;
    }

    // Successfully reallocated, now point destination to newStr
    *destination = newStr;

    // Append the source to the new string space
    strcpy(*destination + originalLength, source);  // Copy directly after the end of the original
}

int main() {
    char *qdsf = "hi there, ";

    appendStrings(&qdsf, "Tim");
    printf("%s\n", qdsf);
} 
