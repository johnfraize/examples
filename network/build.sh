g++ -Wall -fsanitize=address -std=c++17 -pthread \
-lboost_date_time -lboost_regex -lboost_system -lboost_thread -lboost_timer -lboost_filesystem \
network.cpp -o network
