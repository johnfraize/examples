#include <string>
#include <algorithm>
#include <iostream>
#include <vector>



bool isPalindrone(std::string input){
    std::string output(input); 
    std::reverse(output.begin(), output.end());
    std::cout << "input:" << input  << ", output:" << output << " ";

    int tmp;
    
    if( (tmp = input.compare(output)) == 0 ){
	std::cout << "match" << std::endl;
	return true;
    }
    else {
    	std::cout << "miss:" <<  tmp <<  std::endl;
	return false;
    }
}






// return a int showing what I need to remove to become a palindrone
// if already a palindrone return -1
int palindroneHackOut(std::string input){

    // interate through changing the string by rememoving one char
    // and see if that makes it a palindrone
    int rv = 0;

    if( isPalindrone(input) ){
	return -1;
    }

    for(int i = 0; i < input.length() ; i++){
	std::string phase0(input);
	phase0.erase(i,1);
	if( isPalindrone(phase0) ){
	    std::cout <<  "kill is:" << i << std::endl;
	    break;
	}
    }

    return rv;
}







int main(int argc, char* argv[]){

    if( argc > 1 ){
	std::string t(argv[1]);
	int rv = palindroneHackOut(t);
	std::cout << t << ":" << rv << std::endl;
	return rv;
    }


    // you can append a char or substirng
    std::string test = "jones";
    //std::string test2(test); // can assign this way
    std::string test2 = test; // or this way

    // can append a char  or bunch
    test2.append(20,'z');  // note size comes first

    // can append a const 
    test2.append("this is the day"); // copies up to the null

    // or append just a portion of a const
    test2.append("this is the day",4);
    

    test2 = "hey jude"; // length and size are the same value

    std::cout << "test:" << test << ", test2:" << test2 << ", test2 len:" << test2.size() << std::endl;

    std::string::iterator i;  // so add interator to the base class to get the interator
    std::string::const_iterator j; // if your not going to change the string this one is read only
    for( i = test2.begin(); i < test2.end(); i++){
	/// *i = 'a'; // can rewrite it this way
	std::cout << *i << std::endl;
    }


    // to go throug it backwards
    std::string::reverse_iterator r;
    for( r = test2.rbegin(); r < test2.rend(); r++){
	///std::cout << *r << std::endl;
    }


    // cbegin returns the const value begin , like cend is to end


    // to get a pointer to root data and
    const char* stuff = test2.c_str(); // it returns a const char* - same as what data() returns 
    
    std::cout << "stuff:" << stuff << std::endl;

    // to convert to a vector
    std::vector<char> vec;

    for(char x : test2 ){
	vec.push_back(x);
    }

    std::cout << std::endl;
    for(char x : vec){
	std::cout << x;
    }
    std::cout << std::endl;    

    std::string test3;

    // to convert from a vector
    //    std::vector<char>::const_iterator vi;

    std::vector<int>::iterator vipper;
    
    for( auto vip : vec ){
	test3.append(1,vip);
    }
    std::cout << "test3:" << test3 << std::endl;
    
    return 0;


    

    std::string input("abba is a great band");
    std::string output(input);
    std::reverse(output.begin(), output.end());
    std::cout << output << std::endl;
    
    return 0;
}
