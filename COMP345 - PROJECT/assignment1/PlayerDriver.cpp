#include "Player.h"
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

void run(Player *pPlayer);

int main() {
    //welcome message
    cout << "Welcome to the player section of WARZONE!" << endl;

    //asks the user for the total number of players
    cout << "\nWARZONE is played with between 2 and 7 players." << endl;

    //ensures that the user enters a valid number of players
    int numberOfPlayers = 0;
    while (true) {
        cout << "Enter the number of people playing today: ";

        try {
            cin >> numberOfPlayers;
            if (!(numberOfPlayers > 1 && numberOfPlayers < 8)) {
                    throw numberOfPlayers;
            }
            break;
        }
        catch (int x) {
            cout << "ERROR: The value you have entered is invalid. Please try again." << endl;
            cin.clear();
            cin.ignore(1000,'\n');
        }
    }

    //intializes each player
    vector <Player*> allPlayers;
    vector <Player*> allPlayersCopy = allPlayers;

    for (int i=0; i<numberOfPlayers; i++) {
        Player *pPlayer = new Player(numberOfPlayers);
        allPlayersCopy.push_back(pPlayer);
    }

    //executes the functions for each player
    for (int i=0; i<numberOfPlayers; i++) {
        cout << "\nPerfect! Let us begin with Player " << (i+1) << "." << endl;
        run(allPlayersCopy[i]);
    }

    //deleting the Player pointers to prevent memory leaks
    for (int i=0; i<numberOfPlayers; i++) {
        delete allPlayersCopy[i];
    }

    allPlayersCopy.clear();

    //goodbye message
    cout << "\nThank you for testing WARZONE!" << endl;
    return 0;
}

//demonstrates functionality of the Player class
void run(Player *pPlayer) {
    //outputs arbitrary territories to defend
    cout << "\nHere are your territories to defend:" << endl;
    pPlayer -> toDefend();

    //outputs arbitrary territories to attack
    cout << "\nHere are your territories to attack:" << endl;
    pPlayer -> toAttack();

    //asks the user for their order, with a maximum of five orders
    for (int i=0; i<5; i++) {
        string orderNumber;

        switch (i) {
        case 0:
            orderNumber = "first";
            break;
        case 1:
            orderNumber = "second";
            break;
        case 2:
            orderNumber = "third";
            break;
        case 3:
            orderNumber = "fourth";
            break;
        case 4:
            orderNumber = "last";
            break;
        }

        cout << "\nWhich order would you like to carry out " << orderNumber << "?" << endl;
        pPlayer -> possibleOrders();

        //ensures that the user enters a valid order
        int chosenOrder = 0;
        while (true) {
                cout << "Enter the number associated with your desired order: ";

            try {
                cin >> chosenOrder;
                if (!(chosenOrder > 0 && chosenOrder < 7)) {
                        throw chosenOrder;
                }
                break;
            }
            catch (int x) {
                cout << "ERROR: The value you have entered is invalid. Please try again." << endl;
                cin.clear();
                cin.ignore(1000,'\n');
            }
        }

        if (chosenOrder == 1) {
            Deploy *deploy = new Deploy();
            pPlayer -> issueOrder(deploy);
        }
        else if (chosenOrder == 2) {
            Advance *advance = new Advance();
            pPlayer -> issueOrder(advance);
        }
        else if (chosenOrder == 3) {
            Bomb *bomb = new Bomb();
            pPlayer -> issueOrder(bomb);
        }
        else if (chosenOrder == 4) {
            Blockade *blockade = new Blockade();
            pPlayer -> issueOrder(blockade);
        }
        else if (chosenOrder == 5) {
            Airlift *airlift = new Airlift();
            pPlayer -> issueOrder(airlift);
        }
        else if (chosenOrder == 6) {
            Negotiate *negotiate = new Negotiate();
            pPlayer -> issueOrder(negotiate);
        }
    }

    //demonstrates the functionality of the issueOrder() function
    pPlayer -> displayOrders();
    //demonstrates that each player has a hand of cards
    pPlayer -> displayHand();
    //demonstates the functionality of the stream insertion operator
    cout << *pPlayer << endl;

    //asks the user whether they want to continue the trial or terminate the program
    while (true) {
        char stop = '.';
        cout << "\nWould you like to continue with this trial? (y/n) ";
        cin >> stop;
        try {
            if (stop == 'y') {
                break;
            }
            else if (stop == 'n') {
                cout << "\nThank you for testing WARZONE!" << endl;
                std::exit(EXIT_FAILURE);
            }
            else {
                throw stop;
            }
            break;
        }
        catch (char x) {
            cout << "ERROR: The value you have entered is invalid. Please try again." << endl;
            cin.clear();
            cin.ignore(1000,'\n');
        }
    }
}
