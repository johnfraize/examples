#include <stdio>




// find all combindations of numbers that would equal n for the allowed values
int sumTermsAtFactor(int maxFactor, std::set<int> terms){


  if( maxFactor == 0 || terms.size() == 0 ){
    return 0;
  }


  
  


  // iterate through each combination
  for( int term : terms ){
    terms.remove(term);
    // remove t from the set
    for(int factor = 0; factor < n; factor++){

      int result = term * factor + findCombinatons( n, 

    }
  }
    


}

int  main( int argc, char* argv[]){

  // a list of lists of ints of all possible results
  std::list<std::list<int>> results;


  findCombindations(5,{1,2,3});
}
