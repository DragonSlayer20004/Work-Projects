#include <iostream>
#include <cstdlib>
#include "Zombie.h"

using namespace std;

/* Default Constructor */
Zombie::Zombie(void)
{
  name = "Blearghh";
  strength = ZOMBIE_DEFAULT_STRENGTH;
}

/**
 * Store the input name into the  member variable name, and do the same with strength. 
 **/
Zombie::Zombie(const string& name, int strength)
{	
	this->name = name;
	this->strength = strength;
}


/**
 * Deconstructor
 * Nothing really needs to be done here.			
 **/
Zombie::~Zombie(void)
{ 
}


/**
 * Return the zombie's name.
 **/
string Zombie::getName()
{	
	return name;
}


/**
 * Return the zombie's strength.
 **/
int Zombie::getStrength()
{	
	return strength;
}


/** 
 * Reduce the zombie's strength by the damage parameter.
 * Return the new strength.
 **/
int Zombie::hurt(int damage)
{
	strength -= damage;
	return strength;
}

/**
 * Generate a random number between 1 and 10
 * which is the power of the attack.  Return this value.
 **/

int Zombie::makeAttack()
{   
	int damage = (rand() % 10) + 1;
	return damage;
}
  
