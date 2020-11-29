#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>


int compareDude( char* a, char* b ){


}

int main( int argc, char* argv[] ){
    char* test = "this is - the day";


    char* key = "this dude";

    char** rootptr = 0;

    

    char* token = NULL;

    token = strtok(test,"-, ");

    printf("the length of %s is %ld\n", test, strlen(test));

    if( token != NULL ) {
        printf("token %s\n",token);
    }
    
    return 0;
}
