#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <functional>
#include <list>
#include <limits>
#include <cmath>


using namespace std;

class VectorDesc{
public:
    VectorDesc(){

    }
    
    VectorDesc(std::vector<int>::const_iterator b,std::vector<int>::const_iterator e) : begin(b), end(e) {
	//std::cout << std::accumulate(begin,end,0) << std::endl;	
    }

    int sum(){
	return std::accumulate(begin,end,0);
    }
    int distance(){
	return std::distance(begin,end);
    }


    std::vector<int>::const_iterator begin;
    std::vector<int>::const_iterator end;

    void dump(){

	std::vector<int>::const_iterator it;
	std::cout << "dump ";
	for(it=begin;it!=end;it++){
	    std::cout << *it << " ";
	}
	std::cout << std::endl;
    }
    
    std::list<VectorDesc> possible(){
	std::list<VectorDesc> retVal;

	std::vector<int>::const_iterator b,e;

	// move the starting postion
	for( b = begin; b!= end; b++){
	    // for every possible width
	    for( e = b + 1; e != end; e++ ){
		//std::cout << *b << ":" << *e << std::endl;
		
		VectorDesc tmp(b,e+1);
		retVal.push_back(tmp);
	    }
	}


	return retVal;
    }
};




int main(int argc, char* argv[]){
    
    std::vector<int>  input{2, -1, -2, 1, -4, 2, 8};

    //std::vector<int>  input{1,2,3};
    std::vector<int>::const_iterator centerline;

    //std::cout << std::distance(input.begin(),input.end()) << ":" << input.size() << std::endl;



    // for each possible pair of arrays - find the biggest delta

    // how to interate through this
    // a center line where by things to the left are one set and the ones on the left are the other
    // we will iterate the center line from the start to the end
    // left and right
    // left and right will have an array for every subset that is possible
    // we will try each subset on the left and on the right and therefore have tried every possible combination
    // for each one we need to remeber the max delta so we know which one is the solution

    int min = std::numeric_limits<int>::min();
    int max = std::numeric_limits<int>::max();
    int maxDelta = std::numeric_limits<int>::min();

    VectorDesc solutionX,solutionY;

    for(centerline=input.begin();centerline!=input.end();++centerline){

	VectorDesc left(input.begin(),centerline+1);
	VectorDesc right(centerline,input.end());

	std::list<VectorDesc> possiblesLeft = left.possible();
	std::list<VectorDesc> possiblesRight = right.possible();
	for( VectorDesc & x : possiblesLeft ){
	    for( VectorDesc & y : possiblesRight ){
		int delta = std::abs(x.sum() - y.sum());

		if( delta > maxDelta ){
		    solutionX = x;
		    solutionY = y;
		    maxDelta = delta;
		}
	    }
	}

    }

    std::cout << solutionX.distance() << ":" << solutionY.distance() << " delta:" << maxDelta
	      << " " << *solutionX.begin << ":" << *solutionY.begin << std::endl;


    solutionX.dump();
    solutionY.dump();
    

    return 0;
}
