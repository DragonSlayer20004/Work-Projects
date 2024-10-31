#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Zombie.h"
#include "Human.h"

using namespace std;

int battle(Human*, Zombie*);

int main()
{
	srand((unsigned long)time(NULL));
	
	//Create Zombie
	string zombieName;
	int zombieStrength;
	cout << "Enter zombie's name: ";
	getline(cin, zombieName);
	cout << "Enter zombie's strength: ";
	cin >> zombieStrength;
	Zombie* zombie = new Zombie(zombieName, zombieStrength);
	getchar(); //Grab newline character
	
	//Create Human
	string humanName;
	int humanStrength;
	cout << "Enter human's name: ";
	getline(cin, humanName);
	cout << "Enter human's strength: ";
	cin >> humanStrength;
	Human* human = new Human(humanName, humanStrength);
	getchar(); //Grab newline character
   
   //Simulates battle between the human and the zombie
	battle(human, zombie); 

	delete zombie;  
	delete human;

	return 0;
}

/**
 * The battle method takes in a Zombie pointer and a Human pointer.
 * They will take turns fighting until one of them is dead.
 * The human goes first.
 **/

int battle(Human* human, Zombie * zombie)
{
	int zombieStrength = zombie->getStrength(); // zombie's strength
	int humanStrength = human->getStrength(); //  human's strength
	int turn = 0;  // whose turn is it
	int damage;

	while( zombieStrength > 0 && humanStrength > 0 )
	{	
		cout << "Turn " << (turn + 1) << ": ";
	
		if( turn % 2 == 0 )
		{
			damage = human->makeAttack();
			zombieStrength = zombie->hurt(damage);
			cout << human->getName();
			cout << " hit " <<
			zombie->getName();
		}	
		else
		{
			damage = zombie->makeAttack();		
			humanStrength = human->hurt(damage);
			cout << zombie->getName();
			cout << " hit " <<
			human->getName();
		}
		
		cout << " for " << damage << " points of damage." << endl;		
		turn++;
	}
	
	cout << endl;

	if(zombieStrength < 0 )
	{
		cout << "The zombie " << zombie->getName();
		cout << " was defeated by the human " << human->getName();
		cout << "!"<<endl;
		return 1;
	}
	else
	{
		cout << "The human " << human->getName();
		cout << " was defeated by the zombie " << zombie->getName();
		cout << "!" << endl;
		return 0;
	}  
}
