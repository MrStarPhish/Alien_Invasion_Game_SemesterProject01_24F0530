// F24-0530 - Semester Project ...
#include<iostream>
#include<iomanip> // only for setw() on High Scores Screen
#include<windows.h> // for Sleep() and gotoxy() and Beep()
#include<conio.h> // for _getch() and _kbhit()
#include<string> // for storing name
#include<fstream> // for file handling


using namespace std;

// ------------All Global Variables

// General Game Stats
int SCORE = 0;
int HEALTH = 0;
int DISTANCE = 0;
string NAME;
// for keeping track of game-frames
unsigned long int FRAME = 0; 
// Game UI
int stage = 0; // 0 for Menu, 1 for Level-1, 2 for Level-2
// Variable for Speed Factor
int relativeSpeedVar = 1;
int sleepSeed = 100;
// Map Related
const int ROWS = 50;
const int COLS = 30;
const int LAST_X = COLS - 1, LAST_Y = ROWS - 1;
char map[ROWS][COLS] = {};
// General Game Mechanics Flags
bool DevNotes = 0;
bool gameOver = 0;
bool gameExit = 0;
bool astHitShip = 0;
bool shipCantMove = 0;
// Menu Related 
int menuPointerLocation = 0;// for storing y-component of Pointer
int& pointer_y = menuPointerLocation;
int pointer_x = 8;
// Name Input / File Handling related
string scoreFileName = "scores.txt";
string name;
string names[50] = {};
int scores[50] = {};
string lines[50] = {};
int scoreEntries = 0;

// Ship Coords
int ship_y = LAST_Y-1, ship_x = LAST_X/2;
// Laser Count and Coords
int laser_count = 0; 
int laser_x[100] = {};
int laser_y[100] = {};
// Asteroids
int ast_count = 0;
int ast_x[100] = {};
int ast_y[100] = {};
// Explosion 
int exp_count = 0;
int exp_x[100] = {};
int exp_y[100] = {};
int exp_state[100] = {};
// Enemy Ship
int enemy_count = 0;
int enemy_x[100] = {};
int enemy_y[100] = {};
int enemy_direction[100] = {}; // Direction of motion of enemy ship. 1-Right, 2-left
int enemy_life[100] = {}; // Keeps track of the duration of life of enemy ship (in frames)
// Enemy Lasers
int e_laser_x[100] = {};
int e_laser_y[100] = {};
int e_laser_count = 0;
// Collectibles
		//void generateCollectible(int x);
// ---Stars
int star_count = 0;
int star_x[100] = {};
int star_y[100] = {};
// ---MedKit
int med_count = 0;
int med_x[100] = {};
int med_y[100] = {};


// Storing keyboard hit key
char key = 0;

// -------------Declaring all functions
// Coding related
void gotoxy(int x, int y);
void hideCursor();
void setColor(int textColor);
void setConsoleSize(int width, int height);
void playBackgroundMusic(const wchar_t* musicFileName);
void debugMsg(int code, int frame);
void printArr(int arr[], int size, int col, int row);
void printStr(string line, int i_frame, int row);
void clearRow(int row);
// Dev Notes
void checkForDevNotesDisplay();
void showDevNotes(bool &a);
void printDevNotes();
void clearDevNotes();
void clearDevRow(int row);
void printLaserStats();
void printAsteroidStats();
void printEnemyStats();
void printEnemyLifetime();
void printEnemyLaserStats();
void printFrameCount();
void printSleepVar();
void printRelativeSpeedVar();
void printScoreboard();
void clearDevNotesScoreboard();
void printName(int num);
void printScore(int num);
// Startup Menu Related
void displayStartupMenu();
void printStartupHeader();
void printStartupWatermark();
void printMenu();
void printMenuInstructions();

void defaultMenuPointer();
void clearPointer();
void printMenuPointer();
void menuKeyListen(char btn);
void menuPointerUp();
void menuPointerDown();
void menuPointerSelect();
// Name Input / File Handling related
void inputName();
void inputCheck();
void countEntries();
void readStats();
void sortStats();
void addStatsEntry();
void rewriteStats();
// HIGH SCORES related
void displayHighScores();
// INSTRUCTIONS REALTED
void displayInstructions();
// GameBar Related
void displayGameBarStats();
void clearGameBarStats();
void printScore();
void printDistance();
void printHealth();
void printHealthBar();
void printName();
// Object Generation Related
void generateObject(int i_frame);
// Game Related
void resetScore();
void resetHealth();
void resetDistance();
void resetFrame();
void cleanBoundary();
void processShip(int x, int y);
void initializeMap();
void renderMap();
void renderGameBar();
void keyListen();
void buttonPressed(char btn);
void moveShipLeft();
void moveShipRight();
void moveShipUp();
void moveShipDown();
void forceGameOver();
// LASER
void shootLaser();
void progressLaser();
void neutralizeLaser(int i);
void sortLaser();
// ASTEROID
void generateAsteroid();
void progressAsteroid();
void neutralizeAsteroid(int i);
void sortAsteroid();
// LASER INTERACTION
bool laserHitAst(int laser_n);
int hitAstNum(int laser_n);
bool laserHitEnemy(int laser_n);
int hitEnemyNum(int laser_n);
bool AstHitShip(int ast_n);
bool EnemyLaserHitShip(int e_laser_n);
bool starHitShip(int star_n);
bool medHitShip(int med_n);
// EXPLOSION RELATED
void generateExplosion(int x, int y);
void progressExplosion();
void sortExplosion();
// ENEMY SHIP RELATED
void generateEnemy(int y);
void progressEnemy();
void clearEnemyCurrentLocation(int num);
void updateEnemyLocation(int num);
void neutralizeEnemy(int i);
void sortEnemy();
// ENEMY LASER RELATED
void generateEnemyLaser();
void shootEnemyLaser(int enemy_num);
void progressEnemyLaser();
void neutralizeEnemyLaser(int e_laser_num);
void sortEnemyLaser();
// COLLECTIBLES
void generateCollectible(int type);
void progressCollectible();
void collectibleCollected(int type);
// ---STARS
void generateStar(int x);
void progressStar();
void neutralizeStar(int star_n);
void sortStar();
void incScore(int amount);
// ---MED KIT
void generateMed(int x);
void progressMed();
void neutralizeMed(int med_n);
void sortMed();
void incHealth(int amount);
void decHealth(int amount);
// ---DISTANCE
void incDistance(int amount);
// SHIP MOVEMENT CHECK
void shipMovementStatus(int callFrame);
// PROGRESSION
void progressObjects();
// Game Over
void gameOverFn();
void printGameOver();
void checkGameStatus();
// Exit
void gameExitFn();
// Sound Related
void menuSFX();
void stopBGM();





int main()
{	
	do{ // -- Program Loop
		// ALL THE INITIALS HERE , aka One Timers
		hideCursor();
		setConsoleSize(1280, 720);
		initializeMap();
		resetScore(); resetHealth(); resetDistance();
		defaultMenuPointer(); // setting pointer to default location at the beginning
		do { // -------------------------- GAME LOOP

			// keeping track of game's progression

			if (stage == 0 && (!gameExit)) // MENU SCREEN
			{
				char menuKey;
				displayStartupMenu();
				menuKey = _getch();
				menuKeyListen(menuKey);

			}
			if (stage == 3) // HIGH SCORES SCREEN
			{
				initializeMap();
				renderMap();
				displayHighScores();
			
				cout << "Press Any Key To Go Back.";
				_getch();
				menuSFX();
				stage = 0;
			}
			if (stage == 4) // INSTRUCTIONS SCREEN
			{
				initializeMap();
				renderMap();
				displayInstructions();

				gotoxy(3, 30);
				setColor(5);
				cout << "Press Any Key To Go Back.";
				setColor(7);

				_getch();
				menuSFX();
				stage = 0;
			}
			if (stage == 1) // Level 1
			{
				FRAME++; // tracking Frame

				// Ship and Render
				processShip(ship_x, ship_y);
				renderMap();
				renderGameBar();
				// Print stuff on SIde of the game
				checkForDevNotesDisplay();
				// Print stuff below the game
				clearGameBarStats();
				displayGameBarStats();
				// Object Generation
				generateObject(FRAME);
				generateEnemyLaser();
				// Slowed Down Progression , For Variable Game Difficulty
				if (FRAME % relativeSpeedVar == 0)
				{
					progressEnemy();
					progressAsteroid();
				}
				// General
				progressLaser();
				progressExplosion();
				progressEnemyLaser();
				progressCollectible();
				// Distance
				incDistance(2);
				if (DISTANCE != 0 && DISTANCE % 100 == 0)
					incScore(20);  // every 100m distance gives +20 score

				cleanBoundary(); // to remove any buggy display at top/bottom
				keyListen();
				checkGameStatus();
				Sleep(sleepSeed); // Game Overall Speed
			}
		} while (gameOver == 0);

	if (gameOver)  // Current Game Session's Over Screen
		{
			readStats();
			addStatsEntry();
			sortStats();
			rewriteStats();
			gameOverFn();

			gameOver = 0;
			stage = 0;
			stopBGM();
			resetFrame();
			_getch();
		}
} while (gameExit == 0);

// Game Exits here
	system("CLS");
	system("pause");
	return 0;
}



// Startup Menu Related
void displayStartupMenu()
{
	renderMap();
	clearPointer();
	printMenuPointer();
	printStartupHeader();
	printStartupWatermark();
	printMenu();
	printMenuInstructions();
}
void printStartupHeader()
{
	gotoxy(8, 15);
	setColor(3);
	cout << "SEMESTER PROJECT";
	gotoxy(11, 16);
	cout << "#F24-0530";
	gotoxy(8, 17);
	cout << "\"Alien Invasion\"";
}
void printStartupWatermark()
{
	gotoxy(6, 25);
	setColor(13);
	cout << "PHISHY'S SPACE COWBOY";
}
void printMenu()
{
	setColor(7); // making sure it is default color
	gotoxy(10,30);
	cout << "START GAME";
	gotoxy(10, 31);
	cout << "INSTRUCTIONS";
	gotoxy(10, 32);
	cout << "HIGH SCORES";
	gotoxy(10, 33);
	cout << "EXIT";
}
void printMenuInstructions()
{
	gotoxy(8,40);
	cout << "W,A,S,D to move";
	gotoxy(8,41);
	cout << "F to select";
}

void defaultMenuPointer()
{   // To set pointer location to default location -> Start Game
	menuPointerLocation = 30;
}
void printMenuPointer()
{
	gotoxy(pointer_x, pointer_y);
	setColor(12);
	cout << ">";
}
void menuKeyListen(char btn)
{
	char z = tolower(btn);
	switch (btn)
	{
	case 's':
		menuPointerDown();
		break;
	case 'w':
		menuPointerUp();
		break;
	case 'f':
		menuPointerSelect();
		break;
	}
}
void menuPointerUp()
{
	if (pointer_y - 1 != 29) // movement check
	{
		pointer_y--;
		menuSFX();
	}
}
void menuPointerDown()
{
	if (pointer_y + 1 != 34) // movement check
	{
		pointer_y++;
		menuSFX();
	}
}
void menuPointerSelect()
{	
	menuSFX();
	switch (pointer_y)
	{
	case 30:
		system("CLS");
		inputName();
		resetFrame();
		stage = 1;
		break;
	case 31:
		stage = 4;
		break;
	case 32:
		stage = 3;
		break;
	case 33:
		gameExitFn();
		break;
	}
}
void clearPointer()
{
	gotoxy(pointer_x, pointer_y);
	cout << " ";
}

// Name Input / File Handling related
void inputName()
{
	initializeMap();
	renderMap(); // clearing screen

	setColor(6);
	gotoxy(5, 20);
	cout << "Input Name";
	gotoxy(5, 21);
	cout << "(5 Characters)";
	gotoxy(5, 25);
	setColor(13);
	cout << "Name:  ";
	setColor(12);
	getline(cin, name);
	menuSFX();
	playBackgroundMusic(L"bgm.wav");
	inputCheck();
}
void inputCheck()
{   // if input is greater than 5 characters, shrink it.
	if (name.length() > 5)
	{
		name = name.substr(0, 5);
	}
}
void countEntries()
{	// for counting no.of entries in the scores file
	ifstream read;
	string temp;
	scoreEntries = 0;
	read.open(scoreFileName, ios::in);
	if (read.is_open()) // if file is opened
	{
		while (getline(read, temp))
		{
			scoreEntries++;
		}
	}
	read.close();
}
void readStats()
{	
	ifstream read;
	string temp;
	scoreEntries = 0;
	read.open(scoreFileName, ios::in);
	if (read.is_open()) // if file is opened
	{
		while (getline(read, temp))
		{
			lines[scoreEntries] = temp;
			scoreEntries++;
		}
	}

	// TODO: Start reading file from shuru say again
	read.close();
	read.open(scoreFileName, ios::in);

	// READING file here, and storing each line in names/strong array
	for (int i = 0; i < scoreEntries; i++) // entry iterator
	{
		string temp = "";
		string temp_name = "";
		int temp_score = 0;

		int column = 0;  
		// since String-Array behaves like 2D Array
		string line = lines[i]; 
		for (int k = 0; k<=line.size(); k++)
		{
			char c = (k < line.size()) ? line[k] : ','; // read each character and store it
			if (c == ',') 
			{
				if (column == 0) {
					names[i] = temp;
				}
				else if (column == 1) 
				{
					scores[i] = stoi(temp);
				}
				column++;
				temp = "";
			}
			else temp += c;
		}
		
		//string temp = "";
		//string temp_name = "";
		//int temp_score = 0;

		//int column = 0;  
		//temp = "\0";
		//// since String-Array behaves like 2D Array
		//string line = lines[i]; 
		//for (int k = 0; line[k] != '\0'; k++)
		//{
		//	char c = line[k]; // read each character and store it
		//	if (c == ',' || c == '\0') {
		//		if (column == 0) {
		//			names[i] = temp + "\0";
		//		}
		//		else if (column == 1) {
		//			scores[i] = stoi(temp);
		//		}
		//		column++;
		//		temp = "\0";
		//	}
		//	else temp += c;
		//}
		
		
		//int j = 0; // first half goes in names[] , second half goes in scores[]
		//for (; temp[j]!=','; j++) 
		//{
		//	temp_name[j] = temp[j];
		//}
		//for (; temp[j] != '\n'; j++)
		//{
		//	temp_score[j] = temp[j];
		//}
		//names[i] = temp_name;
		//scores[i] = temp_score;
	}
}
void sortStats()
{   // Bubble Sorting the arrays for File, in Descending Order
	for (int i = 0; i < scoreEntries; i++)
	{
		for (int j = 0; j < scoreEntries - 1; j++)
		{
			if (scores[i] < scores[i + 1])
			{
				int temp = scores[i]; // Swapping scores
				scores[i] = scores[i + 1];
				scores[i + 1] = temp;

				string temp2 = names[i]; // Swapping respective names
				names[i] = names[i + 1];
				names[i + 1] = temp2;
			}
		}
	}
}
void addStatsEntry()
{
	names[scoreEntries] = name;
	scores[scoreEntries] = SCORE;
	scoreEntries++;
}
void rewriteStats()
{
	ofstream write;
	write.open(scoreFileName, ios::out);
	
	for (int i = 0; i < scoreEntries; i++)
	{
		write << names[i] << "," << scores[i] << endl;
	}
	write.close();
}
// HIGH SCORES related
void displayHighScores()
{
	readStats();
	sortStats();

	int limit;
	if (scoreEntries > 10)
		limit = 10;
	else
		limit = scoreEntries;

	gotoxy(4, 10);
	setColor(6);
	cout << "Top 10 High Scores";

	
	setColor(7);
	gotoxy(7,12);
	cout << left << setw(5) << "NAME" << " : SCORE";
	gotoxy(7, 13);
	cout << "=============";
	for (int i = 0; i < limit; i++)
	{	
		gotoxy(7,15+i);
		setColor(9);
		printName(i);
		setColor(7);
		cout << " : ";
		setColor(5);
		printScore(i);
	}
	gotoxy(3, 15+limit);
}
// INSTRUCTIONS realted
void displayInstructions()
{
	gotoxy(1, 5);
	setColor(11);
	cout << "CONTROLS";
	setColor(7);
	gotoxy(1, 6);
	cout << "W,A,S,D to move spaceship.";
	gotoxy(1, 7);
	cout << "Space/B to shoot laser.";
	gotoxy(1, 8);
	cout << "0 to toggle Developer Notes.";
	gotoxy(1, 10);
	setColor(11);
	cout << "DEV CONTROLS";
	setColor(7);
	gotoxy(1, 11);
	cout << "P for Asteroid";
	gotoxy(1, 12);
	cout << "O for Enemy Ship";
	gotoxy(1, 13);
	cout << "9 for Enemy Laser";
	gotoxy(1, 14);
	cout << "I for Collectibles";

	gotoxy(1, 17);
	setColor(11);
	cout << "INSTRUCTIONS";
	setColor(7);
	gotoxy(1, 18);
	cout << "-Dodge Asteroids (#)";
	gotoxy(1, 19);
	cout << "-Shoot Asteroids and Enemy{V}";
	gotoxy(1, 20);
	cout << "-Collect Money($) for Score";
	gotoxy(1, 21);
	cout << "-Pick Up Med-Kit(+) for Health";
	gotoxy(1, 23);
	cout << "Destroy Asteroids: +20 Score";
	gotoxy(1, 24);
	cout << "Destroy Enemy: +50 Score";
}

// Object Generation Related
void generateObject(int i_frame)
{
	int z = i_frame;
	int delay = 15; // Basic frame-difference/timing between generation of each type of object
	static int asteroidTimer = 0;
	static int enemyTimer = 0;
	static int collectibleTimer1 = 0;
	static int collectibleTimer2 = 0;

	// Generating Objects
	if ((z % delay == 0 && asteroidTimer <= 0) || z==10)
	{
		generateAsteroid();
		asteroidTimer = delay;
	}
	if (z % (2*delay) == 0 && enemyTimer <= 0)
	{
		if (enemy_count <= 5)
		{
			int temp = rand() % (35 - 5 + 1) + 5; // (max-min+1)+min , generating random number in range [min,max]
			generateEnemy(temp);
			enemyTimer = 2 * delay;
		}
	}
	if (z % (3*delay) == 0 && collectibleTimer1 <= 0)
	{
		generateCollectible(1); // Star
		collectibleTimer1 = delay;
	}
	if (z % (4 * delay) == 0 && collectibleTimer2 <= 0)
	{
		generateCollectible(2); // Med Kit
		collectibleTimer2 = 3*delay;
	}

	// Resetting Timers of Generated Objects
	if (asteroidTimer > 0)
		asteroidTimer--;
	if (enemyTimer > 0)
		enemyTimer--;
	if (collectibleTimer1 > 0)
		collectibleTimer1--;
	if (collectibleTimer2 > 0)
		collectibleTimer2--;

}

// Render Related
void resetScore()
{
	SCORE = 0;
}
void resetHealth()
{
	HEALTH = 100;
}
void resetDistance()
{
	DISTANCE = 0;
}
void resetFrame()
{
	FRAME = 0;
}
void processShip(int x, int y)
{ // To store the location of ship
	map[y][x-1] = '<';
	map[y][x] = '^';
	map[y][x+1] = '>';
}
void initializeMap() {
	// Initializes the whole map array
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			map[i][j] = ' ';
		}
	}
}
void renderMap_OLD() 
{ // To print everything
//	gotoxy(0, 0);
//	for (int i = 0; i < COLS + 2; i++) // Top Boundary
//	{
//		cout << "=";
//	}
//	cout << endl;
//
//	for (int i = 0; i < ROWS; i++) // Inner Game
//	{
//		gotoxy(0,i+1);
//		cout << "|";
//		for (int j = 0; j < COLS; j++)
//		{
//			cout << map[i][j];
//			
//		}
//		cout << "|";
//	}
//	gotoxy(0, 51);
//	for (int i = 0; i < COLS + 2; i++) // Top Boundary
//	{
//		cout << "=";
//	}
//	gotoxy(0, 0);
} 
// ^ Traditional Rendering with gotoxy() implementation
void renderGameBar()
{
	gotoxy(0, 60);
	for (int i = 0; i < 32; i++)
	{
		cout << "=";
	}
	for (int i = 52; i < 60; i++)
	{
		gotoxy(0,i);
		cout << "|";
		gotoxy(31,i);
		cout << "|";
	}
	
}
void cleanBoundary()
{	// To clean Top and Bottom boundary to refrain from any buggy still display
	if (FRAME % 25 == 0)
	{
		for (int i = 0; i < 30; i++)
		{
			map[49][i] = map[0][i] = ' ';
		}
	}
}
// DevNotes Related
void checkForDevNotesDisplay()
{
	if (DevNotes)
	{
		if (FRAME % 10 == 0)
			clearDevNotes();
		printDevNotes();
	}
	else
	{
		clearDevNotes();
		clearDevNotesScoreboard();
	}
}
void showDevNotes(bool &a)
{
	static int button = 0;
	if (button == 0)
	{
		button = 1;
		a = 1;
	}
	else if (button == 1)
	{
		button = 0;
		a = 0;
	}
}
void printDevNotes()
{
	gotoxy(125, 0);
	cout << "-- DEVELOPER NOTES --";
	gotoxy(125, 1);
	printLaserStats();
	printAsteroidStats();
	printEnemyStats();
	printEnemyLaserStats();
	printEnemyLifetime();
	gotoxy(125, 14); setColor(13); cout << "24F-0530"; setColor(7);
	printFrameCount();
	printSleepVar();
	printRelativeSpeedVar();
	printScoreboard();
}
void printLaserStats()
{
	gotoxy(125, 1);
	cout << "LASER RELATED";
	gotoxy(125, 2);
	cout << "LASER Y:"; printArr(laser_y, laser_count, 135, 2);
	gotoxy(125, 3);
	cout << "LASER X:"; printArr(laser_x, laser_count, 135, 3);
}
void printAsteroidStats()
{
	gotoxy(125, 4);
	cout << "ASTEROID RELATED";
	gotoxy(125, 5);
	cout << "AST Y:"; printArr(ast_y, ast_count, 135, 5);
	gotoxy(125, 6);
	cout << "AST X:"; printArr(ast_x, ast_count, 135, 6);
}
void printEnemyStats()
{
	gotoxy(125, 7);
	cout << "ENEMY RELATED";
	gotoxy(125, 8);
	cout << "ENEMY Y:"; printArr(enemy_y, enemy_count, 135, 8);
	gotoxy(125, 9);
	cout << "ENEMY X:"; printArr(enemy_x, enemy_count, 135, 9);
}
void printEnemyLaserStats()
{
	gotoxy(125, 10);
	cout << "ENEMY RELATED";
	gotoxy(125, 11);
	cout << "E_LASER Y:"; printArr(e_laser_y, e_laser_count, 135, 11);
	gotoxy(125, 12);
	cout << "E_LASER X:"; printArr(e_laser_x, e_laser_count, 135, 12);
}
void printEnemyLifetime()
{
	gotoxy(125, 13);
	cout << "LIFETIME: ";
	printArr(enemy_life, enemy_count, 135, 13);
}
void printFrameCount()
{
	gotoxy(125, 15);
	cout << "FRAME: " << FRAME;
}
void printSleepVar()
{
	gotoxy(125, 16);
	cout << "SleepVariable: " << sleepSeed;
}
void printRelativeSpeedVar()
{
	gotoxy(125, 17);
	cout << "RelativeSpeedVar: " << relativeSpeedVar;
}
void clearDevNotes()
{
	for (int i = 0; i <= 17; i++)
	{
		clearDevRow(i);
	}
}
void clearDevNotesScoreboard()
{
	for (int i = 20; i <= 30; i++)
	{
		clearDevRow(i);
	}
}
void clearDevRow(int row)
{
	gotoxy(125, row);
	for (int i = 0; i < 80; i++)
	{
		cout << " ";
	}
}
void printScoreboard()
{
	static int button = 0;
	if (button == 0)
	{
		readStats();
		sortStats();
		for (int i = 0; i < scoreEntries; i++)
		{
			gotoxy(125, 20+i);
			printName(i);
			cout << " : ";
			printScore(i);
		}
		button = 1;
	}
}
void printName(int num) // num'th name
{
	cout <<left<< setw(5) << names[num];
}
void printScore(int num) // num'th score
{
	cout << scores[num];
}
// GameBar Related
void displayGameBarStats()
{
	printScore();
	printDistance();
	printHealth();
	printName();
}
void clearGameBarStats()
{
	if (FRAME % 15 == 0) // updates every 15 frames
	{
		for (int i = 53; i < 60; i++)
		{
			gotoxy(1,i);
			for (int j = 0; j < 30; j++)
			{
				cout << " ";
			}
		}
	}
}
void printScore()
{
	gotoxy(2, 53);
	setColor(2);
	cout << "SCORE: " <<"$ "<< SCORE;
}
void printDistance()
{
	gotoxy(2, 55);
	setColor(9);
	cout << "DISTANCE: " << DISTANCE<<" m";
}
void printHealth()
{
	gotoxy(2, 57);
	setColor(7);
	cout << "SHIP HEALTH: ";
	printHealthBar();
}
void printHealthBar()
{
	setColor(7);
	cout << "[";
	// dynamic coloring of Health Bar
	if (HEALTH / 10 > 7) setColor(2);
	else if (HEALTH / 10 >= 5) setColor(14);
	else if (HEALTH / 10 >= 2) setColor(6);
	else setColor(12);
	for (int i = 1; i <= (HEALTH / 10); i++)
	{	
		cout << "/";
	}
	setColor(7);
	gotoxy(26,57);
	cout << "]";
}
void printName()
{
	gotoxy(2, 59);
	cout << "PLAYER NAME: ";
	setColor(13);
	cout << name;
}
// Button Listener
void keyListen()
{
	if (_kbhit()) // if keyboard key pressed
	{
		key = _getch();
		buttonPressed(key);
	}
}
void buttonPressed(char btn) 
{ // To process the pressed key
	switch (btn)
	{
	case 'a': // Ship Left
	case 'A':
		if (!shipCantMove)
		moveShipLeft();
		break;

	case 'd': // Ship Right 
	case 'D':
		if (!shipCantMove)

		moveShipRight();
		break;

	case 'w': // Ship Up 
	case 'W':
		if (!shipCantMove)

		moveShipUp();
		break;

	case 's': // Ship Down
	case 'S':
		if (!shipCantMove)

		moveShipDown();
		break;

	case 'b': // Shoot Laser
	case 'B':
	case ' ':
		shootLaser();
		break;
	case 'p':
	case 'P':
		generateAsteroid();
		break;
	case 'o':
	case 'O':
		generateEnemy(10);
		break;
	case 'i':
	case 'I':
		generateCollectible(2);
		break;
	case '0':
		showDevNotes(DevNotes);
		break;
	case '9':
		generateEnemyLaser();
		break;
	case '=':
		forceGameOver();
		break;
	case ']':
		if (sleepSeed + 10 <= 150)
			sleepSeed += 10;
		break;
	case '[':
		if (sleepSeed - 10 >= 10)
			sleepSeed -= 10;
		break;
	case '/':
		if (relativeSpeedVar+1 <= 10)
			relativeSpeedVar++;
		break;
	case '.':
		if (relativeSpeedVar - 1 > 0)
			relativeSpeedVar--;
		break;
	}
	


}

// Button Related or Generation Actions
void moveShipLeft()
{
	if ((ship_x - 1) != 0)  // if Left Wing doesn't touch Boundary
	{
		map[ship_y][ship_x + 1] = ' ';
		ship_x--;
	}
}
void moveShipRight()
{
	if ((ship_x + 1) != LAST_X) // if Right Wing doesn't touch Boundary
	{
		map[ship_y][ship_x - 1] = ' ';
		ship_x++;
	}
}
void moveShipUp()
{
	if ((ship_y + 1) != 40) // if Ship doesn't touch Row 30
	{
		map[ship_y][ship_x - 1] = ' ';
		map[ship_y][ship_x] = ' ';
		map[ship_y][ship_x + 1] = ' ';
		ship_y--;
	}
}
void moveShipDown()
{
	if ((ship_y - 1) != LAST_Y - 1) // if Shit doesn't touch Bottom Boundary
	{
		map[ship_y][ship_x - 1] = ' ';
		map[ship_y][ship_x] = ' ';
		map[ship_y][ship_x + 1] = ' ';
		ship_y++;
	}
}
void shootLaser() 
{ // Generates a laser
	laser_x[laser_count] = ship_x;
	laser_y[laser_count] = ship_y - 1;
	map[ship_y - 1][ship_x] = '|';
	laser_count++;
}
void generateAsteroid()
{ // To create an asteroid
	int random = rand() % 30;
	ast_x[ast_count] = random;
	ast_y[ast_count] = 0;
	map[ast_y[ast_count]][ast_x[ast_count]] = '#';
	ast_count++;
}
void generateExplosion(int x, int y)
{
	exp_x[exp_count] = x;
	exp_y[exp_count] = y;
	exp_state[exp_count] = 3;
	map[y][x] = 'O';
	exp_count++;
}
void generateEnemy(int y) // y-axis as parameter
{
	int x = 0;
	if (FRAME % 2 == 0) // Randomizing the spawn and flow direction
	{
		enemy_x[enemy_count] = x = 1;
		enemy_direction[enemy_count] = 1;
		// Left Side, Towards Right
	}
	else
	{
		enemy_x[enemy_count] = x = LAST_X - 1;
		enemy_direction[enemy_count] = 2;
		// Right Side, Towards Left
	}
	enemy_y[enemy_count] = y;

	map[y][x - 1] = '{';
	map[y][x] = 'V';
	map[y][x + 1] = '}';

	enemy_count++;
	enemy_life[enemy_count] = 1;
}
void generateCollectible(int type) //
{
	if (type == 1)
	{
		generateStar(rand() % 30);
	}
	if (type == 2)
	{
		generateMed(rand() % 30);
	}
}
void generateStar(int x) // checkpoint
{
	star_y[star_count] = 0;
	star_x[star_count] = x;
	map[0][x] = '$';
	star_count++;
}
void generateMed(int x)
{
	med_y[med_count] = 0;
	med_x[med_count] = x;
	map[0][x] = '+';
	med_count++;
}
void forceGameOver()
{
	gameOver = 1;
}


// Flags Functions
bool laserHitAst(int laser_n)
{ // To check whether Laser is Hitting any Asteroid or not
	for (int j = 0; j < ast_count; j++)
	{
		if (laser_x[laser_n] == ast_x[j])
		{
			if ( (laser_y[laser_n] == ast_y[j]) || (laser_y[laser_n]-1 == ast_y[j]))
			{
				incScore(20);
				return 1;
			}
		}
	}
	return 0;
}
bool laserHitEnemy(int laser_n)
{
	int laserVar = laser_x[laser_n];
	for (int j = 0; j < enemy_count; j++)
	{
		if ( (laserVar == enemy_x[j]) || (laserVar == enemy_x[j]-1) || (laserVar == enemy_x[j]+1) ) // checking collision for left and right wing as well
		{
			if ((laser_y[laser_n] == enemy_y[j]) || (laser_y[laser_n] - 1 == enemy_y[j]))
			{
				incScore(50);
				return 1;
			}
		}
	}
	return 0;
}
bool AstHitShip(int ast_n)
{
	int ax = ast_x[ast_n];
	int ay = ast_y[ast_n];
	if (ay == ship_y)
	{
		if ((ax==ship_x) || (ax==ship_x+1) || (ax==ship_x-1)) // if asteroid hits Ship or its wing
		{
			astHitShip = 1;
			decHealth(10);
			return 1;
		}
	}
	astHitShip = 0;
	return 0;
}
bool EnemyLaserHitShip(int e_laser_n)
{
	int ex = e_laser_x[e_laser_n];
	int ey = e_laser_y[e_laser_n];
	if (ey == ship_y)
	{
		if ((ex == ship_x) || (ex == ship_x + 1) || (ex == ship_x - 1)) // if enemy laser hits Ship or its wing
		{
			decHealth(20);
			return 1;
		}
	}
	return 0;
}
bool starHitShip(int star_n)
{
	int sx = star_x[star_n];
	int sy = star_y[star_n];
	if (sy == ship_y)
	{
		if ((sx == ship_x) || (sx == ship_x + 1) || (sx == ship_x - 1)) // if asteroid hits Ship or its wing
		{
			collectibleCollected(1);
			return 1;
		}
	}
	return 0;
}
bool medHitShip(int med_n)
{
	int mx = med_x[med_n];
	int my = med_y[med_n];
	if (my == ship_y)
	{
		if ((mx == ship_x) || (mx == ship_x + 1) || (mx == ship_x - 1)) // if asteroid hits Ship or its wing
		{
			collectibleCollected(2);
			return 1;
		}
	}
	return 0;
}
int hitAstNum(int laser_n)   
{ // To know, which Nth Asteroid is getting hit with laser
	for (int j = 0; j < ast_count; j++)
	{
		if (laser_x[laser_n] == ast_x[j])
		{
			if ((laser_y[laser_n] == ast_y[j]) || (laser_y[laser_n] - 1 == ast_y[j]))
			{
				return j;
			}
		}
	}
	return 0;
}
int hitEnemyNum(int laser_n)   
{ // To know, which Nth Asteroid is getting hit with laser
	int laserVar = laser_x[laser_n];
	for (int j = 0; j < enemy_count; j++)
	{
		if ((laserVar == enemy_x[j]) || (laserVar == enemy_x[j] - 1) || (laserVar == enemy_x[j] + 1)) // checking collision for left and right wing as well
		{
			if ((laser_y[laser_n] == enemy_y[j]) || (laser_y[laser_n] - 1 == enemy_y[j]))
			{
				gotoxy(0, 60); cout << j;
				return j;
			}
		}
	}
	return 0;
}

// Main Functions
void progressLaser()
{ // Movement of Laser(s)
	for (int i = 0; i < laser_count; i++) // i'th laser
	{
		if (laserHitAst(i))
		{
			int astNum = hitAstNum(i);
			map[laser_y[i]][laser_x[i]] = ' ';
			map[ast_y[i]][ast_x[i]] = ' ';
			map[ast_y[i]+1][ast_x[i]] = ' ';
			neutralizeAsteroid(astNum);
			generateExplosion(laser_x[i],laser_y[i]);
			neutralizeLaser(i);
		}
		else if (laser_y[i] != 0) // if laser not hitting top boundary, move them upward
		{
			map[laser_y[i]][laser_x[i]] = ' ';
			laser_y[i]--;
			map[laser_y[i]][laser_x[i]] = '|';
		}
		else if (laser_y[i]==0) // If laser hit boundary, 
		{
			neutralizeLaser(i);
		}
		if (laserHitEnemy(i))
		{
			int enemyNum = hitEnemyNum(i);
			neutralizeEnemy(enemyNum);
			generateExplosion(laser_x[i], laser_y[i]);
			neutralizeLaser(i);
		}
	}
}
void neutralizeLaser(int i)
{ // Remove a specific laser and perform related operations
	laser_y[i] = 0;
	map[laser_y[i]][laser_x[i]] = ' '; // remove that laser
	sortLaser();
	laser_count--;
}
void sortLaser()
{  // Sort the Array of Laser Coordinates, so the Neutralized Lasers get aside
	for (int j = 0; j < laser_count; j++) // Re-arrange the laser arrays
	{
		for (int k = 0; k < laser_count; k++)
		{
			if (laser_y[k] == 0) // rearranges if this condiion follows
			{
				int temp = 0;
				temp = laser_y[k];
				laser_y[k] = laser_y[k + 1];
				laser_y[k + 1] = temp;

				temp = 0; laser_x[k] = 0;
				temp = laser_x[k];
				laser_x[k] = laser_x[k + 1];
				laser_x[k + 1] = temp;
			}
		}
	}
}
void progressAsteroid()
{ // Movement of Asteroid
	for (int i = 0; i < ast_count; i++)
	{
		if (ast_y[i] != 49) // if Asteroid not hitting bottom boundary
		{
			map[ast_y[i]][ast_x[i]] = ' ';
			ast_y[i]++;
			map[ast_y[i]][ast_x[i]] = '#';
		}
		else if (ast_y[i] == 49) // if it is hitting bottom boundary
		{
			neutralizeAsteroid(i);
		}
		if (AstHitShip(i))
		{
			neutralizeAsteroid(i);
		}
	}
}
void neutralizeAsteroid(int i)
{ // Removal of Asteroid and other related functions
	map[ast_y[i]][ast_x[i]] = ' ';
	ast_y[i] = 49;
	sortAsteroid();
	ast_count--;
}
void progressExplosion()
{ // For creation of Explosion Animation :>
	for (int i = 0; i < exp_count; i++)
	{
		if (exp_state[i] == 3) // first stage of explosion
		{
			map[exp_y[i]][exp_x[i]] = 'O';
		}
		else if (exp_state[i] == 2) // second stage of explosion
		{
			map[exp_y[i]][exp_x[i]] = 'o';
		}
		else if (exp_state[i] == 1) // second stage of explosion
		{
			map[exp_y[i]][exp_x[i]] = '.';
		}
		else if (exp_state[i] == 0) // removal/final stage of explosion
		{
			map[exp_y[i]][exp_x[i]] = ' ';
		}
	}
	for (int i = 0; i < exp_count; i++)
	{
		if (exp_state[i] > 0)
			exp_state[i]--;
	}
}
void sortExplosion()
{	// Sort the Array of Explosion Coordinates, so the Neutralized Lasers get aside
		for (int j = 0; j < exp_count; j++) // Re-arrange the laser arrays
		{
			for (int k = 0; k < exp_count; k++)
			{
				if (exp_state[k] == 0) // rearranges if this condiion follows
				{
					int temp = 0;
					temp = exp_y[k];
					exp_y[k] = exp_y[k + 1];
					exp_y[k + 1] = temp;

					temp = 0; exp_x[k] = 0;
					temp = exp_x[k];
					exp_x[k] = exp_x[k + 1];
					exp_x[k + 1] = temp;

					temp = 0; exp_state[k] = 0;
					temp = exp_state[k];
					exp_state[k] = exp_state[k + 1];
					exp_state[k + 1] = temp;
				}
			}
		}
		exp_count--;
}
void sortAsteroid()
{ // To sort the Array of Asteroids Coordinates
	int count = 0; 
	for (int i = 0; i < ast_count; i++) // Removing all indexes with value 49
	{
		if (ast_y[i] != 49) { 
			ast_y[count] = ast_y[i];
			ast_x[count] = ast_x[i];
			count++;
		}
	}
	for (int i = count; i < ast_count; i++) {
		ast_y[i] = 0;
		ast_x[i] = 0; 
	}
}
void progressEnemy()
{
	for (int i = 0; i < enemy_count; i++)
	{
		if (enemy_direction[i] == 2)
			if ((enemy_x[i] - 1) != 0) // if Left Wing of Enemy is NOT at column = 0 / very left , then move ship left
			{
				clearEnemyCurrentLocation(i);
				enemy_x[i]--;
				updateEnemyLocation(i);
				if (enemy_x[i] == 1) // switching direction
					enemy_direction[i] = 1;
			}
		if (enemy_direction[i] == 1)
		{
			if ((enemy_x[i] + 1) != LAST_X) // if Right Wing of Enemy is NOT at column = 29 / very right , then move ship right
			{
				clearEnemyCurrentLocation(i);
				enemy_x[i]++;
				updateEnemyLocation(i);
				if (enemy_x[i] == LAST_X - 1) // switching direction
					enemy_direction[i] = 2;
			}

		}
		{
			enemy_life[i]++;  // updating enemy's life time
		}
	}
}
void clearEnemyCurrentLocation(int num)
{
	// To Clear the current position of the enemy ship
	map[enemy_y[num]][enemy_x[num] - 1] = ' ';
	map[enemy_y[num]][enemy_x[num]] = ' ';
	map[enemy_y[num]][enemy_x[num] + 1] = ' ';
}
void updateEnemyLocation(int num)
{
	// To reprint the Enemy Ship (Being used after updating the coordinates of the ship)
	map[enemy_y[num]][enemy_x[num] - 1] = '{';
	map[enemy_y[num]][enemy_x[num]] = 'V';
	map[enemy_y[num]][enemy_x[num] + 1] = '}';
}
void neutralizeEnemy(int i)
{
	clearEnemyCurrentLocation(i);
	enemy_y[i] = enemy_x[i] = 49;
	enemy_life[i] = 0;
	sortEnemy();
	enemy_count--;
}
void sortEnemy()
{// To sort the Array of Enemy Ships Coordinates
	int count = 0;
	for (int i = 0; i < enemy_count; i++) // Removing all indexes with value 49
	{
		if (enemy_y[i] != 49) {
			enemy_y[count] = enemy_y[i];
			enemy_x[count] = enemy_x[i];
			if (enemy_life[i] != 0)
			{
				enemy_life[count] = enemy_life[i]; // sorting enemy_life array
			}
			count++;
		}
		
	}
	for (int i = count; i < enemy_count; i++) {
		enemy_y[i] = 0;
		enemy_x[i] = 0;
		enemy_life[i] = 0;
	}
}
void progressStar()
{
	for (int i = 0; i < star_count; i++)
	{
		if (star_y[i] != 49)
		{
			map[star_y[i]][star_x[i]] = ' ';
			star_y[i]++;
			map[star_y[i]][star_x[i]] = '$';
		}
		else if (star_y[i] == 49)
		{
			neutralizeStar(i);
		}
		if (starHitShip(i))
		{
			neutralizeStar(i);
		}
	}
}
void neutralizeStar(int star_n)
{
	map[star_y[star_n]][star_x[star_n]] = ' ';
	star_y[star_n] = 49;
	sortStar();
	star_count--;
}
void sortStar()
{ // To sort the Array of Stars Coordinates
	int count = 0;
	for (int i = 0; i < star_count; i++) // Removing all indexes with value 49
	{
		if (star_y[i] != 49) {
			star_y[count] = star_y[i];
			star_x[count] = star_x[i];
			count++;
		}
	}
	for (int i = count; i < star_count; i++) {
		star_y[i] = 0;
		star_x[i] = 0;
	}
}
void progressMed()
{
	for (int i = 0; i < med_count; i++)
	{
		if (med_y[i] != 49)
		{
			map[med_y[i]][med_x[i]] = ' ';
			med_y[i]++;
			map[med_y[i]][med_x[i]] = '+';
		}
		else if (med_y[i] == 49)
		{
			neutralizeMed(i);
		}
		if (medHitShip(i))
		{
			neutralizeMed(i);
		}
	}
}
void neutralizeMed(int med_n)
{
	map[med_y[med_n]][med_x[med_n]] = ' ';
	med_y[med_n] = 49;
	sortMed();
	med_count--;
}
void sortMed()
{
	int count = 0;
	for (int i = 0; i < med_count; i++) // Removing all indexes with value 49
	{
		if (med_y[i] != 49) {
			med_y[count] = med_y[i];
			med_x[count] = med_x[i];
			count++;
		}
	}
	for (int i = count; i < med_count; i++) {
		med_y[i] = 0;
		med_x[i] = 0;
	}
}
void collectibleCollected(int type)
{
	if (type == 1) // Star Collected
	{
		incScore(20);
	}
	if (type == 2) // Med Kit collected
	{
		incHealth(25);
	}
}
void incScore(int amount)
{
	SCORE += amount;
}
void incHealth(int amount)
{
	if (HEALTH != 100)
	{
		if (HEALTH + amount >= 100)
			HEALTH = 100;
		else
			HEALTH += amount;
	}
}
void decHealth(int amount)
{
	HEALTH -= amount;
}
void incDistance(int amount)
{
	DISTANCE += amount;
}
// Enemey Laser Related
void generateEnemyLaser()
{
	for (int i = 0; i < enemy_count; i++)
	{
		if (enemy_life[i] % 25 == 0) // Laser Generation Delay
			shootEnemyLaser(i);
	}
}
void shootEnemyLaser(int enemy_num)
{
	// Generates an enemy laser
	int z = enemy_num;
	e_laser_x[e_laser_count] = enemy_x[z]; // same X-axis
	e_laser_y[e_laser_count] = enemy_y[z] + 1; // +1 Y-Axis
	map[enemy_y[z] + 1][enemy_x[z]] = '!';
	e_laser_count++;
}
void progressEnemyLaser()
{
	for (int i = 0; i < e_laser_count; i++)
	{
		if (e_laser_y[i] != 49) // if Asteroid not hitting bottom boundary
		{
			map[e_laser_y[i]][e_laser_x[i]] = ' ';
			e_laser_y[i]++;
			map[e_laser_y[i]][e_laser_x[i]] = '!';
		}
		else if (e_laser_y[i] == 49) // if it is hitting bottom boundary
		{
			neutralizeEnemyLaser(i);
		}
		if (EnemyLaserHitShip(i))
		{
			neutralizeEnemyLaser(i);
		}
	}
}
void neutralizeEnemyLaser(int e_laser_num)
{ // Removal of Enemy Laser and other related functions
	int z = e_laser_num;
	map[e_laser_y[z]][e_laser_x[z]] = ' ';
	e_laser_y[z] = 49;
	e_laser_x[z] = 49;
	sortEnemyLaser();
	e_laser_count--;
}
void sortEnemyLaser()
{
	// To sort the Array of Enemy Lasers Coordinates
	int count = 0;
	for (int i = 0; i < e_laser_count; i++) // Removing all indexes with value 49
	{
		if (e_laser_y[i] != 49) {
			e_laser_y[count] = e_laser_y[i];
			e_laser_x[count] = e_laser_x[i];
			count++;
		}
	}
	for (int i = count; i < e_laser_count; i++) {
		e_laser_y[i] = 0;
		e_laser_x[i] = 0;
	}
}


// Progression of Objects
void progressObjects() { // Progression for multiple objects
	progressLaser();
	progressAsteroid();
}
void progressCollectible()
{
	progressStar();
	progressMed();
}

// General Game
void gameOverFn() { // Prints Game Over on screen
	printGameOver();
}
void printGameOver()
{
	initializeMap();
	renderMap();
	gotoxy(10, 20);
	setColor(12);
	cout << "GAME OVER!";
	setColor(7);
	gotoxy(3,25);
	cout << "Press Any Key To Continue";
}
void checkGameStatus()
{
	if (HEALTH <= 0)
		gameOver = 1;
}
// Game Exit
void gameExitFn()
{
	gameExit = 1;
}
// Sound Related
void menuSFX() 
{	// generating simple sound for Menu Interaction
	Beep(800,150);
}
void stopBGM()
{
	PlaySound(NULL,0,0);
}



// Coding and Debugging Related
void renderMap()
{
	setColor(7); // default color
	std::string buffer; // Initializing the buffer
	// Loading Top Boundary here
	buffer.append(COLS + 2, '=');
	buffer.append("\n");
	// Loading inner array
	for (int i = 0; i < ROWS; i++) {
		buffer.append("|");
		for (int j = 0; j < COLS; j++) {
			buffer.push_back(map[i][j]);
		}
		buffer.append("|\n");
	}
	// loading bottom boundary here
	buffer.append(COLS + 2, '=');
	buffer.append("\n");

	// Printing every now
	gotoxy(0, 0);
	std::cout << buffer;
}   // using Buffer Method
 // ^ Rendering with Buffer Method
void gotoxy(int x, int y) // to get rid of System("CLS") and, get better rendering
{
	COORD coord;
	coord.X = x; // Column
	coord.Y = y; // Row
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void setColor(int textColor)
{   // To change the color of the text to be printed ahead
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (0) | textColor);
}
void hideCursor()
{   // To hide the blinking cursor on the console, for better visibility
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = false; 
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}
void setConsoleSize(int width, int height)
{   // To make sure that the console opens up in 720p dimensions, to refrain from any buggy display
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD bufferSize;
	bufferSize.X = width;
	bufferSize.Y = height;
	SetConsoleScreenBufferSize(consoleHandle, bufferSize);
	SMALL_RECT windowSize;
	windowSize.Left = 0;
	windowSize.Top = 0;
	windowSize.Right = width - 1;
	windowSize.Bottom = height - 1;
	SetConsoleWindowInfo(consoleHandle, TRUE, &windowSize);
}
void playBackgroundMusic(const wchar_t* musicFileName)
{
	PlaySound(musicFileName, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // Play the sound file in a loop and asynchronously
}
void debugMsg(int code, int i_frame)
{
	if (code == 1)
	{
		gotoxy(0,55);
		cout << "Asteroid Hit : " << i_frame;
	}
}
void printArr(int arr[],int size, int col, int row)
{
	gotoxy(col, row);
	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << " ";
	}
}
void clearRow(int row)
{
	gotoxy(0, row);
	for (int i = 0; i < 30; i++)
	{
		cout << " ";
	}
}
void printStr(string line, int i_frame, int row)
{
	static int ity = i_frame+10;
	
	gotoxy(0, row);
	cout << line;
	//if (ity == FRAME)
		//clearRow(58);
	//static int activationFrame = -1; // Track when the text was printed
	//static int asteroidHitCount = 0;

	//if (activationFrame == -1) {
	//	activationFrame = i_frame; // Initialize activation frame only on first call
	//}

	//// Print the string at the specified position
	//gotoxy(0, row);
	//cout << line << " : " << ++asteroidHitCount;

	//// Check if the time to clear has arrived
	//if (FRAME >= activationFrame + 20) 
	//{
	//	clearRow(row); // Clear the specified row
	//	activationFrame = -1; // Reset activation frame for the next activation
	//}
}

void shipMovementStatus(int callFrame)
{	// Ship cannot move for 20 frames if it got hit with Asteroid (calling in Asteroid function)
	//static int activationFrame = -1;  // storing the frame at which this function is activated
	//if (shipCantMove == 0)
	//{
	//	activationFrame = callFrame;
	//	shipCantMove = 1;
	//}
	//else if (FRAME >= activationFrame + 20) // restore after 20 frames
	//{
	//	shipCantMove = 0;
	//	activationFrame = -1;
	//}
}
// ^ Cooking something, but not working

//PRogress Note:
// Fix File-Writing
