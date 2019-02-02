#include <iostream>
#include <memory>
#include "Rocky.h"

using namespace std;


shared_ptr<Rocky> dude(){

    shared_ptr<Rocky> tmp  = make_shared<Rocky>(1);

    return tmp;
}

//template <class T>

// How would you implement swap
template <class T>

// how would I do a swap
void johns_swap(shared_ptr<T> & a, shared_ptr<T> & b){

    T* z = new T; // a place to hold stuff

    *z = *a.get();  // copy over a value to holding tank
    

    *a.get() = *b.get(); // now over write a

    *b.get() = *z;  // update b 

    //*z = *a; // copy the crap 

    // swap ownership

    //    T  = make_shared<T>(*y);

    //    a.swap(b);
}



// fun and games with smart pointers
int main(int argc, char* argv[]){

    Rocky a;

    Rocky b(2);

    // multiple scope  can use as the shared_ptr, and the active scopes are kept trac
    // of with use_count() 
    shared_ptr<Rocky> sP;

    // only one thing but when this pointer drops out of scope so dose the object it is pointing to

    // When this pointer is assigned another value the thing that this was pointing to is deleted
    // so unique_ptr is tied to the object it references, sort of
    // there is no use_count() pointer
    unique_ptr<Rocky> uP; // It has no use_count pointer - it can only point to one thing

    //    Rocky c = a+b;
    Rocky c = a;

    uP = make_unique<Rocky>(1);

    sP = make_shared<Rocky>(2);
    //    uP = b;

    std::shared_ptr<int> foo(new int(1));
    std::shared_ptr<int> bar(new int(1));


    *foo = 1;
    *bar = 2;

    cout << *foo << *bar << endl;

    foo.swap(bar);

    cout << *foo << *bar << endl;

    johns_swap(foo,bar);
    
    cout << *foo << *bar << endl;

    
    //std::cout << "use " << dude().use_count() << "  " << uP.get()  << std::endl;

    return 0;
}
