#pragma once
#include "Player.h"
#include "Cards.h"
#include "Map.h"

#include <iostream>
#include <string>
#include <vector>
//using namespace std;


using std::string;
using std::ostream;
using std::vector;


class Order {
public:
	Order(); //Default constructor
	Order(Deck* new_deck);


	bool validate();
	virtual void execute();

	std::string Type;
	Deck* deck;


	Order& operator=(const Order& right); //Assignment operator

	bool getHasBeenExecuted() {
		return hasBeenExecuted;
	}

	void setHasBeenExecuted(bool executed) {
		hasBeenExecuted = executed;
	}

private:
	bool hasBeenExecuted = false;
	friend ostream& operator<<(ostream&, const Order&);
};


class Deploy : public Order {

public:
	Deploy();
	Deploy(Deck* new_deck);
	bool validate(Player* player, Territory* ter);
	void execute(Player* player, Territory* ter, int reinforcement);
	Deploy(const Deploy& d);//Copy constructor
};


class Bomb : public Order {
public:
	Bomb();
	Bomb(Deck* new_deck);
	bool validate(Player* player, Territory* target);
	void execute(Player* player, Territory* target);
	Bomb(const Bomb& b);//Copy constructor

};


class Advance : public Order {
public:
	Advance();
	Advance(Deck* new_deck);
	bool validate(Player* player, Territory* source, Territory* target);
	void execute(Player* player, Territory* source, Territory* target, int armyunits);
	Advance(const Advance& d);//Copy constructor
};


class Blockade : public Order {
public:
	Blockade();
	Blockade(Deck* new_deck);
	Blockade(Deck* new_deck,Player* neutral);
	bool validate(Player* player, Territory* target);
	void execute(Player* player, Territory* target);
	Blockade(const Blockade& bl);//Copy constructor
private:
	Player* neutral_player;
};


class Airlift : public Order {
public:
	Airlift();
	Airlift(Deck* new_deck);
	bool validate(Player* player, Territory* source, Territory* target);
	void execute(Player* player, Territory* source, Territory* target, int armyunits);
	Airlift(const Airlift& a);//Copy constructor
};


class Negotiate : public Order {
public:
	Negotiate();
	Negotiate(Deck* new_deck);
	bool validate(Player* player, Player* target_player);
	void execute(Player* player, Player* target_player);
	Negotiate(const Negotiate& n);//Copy constructor
};


class OrdersList {
public:
	~OrdersList(); //Destructor
	void remove(int position);

	void move(int currentPosition, int desiredPosition);

	void setList(vector<Order*> list);

	vector<Order*> getList();
private:
	vector<Order*> ListOfOrders;
};




