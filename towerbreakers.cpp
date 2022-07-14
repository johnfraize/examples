#include <vector>
#include <iostream>
/**


Two players are playing a game of Tower Breakers! Player  always moves first, and both players always play optimally.The rules of the game are as follows:

Initially there are  towers.
Each tower is of height .
The players move in alternating turns.
In each turn, a player can choose a tower of height  and reduce its height to , where  and  evenly divides .
If the current player is unable to make a move, they lose the game.
Given the values of  and , determine which player will win. If the first player wins, return . Otherwise, return .

Example. 

There are  towers, each  units tall. Player  has a choice of two moves:
- remove  pieces from a tower to leave  as 
- remove  pieces to leave 

Let Player  remove . Now the towers are  and  units tall.

Player  matches the move. Now the towers are both  units tall.

Now Player  has only one move.

Player  removes  pieces leaving . Towers are  and  units tall.
Player  matches again. Towers are both  unit tall.

Player  has no move and loses. Return .

Function Description

Complete the towerBreakers function in the editor below.

towerBreakers has the following paramter(s):

int n: the number of towers
int m: the height of each tower
Returns

int: the winner of the game
Input Format

The first line contains a single integer , the number of test cases.
Each of the next  lines describes a test case in the form of  space-separated integers,  and .

Constraints

Sample Input

STDIN   Function
-----   --------
2       t = 2
2 2     n = 2, m = 2
1 4     n = 1, m = 4
Sample Output

2
1
Explanation

We'll refer to player  as  and player  as 

In the first test case,  chooses one of the two towers and reduces it to . Then  reduces the remaining tower to a height of . As both towers now have height ,  cannot make a move so  is the winner.

In the second test case, there is only one tower of height .  can reduce it to a height of either  or .  chooses  as both players always choose optimally. Because  has no possible move,  wins.

Welcome to the dark side!
We've introduced a new Dark Mode for a better coding experience.


Try Dark Mode
Dismiss the introduction
Language
C++


More
12345678910111213141516171819202122

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}

**/



void play(std::vector<int> & towers ){


    // we only got here because we are playable
    std::vector<std::pair<int,int>> freeTowers;

    // check that we can simply take the last one
    for(int i=0;i<towers.size();i++){
	if( int v = towers[i] > 1 ){
	    freeTowers.push_back(std::pair<int,int>(i,v)); // hold index and value
	}
    }

    // if only one left take the last one
    if( freeTowers.size() == 1){
	int j = freeTowers[0].first;
	towers[j] = 1;
    }

    // if there are two left take only half
    for(int i=0;i<freeTowers.size();i++){
	int sample = freeTowers[i].second;
	// want to enter the biggest number possible that is even
	if( !(sample % 2) ){
	    sample /= 2;
	}
	// put back half
	towers[freeTowers[i].first] = sample;
    }
    
    for(int i=0;i<towers.size();i++){
	if( towers[i] > 1 ){
	    towers[i] = 1;
	    break;
	}
    }
}

bool canMove(std::vector<int> towers ){

    for( auto t : towers ){
	if( t != 1 ){
	    return true;
	}
    }

    return false;
}

template <typename T>
void dumpVec(std::vector<T> myVec){
    int idx = 0;
    for( auto v : myVec ){
	std::cout << "[" << idx++ << "]:" << v << std::endl;
    }
}

int towerBreakers(int n, int m){
    int winner = 0;

    bool first = true; // or not 

    // construct the towers
    std::vector<int> towers(n); // reserve n towers for speed

    for(int i=0;i<m;i++){
	towers[i] = m;
    }

    int turn = 1;

    dumpVec(towers);
    
    // play the game 
    while( turn++ < 7 ){

	std::cout << "player:" << (first ? 1 : 2) << "'s turn" << std::endl;

	play(towers);
	if( canMove(towers) == false){
	    winner = first ? 1 : 2;
	    break;
	}

	first = !first;
	dumpVec(towers);
    }
    
    return winner;
}


int main(int argc, char* argv[]){

    std::vector<std::pair<int,int>> games = { {4,6}, {3,3} };

    int towers;
    int height;
    std::cout << std::endl;

    for(std::pair<int,int> g : games ){

	towers = g.first;
	height = g.second;
	int winner = towerBreakers(towers,height);
	
	std::cout << "towers:"<< towers
		  << ", height:" << height
		  << ", winner:" << winner
		  << std::endl;
	    
    }



}
