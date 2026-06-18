#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 4096



// reverse a the spelling of a input line
int main(int argc, char* argv[]){
		char data[MAXLINE];
		char pattern[MAXLINE];


		// lets get the big line 
		if( fgets(data,MAXLINE,stdin) != NULL){
				data[strcspn(data,"\n")] = 0; // make a null terminator
		}


		if( fgets(pattern,MAXLINE,stdin) != NULL){
				pattern[strcspn(pattern,"\n")] = 0; // make a null terminator
		}

		int count=0;


		char* current = data;
		char* foundat = 0;

		if( strlen(pattern) > 0 ){

				while((foundat = strstr(current,pattern))){

						// foundat points to the start of the pattern
						count++;
						current = foundat + 1; // search for overlaps
				}

		}



		printf("%d\n",count);



		return 0;
}
