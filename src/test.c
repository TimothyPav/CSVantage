#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool lossey_str_cmp(char *input, char *existing_field){
    int i = 0;
    int j = 0;
    while(existing_field[i] != '\0'){
        if(existing_field[i] == input[j]){
            j++;
            if(input[j] == '\0') return true;
        }
        else {
            j = 0;
        }
        i++;
    }
    return false;
}

int main() {
    char *a = "2016";
    char *b = "qwerty 1 2015 qwrety";

    bool x = lossey_str_cmp(a, b);
    printf("Your boolean variable is: %s\n", x ? "true" : "false");
}