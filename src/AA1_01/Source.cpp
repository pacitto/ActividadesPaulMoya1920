#include "Enemy.h"


int main()
{
	std::vector<Enemy> enemies;

	rapidxml::xml_document<> doc;
	std::ifstream file("config.xml");
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	rapidxml::xml_node<> *pRoot = doc.first_node();


	rapidxml::xml_node<> *enemy = pRoot->first_node("enemy");


	for (rapidxml::xml_node<> *enemy = pRoot->first_node("enemy"); enemy != nullptr; enemy = enemy->next_sibling())
	{
		Enemy auxEnemy;

		auxEnemy.name = enemy->first_node("name")->value(); 
		auxEnemy.attack = std::stoi(enemy->first_node("attack")->value()); 
		auxEnemy.health = std::stoi(enemy->first_node("health")->value()); 
		auxEnemy.defense = std::stoi(enemy->first_node("defense")->value());
		auxEnemy.exp_given = std::stoi(enemy->first_node("experience")->value());
		
		
		
		rapidxml::xml_node<> *weapons = enemy->first_node("weapons");
		rapidxml::xml_node<> *weapon = weapons->first_node("weapon");
		
		playerTypes::weapon weapon1;
		
		weapon1.name = weapon->first_attribute("name")->value(); 
		weapon1.type = weapon->first_node("type")->value(); 
		weapon1.range = std::stoi( weapon->first_node("range")->value());

		auxEnemy.weapons.push_back(weapon1);

		weapon = weapon->next_sibling();
	
		playerTypes::weapon weapon2;
		
		weapon2.name = weapon->first_attribute("name")->value();
		weapon2.type = weapon->first_node("type")->value();
		weapon2.range = std::stoi(weapon->first_node("range")->value());

		auxEnemy.weapons.push_back(weapon2); 

		enemies.push_back(auxEnemy); 

	}


		//system("pause"); 
		return 0 ; 
	

	
}

