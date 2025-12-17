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

using namespace std;

const int boardSize = 10;
const char WATER = '~';
const char SHIP = '#';
const char HIT = 'H';
const char MISS = 'M';

// Function Prototypes
void startingGame();
void arrayInitializer(char board[boardSize][boardSize]);
void displayBoard(char board[boardSize][boardSize]);
void placeShip(char board[boardSize][boardSize], int row, int col, int size, char orientation);
bool canPlaceShip(char board[boardSize][boardSize], int row, int col, int size, char orientation);
void placePlayerShips();
void placeComputerShips();
void playingGame();
void attack();
void computerAttack();
void surrender();
bool allShipsSunk(char board[boardSize][boardSize]);
void checkWinLoss(bool& gameOver);

// Global Boards
char playerBoard[boardSize][boardSize];
char playerAttackBoard[boardSize][boardSize];
char computerBoard[boardSize][boardSize];
char computerAttackBoard[boardSize][boardSize];

// Ship sizes
const int shipSizes[5] = { 5, 4, 3, 3, 2 };
const string shipNames[5] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };

// Win/Loss tracking
int playerWins = 0;
int computerWins = 0;

int main()
{
    srand(time(0)); // Seed random generator

    cout << "Welcome to Battleship!\n";

    bool playAgain = true;

    while (playAgain)
    {
        startingGame();
        playingGame();

        cout << "\nCurrent Stats - Wins: " << playerWins << " | Losses: " << computerWins << "\n";

        string answer;
        cout << "\nPlay again? (yes/no): ";
        cin >> answer;
        playAgain = (answer == "yes" || answer == "Yes");
    }

    cout << "Thank you for playing Battleship!\n";
    return 0;
}

/*
Preconditions: None
Postconditions: Initializes all boards and places ships for player and computer
Description: Sets up the game by initializing boards and placing ships
*/
void startingGame()
{
    arrayInitializer(playerBoard);
    arrayInitializer(playerAttackBoard);
    arrayInitializer(computerBoard);
    arrayInitializer(computerAttackBoard);

    cout << "\nYour Board:\n";
    displayBoard(playerBoard);

    placePlayerShips();
    placeComputerShips();
}

/*
Preconditions: A 10x10 char array
Postconditions: Fills the board with WATER symbol
Description: Initializes the board with '~' for water
*/
void arrayInitializer(char board[boardSize][boardSize])
{
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            board[i][j] = WATER;
}

/*
Preconditions: Board must be initialized
Postconditions: Displays board with coordinates
Description: Prints the board with row and column numbers
*/
void displayBoard(char board[boardSize][boardSize])
{
    cout << "   ";
    for (int col = 1; col <= boardSize; col++)
        cout << col << " ";
    cout << "\n  --------------------\n";

    for (int row = 0; row < boardSize; row++)
    {
        if (row + 1 < 10) cout << row + 1 << " |";
        else cout << row + 1 << "|";

        for (int col = 0; col < boardSize; col++)
        {
            cout << board[row][col] << " ";
        }
        cout << endl;
    }
}

/*
Preconditions: Valid row, col, size, orientation
Postconditions: Places ship on the board
Description: Fills board cells with SHIP symbol in given direction
*/
void placeShip(char board[boardSize][boardSize], int row, int col, int size, char orientation)
{
    if (orientation == 'H')
    {
        for (int i = 0; i < size; i++)
            board[row][col + i] = SHIP;
    }
    else
    {
        for (int i = 0; i < size; i++)
            board[row + i][col] = SHIP;
    }
}

/*
Preconditions: Board, starting row/col, size, orientation
Postconditions: Returns true if ship can be placed
Description: Checks if placement is valid
*/
bool canPlaceShip(char board[boardSize][boardSize], int row, int col, int size, char orientation)
{
    if (orientation == 'H')
    {
        if (col + size > boardSize) return false;
        for (int i = 0; i < size; i++)
            if (board[row][col + i] != WATER) return false;
    }
    else
    {
        if (row + size > boardSize) return false;
        for (int i = 0; i < size; i++)
            if (board[row + i][col] != WATER) return false;
    }
    return true;
}

/*
Preconditions: Board initialized
Postconditions: Player places all 5 ships manually
Description: Prompts user for coordinates and orientation for each ship
*/
void placePlayerShips()
{
    for (int i = 0; i < 5; i++)
    {
        bool placed = false;
        while (!placed)
        {
            cout << "\nPlace your " << shipNames[i] << " (size " << shipSizes[i] << ")\n";

            int x, y;
            char orientation;
            cout << "Enter x (column 1-10): ";
            cin >> x;
            cout << "Enter y (row 1-10): ";
            cin >> y;
            cout << "Enter orientation (H for horizontal, V for vertical): ";
            cin >> orientation;

            int row = y - 1;
            int col = x - 1;

            if (row < 0 || row >= boardSize || col < 0 || col >= boardSize)
            {
                cout << "Coordinates out of bounds. Try again.\n";
                continue;
            }

            if (orientation != 'H' && orientation != 'V')
            {
                cout << "Invalid orientation. Try again.\n";
                continue;
            }

            if (canPlaceShip(playerBoard, row, col, shipSizes[i], orientation))
            {
                placeShip(playerBoard, row, col, shipSizes[i], orientation);
                displayBoard(playerBoard);
                placed = true;
            }
            else
            {
                cout << "Cannot place ship here. Try again.\n";
            }
        }
    }
}

/*
Preconditions: Board initialized
Postconditions: Computer places all 5 ships randomly
Description: Randomly places computer ships without overlap
*/
void placeComputerShips()
{
    for (int i = 0; i < 5; i++)
    {
        bool placed = false;
        while (!placed)
        {
            int orientation = rand() % 2; // 0=H, 1=V
            char dir = (orientation == 0) ? 'H' : 'V';
            int row = rand() % boardSize;
            int col = rand() % boardSize;

            if (canPlaceShip(computerBoard, row, col, shipSizes[i], dir))
            {
                placeShip(computerBoard, row, col, shipSizes[i], dir);
                placed = true;
            }
        }
    }
}

/*
Preconditions: Boards initialized
Postconditions: Handles game turns until win, loss, or surrender
Description: Main game loop
*/
void playingGame()
{
    bool gameOver = false;

    while (!gameOver)
    {
        // Show attack board each turn
        cout << "\nYour Attack Board:\n";
        displayBoard(playerAttackBoard);

        int choice;
        cout << "\nChoose an option:\n1. Fire an attack\n2. View your board\n3. Surrender\n";
        cin >> choice;

        switch (choice)
        {
        case 1:
            attack();
            checkWinLoss(gameOver);
            if (gameOver) break;

            computerAttack();
            checkWinLoss(gameOver);
            break;

        case 2:
            cout << "\nYour Board:\n";
            displayBoard(playerBoard);
            break;

        case 3:
            surrender();
            gameOver = true;
            break;

        default:
            cout << "Invalid choice.\n";
        }
    }
}

/*
Preconditions: Computer board initialized
Postconditions: Updates attack board with HIT/MISS
Description: Player fires at computer ships
*/
void attack()
{
    cout << "\nYour Attack Board:\n";
    displayBoard(playerAttackBoard);

    int x, y;
    bool fired = false;

    while (!fired)
    {
        cout << "Enter x (column 1-10): ";
        cin >> x;
        cout << "Enter y (row 1-10): ";
        cin >> y;

        int row = y - 1;
        int col = x - 1;

        if (row < 0 || row >= boardSize || col < 0 || col >= boardSize)
        {
            cout << "Invalid coordinates.\n";
            continue;
        }

        if (playerAttackBoard[row][col] == HIT || playerAttackBoard[row][col] == MISS)
        {
            cout << "You already attacked this location. Try again.\n";
            continue;
        }

        if (computerBoard[row][col] == SHIP)
        {
            cout << "Hit!\n";
            computerBoard[row][col] = HIT;
            playerAttackBoard[row][col] = HIT;
        }
        else
        {
            cout << "Miss!\n";
            computerBoard[row][col] = MISS;
            playerAttackBoard[row][col] = MISS;
        }

        fired = true;
    }
}

/*
Preconditions: Player board initialized
Postconditions: Computer attacks player
Description: Computer randomly fires at player ships
*/
void computerAttack()
{
    int row, col;
    bool attacked = false;

    cout << "\nComputer is attacking...\n";

    while (!attacked)
    {
        row = rand() % boardSize;
        col = rand() % boardSize;

        if (computerAttackBoard[row][col] == HIT || computerAttackBoard[row][col] == MISS)
            continue;

        if (playerBoard[row][col] == SHIP)
        {
            cout << "Computer hit your ship at (x=" << col + 1 << ", y=" << row + 1 << ")!\n";
            playerBoard[row][col] = HIT;
            computerAttackBoard[row][col] = HIT;
        }
        else
        {
            cout << "Computer missed at (x=" << col + 1 << ", y=" << row + 1 << ").\n";
            computerAttackBoard[row][col] = MISS;
        }

        attacked = true;
    }

    cout << "\nYour Board After Computer Attack:\n";
    displayBoard(playerBoard);
}

/*
Preconditions: None
Postconditions: Handles player surrender
Description: Ends current game turn
*/
void surrender()
{
    cout << "You have surrendered.\n";
}

/*
Preconditions: Board initialized
Postconditions: Returns true if all ships are destroyed
Description: Checks if all ship cells have been hit
*/
bool allShipsSunk(char board[boardSize][boardSize])
{
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++)
            if (board[i][j] == SHIP)
                return false;
    return true;
}

/*
Preconditions: Boards initialized
Postconditions: Updates gameOver flag and increments win/loss counters
Description: Checks for win/loss conditions after each turn
*/
void checkWinLoss(bool& gameOver)
{
    if (allShipsSunk(computerBoard))
    {
        cout << "\nYou win! All enemy ships have been sunk.\n";
        playerWins++;
        gameOver = true;
    }
    else if (allShipsSunk(playerBoard))
    {
        cout << "\nYou lost! All your ships have been sunk.\n";
        computerWins++;
        gameOver = true;
    }
}
