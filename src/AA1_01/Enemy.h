#pragma once

#include <iostream>
#include <string.h>
#include <vector>
#include <sstream>
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"

namespace playerTypes{
	struct weapon
	{
		std::string name;
		std::string type;
		int range;

	};
}


class Enemy
{
public:

	std::string name;
	int attack;
	int health; 
	int defense;
	int exp_given;
	std::vector<playerTypes::weapon> weapons; 
	Enemy();
	~Enemy();
};

