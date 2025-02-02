#pragma once

#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <string>

#include "Player.h"
#include "Map.h"
#include "GameObservers.h"

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::numeric_limits;
using std::streamsize;

#define MAX_PLAYERS 5
# define MIN_REINFORCEMENT 3

class MainGameLoop {
private:
	bool phase;
	bool statistics;

public:
	int get_player_id(Player* p, vector <Player*> v);
	int* reinforcementPhase(vector<Player*> allPlayers, int num_players, Map* map);
	int issueOrderPhase(vector<Player*> allPlayers, int num_players, Map* map, int* reinforcements);
	int executeOrdersPhase(vector<Player*> allPlayers);
	void setPhaseObserver(bool phaseMode);
	bool getPhaseObserver();
	void setStatisticsObserver(bool statisticsMode);
	bool getStatisticsObserver();

	Player* mainGameLoop(vector<Player*> allPlayers, Map* map, bool phaseMode, bool statisticsMode);
};

//	Read an integer from the user only
static int get_integer_option() {
	int option = -1;

	while (!(cin >> option)) {
		cin.clear();              //to clear the buffer memory
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please enter a valid number";
	}
	return option;
}