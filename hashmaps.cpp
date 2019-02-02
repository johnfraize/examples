#include <algorithm>
#include <iostream>
#include <unordered_map>
int main(int argc, char* argv[]){

    std::unordered_map<std::string,int> family = {
	{"Gabe",2001},
	{"Josh",1999},
	{"Isaac",1997},
	{"Sam",1995},
	{"Ryan",1987},
	{"Justin",1983},
	{"Dina",1968},
	{"John",1958}
    };

    int j = 0;
    // to use algorthems
    for_each(family.begin(),family.end(),[&j](std::pair<std::string,int>  v)
	     {
		 std::cout << v.first << " " << v.second << ":" << j++ <<  std::endl;
	     });



    

    family.clear();

    for(std::unordered_map<std::string,int>::iterator it=family.begin();
	it!=family.end();
	++it){
	std::cout << it->first << ":" << it->second << std::endl;

    }



    return 0;
}
