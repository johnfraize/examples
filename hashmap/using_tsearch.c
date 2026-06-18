#include <stdio.h>
#include <stdlib.h>
#include <search.h>  // incudes the tsearch stuff
#include <string.h>

typedef struct{
		const char* key;		
		int value;
}NodeDa;


int compare_nodes(const void* a, const void* b){
		const NodeDa* nodeA = (NodeDa*)a;
		const NodeDa* nodeB = (NodeDa*)b;
		return strcmp(nodeA->key,nodeB->key);
}

void print_node(const void* nodep, VISIT which, int depth){
		if ( nodep == NULL ){
				return;
		}
		// read one level of pointer inderection
		const NodeDa *data = *(const NodeDa**)nodep;
		if (which == postorder || which == leaf) {
				printf("Key: %-10s Value: %-5d (Depth: %d)\n",
						data->key, data->value, depth);
		}

}

int main(int argc, char* argv[]){
		void* root  = NULL;
		int index=0;


		NodeDa items[] = {
				{"Justin",1983},
				{"Ryan",1987},
				{"Sam",1995},
				{"Isaac",1997},
				{"Josh",1999},
				{"Laura",2001}
		};


// creating a hashmap
		printf("-- inserting --\n");
		for(int i= 0;i< sizeof(items)/sizeof(NodeDa);i++){
				tsearch(&items[i],&root,compare_nodes);
		}
		printf("walkies gromett\n");
		twalk(root,print_node);

		// now lets search for some data
		NodeDa lookFor = {"Ryan"};

		void *result = tfind(&lookFor,&root,compare_nodes);

		if( result != NULL) {
				NodeDa* found = *(NodeDa**)result;
				printf("%s is %d\n",found->key, found->value);
		}
		else{
				printf("could not find it\n");
		}


		return 0;
}
