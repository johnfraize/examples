struct Node {
    struct Node* left;
    struct Node* right;
    int data;
    int duplicates; // how many copies of this value so if there are two values duplicates would be one
};


struct Node* newNode(int val);
void addNode(struct Node* root, struct Node* branch);
void walkFoward(struct Node* tree);
void walkBackward(struct Node* tree);


