#include <iostream>

using namespace std;
class Rocky{
    int num;
    int buck;
public:

    // setup the current members to be initialized
    Rocky() : num(9), buck(10) {
	cout << __PRETTY_FUNCTION__ << endl;
    }


    // make a constructor call it's default constructor first
    Rocky(int beef) : Rocky() {
	num = beef;
    }


    // So I can add one to another
    Rocky operator + (Rocky& tmp){
	Rocky aNewOne;
	// now read out number add it to the tmp passed in and return a new one
	aNewOne.num = tmp.num + num;
	return aNewOne;
    }


    // This is how I overload the << operator so I can print crap out
    friend std::ostream &operator <<(std::ostream &os, const Rocky& dude){
	os << "  " << dude.num << ":" << dude.buck << "   ";
	return os;
    }

};
