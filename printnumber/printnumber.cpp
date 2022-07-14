#include <iostream>
#include <vector>
#include <sstream>

std::string decode(int v){

    std::stringstream ss;

    std::vector<std::string> first_score =
	{
	 "",
	 "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
	 "ten","elleven","tweleve","thirteen","fourteen","fifteen", "sixteen","seventeen","eightteen","nineteen",
	};
    if( v <= 19 ){
	ss << first_score[v];
    }else if( v >= 20 && v < 20+10){ss << "twenty" << first_score[v % 20];
    }else if( v >= 30 && v < 30+10){ss << "thirty" << first_score[v % 30];
    }else if( v >= 40 && v < 40+10){ss << "forty" << first_score[v % 40];
    }else if( v >= 50 && v < 50+10){ss << "fifty" << first_score[v % 50];
    }else if( v >= 60 && v < 60+10){ss << "sixty" << first_score[v % 60];
    }else if( v >= 70 && v < 70+10){ss << "seventy" << first_score[v % 70];
    }else if( v >= 80 && v < 80+10){ss << "eighty" << first_score[v % 80];
    }else if( v >= 90 && v < 90+10){ss << "ninety" << first_score[v % 90];
    }else if( v >= 100 && v < 1000 ){ss << decode(v / 100) << "hundred " << decode(v % 100);
    }else if( v >= 1000 && v < 1000000 ){ss << decode(v / 1000) << "thousand " << decode(v % 1000);
    }else if( v >= 1000000 && v < 1000000000 ){ss << decode(v / 1000000) << "million " << decode(v % 1000000);	
    }else{
	ss << "out of range";
    }

    return ss.str();
}


int main(int argc, char* argv[]){
    const char* usage = "enter a number and I will print it, for example 3 then I print three";
    int value = 0;

    if( argc < 2 ){
	std::cerr << usage << std::endl;
	return __LINE__;
    }

    // convert text string to int
    value = atoi(argv[1]);

    if( value < 0 ){
	std::cout << "negative ";
	value = abs(value); // convert it to positive
    }

    if( value == 0 ){
	std::cout << "zero" << std::endl;
	return 0; // done 
    }
    
    std::cout << decode(value) << std::endl;
    
    return 0;
}
