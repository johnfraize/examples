#include <iostream>
#include <thread>
#include <mutex>

const int HowMuch = 11;

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

class MessageThing{
    int num_things;
    public:
    MessageThing() {

    }
    std::queue<std::string> messagesReceived;
    std::string receiveMessage(){
        
    }
    void idleLoop() {
        while (true) {
            std::string message = receiveMessage();
            if (!message.empty()) {
                std::cout << "Received message: " << message << std::endl;
            }

          


        }
    }


    void queueMessage(const std::string& message) { 
          messagesReceived.push(message);
    }
};






int main(int argc, char* argv[] ){
    std::thread first(silver);
    std::thread second(gold,3);

    MessageThing tingOne;
    MessageThing tingTwo;


    tingOne.queueMessage("Hello from tingOne!");
    tingTwo.queueMessage("Hello from tingTwo!");

    tingOne.idleLoop();
    tingTwo.idleLoop();


    return 0;
}









