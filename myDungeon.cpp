#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "dungeonVariables.h"

using namespace std;

// Declare function prototypes
void constructTerrain();
void printTerrain ();
void addEnemyPositions ();

void gameLoop ();
int levelLoop ();

void incrementScore ();
void advanceLevel ();
bool restartGame ();

int movementLoop ();
int movePlayer (char directionKey);
int moveEnemies ();

int readPlayerTile ();
void openGate ();


int main () {
    bool playAgain;
    
    do {
        // Play game
        gameLoop();
        
        // When finished, player is dead, ask to play again
        playAgain = restartGame ();
        
    } while (playAgain == true);
    
    return 0;
}

bool restartGame () {
    char playAgain;
    
    // Prompt player to play again
    do {
        cout << " Play again? [y]es / [n]o: ";
        cin >> playAgain;
    } while (playAgain != 'y' && playAgain != 'n');
    
    // Convert input to boolean
    bool playAgainBool;
    if (playAgain == 'y')
        playAgainBool = true;
    else if (playAgain == 'n')
        playAgainBool = false;
        
    // Reset score and level number
    score = 0;
    levelNumber = 1;
   
    return playAgainBool;   
}


void gameLoop () {
    int gameResult;
    
    // Set up and play level
    do {
        constructTerrain ();
        
        printTerrain ();
        
        gameResult = levelLoop ();
        
        // Check for level change
        if (gameResult == 2) {
            // Advance variables to next level
            advanceLevel ();
        }
        
    } while (gameResult != 0);  // gameResult 0 = player is dead
}

int levelLoop () {
    int moveResult;
    
    // Enter movement loop    
    do { 
        // Move player and get result
        moveResult = movementLoop();
        
        // If result is 3, player moves to next level
        if (moveResult == 2) 
            return 2;
        
    } while (moveResult != 0);
    
    // If player is dead
    return 0;
}


void advanceLevel () {
    // Increment level variables
    levelNumber = levelNumber + 1;
    int treasureScoreValue = 50 * levelNumber;
    gateIsOpen = false;
            
    // Output level change message
    system ("clear");
    cout << "Level complete!" << endl << endl;  
    cout << "Entering level " << levelNumber << '.' << endl << endl << "Press any key then enter to continue...";
            
    // Wait for input to proceed
    char anyCharInput;
    cin >> anyCharInput;   
}

void constructTerrain () {
    // Seed random generator
    srand(time(0));
    
    // Fill terrain with ' '
    for (int y = 0; y < (lengthOfColumns - 1); ++y) {
        for (int x = 0; x < (lengthOfRows - 1); ++x) {
            terrain[y][x] = ' ';
        }
    }  
    
    // Insert borders
    for (int i = 0; i < (lengthOfColumns -1); ++i) {
        terrain[i][0] = '^';
        terrain[0][i] = '^';
        terrain[i][lengthOfColumns - 2] = '^';
        terrain[lengthOfRows - 2][i] = '^';
    }
    
    // Insert walls
    for (int walls = 0; walls < numberOfWalls; ++walls) {
        // Get wall origin
        int wallYCoord = rand() % (lengthOfColumns - 1) + 2;
        int wallXCoord = rand() % (lengthOfColumns - 1) + 2;
        
        // Print first block of wall
        terrain[wallYCoord][wallXCoord] = wallSymbol;
        
        // Randomly get 0 or 1 as boolean
        int verticalWallDecider = rand() % 2;
        
        // Extend wall according to verticalWallDecider
        for (int length = 0; length < wallLength; ++length) {
            // Check wallDecider and extend the wall respectively
            (verticalWallDecider == 1) ? (++wallYCoord) : (++wallXCoord);
            
            // Print new wall block on terrain
            terrain[wallYCoord][wallXCoord] = wallSymbol;
        }

    }
    
    // Place gate to progress to next level
    int verticalGateDecider = rand() % 2;
    bool verticalGate = static_cast<bool>(verticalGateDecider);
    
    // Generate location of start block of gate
    int gateCoord = rand() % (lengthOfColumns/2);
    
    if (verticalGate == true) {
        // Print start coordinate
        terrain[gateCoord][0] = gateSymbol;
        
        // Extend gate downwards and print bricks
        for (int gate = 0; gate < gateLength; ++ gate) {
            gateCoord = ++gateCoord;
            terrain[gateCoord][0] = gateSymbol;
        }
    } else {
        // Print start coordinate
        terrain[0][gateCoord] = gateSymbol;
        
        // Extend gate right and print bricks
        for (int gate = 0; gate < gateLength; ++ gate) {
            gateCoord = ++gateCoord;
            terrain[0][gateCoord] = gateSymbol;
        }    
    }    

    // Place enemies at random locations
    for (int enemy = 0; enemy < numberOfEnemies; ++enemy) {
        // Get random start coordinates
        int enemyYCoord = rand() % (lengthOfColumns - 3) + 1; 
        int enemyXCoord = rand() % (lengthOfRows - 3) + 1;
        
        // Assign to new enemy
        enemyPositions[enemy][0] = enemyXCoord;
        enemyPositions[enemy][1] = enemyYCoord;        
    }
    
    // Merge enemy positions to terrain
    addEnemyPositions();
    
    // Generate treasure
    numberOfTreasureItems = defaultNumberOfTreasureItems + levelNumber;
    for (int treasureItem = 0; treasureItem < numberOfTreasureItems; ++treasureItem) {
        int treasureYCoord;
        int treasureXCoord;
        
        // Avoid colision with enemy
        bool terrainIsOccupied = true;
        
        do {    
            // Get random location for treasure
            treasureYCoord = rand() % (lengthOfColumns - 4) + 2; 
            treasureXCoord = rand() % (lengthOfRows - 4) + 2;
        
            if (terrain[treasureYCoord][treasureXCoord] != '@') {
                terrainIsOccupied = false;
            }
        } while (terrainIsOccupied == true);
        // Add treasure to terrain
        terrain[treasureYCoord][treasureXCoord] = treasureSymbol;     
    }
    
    // Place player at [0][0]
    playerPosition[0] = 1;
    playerPosition[1] = 1;
    terrain[playerPosition[0]][playerPosition[1]] = playerSymbol;
}

void printTerrain () {
    // Clear screen
    system("clear");
    
    // Get extra line at top for status messages
    cout << endl << "Level: " << levelNumber << "        Score: " << score  <<  "      Hi-score: " << highScore << endl << endl;
    
    // Print terrainLayout 
    for (int y = 0; y < (lengthOfColumns - 1); ++y) {
        for (int x = 0; x < (lengthOfRows - 1); ++x) {
            cout << terrain[y][x] << ' ';
        }
        cout << endl;
    }
}


void addEnemyPositions () {
    // Print all enemy locations
    for (int enemy = 0; enemy < numberOfEnemies; ++enemy) {      
        terrain[(enemyPositions[enemy][1])][(enemyPositions[enemy][0])] = enemySymbol;
    }
}


int movementLoop () {    
    // Move enemies
    int enemyMove = moveEnemies();
    
    // Deal with enemy/player collision
    if (enemyMove == 1) {
        cout << endl << "You got caught by a ghost! You have died!";
        return 0;
    }
    
    // Get player directional input
    char playerInput;
    cin >> playerInput;
        
    // Validate input
    int moveResult;
    if (!(playerInput != 'a' && playerInput != 's' && playerInput != 'd' && playerInput != 'w'))
        // Move player
        moveResult = movePlayer (playerInput);
    else
        cout << "To move, press [a], [w], [s] or [d] and hit enter!";
           
    // Print map
    printTerrain ();
    
    // Take action on moveResult
    switch (moveResult) {
        case 0: // Normal result
            // Check for completion
            if (numberOfTreasureItems == 0) {
                openGate ();
                cout << "You collected all the keys! The gate has now opened!" << endl << endl;
                return 1;
            }
            return 1;
        case 1: // Player dead
             cout << endl << "You have died!";
            return 0;
        case 2: // Player at gate, move to next level
            return 2;
        default:
            return 1;        
    }
}

void openGate () {
    if (gateIsOpen == false) {
        for (int y = 0; y < lengthOfColumns; ++y) {
            for (int x = 0; x < lengthOfRows; ++x) {
                char tile = terrain[y][x];
                if (tile == 'X') {
                    terrain[y][x] = '\'';
                }
            }
        }
        // Update printed map
        printTerrain();
        
        // Update gate status
        gateIsOpen = true;   
    }
    
}


int moveEnemies () {
    for (int enemy = 0; enemy < numberOfEnemies; ++enemy) {  
        // Save old position
        int oldEnemyYPosition = enemyPositions[enemy][1], oldEnemyXPosition = enemyPositions[enemy][0];
        
        // Overwrite old position on terrain
        terrain[oldEnemyYPosition][oldEnemyXPosition] = ' ';
    
        // Get random between 1-4 to decide direction of move
        int randomMove = rand() % 4 + 1;
        
        // Add move to location
        switch (randomMove) {
            case 1:
                enemyPositions[enemy][1] = enemyPositions[enemy][1] - 1;
                break;
            case 2:
                enemyPositions[enemy][1] = enemyPositions[enemy][1] + 1;
                break;
            case 3:
                enemyPositions[enemy][0] = enemyPositions[enemy][0] + 1;
                break;
            case 4:
                enemyPositions[enemy][0] = enemyPositions[enemy][0] - 1;
                break;
            default:
                break;
        }
    
        // Update location to target tile
        int newYPosition = enemyPositions[enemy][1], newXPosition = enemyPositions[enemy][0];
        
        // Evaluate target tile collisions
        char targetTile = terrain[newYPosition][newXPosition];
        
        // If player has been hit
        if (targetTile == '#') {
            return 1;
        }
        
        // Deal with obstacles, reset to old position (don't move)
        if (targetTile == '^' || targetTile == '*' || targetTile == '@' || targetTile == 'X' || targetTile == '\'') {
            // Reset player position
            enemyPositions[enemy][1] = oldEnemyYPosition;
            enemyPositions[enemy][0] = oldEnemyXPosition;
        }
    }
    
    // Merge new positions
    addEnemyPositions();
    
    return 0;
}


int movePlayer (char directionKey) {
    // Delete original player symbol from terrain
    int oldPlayerYPosition = playerPosition[0], oldPlayerXPosition = playerPosition[1];
    terrain[playerPosition[0]][playerPosition[1]] = ' ';
    
    // Update player position coordinates
    switch (directionKey) {
        case 'a':
            playerPosition[1] = playerPosition[1] - 1;
            break;
        case 's':
            playerPosition[0] = playerPosition[0] + 1;
            break;
        case 'd':
            playerPosition[1] = playerPosition[1] + 1;
            break;
        case 'w':
            playerPosition[0] = playerPosition[0] - 1;
            break;
        default:
            break;
    }
    
    // Get info on tile player moved to
    int changePlayerPosition = readPlayerTile ();
    
    switch (changePlayerPosition) {
        case 0: // Wall or obstacle
            // Reset player position
            playerPosition[0] = oldPlayerYPosition;
            playerPosition[1] = oldPlayerXPosition;
            break;
        case 1: // Move normally
            break;
        case 2: // Player is dead
            return 1;
            break;
        case 3: // Player is on opened gate
            return 2;
            break;
        default:
            break;
    }

    // Print player symbol on terrain
    terrain[playerPosition[0]][playerPosition[1]] = playerSymbol;
    
    return 0;
}
void incrementScore () {
    score = score + treasureScoreValue;
    
    // Check if hi-score has been beaten and if so update it
    if (highScore <= score) {
        highScore = score;
    }
}

int readPlayerTile () {
    char playerTile = terrain[playerPosition[0]][playerPosition[1]];
    switch (playerTile) {
        case '*':
            cout << "You found a key!";
            numberOfTreasureItems = numberOfTreasureItems - 1;
            
            // Increment score
            incrementScore();
            
            return 1;
        case '^':
            return 0;
        case 'X':
            cout << "To unlock this gate collect all the keys (*)";
            return 0;
        case '@':
            cout << "You die." << endl;
            return 2;
        case '\'':
            return 3; // Player is at unlocked gate
        default:
            return 1;
    }
}