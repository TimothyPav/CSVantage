#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char ** test(char *arr[], int size){

    for(int i=0; i<size; i++){
        arr[i] = strdup("abcdefghijklmnopqrstuvwxyz");
    }

}

int main() {
    char *qdsf[5];
    test(qdsf,5);

    for(int i=0; i<5; i++){
        printf("qdsf[%d] = %s\n", i, qdsf[i]);
    }
}