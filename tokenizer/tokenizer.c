#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 4096

void reverse(char* output, const char* input){
		char c;
		int len = strlen(input);
		int ii,oi;

		output[len] = 0; // null terminate

		// now i is after the null
		for(ii=0,oi=len-1;ii<len;ii++,oi--){
				c = input[ii];
				output[oi] = c;
		}


}



// reverse a the spelling of a input line
int main(int argc, char* argv[]){
		char data[MAXLINE];
		char pattern[MAXLINE];
		char* token;
		char* input;
		const char* delimiters = " ,-";
		char* saveptr = 0;


		// lets get the big line 
		if( fgets(data,MAXLINE,stdin) != NULL){
				data[strcspn(data,"\n")] = 0; // make a null terminator
		}


		if( fgets(pattern,MAXLINE,stdin) != NULL){
				data[strcspn(pattern,"\n")] = 0; // make a null terminator
		}



		char* current = data;

		int pos;
		int count=0;

		while( pos = strstr(current,pattern) != EOF ){
				count++;
		}


		token = strtok_r(buffy, delimiters, &saveptr);

		int count = 0;

		// now lets reverse each word
		while( token != NULL){
				reverse(output,token);
				if( count++ > 0 ){
						printf(" %s",output);
				}
				else {
						printf("%s",output); // no prior space first time
				}

				token = strtok_r(NULL, delimiters, &saveptr);
		}

		printf("\n");


		return 0;
}
