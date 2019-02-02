namespace BST{

    template <class T>
	
    class Node{
    public:
	Node* left;
	Node* right;	
	T data;
    };

    template <class T>
    class Tree{
	Node<T>* root;
	
    public:
	Tree(){
	    root = 0;
	}
	void insert(T v){
	    if( root == 0 ){
		root = new Node<T>;
	    }
	}


	
	void walk(){
	    
	}
    };

}
