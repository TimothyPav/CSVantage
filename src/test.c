#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char *a = "field1";
    char b[10] = " Adding";

    strcpy(a,b);
    printf("%s\n", a);
}