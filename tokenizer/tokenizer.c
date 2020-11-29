#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char* argv[]){
    char* token;
    char* input;
    const char* delimiters = " ,-";
    char* saveptr = 0;
    if( argc < 2 ){
        printf("pass in data to tokenize - like \"this is the day the lord has made\"\n");
        exit (0);
    }
    
    input = argv[1];

    token = strtok_r(input, delimiters, &saveptr);

    while( token != NULL){
        printf("%s \n", token);
        token = strtok_r(NULL, delimiters, &saveptr);
    }


    return 0;
}
