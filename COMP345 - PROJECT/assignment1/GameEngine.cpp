#include "GameEngine.h"
#include "GameEngine.h"
#include "MapLoader.h"
#include "Player.h"
#include <fstream>
#include<iostream>
#include<filesystem>



/////////////////////////////////// Constructors/Destructor /////////////////////////////////////
GameEngine::GameEngine() :  game_map()
{}
GameEngine::~GameEngine()
{
	delete game_map;
	game_map = NULL;
}
GameEngine::GameEngine(const GameEngine& old_copy)
{
	this->game_map = new Map(*(old_copy.game_map));
}



/////////////////////////////////// Sets and gets /////////////////////////////////////
//used to pick a map
std::string GameEngine::getmap()
{
	std::vector<std::string> maps;
	std::cout << "Let's Pick a map!\n\n";
	try {
		for (auto& p : std::filesystem::directory_iterator("map_dir"))
			maps.push_back(p.path().string());
	}
	catch (int e) {std::cout << "I/O error";}

	//need to remove the first 8 characters to show only map and not path when asking for choice
	for (int i = 0; i < maps.size(); i++)
	{
		std::cout << "Map #"<<i<<" -> "<<maps[i].substr(8)<<"\n";
	}
	std::cout << "Please Enter the number of the map which you would like to select: ";
	int map_number;
	std::cin>> map_number;
	std::string selected_map = maps[map_number];

	//empty vectors to avoid memory leak
	maps.clear();

	return selected_map;
}
Map* GameEngine::loadmap(std::string map)
{
	std::string map_name = map.substr(8);
	map.append("\\");
	map.append(map_name);
	map.append(".map");
	std::cout << map;
	try
	{
		MapLoader* loader = new MapLoader(map);
		//for efficiency drop the loader and keep only a pointer to the map
		Map *loaded_map = new Map(*(loader->getmap()));
		//then dereference the map for the loader since it is now kept by the pointer above
		loader->setmap(NULL);
		//now we delete the loader without deleting the map
		delete loader;
		return loaded_map;
	}
	catch (int e) { std::cout << "error"; }

	
}
GameEngine& GameEngine::operator=(const GameEngine& oldengine)
{
	this->game_map = new Map(*(oldengine.game_map));
	return *this;
}


std::ostream& operator<<(std::ostream& ostrm, const GameEngine& game_engine)
{
	return ostrm << "Game Engine Map: \n (" << game_engine.game_map << ")";
}



int main()
{
	std::cout << "-----Welcom To Risk!-----\n\n";
	GameEngine game;
	std::string mapfile;
	mapfile = game.getmap();
	Map* loaded_map = game.loadmap(mapfile);

	std::cout << "\n\n";
	int num_players = 0;
	do
	{
		std::cout << "-----Please type the number of players that will play----- [2-5]: ";
		std::cin >> num_players;
	} while (num_players < 2 || num_players > 5);
	//create the appropriate number of players.
	std::vector<Player*>players;
	for (int i = 0; i < num_players; i++) { players.push_back(new Player(0)); } //the player.h is strange it does not really make sense for each player to store the number of players...
	


	std::cout << "\n\n";
	char P_obs, S_obs;
	do
	{
		std::cout << "-----PHASE OBSERVER ON?----- [y/n]: ";
		std::cin >> P_obs;
		std::cout << "-----GAME STATISTICS OBSERVER ON?----- [y/n]: ";
		std::cin >> S_obs;
	} while ((P_obs!='y' && P_obs!='n' ) || (S_obs != 'y' && S_obs != 'n'));


	//taking care of memory leak
	delete loaded_map;
	loaded_map = NULL;
	//the player destructor is broken !-----------------------------------------------------------------
	/*for (int i = 0; i < num_players; i++) { delete players[i]; }
	for (int i = 0; i < num_players; i++) { players[i] = NULL; }

	game.~GameEngine();*/

	return 0;
}