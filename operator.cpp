#include <iostream>
#include "Rocky.h"



int main(int argc, char* argv[]){

    Rocky a;

    Rocky b(2);


    //    Rocky c = a+b;
    Rocky c = a;
    
    std::cout << a << b << c << std::endl;

    return 0;
}
