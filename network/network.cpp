// okay my hacks for facebook
#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <nlohmann/json.hpp>
#include <mutex>
#include <thread>

void someWorker(){
    std::mutex m;
    boost::posix_time::seconds onTheJob(3);
    auto tid = std::this_thread::get_id();

    m.lock();
    std::cout << "working on the job:" << tid << std::endl;
    m.unlock();

    boost::this_thread::sleep(onTheJob);
    m.lock();
    std::cout << "nighty night:" << tid << std::endl;
    m.unlock();
}



int main(int argc, char* argv[]){
    std::mutex m;
    boost::thread myThread(someWorker); // create a thread
    boost::thread myThread2(someWorker); // create a thread

    std::cout << "main waiting--" << std::endl;

    myThread.join();
    myThread2.join();


    std::ostringstream dude;

    dude << "always look on" <<
	" bright side"  <<
	" of life" << std::endl;


     std::cout << dude.str() << std::endl;

    nlohmann::json stuff = "{\"coffe\": true }"_json;

    //    auto parsed = nlohmann::json::parse(stuff);
    
    
    //    std::cout << "main done:"  <<  parsed.dump() <<std::endl;    

    return 0;
}
