how to build
clear; g++ -std=c++11 goldpots.cpp >& err; head err

Pots of gold game: Two players A & B. There are pots of gold arranged in a line, each containing some gold coins (the players can see how many coins are there in each gold pot - perfect information). They get alternating turns in which the player can pick a pot from one of the ends of the line. The winner is the player which has a higher number of coins at the end. The objective is to "maximize" the number of coins collected by A, assuming B also plays optimally. A starts the game. 

The idea is to find an optimal strategy that makes A win knowing that B is playing optimally as well. How would you do that? 

At the end I was asked to code this strategy!


