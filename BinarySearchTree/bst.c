#include "bst.h"
#include <cstdio>
#include <cstdlib>

struct Node* newNode(int data){
    struct Node* n = (Node*)malloc(sizeof(struct Node));
    n->left = n->right = 0;
    n->duplicates = 0;
    n->data = data;
    return n;
}

void addNode(struct Node* tree, struct Node* limb){
    // in oder add
    if (limb->data < tree->data){
	if (tree->left){
	    addNode(tree->left,limb);
	}
	else{
	    tree->left = limb;
	}
    }
    else if( tree->data == limb->data ){
	tree->duplicates++;  // not including the original copy
    }
    else{  // must be greater than
	if (tree->right){
	    addNode(tree->right,limb);	    
	}
	else{
	    tree->right = limb;
	}
    }
}

void walkForward(struct Node* tree){
    if (tree->left){
	walkForward(tree->left);
    }
    for(int i=0;i<=tree->duplicates;i++){
	printf("%d\n",tree->data);
    }
    if (tree->right){
	walkForward(tree->right);
    }
}

void walkBackward(struct Node* tree){
    if (tree->right){
	walkBackward(tree->right);
    }
    for(int i=0;i<=tree->duplicates;i++){
	printf("%d\n",tree->data);
    }
    if (tree->left){
	walkBackward(tree->left);
    }
}


void sort(int* input, int num){
    if( num < 1 ){
	return;
    }
    
    struct Node* tree = newNode(input[0]); // take the first one for the node

    for(int i=1;i<num;i++){
	addNode(tree,newNode(input[i]));
    }

    walkForward(tree);
    printf("\n");
    walkBackward(tree);
}



int main(int argc, char** argv){

    int values[] = {10,21,58,12,6,68,21,3,1,3,3,5555,5,3};

    sort(values,sizeof(values)/sizeof(int));

    return 0;
}
