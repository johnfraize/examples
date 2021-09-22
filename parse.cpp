#include <iostream>
#include <stack>
using std::stack;

int main(int argc, char* argv[]){

  std::string input(argv[1]);

  if( argc > 0 ){
    std::cout << "input is " << input << std::endl;
  }


  try {
      std::stack<std::string> stk;
      std::string tmp;

      for( char c : input){
          std::cout << c ;
          if( c == '(' ){
              stk.push(tmp);
          }
          else if( c == ')') {
              stk.pop();
              tmp = stk.top();
          }
          else{
              tmp += c;
          }
      }
  }
  catch (...) {

  }
  

  return 0;
}
