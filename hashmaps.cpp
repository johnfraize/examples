#include <iostream>
#include <map>
#include <list>




std::map<std::string,int>::iterator findNextOldest(int olderbrother, std::map<std::string,int> & m ){

    std::map<std::string,int>::iterator i;
    std::map<std::string,int>::iterator iret;

    std::map<int,std::map<std::string,int>::iterator> dmap;
    std::map<int,std::map<std::string,int>::iterator>::iterator dmap_i;


    int nextoldest = 0;

    for(i=m.begin();i!=m.end();++i){

	if( i->second < olderbrother ){
	    dmap[olderbrother - i->second] = i; // save the delta
	    if( nextoldest < i->second ){
		nextoldest = i->second;
		iret = i;
	    }
	}
	//	std::cout << mit->first << " birtday is " << mit->second << std::endl;
    }

    
    return iret;
}


int main(int argc, char* argv[]){

    std::map<std::string,int> mymap;
    std::map<std::string,int>::iterator mit;

    std::list<std::map<std::string>::iterator,int> ordered;
    std::list<std::map<std::string>::iterator,int>::iterator> ordered_i;

    mymap["justin"]=1983;
    mymap["ryan"]=1987;
    mymap["sam"]=1995;
    mymap["isaac"]=1997;
    mymap["josh"]=1999;
    mymap["gabe"]=2001;


    // now print out everbodys birthday
    for(mit=mymap.begin();mit!=mymap.end();mit++){
	std::cout << mit->first << " birtday is " << mit->second << std::endl;
    }

    // notice that they are not in order
    // how would I print them out in order them?
    // 1) create an ordered list
    // 2) print out the ordered list
    int oldest = 100000;
    // create an ordered list of iterators
    while( ordered.size() < mymap.size() ){
	std::map<std::string,int>::iterator nextkid = findNextOldest(oldest,mymap);
	oldest = nextkid->second;
	ordered.push_back(nextkid);
    }

    // now print them out in order
    for(ordered_i=ordered.begin();ordered_i!=ordered.end();++ordered_i){
	//	std::cout << ordered_i->second->first << " birtday is " << ordered_t->second->second << std::endl;
    }

    
    

    return 0;
}
