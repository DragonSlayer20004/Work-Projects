#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Human.h"

using namespace std;


/* Default Constructor */
Human::Human(void)
{
  name = "Bruce Campbell";
  strength = HUMAN_DEFAULT_STRENGTH;
}


/**
 * Store the input name into the  member variable name, and do the same with strength.
 **/
Human::Human(const string &name, int strength)
{
	this->name = name;
	this->strength = strength;
}

/**
 * Deconstructor
 * Nothing really needs to be done here.	
 **/
Human::~Human(void)
{

}

/**
 * return the human's name.
 **/
string Human::getName()
{
	return name;
}

/**
 * Return the strength variable.
 **/
int Human::getStrength()
{
	return strength;
}

/** 
 * If the human's strength is less than HUMAN_DEFAULT_STRENGTH, increase their strength by 1.
 * Then reduce the human's strength by the damage parameter.
 * Return the new strength.
 **/
int Human::hurt(int damage)
{
	if(strength < HUMAN_DEFAULT_STRENGTH){
		strength++;
	}
	
	strength -= damage;
	return strength;
}


/**
 * Generate a random number between 1 and 10.
 * If the number is between 2 and 9, inclusive, return the value.
 * If the number is 1, it's a critical miss.  Print "*Critical miss!* " and return 0.
 * If the number is 10, it's a critical hit.  Print "*Critical hit!* " and return 20.
 **/

int Human::makeAttack()
{
	int damage = (rand() % 10) + 1;
	
	if(damage >= 2 && damage <= 9){
		return damage;
	} else if(damage == 1){
		cout << "*Critical miss!* ";
		return 0;
	} else if(damage == 10){
		cout << "*Critical hit!* ";
		return 20;
	} else {
		cout << "Invalid Damage case: " << damage;
		return damage;
	}
}


