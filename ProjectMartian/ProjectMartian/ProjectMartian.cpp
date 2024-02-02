// Project Martian
// Johnathan Church

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Class prototypes
class player;
class enemy;
class weapon;
class spell;

// Helper function prototypes.
int findLowestStat(player user);
void enemyAttack(player& user, enemy target, int attackIndex);
int weaponAttack(weapon userWeapon, int strengthModifier);
void castSpell(player& user, enemy& target, int spellIndex);

// Function prototypes.
void titleScreen();
int inputValidation();
bool abilityCheck(player user, int stat, int dc);
void battle(player& user, enemy target);
bool checkClear(vector<bool> cleared);

// Room function prototypes.
void entrance(player& user);
int room1(player& user, int layout[2][5], vector<bool>& cleared, bool first);
int room2(player& user, int layout[2][5], vector<bool>& cleared, bool first);
int room3(player& user, int layout[2][5], vector<bool>& cleared, bool first);
int room4(player& user, int layout[2][5], vector<bool>& cleared, bool first);
int room5(player& user, int layout[2][5], vector<bool>& cleared, bool first);
int room6(player& user, int layout[2][5], vector<bool>& cleared, bool first);
int room7(player& user, int layout[2][5], vector<bool>& cleared, bool first);
int room8(player& user, int layout[2][5], vector<bool>& cleared, bool first);
int room9(player& user, int layout[2][5], vector<bool>& cleared, bool first);
int room10(player& user, int layout[2][5], vector<bool>& cleared, bool first);
int end(player& user);
void moveRooms(int layout[2][5], vector<bool> pathways, vector<int>& pathwaySelection);
int nextRoom(int layout[2][5], int& room, int direction);
int runRoom(player& user, int layout[2][5], vector<bool>& cleared, bool first, int room);
void generateLayout(int layout[2][5]);
vector<bool> getPathways(int room, int layout[2][5]);
void displayMap(int layout[2][5], int room);

// Weapon Class
class weapon {

public:

	// Attributes, including name and maximum and minimum damage.
	string name;
	int maxDamage;
	int minDamage;

	// This boolean determines whether the weapon deals magic damage.
	bool magical;

	// Constructors
	weapon() {

		name = "Default";
		maxDamage = 0;
		minDamage = 0;
		magical = false;

	}
	weapon(string name, int maxDamage, int minDamage, bool magical) {

		this->name = name;
		this->maxDamage = maxDamage;
		this->minDamage = minDamage;
		this->magical = magical;

	}

};

// Spell Class
class spell {

public:

	// Attributes, including the spell's name, damage, and health regeneration.
	string name;
	int maxDamage;
	int minDamage;
	int maxHeal;
	int minHeal;
	int maxRecharge;
	int minRecharge;
	int manaCost;

	// Constructor
	spell(string name, int maxDamage, int minDamage, int maxHeal, int minHeal, int maxRecharge, int minRecharge, int manaCost) {

		this->name = name;
		this->maxDamage = maxDamage;
		this->minDamage = minDamage;
		this->maxHeal = maxHeal;
		this->minHeal = minHeal;
		this->maxRecharge = maxRecharge;
		this->minRecharge = maxRecharge;
		this->manaCost = manaCost;

	}

};

// Player Class
class player {

public:

	// Attributes, including the player's name, HP, MP, AC, stats, weapon, and spells.
	string name;
	int health;
	int maxHealth;
	int mana;
	int maxMana;
	int armorClass;
	int stats[6];	// Strength, Dexterity, Constitution, Intelligence, Wisdom, Charisma
	int statModifiers[6];
	weapon userWeapon;
	vector<spell> spells;

	// Default constructor
	player() {

		name = "Astronaut";
		health = 0;
		maxHealth = 0;
		mana = 0;
		maxMana = 0;
		armorClass = 10;
		
		for (int i = 0; i < 6; i++) {

			stats[i] = 8;

		}

		for (int i = 0; i < 6; i++) {

			statModifiers[i] = -1;

		}

		userWeapon = weapon();
		spells = { };

	}

	// This function will be used during character creation to show the player their stats.
	void displayStats() {

		cout << "Strength: " << stats[0]
			 << "\nDexterity: " << stats[1]
			 << "\nConstitution: " << stats[2]
			 << "\nIntelligence: " << stats[3]
			 << "\nWisdom: " << stats[4]
			 << "\nCharisma: " << stats[5];

	}

	// Helper function for the charater creation function.
	void pointAssignment(string statName, int stat, int& points) {

		// Clear any information on screen.
		system("cls");

		// This variable is used to assign values to the stat being changed.
		int assignment = 0;

		// Ensure that the user's inputs are valid with a while loop.
		bool valid = false;
		while (!valid) {

			// Prompt the user for their point assignment.
			cout << "Points remaining: " << points
				<< "\nYour " << statName << " score: " << stats[stat]
				<< "\n\nHow many points would you like to add to " << statName << "?\n";
			assignment = inputValidation();

			if (assignment <= points && assignment >= 0) {

				// Increase the user's stat by the number of points and remove points from the total.
				stats[stat] += assignment;
				points -= assignment;
				valid = true;

			}
			else {

				system("cls");
				cout << "Please enter a valid point assignment. Press ENTER to continue.\n";
				cin.ignore();
				cin.get();
				system("cls");

			}

		}

	}

	// Character creation function: Assigns the player stats and a name, filling their attributes.
	void characterCreator() {

		// Assign default values to stats (points will be added to these values).
		for (int i = 0; i < 6; i++) {

			stats[i] = 8;

		}

		/* These variable represents the number of points the player can assign to different stats.
		   Player stats will be determined by a point-buy system. The more points a player puts into a single stat
		   the less will be left for them to use on others. */
		int points = 24;

		// Clear any information on screen.
		system("cls");

		// Prompt the user for their character's name.
		cout << "Welcome to Project Martian."
			<< "\n\nPlease enter your name.\n";
		getline(cin, name);

		// Do-while loop, in case the player decides to rethink their stats.
		bool done = false;
		do {

			// Provide the effects of each stat.
			system("cls");
			cout << "Next, we will determine your stats.\n\n"
				<< "Strength governs the amount of damage you'll deal with melee attacks.\n"
				<< "Dexterity offers higher chances to avoid enemy attacks.\n"
				<< "Constitution the total amount of hit points you will have.\n"
				<< "Intelligence determines how much mana you will have.\n"
				<< "Wisdom governs the amount of damage you'll deal with spells.\n"
				<< "Charisma offers higher chances to succesffully negotiate your way out of combat with intelligent monsters.\n\n";

			// Progress to stat assignment.
			cout << "Press ENTER to continue to the stat assignment menu.\n";
			cin.ignore();

			// Determine each statistic using the pointAssignment helper function.
			pointAssignment("Strength", 0, points);
			pointAssignment("Dexterity", 1, points);
			pointAssignment("Constitution", 2, points);
			pointAssignment("Intelligence", 3, points);
			pointAssignment("Wisdom", 4, points);
			pointAssignment("Charisma", 5, points);

			// Clear any information on screen.
			system("cls");

			// Determine the player's stat modifiers (10-11: 0, 12-13: +1, etc.).
			for (int i = 0; i < 6; i++) {

				statModifiers[i] = (stats[i] - 10) / 2;

			}

			// Display the user's stats and prompt them to confirm.
			int confirm;
			displayStats();
			cout << "\n\nEnter '1' to confirm your stats, and any other number to reassign points.\n";
			confirm = inputValidation();

			// Check whether or not to repeat the loop.
			if (confirm == 1) {

				done = true;

			}
			else {

				// Reassign default values to stats and reset points.
				for (int i = 0; i < 6; i++) {

					stats[i] = 8;

				}
				points = 24;

			}

		} while (!done);

		// Determine the player's health based on their constitution modifier.
		maxHealth = 30 + statModifiers[2] * 10;
		health = maxHealth;

		// Embed weapon selection within a loop to insure valid input.
		bool valid = false;
		while (!valid) {

			// Clear any information on screen.
			system("cls");

			// Prompt the player for their starting weapon of choice.
			int select;
			cout << "Finally, you equipped yourself with a melee weapon before arriving. What was it again?\n\n"
				<< "1. Spear: Spears deal more consistent damage, but have the potential for less. (3-6)\n"
				<< "2. Mace: Maces have the potential for both a high and low damage output. (1-10)\n"
				<< "3. Sword: Swords fall somewhere in the middle. (2-8)\n\n";
			select = inputValidation();

			switch (select) {

			case 1:

				userWeapon = weapon("Spear", 6, 3, false);
				valid = true;
				break;

			case 2:

				userWeapon = weapon("Mace", 10, 1, false);
				valid = true;
				break;

			case 3:

				userWeapon = weapon("Sword", 8, 2, false);
				valid = true;
				break;

			default:

				system("cls");
				cout << "Please enter a valid selection (1-3). Press ENTER to continue.\n";
				cin.ignore();
				cin.get();

			}

		}

	}

	// Testing function: Displays the player's attributes.
	void displayPlayer() {

		cout << "Name: " << name
		 	 << "\nHealth: " << health << "/" << maxHealth
			 << "\nMana: " << mana << "/" << maxMana
			 << "\nArmor Class: " << armorClass
		     << "\n\nStrength: " << stats[0]
			 << "\nDexterity: " << stats[1]
			 << "\nConstitution: " << stats[2]
			 << "\nIntelligence: " << stats[3]
			 << "\nWisdom: " << stats[4]
			 << "\nCharisma: " << stats[5]
			 << "\n\nName: " << userWeapon.name
			 << "\nMaximum Damage: " << userWeapon.maxDamage
			 << "\nMinimum Damage: " << userWeapon.minDamage
			 << "\nMagical Weapon: " << ((userWeapon.magical == false) ? ("No") : ("Yes"));

	}

};

// Enemy Class
class enemy {

public:

	// Attributes, including the enemy's name, description, HP, AC, stats, and attacks.
	string name;
	string description;
	int health;
	int maxHealth;
	int armorClass;
	int stats[3];	// Strength, Intelligence, Wisdom
	vector<weapon> attacks;
	
	// This boolean governs nonmagical resistance. When true, enemies take 0.5x damage from nonmagical attacks.
	bool resistance;

	// This boolean governs magical vulnerability. When true, enemies take 2x damage from magical attacks.
	bool vulnerability;

	// Constructor
	enemy(string name, string description, int health, int maxHealth, int armorClass, int stats[3], vector<weapon> attacks,
		  bool resistance, bool vulnerability) {

		this->name = name;
		this->description = description;
		this->health = health;
		this->maxHealth = maxHealth;
		this->armorClass = armorClass;
		
		for (int i = 0; i < 3; i++) {

			this->stats[i] = stats[i];

		}
		for (int i = 0; i < attacks.size(); i++) {

			this->attacks[i] = attacks[i];

		}

		this->resistance = resistance;
		this->vulnerability = resistance;

	}

};

// The main function: Program execution begins and ends here.
int main() {

	srand(time(NULL));

	player user;

	do {

		// Display the title screen.
		titleScreen();

		// Reset user spells.
		user.spells = {};

		// This vector will track which rooms the player has cleared.
		vector<bool> cleared = { false, false, false, false, false, false, false, false, false, false };

		// Take the player through character creation.
		user.characterCreator();

		// Thus begins the start of the dungeon. The entrance function will provide context, have them enter the dungeon, and provide them with magic.
		entrance(user);

		// Generate the dungeon's layout.
		int layout[2][5];
		generateLayout(layout);

		// The user should begin in the room with coordinates [1][2] and be able to navigate from there.
		int currentRoom = layout[1][2];
		int direction = runRoom(user, layout, cleared, true, layout[1][2]);

		while (user.health > 0 && !checkClear(cleared)) {
			
			// Determine the location of and run the next room.
			direction = runRoom(user, layout, cleared, false, nextRoom(layout, currentRoom, direction));

		}

		if (user.health > 0) {

			end(user);

			// This line just ensures that the game will return to the title screen so the player can play again.
			user.health = 0;

		}

	} while (user.health <= 0);

}

// Title screen function: Displays the game's title and allows the player to start or exit the game.
void titleScreen() {

	// These variables will be used to navigate the title screen.
	int select;
	bool start = false;
	
	while (!start) {

		// Clear any information on screen.
		system("cls");

		// Display the title screen and receive user input.
		cout << "Project Martian\n\n"
			 << "1. Start Game\n"
			 << "2. About\n"
			 << "3. Exit\n\n";
		select = inputValidation();
		cin.ignore();

		switch (select) {

			case 1:

				start = true;
				break;

			case 2:

				system("cls");
				cout << "Welcome to Project Martian v. 1.0\n"
					 << "Created by Johnathan Church\n\n"
					 << "Project Martian is a small text-based RPG wherein the player inhabits the character of\n"
					 << "an independent astronaut from Earth traveling to Mars.\n\n"
					 << "Each character's reason for traveling to the planet is totally different. Try playing around with\n"
					 << "your lowest stats; you might see some differing origins for your character.\n\n"
					 << "Throughout your adventure, you will delve into an ancient Martian temple built within the planet's\n"
					 << "largest and most iconic volcano, Olympus Mons. The temple is made up of 10 pre-made rooms, yet the\n"
					 << "layout is different each time the game is played. You are encouraged to make your own map of the dungeon\n"
					 << "to keep track of rooms you'd like to revisit or tackle later, but a map is provided should you need to see\n"
					 << "your current position within the layout.\n\n"
					 << "Have fun playing and good luck in restoring Mars to its former glory. Happy trails!\n\n"
					 << "Press ENTER to return to the main menu.\n";
				cin.get();
				break;

			case 3:

				system("cls");
				exit(0);
				break;

			default:

				cout << "\nPlease select an option! Press ENTER to continue.\n";
				cin.get();
				cin.ignore();

		}

	}

}

// Input validation function: Ensures the user does not enter anything other than an integer.
int inputValidation() {

	int input;
	bool valid = false;

	do {

		// Receives user input.
		cin >> input;

		// If the user enters a value that cannot be stored inside an int variable, such as a string, it will catch the fail state.
		if (cin.fail() || input < 0) {

			// Prompt the user for valid input.
			cout << "\nPlease enter a positive number to proceed!\n";
			cin.clear();	// Clears the fail state.
			cin.ignore();	// Clears cin so new input can be received.

		}
		else {

			// If the user enters a value that can be stored inside an int variable, their input is valid and input can be returned.
			valid = true;

		}

	} while (!valid);

	return input;

}// Battle function: The user and the target should alternate turns until one of them reaches a health value of 0 or lower.

// Ability check and battle function.
bool abilityCheck(player user, int stat, int dc) {

	int roll = (rand() % 20 + 1) + user.statModifiers[stat];

	if (roll >= dc) {

		return true;

	}

	return false;

}
void battle(player& user, enemy target) {

	// This variable will be used to navigate the battle menu.
	int select;

	// The battle will loop until either the player or the enemy's health drops to 0.
	do {

		// Player's Turn

		// Embed the player's turn within a loop to ensure valid selections.
		bool valid = false;
		while (!valid) {

			// Clear any information on screen.
			system("cls");

			// Display battle menu.
			cout << user.name << " v. " << target.name << "\n\n"					// Player v. Enemy
				 << "HP: " << user.health << "/" << user.maxHealth << "\n"
				 << "MP: " << user.mana << "/" << user.maxMana << "\n\n"            // HP: 50/50
				 << "1. " << user.userWeapon.name << "\n"                           // MP: 50/50
				 << "2. " << "Magic" << "\n"
				 << "3. " << "Information\n\n";                                     // 1. Weapon    (Uses the player's weapon)
																					// 2. Magic     (Displays a menu of spells)
			// Accept the player's selection.                                       // 3. Inventory (Allows the player to inspect the enemy)
			select = inputValidation();

			// This switch-case will perform the action corresponding with the user's selection.
			switch (select) {

				case 1: {

					// Clear the menu.
					system("cls");

					// Roll to hit.
					int hit = (rand() % 18) + 3 + user.statModifiers[0];

					// Test whether the hit lands or not (compare hit with the target's armor class).
					if (hit >= target.armorClass) {

						// Determine the attack's damage.
						int damage = weaponAttack(user.userWeapon, user.statModifiers[0]);
						
						// Check whether the target is resistant/vulnerable.
						if (target.vulnerability == true && user.userWeapon.magical == true) {

							damage *= 2;

						}
						else if (target.resistance == true && user.userWeapon.magical == false) {

							damage *= 0.5;

						}

						// Display the attack's effects.
						cout << user.name << " used " << user.userWeapon.name << ", dealing " << damage << " damage.";

						// Subtract the appropriate hit points from the enemy.
						target.health -= damage;

					}
					else {

						// Inform the player of their missed attack.
						cout << user.name << " used " << user.userWeapon.name << ", but it missed!";

					}

					// Progress to the enemy's turn.
					cout << "\n\nPress ENTER to continue.\n";
					cin.ignore();
					cin.get();

					valid = true;
					break;

				}

				case 2: {

					// Clear the menu.
					system("cls");

					/* This vector contains a list of valid entries to ensure that castSpell
						   is not called if the player changes their mind about spellcasting. */
					vector<int> validEntries;

					// Print the spell menu and accept the player's selection.
					cout << "Spell Menu\n\n";
					for (int i = 0; i < user.spells.size(); i++) {

						cout << i + 1 << ". " << user.spells[i].name << "\n";

						validEntries.push_back(i + 1);

					}
					cout << "\n";

					// Accept the player's selection.
					cout << "Enter any other number to return to the battle menu.\n";
					select = inputValidation();
					bool exit = true;

					for (int i = 0; i < validEntries.size(); i++) {
						
						if (select == validEntries[i]) {

							exit = false;

						}

					}

					// If the player decided to cast a spell
					if (!exit) {

						// Clear the spell menu.
						system("cls");

						// Check if the player has enough mana.
						if (user.spells.at(select - 1).manaCost > user.mana) {

							cout << "You don't have enough mana..."
								 << "\n\nPress ENTER to return to the battle menu.\n";
							cin.ignore();
							cin.get();
							break;

						}

						// Have the player cast a spell.
						castSpell(user, target, select - 1);

						// Progress to the enemy's turn.
						cout << "\n\nPress ENTER to continue.\n";
						cin.ignore();
						cin.get();

						valid = true;
						break;

					}

					break;

				}

				case 3:

					// Clear the menu.
					system("cls");

					// Print the enemy's description.
					cout << target.description
						<< "\n\nPress ENTER to continue.\n";
					cin.ignore();
					cin.get();

					valid = true;
					break;

				default:

					// Clear the menu.
					system("cls");

					// The player did not make a valid selection. Let them know and return them to the menu.
					cout << "Please select a valid option. Press ENTER to return to the menu.\n";
					cin.ignore();
					cin.get();

			}

		}

		// Test if the enemy has been defeated and exit the loop.
		if (target.health <= 0) {

			break;

		}

		// Enemy's Turn

		// This random integer will determine which of the enemy's actions it takes.
		int action = rand() % target.attacks.size();

		// Clear any information on screen and perform the enemy's attack.
		system("cls");
		enemyAttack(user, target, action);

		// Return to the battle menu
		cout << "\n\nPress ENTER to continue.\n";
		cin.get();


	} while (user.health > 0 && target.health > 0);

	if (user.health > 0) {

		system("cls");
		cout << "You won against " << target.name << "!\n\n"
			 << "Press ENTER to continue.\n";
		cin.get();

	}
	else {

		system("cls");
		cout << "You were vanquished by " << target.name << "...\n\n"
			 << "Press ENTER to return to the main menu.\n";
		cin.get();

	}

}

// Clear check function: Acts as the flag for the game's end state.
bool checkClear(vector<bool> cleared) {

	int clearCount = 0;

	for (int room = 0; room < cleared.size(); room++) {

		if (cleared[room] == true) {

			clearCount++;

		}

	}

	if (clearCount == 10) {

		return true;

	}

	return false;

}

// Helper function for the entrance function.
int findLowestStat(player user) {

	int lowestStat = user.stats[0];
	int lowestIndex = 0;

	for (int stat = 1; stat < 6; stat++) {

		if (user.stats[stat] < lowestStat) {

			lowestStat = user.stats[stat];
			lowestIndex = stat;

		}

	}

	return lowestIndex;

}

// Helper functions for the battle function.
void enemyAttack(player& user, enemy target, int attackIndex) {

	// Roll to hit.
	int hit = (rand() % 18) + 5;

	// Test whether the hit lands or not (compare hit with the target's armor class).
	if (hit >= user.armorClass) {

		// Determine the attack's damage.
		int damage = weaponAttack(target.attacks[attackIndex], 0);

		// Display the attack's effects.
		cout << target.name << " used " << target.attacks.at(attackIndex).name << ", dealing " << damage << " damage.";

		// Subtract the appropriate hit points from the player.
		user.health -= damage;

	}
	else {

		// Inform the player of the enemy's missed attack.
		cout << target.name << " used " << target.attacks.at(attackIndex).name << ", but it missed!";

	}

}
int weaponAttack(weapon userWeapon, int strengthModifier) {

	int damage = (rand() % (userWeapon.maxDamage - userWeapon.minDamage + 1) + userWeapon.minDamage) + strengthModifier;

	return damage;

}
void castSpell(player& user, enemy& target, int spellIndex) {

	spell userSpell = user.spells[spellIndex];
	
	// Deplete the user's mana by the spell's mana cost.
	user.mana -= userSpell.manaCost;

	// Detect whether the spell is a healing, damaging, or recharging spell.
	if (userSpell.maxHeal > 0) {

		// Determine the spell's healing effects.
		int heal = (rand() % (userSpell.maxHeal - userSpell.minHeal + 1) + userSpell.minHeal) + user.statModifiers[4];

		// Clear the menu.
		system("cls");

		// Print the effects of the spell.
		cout << user.name << " casted " << userSpell.name << ", healing for " << heal << " hit points.";
		user.health += heal;

		// Make sure the player's health does not exceed their max health.
		if (user.health > user.maxHealth) {

			user.health = user.maxHealth;

		}

	}
	else if (userSpell.maxDamage > 0) {

		// Roll to hit.
		int hit = (rand() % 18) + 3 + user.statModifiers[4];

		// Test whether the hit lands or not (compare hit with the target's armor class).
		if (hit >= target.armorClass) {

			// Determine the spell's damage.
			int damage = (rand() % (userSpell.maxDamage - userSpell.minDamage + 1) + userSpell.minDamage) + user.statModifiers[4];

			// Check whether the target is vulnerable.
			if (target.vulnerability == true) {

				damage *= 2;

			}

			// Clear the menu.
			system("cls");

			// Print the effects of the spell.
			cout << user.name << " casted " << userSpell.name << ", dealing " << damage << " points of damage.";
			target.health -= damage;

		}
		else {

			// Inform the player of their missed attack.
			cout << user.name << " casted " << userSpell.name << ", but it missed!";

		}

	}
	else {

		// Determine the spell's recharging effects.
		int recharge = (rand() % (userSpell.maxRecharge - userSpell.minRecharge + 1) + userSpell.minRecharge);

		// Clear the menu.
		system("cls");

		// Print the effects of the spell.
		cout << user.name << " casted " << userSpell.name << ", recharging for " << recharge << " mana points.";
		user.mana += recharge;

		// Make sure the player's mana does not exceed their max mana.
		if (user.mana > user.maxMana) {

			user.mana = user.maxMana;

		}

	}

}

/* Room functions : A total of ten rooms will make up the dungeon. Each room is unique and has its own function.
 
					At the start of the program, the generateLayout function will randomly choose a layout for these rooms.

					The getPathways function uses the generated 2D array and the room number to find out how many paths
					the player can take in that room.

					The map function will display the array as a map that shows the player's current position.
					
					The entrance function provides the player with context to their adventure and grants them magic. */
void entrance(player& user) {

	// Clear any information on screen.
	system("cls");

	// This variable holds the index of the user's lowest stat and will affect their reason for crash landing on Mars.
	int lowestStat = findLowestStat(user);

	// Print the introduction.
	switch (lowestStat) {

	case 0:

		cout << "You are an astronaut from planet Earth. Caught in a solar flare, your spacecraft lost power above"
			<< "\nthe red planet, Mars. Failing to pull away from the planet's graviational pull, you were forced"
			<< "\nto make a crash landing at the base of the solar system's largest volcano: Olympus Mons.";
		break;

	case 1:

		cout << "You are an astronaut from planet Earth. Attempting to maneuver between an asteroid field, you were"
			<< "\nunable to weave between the space debris and your spacecraft was damaged. You were forced to make"
			<< "\na crash landing on the red planet, Mars, at the base of the solar system's largest volcano:"
			<< "\nOlympus Mons.";
		break;

	case 2:

		cout << "You are an astronaut from planet Earth. Drifting through space, you were unable to withstand the"
			<< "\ngravitational forces of space travel and began falling to the surface of the red planet, Mars,"
			<< "\nmaking a crash landing at the base of the solar system's largest volcano: Olympus Mons.";
		break;

	case 3:

		cout << "You are an astronaut from planet Earth. Accidentally drifting too close to the gravitational pull of"
			<< "\nthe red planet, Mars, you were forced to make a crash landing at the base of the solar system's"
			<< "\nlargest volcano: Olympus Mons.";
		break;

	case 4:

		cout << "You are an astronaut from planet Earth. Your captain, having succumbed to the gravitational forces"
			<< "\nduring takeoff, is now slumped over dead in the cockpit. A rookie, too inexperienced to fly the craft"
			<< "\nyourself, you were forced to make a crash landing on the red planet, Mars, at the base of the solar"
			<< "\nsystem's largest volcano: Olympus Mons.";
		break;

	case 5:

		cout << "You are a refugee from planet Earth. Ostracized from society, you were condemned to drift through the vast"
			<< "\nreaches of space in search for a new home. Having lost power long ago, your spacecraft drifted too close to"
			<< "\nthe gravitational pull of the red planet, Mars, forcing you to make a crash landing at the base of the solar"
			<< "\nsystem's largest volcano: Olympus Mons.";
		break;

	default:

		cout << "Error. Could not determine the player's lowest stat.";
		exit(0);

	}

	cout << "\n\nClimbing out of the wreckage, you stand beforea grand Martian temple carved into the side"
		<< "\nof the mountain. Stone monuments to the ancient Martians line the temple steps, looking down"
		<< "\non you as you climb to the entrance, seeking shelter from the incoming dust storm.";

	cout << "\n\nPress ENTER to enter the temple doors.\n";
	cin.ignore();
	cin.get();

	// Clear any information on screan.
	system("cls");

	// Room description and magic rune.
	cout << "You push open the temple doors to reveal a grand chamber with grand statues of the Martians lining the walls."
		 << "\nIn the center of the room is a raised dais, atop which a glowing green rune shines through the red dirt."
		 << "\nThe walls are lined with large stone pillars carved into what resemble humanoid figures, but foreign and harsh."
		 << "\nYou walk towards the dais in the central room, mesmerized by the bright green glow illuminating the path ahead."
		 << "\nAs you step upon the pedestal, the rune alights to glow even brighter, surrounding you with the magic of the ancient"
		 << "\nMartians."
		 << "\n\nYou look down, suddenly finding it difficult to control the mechanisms of your body, and, as if by instinct, launch a "
		 << "\nflaming ball of energy towards the walls of the temple, destroying one of the pillars ahead."
		 << "\n\nYou dash forward, hoping to make it through the large stone doorway towards the back wall before it crumbles."
		 << "\nYou just make it. The doorway crumbles behind you as you find yourself in a new space, grappling with the"
		 << "\nimplications of your new abilities.";

	cout << "\n\nYou have been granted the gift of magic. Your intelligence stat influences your mana, the magical currency which governs"
		 << "\nyour capacity to cast spells.";

	// Calculate the player's mana.
	user.maxMana = 30 + user.statModifiers[3] * 10;
	user.mana = user.maxMana;

	// Assign the player two starting spells: Fireball and Restoration.
	spell fireball("Fireball", 10, 1, 0, 0, 0, 0, 8);
	spell restoration("Restoration", 0, 0, 12, 4, 0, 0, 6);
	user.spells.push_back(fireball);
	user.spells.push_back(restoration);

	// Inform the player of their new abilities and progress to the first generated room.
	cout << "\n\nYou learned a new spell! Fireball allows you to burn enemies in a wave of flame."
		 << "\nYou learned a new spell! Restoration allows you to heal during the heat of battle."
		 << "\n\nPress ENTER to continue to the first room.\n";
	cin.get();

}
int room1(player& user, int layout[2][5], vector<bool>& cleared, bool first) {

	// Clear any information on screen.
	system("cls");

	// Check whether the player has cleared this room.
	if (!cleared[0]) {

		/* Enemies: Rockling(2)
	       The variables/objects below will make up the enemy's attacks and description, respectively. */
		int rocklingStats[3] = { 11, 2, 2 };
		vector<weapon> attacks;
		weapon tackle("Tackle", 6, 1, false);
		string rocklingDesc = "A small stone construct animated by Martian magic, these creatures are lacking in features\n"
							  "other than a rough stone exterior, two stubby legs, and swirling green magic.\n\n"
							  "It seems as though this creature may be weak to magic...";
		enemy rockling("Rockling", rocklingDesc, 15, 15, 13, rocklingStats, attacks, false, true);
		rockling.attacks.push_back(tackle);

		// Provide the user with a brief introduction to the room.
		cout << "You enter the room and briefly take a look around. Upon first glance, you notice a large tainted rug\n"
			 << "at the center, as well as a mural carved into the walls depicting the Martians' colonization of the\n"
			 << "mountain. As you gather familiarity with your surroundings, two pieces of the wall begin to glow and break\n"
			 << "away. The two constructs begin charging you. You'll have to defend yourself.\n\n"
			 << "Press ENTER to begin battling.\n";
		if (!first)
			cin.ignore();
		cin.get();

		// Have the player battle the two enemies.
		battle(user, rockling);
		system("cls");

		if (user.health <= 0)
			return -1;

		cout << "Panting, you turn to focus your attention to the second rockling.\n\n"
			 << "Press ENTER to begin battling.\n";
		cin.get();

		battle(user, rockling);

		if (user.health <= 0)
			return -1;

	}
	else {

		// Provide the user with a reminder about the room.
		cout << "You enter the room with the mural and rug.\n\n"
			 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}

	// Check the dungeon layout for available pathways.
	vector<bool> pathways = getPathways(1, layout);

	// Give the player a list of ways to go, a map, and the option to look around. select will be used to navigate these menus.
	int select;
	bool checked = false;

	do {

		// Clear any information on screen.
		system("cls");

		cout << "Having vanquished the monsters, you have some time to look around the room.\n\n"
		 << "1. Move\n"
		 << "2. Check Map\n"
		 << "3. (Intelligence) Look Around\n\n";
		select = inputValidation();

		switch (select) {

			case 1: {

				// Clear any information on screen.
				system("cls");

				// This vector will store the selection values of each path the user can take.
				vector<int> pathwaySelection = { -1, -1, -1, -1 };

				moveRooms(layout, pathways, pathwaySelection);

				// Accept user input and check pathwaySelection for the correct room.
				cout << "\n";
				select = inputValidation();

				if (select == pathwaySelection[0]) {

					cleared[0] = true;
					return 0;

				}
				else if (select == pathwaySelection[1]) {

					cleared[0] = true;
					return 1;

				}
				else if (select == pathwaySelection[2]) {

					cleared[0] = true;
					return 2;

				}
				else if (select == pathwaySelection[3]) {

					cleared[0] = true;
					return 3;

				}

				break;

			}

			case 2:

				// Clear any information on screen.
				system("cls");

				// Display the map.
				displayMap(layout, 1);

				cout << "\nPress ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

			case 3: {

				// Clear any information on screen.
				system("cls");

				if (!checked) {

					checked = true;

					// Run the player through an ability check to see what they find.
					cout << "You look around the room, inspecting the mural and large rug.\n\n";

					if (abilityCheck(user, 3, 12)) {

						cout << "Lifting the rug, you notice a missing brick, in which lies a dusty tome with the symbol of a lightning bolt.\n\n";

						spell lightningBolt("Lightning Bolt", 12, 1, 0, 0, 0, 0, 8);
						user.spells.push_back(lightningBolt);

						cout << "You learned a new spell! Lightning Bolt allows you to call electricity upon your enemies.\n\n"
							<< "Press ENTER to return to the menu.\n";
						cin.get();
						cin.ignore();

					}
					else {

						cout << "Despite your best efforts, you are unable to find anything of note.\n\n"
							<< "Press ENTER to return to the menu.\n";
						cin.get();
						cin.ignore();

					}

				}
				else {

					cout << "You've checked this room already!\n\n"
						 << "Press ENTER to return to the menu.\n";
					cin.get();
					cin.ignore();

				}

				break;

			}

			default:

				// Clear any information on screen.
				system("cls");

				cout << "Please select an option!\n\n"
					 << "Press ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

		}
		
	} while (true);

}
int room2(player& user, int layout[2][5], vector<bool>& cleared, bool first) {

	// Clear any information on screen.
	system("cls");

	// Check whether the player has cleared this room.
	if (!cleared[1]) {

		/* Enemies: Warbling Matter
		   The variables/objects below will make up the enemy's attacks and description, respectively. */
		int warblingMatterStats[3] = { 10, 18, 14 };
		vector<weapon> attacks;
		weapon matterLash("Matter Lash", 8, 3, false);
		weapon reverberate("Reverberate", 10, 1, false);
		string warblingMatterDesc = "A rift in reality brought into life by the residual anger and hatred of the Martians imprisoned here.\n"
									"It seems to indiscriminately lash out against nearby life, hoping to bring it into its antimatter form.\n\n"
									"It seems as though this creature may be weak to magic...";
		enemy warblingMatter("Warbling Matter", warblingMatterDesc, 25, 25, 13, warblingMatterStats, attacks, false, true);
		warblingMatter.attacks.push_back(matterLash);
		warblingMatter.attacks.push_back(reverberate);

		// Provide the user with a brief introduction to the room.
		cout << "You enter the room and are met with intense vertigo as you stare upon a large pit in the center of the room.\n"
			 << "Stone chains hang from the ceiling into the pit below, at varying lengths. It looks as though this room served\n"
			 << "as a prison of some sort. As you look about the room, you notice a rising darkness from the center\n"
			 << "of the pit, as a warbling amalgamation of dark matter begins to wildly lash out at you.\n\n"
			 << "Press ENTER to begin battling.\n";
		if (!first)
			cin.ignore();
		cin.get();

		// Have the player battle the two enemies.
		battle(user, warblingMatter);
		system("cls");

		if (user.health <= 0)
			return -1;

	}
	else {

		// Provide the user with a reminder about the room.
		cout << "You enter the room with the large pit and dangling stone chains.\n\n"
			 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}

	// Check the dungeon layout for available pathways.
	vector<bool> pathways = getPathways(2, layout);

	// Give the player a list of ways to go, a map, and the option to look around. select will be used to navigate these menus.
	int select;
	bool checked = false;

	do {

		// Clear any information on screen.
		system("cls");

		cout << "Having vanquished the monster, you have some time to look around the room.\n\n"
		 << "1. Move\n"
		 << "2. Check Map\n"
		 << "3. (Intelligence) Look Around\n\n";
		select = inputValidation();

		switch (select) {

			case 1: {

				// Clear any information on screen.
				system("cls");

				// This vector will store the selection values of each path the user can take.
				vector<int> pathwaySelection = { -1, -1, -1, -1 };

				moveRooms(layout, pathways, pathwaySelection);

				// Accept user input and check pathwaySelection for the correct room.
				cout << "\n";
				select = inputValidation();

				if (select == pathwaySelection[0]) {

					cleared[1] = true;
					return 0;

				}
				else if (select == pathwaySelection[1]) {

					cleared[1] = true;
					return 1;

				}
				else if (select == pathwaySelection[2]) {

					cleared[1] = true;
					return 2;

				}
				else if (select == pathwaySelection[3]) {

					cleared[1] = true;
					return 3;

				}

				break;

			}

			case 2:

				// Clear any information on screen.
				system("cls");

				// Display the map.
				displayMap(layout, 2);

				cout << "\nPress ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

			case 3: {

				// Clear any information on screen.
				system("cls");

				if (!checked) {

					checked = true;

					// Run the player through an ability check to see what they find.
					cout << "You look around the room, inspecting the pit and what remains of the warbling matter.\n\n";

					if (abilityCheck(user, 3, 14)) {

						cout << "You take a knee and begin sifting through the foggy remnants of the monster.\n"
							 << "As you do so, you feel yourself beginning to absorb its knowledge.\n\n";

						spell Distortion("Distortion", 16, 2, 0, 0, 0, 0, 10);
						user.spells.push_back(Distortion);

						cout << "You learned a new spell! Distortion allows you to shift the material forms of your enemies.\n\n"
							 << "Press ENTER to return to the menu.\n";
						cin.get();
						cin.ignore();

					}
					else {

						cout << "Despite your best efforts, you are unable to find anything of note.\n\n"
							 << "Press ENTER to return to the menu.\n";
						cin.get();
						cin.ignore();

					}

				}
				else {

					cout << "You've checked this room already!\n\n"
						 << "Press ENTER to return to the menu.\n";
					cin.get();
					cin.ignore();

				}

				break;

			}

			default:

				// Clear any information on screen.
				system("cls");

				cout << "Please select an option!\n\n"
					 << "Press ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

		}
		
	} while (true);

}
int room3(player& user, int layout[2][5], vector<bool>& cleared, bool first) {

	// Clear any information on screen
	system("cls");

	// This boolean will track whether the player has obtained the spell in this room.
	static bool spellCheck;

	// Check whether the player has cleared this room.
	if (!cleared[2]) {

		spellCheck = false;

		// Provide the user with a brief introduction to the room.
		cout << "You enter the room and brace for attack, yet are met with nothing. Bathed in golden light,\n"
			 << "you look upon yet another rune, this time glowing a bright yellow rather than green.\n\n"
			 << "At one side of the room, rather than a doorway, you see a small set of stone steps leading up\n"
			 << "to an altar. It seems as though this room may have been used for some kind of worship, as\n"
			 << "twin statues can be seen praying at either side of the altar.\n\n"
		 	 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}
	else {

		// Provide the user with a reminder about the room.
		cout << "You enter the room with the golden rune and altar.\n\n"
			 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}

	// Check the dungeon layout for available pathways.
	vector<bool> pathways = getPathways(3, layout);

	// Give the player a list of ways to go, a map, and the option to look around. select will be used to navigate these menus.
	int select;
	bool checked = false;

	do {

		// Clear any information on screen.
		system("cls");

		cout << "With this brief respite, you have some time to look around the room.\n\n"
		 	 << "1. Move\n"
		 	 << "2. Check Map\n"
		 	 << "3. Check Rune\n\n";
		select = inputValidation();

		switch (select) {

			case 1: {

				// Clear any information on screen.
				system("cls");

				// This vector will store the selection values of each path the user can take.
				vector<int> pathwaySelection = { -1, -1, -1, -1 };

				moveRooms(layout, pathways, pathwaySelection);

				// Accept user input and check pathwaySelection for the correct room.
				cout << "\n";
				select = inputValidation();

				if (select == pathwaySelection[0]) {

					cleared[2] = true;
					return 0;

				}
				else if (select == pathwaySelection[1]) {

					cleared[2] = true;
					return 1;

				}
				else if (select == pathwaySelection[2]) {

					cleared[2] = true;
					return 2;

				}
				else if (select == pathwaySelection[3]) {

					cleared[2] = true;
					return 3;

				}

				break;

			}

			case 2:

				// Clear any information on screen.
				system("cls");

				// Display the map.
				displayMap(layout, 3);

				cout << "\nPress ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

			case 3:

				// Clear any information on screen.
				system("cls");

				// Provide the player details about the rune's effects.
				cout << "You approach the rune glowing in the center of the room. As you step inside,\n"
					 << "you feel yourself growing comfortable and calm as your wounds are fully healed.\n\n";

				// Check whether the player has obtained Greater Restoration yet.
				if (!spellCheck) {

					cout << "Filled with wisdom, you learn a new spell! Greater Restoration allows you to heal for a larger amount of hit points.\n\n";

					// Add Greater Restoration to the user's spell list.
					spell greaterRestoration("Greater Restoration", 0, 0, 24, 8, 0, 0, 10);
					user.spells.push_back(greaterRestoration);

					spellCheck = true;

				}
				
				// Set the player's health to their max health.
				user.health = user.maxHealth;

				cout << "Press ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

		}

	} while (true);

}
int room4(player& user, int layout[2][5], vector<bool>& cleared, bool first) {

	// Clear any information on screen
	system("cls");

	// This boolean will track whether the player has obtained the spell in this room.
	static bool spellCheck;

	// Check whether the player has cleared this room.
	if (!cleared[3]) {

		spellCheck = false;

		// Provide the user with a brief introduction to the room.
		cout << "You enter the room and brace for attack, yet nothing seems to come to you.\n"
			 << "Looking about the room, you notice that it is significantly smaller than the others. On\n"
			 << "the walls are murals of humanoid figures in a meditative state, and in the center of the room\n"
			 << "is a glowing green rune.\n\n"
		 	 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}
	else {

		// Provide the user with a reminder about the room.
		cout << "You enter the room with the green rune.\n\n"
			 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}

	// Check the dungeon layout for available pathways.
	vector<bool> pathways = getPathways(4, layout);

	// Give the player a list of ways to go, a map, and the option to look around. select will be used to navigate these menus.
	int select;
	bool checked = false;

	do {

		// Clear any information on screen.
		system("cls");

		cout << "With this brief respite, you have some time to look around the room.\n\n"
		 	 << "1. Move\n"
		 	 << "2. Check Map\n"
		 	 << "3. Check Rune\n\n";
		select = inputValidation();

		switch (select) {

			case 1: {

				// Clear any information on screen.
				system("cls");

				// This vector will store the selection values of each path the user can take.
				vector<int> pathwaySelection = { -1, -1, -1, -1 };

				moveRooms(layout, pathways, pathwaySelection);

				// Accept user input and check pathwaySelection for the correct room.
				cout << "\n";
				select = inputValidation();

				if (select == pathwaySelection[0]) {

					cleared[3] = true;
					return 0;

				}
				else if (select == pathwaySelection[1]) {

					cleared[3] = true;
					return 1;

				}
				else if (select == pathwaySelection[2]) {

					cleared[3] = true;
					return 2;

				}
				else if (select == pathwaySelection[3]) {

					cleared[3] = true;
					return 3;

				}

				break;

			}

			case 2:

				// Clear any information on screen.
				system("cls");

				// Display the map.
				displayMap(layout, 4);

				cout << "\nPress ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

			case 3:

				// Clear any information on screen.
				system("cls");

				// Provide the player details about the rune's effects.
				cout << "You approach the rune glowing in the center of the room. As you step inside,\n"
				 	 << "you feel yourself growing comfortable and calm as your mana fully recharges.\n\n";

				// Check whether the player has obtained Greater Restoration yet.
				if (!spellCheck) {

					cout << "Filled with wisdom, you learn a new spell! Meditation allows you a chance to regain some mana.\n\n";

					// Add Meditation to the user's spell list.
					spell meditation("Meditation", 0, 0, 0, 0, 8, 1, 3);
					user.spells.push_back(meditation);

					spellCheck = true;

				}

				// Set the player's health to their max health.
				user.mana = user.maxMana;

				cout << "Press ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

		}

	} while (true);

}
int room5(player& user, int layout[2][5], vector<bool>& cleared, bool first) {

	// Clear any information on screen.
	system("cls");

	// Check whether the player has cleared this room.
	if (!cleared[4]) {

		/* Enemies: Shadow
		   The variables/objects below will make up the enemy's attacks and description, respectively. */
		int shadowStats[3] = { 14, 12, 10 };
		vector<weapon> attacks;
		weapon shadowWhip("Shadow Whip", 8, 3, false);
		string shadowDesc = "The undead spectre of an ancient Martian creature inhabits this incorporeal monster.\n"
							"Watch out for its necrotic whip!";
		enemy shadow("Shadow", shadowDesc, 25, 25, 13, shadowStats, attacks, false, true);
		shadow.attacks.push_back(shadowWhip);

		// Provide the user with a brief introduction to the room.
		cout << "You enter the room and immediately notice that the floor is covered in a shallow pool of water. Looking around\n"
			 << "at the walls, you aren't able to see anything of note within this room, so you begin walking to another doorway.\n"
			 << "As you cross the midpoint of the room, you hear a deep laughter as the room runs cold. Turning around, you see\n"
			 << "your own shadow rising to do battle.\n\n"
			 << "Press ENTER to begin battling.\n";
		if (!first)
			cin.ignore();
		cin.get();

		// Have the player battle the two enemies.
		battle(user, shadow);
		system("cls");

		if (user.health <= 0)
			return -1;

	}
	else {

		// Provide the user with a reminder about the room.
		cout << "You enter the room with the shallow pool of water lining the ground.\n\n"
			 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}

	// Check the dungeon layout for available pathways.
	vector<bool> pathways = getPathways(5, layout);

	// Give the player a list of ways to go, a map, and the option to look around. select will be used to navigate these menus.
	int select;
	bool checked = false;

	do {

		// Clear any information on screen.
		system("cls");

		cout << "With this brief respite, you have some time to look around the room.\n\n"
		 	 << "1. Move\n"
		 	 << "2. Check Map\n\n";
		select = inputValidation();

		switch (select) {

			case 1: {

				// Clear any information on screen.
				system("cls");

				// This vector will store the selection values of each path the user can take.
				vector<int> pathwaySelection = { -1, -1, -1, -1 };

				moveRooms(layout, pathways, pathwaySelection);

				// Accept user input and check pathwaySelection for the correct room.
				cout << "\n";
				select = inputValidation();

				if (select == pathwaySelection[0]) {

					cleared[4] = true;
					return 0;

				}
				else if (select == pathwaySelection[1]) {

					cleared[4] = true;
					return 1;

				}
				else if (select == pathwaySelection[2]) {

					cleared[4] = true;
					return 2;

				}
				else if (select == pathwaySelection[3]) {

					cleared[4] = true;
					return 3;

				}

				break;

			}

			case 2:

				// Clear any information on screen.
				system("cls");

				// Display the map.
				displayMap(layout, 5);

				cout << "\nPress ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

		}

	} while (true);

}
int room6(player& user, int layout[2][5], vector<bool>& cleared, bool first) {

	// Clear any information on screen.
	system("cls");

	// select will be used to navigate the multiple menus of this room.
	int select;

	// Check whether the player has cleared this room.
	if (!cleared[5]) {

		// Encounter: Crystal Eye
		cout << "You enter the room and immediately notice a large chamber stretching towards the top of the mountain. Above you,\n"
			 << "you can see the starry night sky of the Milky Way. In the center of the room is a large pit and, hovering\n"
			 << "above it, a crystal eye staring you down.\n\n"
			 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

		// These variables will be used to continue the encounter and/or check for an end state.
		bool end = false;
		bool wisdomCheck = false;
		bool charismaCheck = false;
		bool turn = true;
		int distanceTracker = 0;

		while (!end && user.health > 0) {

			// Clear any information on screen.
			system("cls");
			
			cout << "Please select an option.\n\n"
				 << "1. Move\n"
				 << "2. (Wisdom) Insight\n"
				 << "3. (Charisma) Talk Down\n\n";
			select = inputValidation();

			switch (select) {

				// Movement: Moving a certain direction increases distanceTracker by 1 or -1. Reaching 3 or -3 defeats the crystal eye.
				case 1:

					// Clear the menu.
					system("cls");

					cout << "Which way would you like to move? Enter any other integer to return to the encounter menu.\n\n"
						 << "1. Left\n"
						 << "2. Right\n\n";
					select = inputValidation();

					if (select == 1) {

						system("cls");
						cout << "You begin running to the left around the pit. The eye follows you.\n\n"
							 << "Press ENTER to continue.\n";
						cin.ignore();
						cin.get();

						distanceTracker++;
						turn = false;

					}
					else if (select == 2) {

						system("cls");
						cout << "You begin running to the right around the pit. The eye follows you.\n\n"
							 << "Press ENTER to continue.\n";
						cin.ignore();
						cin.get();

						distanceTracker--;
						turn = false;

					}

					// Check distanceTracker for a sufficient value.
					if (abs(distanceTracker) == 3) {

						// Clear any information on screen.
						system("cls");

						cout << "You watch as the crystal eye grows dizzy and begins spinning uncontrollably.\n"
							 << "The eye begins crumbling and falls into the abyss, with some pieces landing on the ground.\n\n"
							 << "Press ENTER to continue.\n";
						cin.get();

						end = true;

					}

					break;

				case 2:

					// Clear the menu.
					system("cls");

					// Check the player's wisdom with a roll to see if they can get any hints.
					if (abilityCheck(user, 4, 12) && !wisdomCheck) {

						cout << "You notice the eye tracking your movements. You think perhaps you could make it dizzy somehow?\n\n"
							 << "Press ENTER to continue.\n";
						cin.get();
						cin.ignore();
						wisdomCheck = true;
						turn = false;

					}
					else if (wisdomCheck) {

						cout << "You've already tried that...\n\n"
							 << "Press ENTER to continue.\n";
						cin.get();
						cin.ignore();

					}
					else {

						cout << "You notice the eye tracking your movements, but aren't able to devise much of a strategy...\n\n"
							 << "Press ENTER to continue.\n";
						cin.get();
						cin.ignore();
						wisdomCheck = true;
						turn = false;

					}

					break;

				case 3:

					// Clear the menu.
					system("cls");

					// Check the player's charisma with a roll to see if they can talk down the monster.
					if (abilityCheck(user, 5, 20) && !charismaCheck) {

						cout << "You do your best to talk the monster down, hoping to avoid its hostility.\n"
							 << "Surprisingly, the eye seems to leave your gaze, before disassembling into colored light and\n"
							 << "shooting into the sky.\n\n"
							 << "Press ENTER to continue.\n";
						cin.get();
						cin.ignore();
						charismaCheck = true;
						end = true;

					}
					else if (charismaCheck) {

						cout << "You've already tried that...\n\n"
							 << "Press ENTER to continue.\n";
						cin.get();
						cin.ignore();

					}
					else {

						cout << "You do your best to talk the monster down, hoping to avoid its hostility.\n"
							 << "Unsurprisingly, the being stares even more intensely...\n\n"
							 << "Press ENTER to continue.\n";
						cin.get();
						cin.ignore();
						charismaCheck = true;
						turn = false;

					}

					break;

				default:

					// Clear the menu.
					system("cls");

					cout << "Please select a valid option. Press ENTER to continue.\n";
					cin.ignore();
					cin.get();

					break;

			}

			// Crystal Eye's turn
			if (!turn && !end) {

				// This weapon will be the crystal eye's primary attack.
				weapon crystalBeam;

				// Roll to hit the player's AC.
				int hit = (rand() % 18) + 5;

				if (hit >= user.armorClass) {

					// Clear any information on screen.
					system("cls");

					crystalBeam = weapon("Crystal Beam", 10, 2, true);
					int damage = weaponAttack(crystalBeam, 0);
					user.health -= damage;

					cout << "The eye fires a beam of light towards you, dealing " << damage << " damage.\n\n"
						 << "Press ENTER to continue.\n";
					cin.ignore();
					turn = true;

				}
				else {

					// Clear any information on screen.
					system("cls");

					cout << "The eye fires a beam of light towards you, but misses.\n\n"
						 << "Press ENTER to continue.\n";
					cin.ignore();
					turn = true;

				}

			}

		}

		if (user.health <= 0) {

			system("cls");
			cout << "You were vanquished by the Crystal Eye...\n\n"
				<< "Press ENTER to return to the main menu.\n";
			cin.get();
			return -1;

		}

	}
	else {

		// Provide the user with a reminder about the room.
		cout << "You enter the room with the broken shards of the crystal eye.\n\n"
			 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}

	// Check the dungeon layout for available pathways.
	vector<bool> pathways = getPathways(6, layout);

	// Give the player a list of ways to go, a map, and the option to look around. select will be used to navigate these menus.
	bool checked = false;

	do {

		// Clear any information on screen.
		system("cls");

		cout << "Having vanquished the monster, you have some time to look around the room.\n\n"
			 << "1. Move\n"
			 << "2. Check Map\n"
			 << "3. (Intelligence) Look Around\n\n";
		select = inputValidation();

		switch (select) {

			case 1: {
			
				// Clear any information on screen.
				system("cls");

				// This vector will store the selection values of each path the user can take.
				vector<int> pathwaySelection = { -1, -1, -1, -1 };

				moveRooms(layout, pathways, pathwaySelection);

				// Accept user input and check pathwaySelection for the correct room.
				cout << "\n";
				select = inputValidation();

				if (select == pathwaySelection[0]) {

					cleared[5] = true;
					return 0;

				}
				else if (select == pathwaySelection[1]) {

					cleared[5] = true;
					return 1;

				}
				else if (select == pathwaySelection[2]) {

					cleared[5] = true;
					return 2;

				}
				else if (select == pathwaySelection[3]) {

					cleared[5] = true;
					return 3;

				}

				break;

			}

			case 2:

				// Clear any information on screen.
				system("cls");

				// Display the map.
				displayMap(layout, 6);

				cout << "\nPress ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

			case 3: {

				// Clear any information on screen.
				system("cls");

				if (!checked) {

					checked = true;

					// Run the player through an ability check to see what they find.
					cout << "You look around the room, inspecting the shards littered about.\n\n";

					if (abilityCheck(user, 3, 18)) {

						cout << "You begin inspecting the shards. As you lift one, you begin absorbing the eyes memories.\n\n";

						spell eyeBeam("Eye Beam", 15, 3, 0, 0, 0, 0, 10);
						user.spells.push_back(eyeBeam);

						cout << "You learned a new spell! Eye Beam allows you to fire beams of radiant light at your enemies.\n\n"
							<< "Press ENTER to continue.\n";
						cin.get();
						cin.ignore();

						system("cls");

						cout << "You continue looking around the room and locate a shard suspiciously shaped like a sword...\n\n"
							 << "Enter '1' to drop your current weapon and take the Crystal Sword, and any other number to decline.\n\n";
						select = inputValidation();

						if (select == 1) {

							// Clear the menu.
							system("cls");

							// Switch the player's weapon.
							user.userWeapon.name = "Crystal Sword";
							user.userWeapon.maxDamage = 12;
							user.userWeapon.minDamage = 4;
							user.userWeapon.magical = true;

							// Inform the player of their choice.
							cout << "You toss your weapon aside and pick up the Crystal Sword! Press ENTER to continue.\n";
							cin.ignore();

						}
						else {

							// Clear the menu.
							system("cls");

							// Inform the player of their choice.
							cout << "You decide to stick with your weapon. Press ENTER to continue.\n";
							cin.ignore();

						}

					}
					else if (abilityCheck(user, 3, 12)) {

						cout << "You begin inspecting the shards. As you lift one, you begin absorbing the eyes memories.\n\n";

						spell eyeBeam("Eye Beam", 15, 3, 0, 0, 0, 0, 10);
						user.spells.push_back(eyeBeam);

						cout << "You learned a new spell! Eye Beam allows you to fire beams of radiant light at your enemies.\n\n"
							<< "Press ENTER to return to the menu.\n";
						cin.get();
						cin.ignore();

					}
					else {

						cout << "Despite your best efforts, you are unable to find anything of note.\n\n"
							<< "Press ENTER to return to the menu.\n";
						cin.get();
						cin.ignore();

					}

				}
				else {

					cout << "You've checked this room already!\n\n"
						 << "Press ENTER to return to the menu.\n";
					cin.get();
					cin.ignore();

				}

				break;

			}

			default:

				// Clear any information on screen.
				system("cls");

				cout << "Please select an option!\n\n"
					 << "Press ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

		}
		
	} while (true);

}
int room7(player& user, int layout[2][5], vector<bool>& cleared, bool first) {

	// Clear any information on screen.
	system("cls");

	// Check whether the player has cleared this room.
	if (!cleared[6]) {

		/* Enemies: Martian Golem
		   The variables/objects below will make up the enemy's attacks and description, respectively. */
		int golemStats[3] = { 16, 8, 8 };
		vector<weapon> attacks;
		weapon rockLaunch("Rock Launch", 8, 1, false);
		weapon slam("Slam", 6, 2, false);
		string golemDesc = "Ancient Martian magic has animated this statue to life. It packs a serious punch, but\n"
						   "looks as though it may be weak to magic.\n\n";
		enemy golem("Golem", golemDesc, 30, 30, 15, golemStats, attacks, false, true);
		golem.attacks.push_back(rockLaunch);
		golem.attacks.push_back(slam);

		// Provide the user with a brief introduction to the room.
		cout << "As you enter the room, you see statues of presumably the ancient Martians lining the walls. As you\n"
			 << "begin walking through, you watch as one of them comes to life. Its lumbering stone form crumbles and\n"
			 << "cracks as it breaks away from the walls, the ground shaking with every step as it walks towards you.\n\n"
			 << "Press ENTER to begin battling.\n";
		if (!first)
			cin.ignore();
		cin.get();

		// Have the player battle the enemy.
		battle(user, golem);
		system("cls");

		if (user.health <= 0)
			return -1;

	}
	else {

		// Provide the user with a reminder about the room.
		cout << "You enter the room with the crumbled remains of the golem..\n\n"
			 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}

	// Check the dungeon layout for available pathways.
	vector<bool> pathways = getPathways(7, layout);

	// Give the player a list of ways to go, a map, and the option to look around. select will be used to navigate these menus.
	int select;
	bool checked = false;

	do {

		// Clear any information on screen.
		system("cls");

		cout << "Having vanquished the monster, you have some time to look around the room.\n\n"
		 << "1. Move\n"
		 << "2. Check Map\n\n";
		select = inputValidation();

		switch (select) {

			case 1: {

				// Clear any information on screen.
				system("cls");

				// This vector will store the selection values of each path the user can take.
				vector<int> pathwaySelection = { -1, -1, -1, -1 };

				moveRooms(layout, pathways, pathwaySelection);

				// Accept user input and check pathwaySelection for the correct room.
				cout << "\n";
				select = inputValidation();

				if (select == pathwaySelection[0]) {

					cleared[6] = true;
					return 0;

				}
				else if (select == pathwaySelection[1]) {

					cleared[6] = true;
					return 1;

				}
				else if (select == pathwaySelection[2]) {

					cleared[6] = true;
					return 2;

				}
				else if (select == pathwaySelection[3]) {

					cleared[6] = true;
					return 3;

				}

				break;

			}

			case 2:

				// Clear any information on screen.
				system("cls");

				// Display the map.
				displayMap(layout, 7);

				cout << "\nPress ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

		}

	} while (true);

}
int room8(player& user, int layout[2][5], vector<bool>& cleared, bool first) {

	// Clear any information on screen.
	system("cls");

	// mirror will be used to track whether the player has checked the mirror, and select will be used to navigate menus.
	bool mirror = false;
	int select;

	// Check whether the player has cleared this room.
	if (!cleared[7]) {

		// Provide the user with a brief introduction to the room.
		cout << "Walking into this room, you notice one, and only one, defining feature. At the back of the room,\n"
			 << "there lies a mirror. Approaching it, you notice that your reflection is static, staring at you and unmoving.\n\n"
			 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

		// Give the player their options: smash the mirror to obtain Mind Lash or speak to themselves to obtain Cosmic Self.
		system("cls");
		cout << "Would you like to interact with the mirror?\n\n"
			 << "1. (Strength) Break Mirror\n"
			 << "2. (Charisma) Speak to Yourself\n\n"
			 << "Enter any other number to ignore the mirror.\n\n";
		select = inputValidation();

		// Test select to check what the player decided.
		switch (select) {

			case 1:

				system("cls");
				mirror = true;

				if (abilityCheck(user, 0, 10)) {

					cout << "Frustrated with the mirror's lack of response, you throw a punch into the glass and watch\n"
						 << "as the mirror shatters into countless pieces. You look into the void left behind and see your\n"
						 << "reflection walk away from you, before the void closes and the wall returns to normal.\n\n"
						 << "You learned a new spell! Mind Lash allows you to channel psychic energy in a violent attack against your enemies.\n\n"
						 << "Press ENTER to continue.\n";
					cin.ignore();
					cin.get();

					spell mindLash("Mind Lash", 15, 5, 0, 0, 0, 0, 12);
					user.spells.push_back(mindLash);

				}
				else {

					cout << "Frustrated with the mirror's lack of response, you throw a punch into the glass. You aren't\n"
						 << "quite able to break the strong glass, however, and watch as your reflection disappears from view\n"
						 << "as the mirror fades into an ordinary pane of glass, acting as a window to the wall behind it.\n\n"
						 << "Press ENTER to continue.\n";
					cin.ignore();
					cin.get();

				}

				break;

			case 2:

				system("cls");
				mirror = true;

				if (abilityCheck(user, 5, 10)) {

					cout << "You begin speaking with your reflection, asking it questions about yourself. You notice\n"
						 << "that it only responds to yes or no questions with simple nods and shakes. Despite this,\n"
						 << "you're able to have quite the insightful conversation with yourself.\n\n"
						 << "You learned a new spell! Cosmic Self allows you to tap into your own consciousness and recharge your mana.\n\n"
						 << "Press ENTER to continue.\n";
					cin.ignore();
					cin.get();

					spell cosmicSelf("Cosmic Self", 0, 0, 0, 0, 15, 5, 8);
					user.spells.push_back(cosmicSelf);

				}
				else {

					cout << "You begin speaking with your reflection, asking it questions about yourself. You notice\n"
						 << "that it only responds to yes or no questions with simple nods and shakes. After a frustrating series\n"
						 << "of questions and answers, you watch as your reflection frowns and begins walking out of view.\n"
						 << "The mirror fades into an ordinary pane of glass, acting as a window to the wall behind it.\n\n"
						 << "Press ENTER to continue.\n";
					cin.ignore();
					cin.get();

				}

				break;

			default:

				system("cls");

				cout << "You decide to ignore the mirror. Press ENTER to continue.\n";
				cin.ignore();
				cin.get();

		}

	}
	else {

		// Provide the user with a reminder about the room.
		cout << "You enter the room with the mirror.\n\n"
			<< "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}

	// Check the dungeon layout for available pathways.
	vector<bool> pathways = getPathways(8, layout);

	// Give the player a list of ways to go, a map, and the option to look around. select will be used to navigate these menus.
	do {

		// Clear any information on screen.
		system("cls");

		cout << "With this brief respite, you have some time to look around the room.\n\n"
			 << "1. Move\n"
			 << "2. Check Map\n"
			 << "3. Check Mirror\n\n";
		select = inputValidation();

		switch (select) {

			case 1: {

				// Clear any information on screen.
				system("cls");

				// This vector will store the selection values of each path the user can take.
				vector<int> pathwaySelection = { -1, -1, -1, -1 };

				moveRooms(layout, pathways, pathwaySelection);

				// Accept user input and check pathwaySelection for the correct room.
				cout << "\n";
				select = inputValidation();

				if (select == pathwaySelection[0]) {

					cleared[7] = true;
					return 0;

				}
				else if (select == pathwaySelection[1]) {

					cleared[7] = true;
					return 1;

				}
				else if (select == pathwaySelection[2]) {

					cleared[7] = true;
					return 2;

				}
				else if (select == pathwaySelection[3]) {

					cleared[7] = true;
					return 3;

				}

				break;

			}

			case 2:

				// Clear any information on screen.
				system("cls");

				// Display the map.
				displayMap(layout, 8);

				cout << "\nPress ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

			case 3:

				if (!mirror) {

					// Clear any information on screen.
					system("cls");

					// Give the player their options: smash the mirror to obtain Mind Lash or speak to themselves to obtain Cosmic Self.
					cout << "Would you like to interact with the mirror?\n\n"
						 << "1. (Strength) Break Mirror\n"
						 << "2. (Charisma) Speak to Yourself\n\n"
						 << "Enter any other number to ignore the mirror.\n\n";
					select = inputValidation();

					// Test select to check what the player decided.
					switch (select) {

					case 1:

						system("cls");
						mirror = true;

						if (abilityCheck(user, 0, 10)) {

							cout << "Frustrated with the mirror's lack of response, you throw a punch into the glass and watch\n"
								<< "as the mirror shatters into countless pieces. You look into the void left behind and see your\n"
								<< "reflection walk away from you, before the void closes and the wall returns to normal.\n\n"
								<< "You learned a new spell! Mind Lash allows you to channel psychic energy in a violent attack against your enemies.\n\n"
								<< "Press ENTER to continue.\n";
							cin.ignore();

							spell mindLash("Mind Lash", 15, 5, 0, 0, 0, 0, 12);
							user.spells.push_back(mindLash);

						}
						else {

							cout << "Frustrated with the mirror's lack of response, you throw a punch into the glass. You aren't\n"
								<< "quite able to break the strong glass, however, and watch as your reflection disappears from view\n"
								<< "as the mirror fades into an ordinary pane of glass, acting as a window to the wall behind it.\n\n"
								<< "Press ENTER to continue.\n";
							cin.ignore();

						}

						break;

					case 2:

						system("cls");
						mirror = true;

						if (abilityCheck(user, 5, 10)) {

							cout << "You begin speaking with your reflection, asking it questions about yourself. You notice\n"
								<< "that it only responds to yes or no questions with simple nods and shakes. Despite this,\n"
								<< "you're able to have quite the insightful conversation with yourself.\n\n"
								<< "You learned a new spell! Cosmic Self allows you to tap into your own consciousness and recharge your mana.\n\n"
								<< "Press ENTER to continue.\n";
							cin.ignore();

							spell cosmicSelf("Cosmic Self", 0, 0, 0, 0, 15, 5, 8);
							user.spells.push_back(cosmicSelf);

						}
						else {

							cout << "You begin speaking with your reflection, asking it questions about yourself. You notice\n"
								<< "that it only responds to yes or no questions with simple nods and shakes. After a frustrating series\n"
								<< "of questions and answers, you watch as your reflection frowns and begins walking out of view.\n"
								<< "The mirror fades into an ordinary pane of glass, acting as a window to the wall behind it.\n\n"
								<< "Press ENTER to continue.\n";
							cin.ignore();

						}

						break;

					default:

						system("cls");

						cout << "You decide to ignore the mirror. Press ENTER to continue.\n";
						cin.ignore();
						cin.get();

					}
				
				}
				else {

					system("cls");
					cout << "The mirror does not respond.\n\n"
						 << "Press ENTER to return to the menu.\n";
					cin.get();
					cin.ignore();

				}

				break;

		}

	} while (true);

}
int room9(player& user, int layout[2][5], vector<bool>& cleared, bool first) {

	// Clear any information on screen.
	system("cls");

	// Check whether the player has cleared this room.
	if (!cleared[8]) {

		/* Enemies: Wyrmling
	       The variables/objects below will make up the enemy's attacks and description, respectively. */
		int wyrmlingStats[3] = { 16, 8, 12 };
		vector<weapon> attacks;
		weapon bite("Bite", 6, 1, false);
		weapon wyrmsBreath("Wyrm's Breath", 8, 1, true);
		string wyrmlingDesc = "The child of a dragon, a wyrmling indicates the mother might be nearby...";
		enemy wyrmling("Wyrmling", wyrmlingDesc, 30, 30, 13, wyrmlingStats, attacks, false, false);
		wyrmling.attacks.push_back(bite);
		wyrmling.attacks.push_back(wyrmsBreath);

		// Provide the user with a brief introduction to the room.
		cout << "You enter the room and are met with the paralyzing stare of a small creature resting atop a nest\n"
			 << "of rubble. Staring you down is what's known as a wyrmling, the child of an adult dragon.\n\n"
			 << "Press ENTER to begin battling.\n";
		if (!first)
			cin.ignore();
		cin.get();

		// Have the player battle the enemy.
		battle(user, wyrmling);
		system("cls");

		if (user.health <= 0)
			return -1;

	}
	else {

		// Provide the user with a reminder about the room.
		cout << "You enter the room with the wyrmling nest.\n\n"
			 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}

	// Check the dungeon layout for available pathways.
	vector<bool> pathways = getPathways(9, layout);

	// Give the player a list of ways to go, a map, and the option to look around. select will be used to navigate these menus.
	int select;
	bool checked = false;

	do {

		// Clear any information on screen.
		system("cls");

		cout << "Having vanquished the monster, you have some time to look around the room.\n\n"
			 << "1. Move\n"
			 << "2. Check Map\n\n";
		select = inputValidation();

		switch (select) {

			case 1: {

				// Clear any information on screen.
				system("cls");

				// This vector will store the selection values of each path the user can take.
				vector<int> pathwaySelection = { -1, -1, -1, -1 };

				moveRooms(layout, pathways, pathwaySelection);

				// Accept user input and check pathwaySelection for the correct room.
				cout << "\n";
				select = inputValidation();

				if (select == pathwaySelection[0]) {

					cleared[8] = true;
					return 0;

				}
				else if (select == pathwaySelection[1]) {

					cleared[8] = true;
					return 1;

				}
				else if (select == pathwaySelection[2]) {

					cleared[8] = true;
					return 2;

				}
				else if (select == pathwaySelection[3]) {

					cleared[8] = true;
					return 3;

				}

				break;

			}

			case 2:

				// Clear any information on screen.
				system("cls");

				// Display the map.
				displayMap(layout, 9);

				cout << "\nPress ENTER to return to the menu.\n";
				cin.get();
				cin.ignore();

				break;

		}

	} while (true);

}
int room10(player& user, int layout[2][5], vector<bool>& cleared, bool first) {

	// Clear any information on screen
	system("cls");

	// Check whether the player has cleared this room.
	if (!cleared[9]) {

		// Provide the user with a brief introduction to the room.
		cout << "Emerging into this room, you brace for attack and begin looking around for anything of note.\n"
			 << "Surprisingly, this room seems to be largely empty save for a mural lining the walls of the room.\n\n"
		 	 << "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}
	else {

		// Provide the user with a reminder about the room.
		cout << "You enter the room with the storied mural.\n\n"
			<< "Press ENTER to continue.\n";
		if (!first)
			cin.ignore();
		cin.get();

	}

	// Check the dungeon layout for available pathways.
	vector<bool> pathways = getPathways(10, layout);

	// Give the player a list of ways to go, a map, and the option to look around. select will be used to navigate these menus.
	int select;
	bool checked = false;

	do {

		// Clear any information on screen.
		system("cls");

		cout << "With this brief respite, you have some time to look around the room.\n\n"
			 << "1. Move\n"
			 << "2. Check Map\n"
			 << "3. Check Mural\n\n";
		select = inputValidation();

		switch (select) {

			case 1: {

				// Clear any information on screen.
				system("cls");

				// This vector will store the selection values of each path the user can take.
				vector<int> pathwaySelection = { -1, -1, -1, -1 };

				moveRooms(layout, pathways, pathwaySelection);

				// Accept user input and check pathwaySelection for the correct room.
				cout << "\n";
				select = inputValidation();

				if (select == pathwaySelection[0]) {

					cleared[9] = true;
					return 0;

				}
				else if (select == pathwaySelection[1]) {

					cleared[9] = true;
					return 1;

				}
				else if (select == pathwaySelection[2]) {

					cleared[9] = true;
					return 2;

				}
				else if (select == pathwaySelection[3]) {

					cleared[9] = true;
					return 3;

				}

				break;

			}

			case 2:

				// Clear any information on screen.
				system("cls");

				// Display the map.
				displayMap(layout, 10);

				cout << "\nPress ENTER to return to the menu.\n";
				cin.ignore();
				cin.get();

				break;

			case 3:

				// Clear any information on screen.
				system("cls");

				// Provide the player with details about the mural.
				cout << "You begin tracing along the walls, studying the mural and getting a sense for the story it tells.\n\n"
					 << "Centuries ago, the ancient Martians flourished, building their society upon the arcane energy flowing\n"
					 << "through the planet's core. One day, the Martians discovered a pendant deep within the caverns of Olympus\n"
					 << "Mons. Fearing its power, they constructed a temple within the mountain to keep it sealed away.\n\n"
					 << "One day, a Martian entered the temple and claimed the pendant for themselves, donning it and transforming\n"
					 << "into a hideous beast: the Dragon of Olympus Mons. The dragon's rage caused the volcano to erupt, its ash\n"
					 << "trapping the Martians within its stone.\n\n"
					 << "The mural ends with a depiction of its carving, created by an earthly astronaut with a time machine.\n\n"
					 << "Press ENTER to continue.\n";
				cin.ignore();
				cin.get();

				break;

		}

	} while (true);

}
int end(player& user) {

	// Clear any information on screen.
	system("cls");

	// Give the player the ending lines and final battle.
	cout << "Upon clearing the previous room, you begin to feel the earth shake.\n"
		 << "The ground caves beneath you and you begin falling into the core of the volcano. As the temple\n"
		 << "crumbles above you, the magic housed inside begins swirling around you and you feel yourself\n"
		 << "grow rejuvenated, ready for another battle.\n\n"
		 << "You stand upon a cracked stone platform above a lake of lava. The volcano walls stretch high\n"
		 << "into the sky. From the lava bed, you feel another rumbling that shakes the earth, as the\n"
		 << "Dragon of Olympus Mons rises from the molten rock to face you. The dragon is seemingly made of\n"
		 << "stone, lava coursing through cracks in its body, resembling glowing scales.\n\n"
		 << "Press ENTER to begin battling.\n";
	cin.ignore();
	cin.get();

	user.health = user.maxHealth;
	user.mana = user.maxMana;

	/* Enemies: Dragon of Olympus Mons
		   The variables/objects below will make up the enemy's attacks and description, respectively. */
	int dragonStats[3] = { 16, 12, 12 };
	vector<weapon> attacks;
	weapon bite("Bite", 8, 2, false);
	weapon breathWeapon("Breath Weapon", 12, 1, true);
	weapon claw("Claw", 6, 3, false);
	string dragonDesc = "A large stone dragon residing within the core of Olympus Mons, the Dragon of Olympus Mons\n"
						"acts as the final challenge in your journey. Conquer it and free Mars.";		 
	enemy dragon("Dragon of Olympus Mons", dragonDesc, 50, 50, 16, dragonStats, attacks, false, false);
	dragon.attacks.push_back(bite);
	dragon.attacks.push_back(breathWeapon);
	dragon.attacks.push_back(claw);

	battle(user, dragon);
	system("cls");

	if (user.health <= 0)
		return -1;

	// Print the aftermath of the battle.
	cout << "Upon the dragon's defeat, you feel the earth begin to violently shake again. You flee the core\n"
		 << "of the mountain as the volcano begins to erupt. As the ceiling caves in, you desperately try to\n"
		 << "make it back to the exit, and you succeed, busting through the rubble. Descending the temple steps,\n"
		 << "you turn to see molten rock chasing you down the mountain. You brace for the incoming flames, but feel\n"
		 << "nothing. Opening your eyes, you see the lava begin to fill a grand moat beneath the temple steps.\n\n"
		 << "As the lava flows down the volcano, you see beads of green mana light up from beneath the stone,\n"
		 << "as humanoid figures are born again from the mountain.\n\n"
		 << "You have brought life to Mars once again by defeating the dragon. The Martians greet you, and use\n"
		 << "their magic to mend your spacecraft as a form of gratitude. You leave the Martians to rebuild their\n"
		 << "society as you rocket into the stars, heading home to planet Earth to tell of your adventure.\n\n"
		 << "The End.\n\n"
		 << "Press ENTER to return to the main menu.\n";
	cin.get();

	return 0;

}
void moveRooms(int layout[2][5], vector<bool> pathways, vector<int>& pathwaySelection) {

	// This variable will be used to track how many options the player has and display them.
	int selectionCount = 0;

	cout << "Please select a room.\n\n";

	// This loop should occur four times to ensure no options are skipped over.
	for (int i = 0; i < pathways.size(); i++) {

		// Check which direction the current index corresponds to.
		switch (i) {

			case 0:

				// Check if the user can move up.
				if (pathways[i]) {

					// Increment selectionCount and display the option to move up.
					selectionCount++;
					cout << selectionCount << ". Up\n";
					pathwaySelection[i] = selectionCount;

				}

				break;

			case 1:

				// Check if the user can move up.
				if (pathways[i]) {

					// Increment selectionCount and display the option to move up.
					selectionCount++;
					cout << selectionCount << ". Down\n";
					pathwaySelection[i] = selectionCount;

				}

				break;

			case 2:

				// Check if the user can move up.
				if (pathways[i]) {

					// Increment selectionCount and display the option to move up.
					selectionCount++;
					cout << selectionCount << ". Left\n";
					pathwaySelection[i] = selectionCount;

				}

				break;

			case 3:

				// Check if the user can move up.
				if (pathways[i]) {

					// Increment selectionCount and display the option to move up.
					selectionCount++;
					cout << selectionCount << ". Right\n";
					pathwaySelection[i] = selectionCount;

				}

				break;

		}

	}

}
int nextRoom(int layout[2][5], int& room, int direction) {

	// These variables will be used to locate the current room's coordinates.
	int roomPosX = 0;
	int roomPosY = 0;

	// Loop through the layout to find the current room.
	for (int row = 0; row < 2; row++) {

		for (int column = 0; column < 5; column++) {

			if (layout[row][column] == room) {

				roomPosX = column;
				roomPosY = row;

			}

		}

	}

	// Return the correct room based on the direction.
	switch (direction) {

		case 0:

			room = layout[roomPosY - 1][roomPosX];
			return layout[roomPosY - 1][roomPosX];

		case 1:

			room = layout[roomPosY + 1][roomPosX];
			return layout[roomPosY + 1][roomPosX];

		case 2:

			room = layout[roomPosY][roomPosX - 1];
			return layout[roomPosY][roomPosX - 1];

		case 3:

			room = layout[roomPosY][roomPosX + 1];
			return layout[roomPosY][roomPosX + 1];

	}

	return -1;

}
int runRoom(player& user, int layout[2][5], vector<bool>& cleared, bool first, int room) {

	int direction = 0;

	switch (room) {

		case 1:

			direction = room1(user, layout, cleared, first);
			break;

		case 2:

			direction = room2(user, layout, cleared, first);
			break;

		case 3:

			direction = room3(user, layout, cleared, first);
			break;

		case 4:

			direction = room4(user, layout, cleared, first);
			break;

		case 5:

			direction = room5(user, layout, cleared, first);
			break;

		case 6:

			direction = room6(user, layout, cleared, first);
			break;

		case 7:

			direction = room7(user, layout, cleared, first);
			break;

		case 8:

			direction = room8(user, layout, cleared, first);
			break;

		case 9:

			direction = room9(user, layout, cleared, first);
			break;

		case 10:

			direction = room10(user, layout, cleared, first);
			break;

	}

	return direction;

}
void generateLayout(int layout[2][5]) {
	
	// This vector will hold the rooms that have been used already.
	vector<int> usedRooms;

	// The following for loops will iterate through dungeon to assign its rooms.
	for (int row = 0; row < 2; row++) {

		for (int column = 0; column < 5; column++) {

			// This do-while loop ensures that a room is not used twice.
			bool used;
			do {

				used = false;
				layout[row][column] = (rand() % 10) + 1;	// Generates a random room for this position.
				
				// Makes sure that usedRooms is not empty. The code inside this if-statement should not execute during the first iteration.
				if (usedRooms.size() > 0) {

					// Loops through usedRooms to check if the room has been used.
					for (int room = 0; room < usedRooms.size(); room++) {

						if (usedRooms[room] == layout[row][column]) {

							used = true;	// Sets the loop condition to true to assign the room a different value.

						}

					}

				}

			} while (used);

			// Adds the assigned room to usedRooms.
			usedRooms.push_back(layout[row][column]);

		}

	}

}
vector<bool> getPathways(int room, int layout[2][5]) {

	// These variables will track where the room is in the dungeon layout.
	int roomPosX = 0;	// Represents the column.
	int roomPosY = 0;	// Represents the row.

	// First, the function will locate the room in the dungeon layout.
	for (int row = 0; row < 2; row++) {

		for (int column = 0; column < 5; column++) {

			if (layout[row][column] == room) {

				roomPosX = column;
				roomPosY = row;

			}

		}

	}

	// This boolean vector represents the ways the player can move. Index 0: Up, 1: Down, 2: Left, 3: Right
	vector<bool> pathways = { false, false, true, true };

	// If the room is in the first row, it will return true for down, and vice-versa for up.
	if (roomPosY == 0) {

		pathways[1] = true;

	}
	else if (roomPosY == 1) {

		pathways[0] = true;

	}

	// If the room is far to the left, it will return false for left, and vice-versa for right.
	if (roomPosX == 0) {

		pathways[2] = false;

	}
	else if (roomPosX == 4) {

		pathways[3] = false;

	}

	return pathways;

}
void displayMap(int layout[2][5], int room) {

	// These variables will track where the room is in the dungeon layout.
	int roomPosX = 0;	// Represents the column.
	int roomPosY = 0;	// Represents the row.

	// Find the room within the dungeon's layout.
	for (int row = 0; row < 2; row++) {

		for (int column = 0; column < 5; column++) {

			if (layout[row][column] == room) {

				roomPosX = column;
				roomPosY = row;

			}

		}

	}

	// Create an empty map.
	char map[2][5] = { {' ', ' ', ' ', ' ', ' '},
					   {' ', ' ', ' ', ' ', ' '} };

	// Find the player's position using their current room.
	map[roomPosY][roomPosX] = 'x';

	// Display the map.
	for (int row = 0; row < 2; row++) {

		for (int column = 0; column < 5; column++) {

			cout << "[" << map[row][column] << "]";

		}

		cout << "\n";

	}

}
