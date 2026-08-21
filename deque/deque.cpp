#include <algorithm>
#include <deque>
#include <iostream>


bool mycompareInts(int a, int b){
  return(a > b);
}


class Song{
public:
  std::string name;
  std::string play;
  int capoPosition;

  Song( std::string nameIn, std::string keyIn = "unknown", int capoIn = 0 ) {
    name = nameIn;
    play = keyIn;
    capoPosition = capoIn;
  }

  // need a copy operator
  //  Song operator = (Song & tmp){
  //    return Song(name,play,capoPosition);
  //  }

};


bool myCompareSongs(Song a, Song b){
  return a.capoPosition > b.capoPosition;
}


int main() {

  // some ints 
  std::deque<int> mynums {10,58,33,1,2,3,77,3,3,3,5};

  // sort the whole thing default from smallest to biggest
  std::sort(mynums.begin(),mynums.end());

  std::cout << "smallest to biggest ";
    
  for( int j : mynums ){
    std::cout << j << " ";
  }    

  std::cout << std::endl;
    
  // sort using my own comparitor from biggest to smallest
  std::sort(mynums.begin(),mynums.end(),mycompareInts);

  std::cout << "from biggest to smallest ";

  for( int j : mynums ){
    std::cout << j << " ";
  }

  for( int i=0;i<mynums.size();i++ ){
    std::cout << "mynum:" << mynums[i];
  }

  std::cout << std::endl;


  Song apple("dude","C",3);

  Song orange = apple;

  std::cout << "apple:" << apple.play << std::endl;
  std::cout << "orange:" << orange.play << std::endl;  


  std::deque<Song> songs =
    {
     Song("Running on Empty","E"),
     Song("Stand Tall","G",1),
     Song("Walking Clean","D"),
     Song("Feed My Lambs","Bm",0),     
     Song("Streets of London","C",3)          
    };

  std::cout << __LINE__ << " " << songs[0].name << std::endl;
  for( Song s : songs ){
    std::cout << __LINE__ << " " << s.name << s.capoPosition << std::endl;
  }

  ///  std::deque<Song>::iterator sp;

  for( auto sp = songs.begin();sp!=songs.end();sp++){
    std::cout << "song:" << sp->name
	      << " play:" << sp->play
	      << " capo:" << sp->capoPosition
	      << std::endl;
  }

  std::cout << std::endl;
  std::sort(songs.begin(),songs.end(),myCompareSongs);

  for( auto sp = songs.begin();sp!=songs.end();sp++){
    std::cout << "song:" << sp->name
	      << " play:" << sp->play
	      << " capo:" << sp->capoPosition
	      << std::endl;
  }


  std::cout << "before" << std::endl;
  // easy way to iterate over a deque using a range-based for loop
  for( Song s : songs ){
    std::cout << "song:" << s.name
        << " play:" << s.play
        << " capo:" << s.capoPosition
        << std::endl;
  } 


  // to insert a new element into the deque, you can use the push_back() method. For example:
  songs.push_front(Song("New Song","A",2));

  std::cout << "after push_back" << std::endl;

  // to remove an element from the deque, you can use the erase() method. For example, to remove the first element:
  //songs.erase(songs.begin());


  std::cout << "after errase" << std::endl;
   for( Song s : songs ){
    std::cout << "song:" << s.name
        << " play:" << s.play
        << " capo:" << s.capoPosition
        << std::endl;
  } 

  // to read the forth element of the deque, you can use the front() method. For example:
  std::cout << "forth element:" << songs.at(3).name << std::endl;

  // to sort the deque, you can use the std::sort() function from the <algorithm> header. For example:
  //std::sort(songs.begin(), songs.end(), myCompareSongs);

  // to remove and return the element removed from the front of the deque, you can use the pop_front() method. For example:
  Song removedSong = songs.front();
  songs.pop_back();
  std::cout << "removed song:" << removedSong.name << std::endl;

  songs.push_front(Song("New Song 2","B",1));






    
  return 0;
}
