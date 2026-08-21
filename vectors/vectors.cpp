#include <algorithm>
#include <vector>
#include <iostream>
#include <climits>

class Glossy {
  public:
  int d =3;
  void dumpit(){
    std::cout << d << std::endl;
  }

  Glossy(int dd){
    d = dd;
    //std::cout << d << std::endl;
  }

};

bool mycompareInts(int a, int b){
  std::cout << "comparing " << a << " and " << b << std::endl;
  return(a > b);
}

bool mycompareGlossy(const Glossy& a, const Glossy& b){
  std::cout << "comparing " << a.d << " and " << b.d << std::endl;
  return(a.d > b.d);
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
  //return swapFun(oneDay);


  

  // some ints 
  std::vector<Glossy> myvec {10,58,33,1,2,3,77,3,3,3,5};

  // sort the whole thing default from smallest to biggest
  std::sort(myvec.begin(),myvec.end(),mycompareGlossy);

  std::cout << "smallest to biggest " << std::endl;
    
  for( const auto& j : myvec ){
    std::cout << j.d << " ";
  }    

  std::cout << std::endl;
    
  // sort using my own comparitor from biggest to smallest
  //std::sort(myvec.begin(),myvec.end(),mycompareGlossy);

  std::cout << "from biggest to smallest " << std::endl;



  myvec.assign({ 10,58,33,1,2,3,77,3,3,3,5});

  myvec.erase(std::remove_if(myvec.begin(),myvec.end(),[](const Glossy& g){ return g.d < 10; }),myvec.end());


  // to insert a new element into the vector, you can use the push_back() method. For example:
  myvec.push_back(99);

  // to remove an element from the vector, you can use the erase() method. For example, to remove the first element:
  myvec.erase(myvec.begin());

  // to remove all elements from the vector, you can use the clear() method. For example:
  myvec.clear();

  // to remove the first half of the vector, you can use the erase() method with a range. For example:
  myvec.erase(myvec.begin(), myvec.begin() + myvec.size() / 2);


  // to declare a huge vector of Glossy objects, you can use the following syntax:
  std::vector<Glossy> hugeVector(1000000, Glossy(0)); // creates a vector of 1 million Glossy objects initialized with 0

  // to declare on the heap, you can use the new operator to create a pointer to the vector. For example:
  std::vector<Glossy>* hugeVectorPtr = new std::vector<Glossy>(1000000, Glossy(0)); // creates a vector of 1 million Glossy objects initialized with 0 on the heap



  for( const auto& j : myvec ){
    std::cout << j.d << " ";
  }

  std::cout << std::endl;

  delete hugeVectorPtr; // don't forget to delete the pointer to avoid memory leaks
 
 
  return 0;
}
