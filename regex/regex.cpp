#include <regex>
#include <iostream>
#include <fstream>

// usage regx "filepath" "pattern" --> prints hits
// find stuff using regx 
int main(int argc, char* argv[]){
    const char* usageString = "file/url regx pattern";

    if( argc < 3){
	std::cerr << usageString << std::endl;
	return 1;
    }

    char* inputFilePath = argv[1];
    char* searchForPattern = argv[2];

    // open the input file
    std::ifstream inputFile(inputFilePath);

    if( !inputFile.is_open() ){
	std::cerr << "failed to open path:" << inputFilePath << std::endl;
	return 2;
    }

    auto pattern = std::regex(searchForPattern);

    // iterate over lines
    std::string curLineText;
    int curLineNumber = 0;
    while(std::getline(inputFile,curLineText)){
	curLineNumber++;

	if( regex_match(curLineText,pattern)) {
	    std::cout << curLineNumber << ":" << curLineText << std::endl;
	}
	
    }


    return 0;
}
