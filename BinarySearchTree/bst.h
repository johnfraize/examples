struct Node {
  struct Node* left;
  struct Node* right;
  int value;
};


struct Node* newNode(int val);
void addNode(struct Node* root, struct* branch);
void walkFoward(struct Node* tree);
void walkBackward(struct Node* tree);


