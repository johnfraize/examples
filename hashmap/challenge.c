
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>

int compare_nodes(const void* ain, const void* bin){
		const int a = *(int*)ain;
		const int b = *(int*)bin;

		if ( a > b ) {
				return 1;
		}

		if ( a < b ) {
				return -1;
		}
		return 0;

		// return (a > b) - (a < b);   // but I like to spell it out
}


int main(int argc, char* argv[]){
		int targetSum = 6;
		int numElements = 0;
		scanf("%d",&targetSum);
		
		scanf("%d",&numElements);

		if( numElements < 2) {
				printf("NO\n");
				return 0;
		}
		

		int* data = (int*)malloc(numElements * sizeof(int));
		void* root = NULL;


		// setup the input data
		for(int i=0;i<numElements;i++){
				scanf("%d",&data[i]);
		}




		// to search the from one way
		int found = 0;

		tsearch(&data[0],&root,compare_nodes); // add the first one
		// now lets walk the tree looking for matching values 
		for(int i = 1;i<numElements;i++){
				int delta = targetSum - data[i];

				// look for match
				void* result = tfind(&delta,&root,compare_nodes);

				if ( result != NULL){
						found = 1;
						break;
				}
				else{
						tsearch(&data[i],&root,compare_nodes);
				}

		}

		if (found){
				printf("YES\n");

		}
		else {
				printf("NO\n");
		}


		free(data);


		return 0;
}
