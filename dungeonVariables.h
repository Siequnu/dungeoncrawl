#ifndef PROGRAM_VARIABLES_H
#define PROGRAM_VARIABLES_H
        
    // Declare game variables
    int levelNumber = 1;
    int score;
    int highScore;
    int treasureScoreValue = 50 * levelNumber;
    
    const int lengthOfRows = 30;
    const int lengthOfColumns = 30;
    char terrain[lengthOfColumns][lengthOfRows];
    
    const char playerSymbol = '#';
    char playerPosition[2] = {1, 1};
    
    const char treasureSymbol = '*';
    const int defaultNumberOfTreasureItems = 1;
    int numberOfTreasureItems;
    
    const char wallSymbol = '^';
    int numberOfWalls = 7;
    const int wallLength = 9;
    
    const int numberOfEnemies = 5;
    int enemyPositions[numberOfEnemies][2];
    const char enemySymbol = '@';
    
    const char gateSymbol = 'X';
    int gateLength = 5;
    
#endif