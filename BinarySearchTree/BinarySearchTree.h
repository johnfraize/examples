namespace BST{

    typedef enum{
         Inorder,  // left,root, right
         Preorder, // root,left,right
         Postorder // left,right,root
    }TraversalType;

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

        // I think I need to provide some kind of comparison operation

        
        void insert(T v){
            if( root == 0 ){
                root = new Node<T>;
            }
            else {

            }
        }

        static bool lessThan(T a, T b){
            return false;
        }


        Node<T>* walk(Node<T>* base, TraversalType tt){
	    
        }
    };
}
