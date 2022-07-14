#include <string>
#include <iostream>

/// Could not get wildcard to work next time
bool isSubString(std::string area, std::string searchFor){

    int matchCount = 0;

    typedef enum {
	  search,
	  verify,
	  escape,
	  wildcard
    }State;


    // start off in search state
    State state = search;
    char sample,expect;
    bool hasWild = false;

    // could not figure out wildcards

    // walk through the expected strings
    for(char sample : area ){
	expect = searchFor[matchCount];

	switch( state ){
	case wildcard:
	    // we are in wildcard
	    if( sample == expect){
		state = verify;
	    }
	    std::cout << __FILE__ << ":" << __LINE__ << " sample:" << sample <<  ", expect:" << expect << std::endl;
	    hasWild = true;
	    break;
	    
	case verify:
	    // we started on string so lets verify
	    if( sample != expect){
		state = search;
	    }
	    else{
		matchCount++;
	    }
	    std::cout << __FILE__ << ":" << __LINE__ << " sample:" << sample <<  ", expect:" << expect << std::endl;	    	    
	    break;
	    
	case search:
	    if( sample == expect ){
		state = verify;
		matchCount++;
	    }
	    else if( expect == '*') {
		state = wildcard;
	    }
	    std::cout << __FILE__ << ":" << __LINE__ << " sample:" << sample <<  ", expect:" << expect << std::endl;	    
	    break;
	defaut:
	    state = search;
	    std::cout << __FILE__ << ":" << __LINE__ << " sample:" << sample <<  ", expect:" << expect << std::endl;
	    matchCount=0;
	}
    };


    int n = searchFor.size();

    std::cout << n << " " << matchCount << std::endl;
    return matchCount == n;
}




int main(int argc, char* argv[]){
    if( argc < 3 ){
	std::cerr << "masterarea stringtosearch" << std::endl;
	return __LINE__;
    }

    std::string area(argv[1]);

    std::string searchfor(argv[2]);

    std::cout << area << 
	(isSubString(area,searchfor) ? " found" : " missing") << std::endl;
    return 0;
}
