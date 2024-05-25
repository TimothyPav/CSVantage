#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* test;

    test = fopen("test.csv", "w+");

    fprintf(test, "qwerty %s,", "please work first try");
    fprintf(test, "hi there %s,\n", "this is the what line?");
    fprintf(test, "this will be on the new line %s", "please work first try");
    fclose(test);

    printf("Everything should've worked\n");
}