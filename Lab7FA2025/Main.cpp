/*
	Olivia Dodds
	C++ FA2025
	Due: Dec 16th, 2025
	Lab 7 Battleship
	Play against the computer in a game of Battleship
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

//Constants
const int boardSize = 10;
const char WATER = '~';
const char SHIP = '#';
const char HIT = 'H';
const char MISS = 'M';

//Function Prototypes
void startingGame();
void playingGame();

void initializeBoard(char board[boardSize][boardSize]);
void displayBoard(const char board[boardSize][boardSize]);

bool canPlaceShip(char board[boardSize][boardSize], int row, int col, int size, char orientation);

void placeShip(char board[boardSize][boardSize], int row, int col, int size, char orientation);
void placeComputerShips();
void placePlayerShips();

void attack();
void surrender();

//Global boards
char playerBoard[boardSize][boardSize];
char playerAttackBoard[boardSize][boardSize];
char computerBoard[boardSize][boardSize];
char computerAttackBoard[boardSize][boardSize];

int main()
{
	srand(time(0));

	std::cout << "Welcome to Battleship! Here is your board. \n" << std::endl;

	bool playAgain = true;

	while (playAgain)
	{
		startingGame();
		playingGame();

		std::string answer;
		std::cout << "Play again? (yes/no): ";
		std::cin >> answer;

		playAgain = (answer == "yes" || answer == "Yes");
	}

	return 0;
}

/*
	Preconditions: None
	Postconditions: Displays user's board and initializes the other boards
	Desc: Initializes all arrays, and formats user's playing board, displaying it for ship placement
*/
void startingGame()
{
	//initializing boards
	initializeBoard(playerBoard);
	initializeBoard(playerAttackBoard);
	initializeBoard(computerBoard);
	initializeBoard(computerAttackBoard);

	placePlayerShips();
	placeComputerShips();
}

/*
	Preconditions: Must have a char array
	Postconditions: Initializes the array
	Desc: Takes an array and fills it with '~' for the water
*/
void initializeBoard(char curArray_[boardSize][boardSize])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			curArray_[i][j] = WATER;
		}
	}
}

void displayBoard(const char board[boardSize][boardSize])
{
	std::cout << "x  ";
	for (int i = 1; i <= boardSize; i++)
		std::cout << i << " ";
	std::cout << "\n   --------------------\n";

	for (int i = 0; i < boardSize; i++)
	{
		std::cout << i + 1;
		if (i < 9) std::cout << " |";
		else std::cout << "|";

		for (int j = 0; j < boardSize; j++)
		{
			std::cout << board[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

/*
	Preconditions:
	Postconditions:
	Desc:
*/
bool canPlaceShip(char board[boardSize][boardSize], int row, int col, int size, char orientation)
{
	if (orientation == 'H')
	{
		if (col + size > boardSize) //out of bounds
		{
			return false;
		}

		for (int i = 0; i < size; i++)
		{
			if (board[row][col + i] != WATER)
				return false;
		}
	}
	else
	{
		if (row + size > boardSize)
		{
			return false;
		}
		for (int i = 0; i < size; i++)
		{
			if (board[row + i][col] != WATER)
			{
				return false;
			}
		}
	}

	return true;
}

void placeShip(char board[boardSize][boardSize], int row, int col, int size, char orientation)
{
	if (orientation == 'H' || orientation == 'h')
	{
		for (int i = 0; i < size; i++)
		{
			board[row][col + i] = SHIP;
		}
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			board[row + i][col] = SHIP;
		}
	}
}

void placeComputerShips()
{
	int shipSizes[5] = { 5, 4, 3, 3, 2 };

	for (int i = 0; i < 5; i++)
	{
		bool placed = false;

		while (!placed) 
		{
			int row = rand() % boardSize;
			int col = rand() % boardSize;
			char orientation = (rand() % 2 == 0) ? 'H' : 'V';

			if (canPlaceShip(computerBoard, row, col, shipSizes[i], orientation))
			{
				placeShip(computerBoard, row, col, shipSizes[i], orientation);
				placed = true;
			}
		}
	}
}

void placePlayerShips()
{
	int shipSizes[5] = { 5, 4, 3, 3, 2 };
	std::string shipNames[5] =
	{
		"Carrier (5)",
		"Battleship (4)",
		"Cruiser (3)",
		"Submarine (3)",
		"Destroyer (2)",
	};

	for (int i = 0; i < 5; i++)
	{
		bool placed = false;

		while (!placed)
		{
			int row, col;
			char orientation;

			displayBoard(playerBoard);

			std::cout << "\nPlacing " << shipNames[i] << std::endl;
			std::cout << "Enter starting column (1-10): ";
			std::cin >> col;
			std::cout << "Enter starting row (1-10): ";
			std::cin >> row;
			std::cout << "Enter orientation (H for horizontal, V for vertical): ";
			std::cin >> orientation;

			//converting to 0-based index
			row--;
			col--;

			if (row < 0 || row > boardSize || col < 0 || col >= boardSize)
			{
				std::cout << "Invalid starting position, try again.\n";
				continue;
			}

			if (!canPlaceShip(playerBoard, row, col, shipSizes[i], orientation))
			{
				std::cout << "Cannot place ship there, try again.\n";
				continue;
			}

			placeShip(playerBoard, row, col, shipSizes[i], orientation);
			placed = true;
		}
	}
}

/*
	Preconditions:
	Postconditions:
	Desc:
*/
void playingGame()
{
	bool isGameOver = false;

	while (!isGameOver)
	{
		displayBoard(playerAttackBoard);

		int choice;
		std::cout << "\nChoose an option:\n 1. Fire an attack\n 2. View your board\n 3.Surrender\n";
		std::cin >> choice;

		switch (choice)
		{
		case 1: attack();
			break;
		case 2: std::cout << "Your board:\n";
			displayBoard(playerBoard);
			break;
		case 3: surrender();
			isGameOver = true;
			break;
		default:
			std::cout << "Invalid choice.\n";
		}
	}
}

/*
	Preconditions:
	Postconditions:
	Desc:
*/
void attack()
{
	int row, col;
	bool hasFired = false; //controls the while loop

	std::cout << "Enter the row and column to attaack (1-10)" << std::endl;

	while (!hasFired)
	{
		std::cin >> col >> row;
		col--;
		row--;

		if (row < 0 || row >= boardSize || col < 0 || col >= boardSize)
		{
			std::cout << "Invalid coordinates.Try again:\n";
			continue;
		}

		if (computerBoard[row][col] == HIT || computerBoard[row][col] == MISS)
		{
			std::cout << "You already attacked this location, try again:";
			continue;
		}

		if (computerBoard[row][col] == SHIP)
		{
			std::cout << "Hit!\n";
			computerBoard[row][col] = HIT;
			playerAttackBoard[row][col] = HIT;
		}
		else 
		{
			std::cout << "Miss!\n";
			computerBoard[row][col] = MISS;
			playerAttackBoard[row][col] = MISS;
		}

		hasFired = true;
	}
}

/*
	Preconditions:
	Postconditions:
	Desc:
*/
void surrender()
{
	std::cout << "You have surrendered.\n" << std::endl;
}