#include <iostream>
#include <list>
#include <regex>


int main(int argc, char* argv[]){

    std::string document = "for score and seven years ago I wwen to the mall";

    std::list<std::string> keyWords = { "dude", "score", "mall" };


    for(std::string kw : keyWords){

	int rv = document.find(kw);

	std::cout << kw << ":" << rv <<  "   " << kw.size() << std::endl;
    }

    // find words starting with sev
    std::regex e ("\\b(sev)([^ ]*)");
    
    std::smatch m;
    
    // search
    if( std::regex_search(document,m,e) == true ){
	std::cout << "found it" << std::endl;
	std::cout << "suffix:" << m.suffix().str() <<  std::endl;
	std::cout << "prefix:" << m.prefix().str() <<  std::endl;
    }



    return 0;
}
