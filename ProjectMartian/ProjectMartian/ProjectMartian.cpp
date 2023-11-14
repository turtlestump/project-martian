// Project Martian
// Johnathan Church

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
	int manaCost;

	// Constructor
	spell(string name, int maxDamage, int minDamage, int maxHeal, int minHeal, int manaCost) {

		this->name = name;
		this->maxDamage = maxDamage;
		this->minDamage = minDamage;
		this->maxHeal = maxHeal;
		this->minHeal = minHeal;
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

	// Constructor
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

// Helper function prototypes.
void pointAssignment(player& user, string statName, int stat, int& points);
int findLowestStat(player user);
void enemyAttack(player& user, enemy target, int attackIndex);
int weaponAttack(weapon userWeapon, int strengthModifier);
void castSpell(player& user, enemy& target, int spellIndex);
void displayWeapon(player user);

// Function prototypes.
int inputValidation();
void characterCreator(player& user);
void displayStats(player user);
void battle(player& user, enemy& target);

// Testing function prototypes.
void displayPlayer(player user);

// Room function prototypes.
void entrance(player& user);
void generateLayout(int layout[2][5]);
vector<bool> getPathways(int room, int layout[2][5]);

// The main function: Program execution begins and ends here.
int main() {

	srand(time(NULL));

	// Initialize a player object and take them through character creation.
	player user;
	characterCreator(user);

	// Thus begins the start of the dungeon. The entrance function will provide context, have them enter the dungeon, and provide them with magic.
	entrance(user);

}

// Input validation function: Ensures the user does not enter anything other than an integer.
int inputValidation() {

	int input;
	bool valid = false;

	do {

		// Receives user input.
		cin >> input;

		// If the user enters a value that cannot be stored inside an int variable, such as a string, it will catch the fail state.
		if (cin.fail()) {

			// Prompt the user for valid input.
			cout << "\nPlease enter a number to proceed!\n";
			cin.clear();	// Clears the fail state.
			cin.ignore();	// Clears cin so new input can be received.

		}
		else {

			// If the user enters a value that can be stored inside an int variable, their input is valid and input can be returned.
			valid = true;

		}

	} while (!valid);

	return input;

}

// Character creation function: Assigns the player stats and a name, filling their attributes.
void characterCreator(player& user) {
	
	// Assign default values to stats (points will be added to these values).
	for (int i = 0; i < 6; i++) {

		user.stats[i] = 8;

	}

	/* These variable represents the number of points the player can assign to different stats.
	   Player stats will be determined by a point-buy system. The more points a player puts into a single stat
	   the less will be left for them to use on others. */
	int points = 24;

	// Prompt the user for their character's name.
	cout << "Welcome to Project Martian."
		 << "\n\nPlease enter your name.\n";
	getline(cin, user.name);

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
		pointAssignment(user, "Strength", 0, points);
		pointAssignment(user, "Dexterity", 1, points);
		pointAssignment(user, "Constitution", 2, points);
		pointAssignment(user, "Intelligence", 3, points);
		pointAssignment(user, "Wisdom", 4, points);
		pointAssignment(user, "Charisma", 5, points);

		// Clear any information on screen.
		system("cls");

		// Determine the player's stat modifiers (10-11: 0, 12-13: +1, etc.).
		for (int i = 0; i < 6; i++) {

			user.statModifiers[i] = (user.stats[i] - 10) / 2;

		}

		// Display the user's stats and prompt them to confirm.
		int confirm;
		displayStats(user);
		cout << "\n\nEnter '1' to confirm your stats, and any other number to reassign points.\n";
		confirm = inputValidation();

		// Check whether or not to repeat the loop.
		if (confirm == 1) {

			done = true;

		}
		else {

			// Reassign default values to stats and reset points.
			for (int i = 0; i < 6; i++) {

				user.stats[i] = 8;

			}
			points = 24;

		}

	} while (!done);

	// Determine the player's health based on their constitution modifier.
	user.maxHealth = 30 + user.statModifiers[2] * 10;
	user.health = user.maxHealth;

	// Embed weapon selection within a loop to insure valid input.
	bool valid = false;
	while (!valid) {

		// Clear any information on screen.
		system("cls");

		// Prompt the player for their starting weapon of choice.
		int select;
		cout << "Finally, you equipped yourself with a melee weapon before arriving. What was it again?\n\n"
			<< "1. Spear: Swords deal more consistent damage, but have the potential for less. (3-6)\n"
			<< "2. Mace: Maces have the potential for both a high and low damage output. (1-10)\n"
			<< "3. Sword: Swords fall somewhere in the middle. (2-8)\n\n";
		select = inputValidation();

		switch (select) {

			case 1:

				user.userWeapon = weapon("Spear", 6, 3, false);
				valid = true;
				break;

			case 2:

				user.userWeapon = weapon("Mace", 10, 1, false);
				valid = true;
				break;

			case 3:

				user.userWeapon = weapon("Sword", 8, 2, false);
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

// This function displays the player's stats and is used in the characterCreation function.
void displayStats(player user) {

	cout << "Strength: " << user.stats[0] << " (" << user.statModifiers[0] << ")"
		 << "\nDexterity: " << user.stats[1] << " (" << user.statModifiers[1] << ")"
		 << "\nConstitution: " << user.stats[2] << " (" << user.statModifiers[2] << ")"
		 << "\nIntelligence: " << user.stats[3] << " (" << user.statModifiers[3] << ")"
		 << "\nWisdom: " << user.stats[4] << " (" << user.statModifiers[4] << ")"
		 << "\nCharisma: " << user.stats[5] << " (" << user.statModifiers[5] << ")";

}

// Battle function: The user and the target should alternate turns until one of them reaches a health value of 0 or lower.
void battle(player& user, enemy& target) {

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
					cout << target.description << "\n"
						<< "\nPress ENTER to continue.";
					cin.ignore();
					cin.get();

					valid = true;
					break;

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

// Helper function for the charater creation function.
void pointAssignment(player& user, string statName, int stat, int& points) {

	// Clear any information on screen.
	system("cls");

	// This variable is used to assign values to the stat being changed.
	int assignment = 0;

	// Ensure that the user's inputs are valid with a while loop.
	bool valid = false;
	while (!valid) {

		// Prompt the user for their point assignment.
		cout << "Points remaining: " << points
		     << "\nYour " << statName << " score: " << user.stats[stat]
			 << "\n\nHow many points would you like to add to " << statName << "?\n";
		assignment = inputValidation();

		if (assignment <= points && assignment >= 0) {

			// Increase the user's stat by the number of points and remove points from the total.
			user.stats[stat] += assignment;
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
	int hit = (rand() % 18) + 3 + ((target.stats[0] - 10) / 2);

	// Test whether the hit lands or not (compare hit with the target's armor class).
	if (hit >= user.armorClass) {

		// Determine the attack's damage.
		int damage = weaponAttack(target.attacks[attackIndex], (target.stats[0] - 10) / 2);

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

	// Detect whether the spell is a healing or damaging spell.
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
	else {

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

}

// Helper function for the player display testing function.
void displayWeapon(player user) {

	cout << "Name: " << user.userWeapon.name
		 << "\nMaximum Damage: " << user.userWeapon.maxDamage
		 << "\nMinimum Damage: " << user.userWeapon.minDamage
		 << "\nMagical Weapon: " << ((user.userWeapon.magical == false) ? ("No") : ("Yes"));

}

// Testing function to display the player's attributes.
void displayPlayer(player user) {

	cout << "Name: " << user.name << "\n"
		<< "Health: " << user.health << "/" << user.maxHealth << "\n"
		<< "Mana: " << user.mana << "/" << user.maxMana << "\n"
		<< "Armor Class: " << user.armorClass << "\n";
	displayStats(user);
	cout << "\n";
	displayWeapon(user);

}

/* Room functions : A total of ten rooms will make up the dungeon. Each room is unique and has its own function.
 
					At the start of the program, the generateLayout function will randomly choose a layout for these rooms.

					The getPathways function uses the generated 2D array and the room number to find out how many paths
					the player can take in that room.
					
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
		<< "\non you as you climb to the entrance, seeeking shelter from the incoming dust storm.";

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
		 << "\nYou just make it. The doorway crumbles behind you as your find yourself in a new space, grappling with the"
		 << "\nimplications of your new abilities.";

	cout << "\n\nYou have been granted the gift of magic. Your intelligence stat influences your mana, the magical currency which governs"
		 << "\nyour capacity to cast spells.";

	// Calculate the player's mana.
	user.maxMana = 30 + user.statModifiers[3] * 10;
	user.mana = user.maxMana;

	// Assign the player two starting spells: Fireball and Restoration.
	spell fireball("Fireball", 10, 1, 0, 0, 8);
	spell restoration("Restoration", 0, 0, 12, 4, 6);
	user.spells.push_back(fireball);
	user.spells.push_back(restoration);

	// Inform the player of their new abilities and progress to the first generated room.
	cout << "\n\nYou learned a new spell! Fireball allows you to burn enemies in a wave of flame."
		 << "\nYou learned a new spell! Restoration allows you to heal during the heat of battle."
		 << "\n\nPress ENTER to continue to the first room.\n";
	cin.get();

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
