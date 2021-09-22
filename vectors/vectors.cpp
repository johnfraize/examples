#include <algorithm>
#include <vector>
#include <iostream>


bool mycompareInts(int a, int b){
    return(a > b);
}

int main() {

    // some ints 
    std::vector<int> myvec {10,58,33,1,2,3};

    // sort the whole thing default from smallest to biggest
    std::sort(myvec.begin(),myvec.end());

    std::cout << "smallest to biggest ";
    
    for( int j : myvec ){
        std::cout << j << " ";
    }    

    std::cout << std::endl;
    
    // sort using my own comparitor from biggest to smallest
    std::sort(myvec.begin(),myvec.end(),mycompareInts);

    std::cout << "from biggest to smallest ";

    for( int j : myvec ){
        std::cout << j << " ";
    }

    std::cout << std::endl;
    
    return 0;
}
