#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 4096



// reverse a the spelling of a input line
int main(int argc, char* argv[]){
		char first[MAXLINE];
		char second[MAXLINE];
		int bucket[256] = {0}; // will zero all

		// lets get the big line 
		if( fgets(first,MAXLINE,stdin) != NULL){
				first[strcspn(first,"\n")] = 0; // make a null terminator
		}


		if( fgets(second,MAXLINE,stdin) != NULL){
				second[strcspn(second,"\n")] = 0; // make a null terminator
		}

		// make sure they are the same lenght 
		if( strlen(first) != strlen(second) ){
				printf("NO\n");
				return 0;
		}


		char c;

		// walk the first one and count 
		for(int i=0;(c=first[i]);i++){
				bucket[(unsigned char)c]++;  // upcount
		}

		for(int i=0;(c=second[i]);i++){
				bucket[(unsigned char)c]--;  // down count
		}

		int missmatch=0;
		// now if is an anagram then the entire bucket should be zero
		for(int i=0;i<256;i++){
				int v  = bucket[i];
				if(v !=0){
						missmatch++;
				}
		}

		if (missmatch){
				printf("NO\n");
		}
		else {
				printf("YES\n");
		}
		

		return 0;
}
