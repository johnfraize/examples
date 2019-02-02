#include <iostream>
#include <thread>
#include <mutex>

const int HowMuch = 10;

using namespace std::chrono_literals;


struct Box{
    explicite Box(int num) : num_things{num} {}
    int number_things;
    std::mutex m;
};




void silver(){

    for(int j=0;j<HowMuch;j++){
	std::cout << "silver:" << j << std::endl;
	std::this_thread::sleep_for(1s);
    }

}

void gold(int n){
    for(int j=0;j<HowMuch;j++){
	std::cout << "gold:" << j << std::endl;
	std::this_thread::sleep_for(2s);	
    }
}

int main(int argc, char* argv[] ){
    std::thread first(silver);
    std::thread second(gold,3);

    first.join();
    second.join();

    return 0;
}
