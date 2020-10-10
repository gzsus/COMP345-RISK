//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//								Continent.cpp - Adrian Marquez
//
//////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Territory.h"

/////////////////// Necessary classes
class Player;
class Bonus;

class Continent
{

private:
	/////////////////////////////////// Member atributes /////////////////////////////////////
	std::vector<Territory*> territories;
	std::string continent_name;
	Player* owner;
	Bonus* bonus;


public:
	/////////////////////////////////// Constructors/Destructor /////////////////////////////////////
	Continent();
	Continent(std::string given_name);
	Continent(std::string given_name,  Bonus *given_bonus);
	Continent(const Continent& given_continent);
	~Continent();


	/////////////////////////////////// Sets and gets /////////////////////////////////////
	void set_territories(std::vector<Territory*> given_vector);
	std::vector<Territory*> get_territories();
	void set_name(std::string given_name);
	std::string get_name();
	void set_owner(Player* given_owner);
	Player* get_owner();
	void set_bonus(Bonus* given_bonus);
	Bonus* get_bonus();


	/////////////////////////////////// Territories manipulation /////////////////////////////////////
	int index_territory(Territory* given_territory);
	bool add_territory(Territory* given_territory);
	void add_territory(Territory* given_territories[], int arr_size);
	bool remove_territory(Territory* given_territory);
	void show_territories();


	/////////////////////////////////// Other methods /////////////////////////////////////
	friend std::ostream& operator<<(std::ostream&, const Continent&);
	void operator=(Continent* c);
};

