#include <iostream>
#include <list>
#include <algorithm>

int main(int argc, char* argv[]){
    int june = 3;

    int a = [&june](int x, int y) {
		june = 4;
		return x * y;
	    }(june,2);

    auto zipper = [](int z){ return z * 10; };


    int g = zipper(10);
    

    std::cout  << "june:" << june  << " a:" << a << " :g" << g <<  std::endl;


    // create a list
    std::list<int> myList = { 1,2,3,4,5 };


    auto i = std::find_if(myList.begin(),myList.end(),[](int n){ return n == 4; });

    if( i != myList.end() ){
	std::cout << "found it:" << *i << std::endl;
    }
    

    return 0;
}
