#include <iostream>
#include <fstream>


int main(int argc, char* argv[] ){

    if( argc < 2 ){
	std::cerr << "no inut file supplied" << std::endl;
	return -1;
    }
    std::string inputFileName = argv[1];

    std::ifstream input;

    input.open(inputFileName);

    std::string line;
    if( input.is_open() ){
	while( getline(input,line )){
	    for( auto c : line ){
		if( c > 0 && c < 127 ){
		    std::cout << c;
		}
		else{
		    //		    std::cout << " ";
		}
	    }
	    std::cout << std::endl;
	}
    }
    
    return 0;
}

