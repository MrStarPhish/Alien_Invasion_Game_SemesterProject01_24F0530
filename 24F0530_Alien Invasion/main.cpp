#include<iostream>
#include<windows.h> // for Sleep() and gotoxy()
#include<conio.h> // for _getch() and _kbhit()
#include<string> // for storing name


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
int stage = 1; // 0 for Menu, 1 for Level-1, 2 for Level-2
// Variable for Speed Factor
int s = 1;
// Map Related
const int ROWS = 50;
const int COLS = 30;
const int LAST_X = COLS - 1, LAST_Y = ROWS - 1;
char map[ROWS][COLS] = {};
// General Game Mechanics Flags
bool DevNotes = 0;
bool gameOver = 0;
bool astHitShip = 0;
bool shipCantMove = 0;
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
void setConsoleSize(int width, int height);
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
// GameBar Related
void displayGameBarStats();
void clearGameBarStats();
void printScore();
void printDistance();
void printHealth();
void printHealthBar();
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
void gameOverFn();
void checkGameStatus();







int main()
{	// ALL THE INITIALS HERE aka One Timers
	hideCursor();
	setConsoleSize(1280, 720);
	initializeMap();
	resetScore(); resetHealth(); resetDistance();


	do { // -------------------------- GAME LOOP
		
	FRAME++;   // keeping track of game's progression

		if (stage == 1) // Level 1
		{
			s = 1; // speed factor
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
			
			// Slowed Down Progression , For Game Difficulty
			if (FRAME % s == 0)
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
			if (DISTANCE!=0 && DISTANCE % 100 == 0)
				incScore(20);  // every 100m distance gives +20 score

			cleanBoundary(); // to remove any buggy display at top/bottom
			keyListen();
			checkGameStatus();
			Sleep(17); // Game Overall Speed
		}
	} while (gameOver==0);

	if (gameOver)
	{
		gameOverFn();
	}

	system("pause");
	return 0;
}


// Object Generation Related
void generateObject(int i_frame)
{
	int z = i_frame;
	int delay = 30; // Basic frame-difference/timing between generation of each type of object
	static int asteroidTimer = 0;
	static int enemyTimer = 0;
	static int collectibleTimer1 = 0;
	static int collectibleTimer2 = 0;

	// Generating Objects
	if (z % delay == 0 && asteroidTimer <= 0)
	{
		generateAsteroid();
		asteroidTimer = delay;
	}
	if (z % (2*delay) == 0 && asteroidTimer <= 0)
	{
		int temp = rand()%(20-10+1)+10; // (max-min+1)+min , generating random number in range [min,max]
		generateEnemy(temp);
		enemyTimer = 2*delay;
	}
	if (z % (1*delay) == 0 && asteroidTimer <= 0)
	{
		generateCollectible(1); // Star
		collectibleTimer1 = delay;
	}
	if (z % (3 * delay) == 0 && asteroidTimer <= 0)
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
void renderMap() 
{ // To print everything
	gotoxy(0, 0);
	for (int i = 0; i < COLS + 2; i++) // Top Boundary
	{
		cout << "=";
	}
	cout << endl;

	for (int i = 0; i < ROWS; i++) // Inner Game
	{
		gotoxy(0,i+1);
		cout << "|";
		for (int j = 0; j < COLS; j++)
		{
			cout << map[i][j];
			
		}
		cout << "|";
	}
	gotoxy(0, 51);
	for (int i = 0; i < COLS + 2; i++) // Top Boundary
	{
		cout << "=";
	}
	gotoxy(0, 0);
}
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
	printFrameCount();
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
void clearDevNotes()
{
	for (int i = 0; i <= 15; i++)
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
// GameBar Related
void displayGameBarStats()
{
	printScore();
	printDistance();
	printHealth();
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
	cout << "SCORE: " <<"$ "<< SCORE;
}
void printDistance()
{
	gotoxy(2, 55);
	cout << "DISTANCE: " << DISTANCE<<" m";
}
void printHealth()
{
	gotoxy(2, 57);
	cout << "SHIP HEALTH: ";
	printHealthBar();
}
void printHealthBar()
{
	cout << "[";
	for (int i = 1; i <= (HEALTH / 10); i++)
	{
		cout << "/";
	}
	gotoxy(26,57);
	cout << "]";
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
			decHealth(20);
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
	cout << endl;
	cout << "Game Over" << endl;
}
void checkGameStatus()
{
	if (HEALTH <= 0)
		gameOver = 1;
}


// Coding and Debugging Related
void gotoxy(int x, int y) // to get rid of System("CLS") and, get better rendering
{
	COORD coord;
	coord.X = x; // Column
	coord.Y = y; // Row
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
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

// Progress Note:  
// Work on Collectibles.

void shipMovementStatus(int callFrame)
{	// Ship cannot move for 20 frames if it got hit with Asteroid (calling in Asteroid function)
	static int activationFrame = -1;  // storing the frame at which this function is activated
	if (shipCantMove == 0)
	{
		activationFrame = callFrame;
		shipCantMove = 1;
	}
	else if (FRAME >= activationFrame + 20) // restore after 20 frames
	{
		shipCantMove = 0;
		activationFrame = -1;
	}
}
