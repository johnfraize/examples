#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 200


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

int main(int argc, char* argv[]){
        char buffy[MAXLINE];
		char output[MAXLINE];
		char* saveptr = 0;
		char* token;

		

		if( fgets(buffy,MAXLINE,stdin) != NULL){
				int indexOfNewLine = strcspn(buffy,"\n");
				buffy[indexOfNewLine] = 0; // make a null terminator
				reverse(output,buffy);
		}


		printf("%s",output);
		


		return 0;
}
