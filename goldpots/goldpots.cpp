#include <iostream>
#include <vector>

using namespace std;

// return choose the first one or last one
int playerB(vector<unsigned> & pots){
	if( pots.at(0) >= pots.at(pots.size()-1) ){
		return 0; 
	}
	else{
		return pots.size()-1;
	}
}

int playerA(vector<unsigned> & pots){
	return playerB(pots);
}



int main(int argc, char* argv[]){



	// Okay the gold pots
	vector<unsigned> pots {10,21,5,9,11,15,2,9};

	if( pots.size() & 1){
		cerr << "not even number of pots" << endl;
		return -1;
	}

	vector<unsigned>::const_iterator i;

	for(i=pots.begin();i!=pots.end();++i){

		cout << " this is it " << *i << endl;
	}

	unsigned sumA = 0;
	unsigned sumB = 0;

	auto funp = playerA;
	unsigned* sump;

	// The game master
	for(int turn = 0;pots.size();turn++){
		// a picks first
		if( turn & 1 ){
			sump = &sumB;
			funp = playerB;
		}
		else{
			sump = &sumA;
			funp = playerA;
		}

		int index = funp(pots);
		*sump += pots.at(index);

		pots.erase(pots.begin()+index);

		

		cout << "sz " << pots.size() << ", sumA:" << sumA << ", sumB:" << sumB << endl;

	}




	return 0;
}