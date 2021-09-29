#include <algorithm>
#include <vector>
#include <iostream>
#include <climits>


bool mycompareInts(int a, int b){
  return(a > b);
}


template <typename T> 
void dump2D(std::vector<std::vector<T>> & input){
  unsigned int rows = input.size();

  for( auto r : input ){
    for( auto c : r ){
      std::cout << c << " ";
    }
    std::cout << std::endl;
  }
}

template <typename T> 
void dump1D(std::vector<T> & v){
  for( auto i : v ){
    std::cout << i << " ";
  }

  std::cout << std::endl;
}




// multi dimention game
int multiDimFun(std::vector<std::vector<int>> & input){
  int max = INT_MIN;
  std::cout << "rows:" << input.size() << " columns: " << input[0].size() <<std::endl;

  for( std::vector<int> row : input ){
    for( int v : row ) {

    }

    /**
    for(int fr = 0; fr < 2; fr++,std::reverse(row.begin(),row.end())){
      for( std::vector<int> column : row ){
	for(int fc = 0; fc < 2; fc++, std::reverse(column.begin(),column.end())){
	  int tmp = column[0] + column[1] + row[0][0] + row[1][0];
	  if( tmp > max){
	    max = tmp;
	  }
	}
      }
    }
    **/
  }

  return max;
}

struct Glossy {
  int d =3;
  void dumpit(){
    std::cout << d << std::endl;
  }

};

int swapFun(std::vector<int> dude){


  //  std::swap(dude[0],dude[2]);

  int mid = dude.size()/2;

  dump1D(dude);
  std::reverse(dude.begin() + 1,dude.end());

  dump1D(dude);

  std::sort(dude.begin() + mid, dude.end());

  //dump1D(dude);
  return 0;
}


int main() {

  // multiDim
  std::vector<std::vector<int>> john
    {
     { 1,2,3},
     { 4,5,6 },
     { 7,8,9},
     { 10,11,12}     
    };


  std::vector<std::vector<std::string>> fraize
    {
     { "this", "is", "the" },
     { "day", "that", "I"},
     { "am" , "going", "to" },
     { "like"}
    };


  std::vector<int> oneDay = { 1,2,3,4,5,6,7 };

						

  //dump2D(john);
  //dump2D(fraize);
  return swapFun(oneDay);


  

  // some ints 
  std::vector<int> myvec {10,58,33,1,2,3,77,3,3,3,5};

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
