#include <array>
#include <iostream>



int main(){
    // The size of the array is determined at compile time and cannot be changed at runtime. cool
    // unlike std::vector, std::array does not have a push_back() method or any other methods to modify its size.
    // std::array is a fixed-size array that is allocated on the stack, which means that it has a limited size and can cause stack overflow if it is too large.
    // can still fault by indexing out of bounds, but it will not resize the array like std::vector would.
    // using the .at() method to access elements of the array will throw an exception if the index is out of bounds, which can help prevent undefined behavior.
    // it is safer than using a raw array because it provides bounds checking and other safety features, but it is still not as flexible as std::vector.
    // the .empty() method returns true if the array is empty (i.e., has no elements), and false otherwise. Since std::array has a fixed size, it will never be empty, so this method will always return false.
    // so empty() is not very useful for std::array, but it can be used to check if the array has been initialized or not. If the array has not been initialized, it will be empty and empty() will return true. If the array has been initialized, it will have a fixed size and empty() will return false.
    // adding elements to a std::array is not possible, since its size is fixed at compile time. However, you can initialize the array with values at the time of declaration or use the fill() method to set all elements to a specific value. For example:
    // unused values are initialized to zero by default, but you can also use the fill() method to set all elements to a specific value. For example:
    // arrays can be cast to 
    // arrays can be of any data type, including user-defined types.
    std::array<int,9>  myarray{1,2,3,4,5,6,7,8,9 };

    std::cout << "size of array:" << sizeof(myarray) << "data:" << myarray.data() << std::endl;

    // simple iteration over the array using a for loop, alternatively you can use a range-based for loop.
    for(int i=0;i<myarray.size();i++){
        std::cout << "myarray[" << i << "]:" << myarray[i] << std::endl;
    }

    // range-based for loop iteration
    for(const auto& element : myarray){
        std::cout << "element:" << element << std::endl;
    }


    // to allocate a std::array on the heap, you can use the new operator to create a pointer to the array. For example:
    std::array<int, 5>* myArrayPtr = new std::array<int, 5>{1, 2, 3, 4, 5};

    // to access elements of the array through the pointer, you can use the -> operator. For example:
    for(int i=0;i<myArrayPtr->size();i++){
        std::cout << "myArrayPtr[" << i << "]:" << myArrayPtr->at(i) << std::endl;
    }

    // don't forget to delete the pointer to avoid memory leaks
    delete myArrayPtr;

    

    return 0;
}
    
