#include <iostream>

template <typename DudeType>
DudeType fastback(const DudeType m){

  std::cout << "sizeof m is " << sizeof(DudeType) << std::endl;

  return m;
}


int main(int argc, char* argv[]){


  
    

  fastback(3.9);
   
  

  std::cout << "this is the day" << std::endl;

  fastback(3);

  return 0;
}
