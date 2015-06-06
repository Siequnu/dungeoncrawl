#ifndef DUNGEONVARIABLES_H
#define DUNGEONVARIABLES_H

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

    const std::string playerHealthSymbol = "♥";
    const char defaultPlayerHealth = 1;
    int playerHealth;
    std::string playerHealthString = "";

    const char treasureSymbol = '*';
    const int defaultNumberOfTreasureItems = 1;
    int numberOfTreasureItems;

    const char wallSymbol = '^';
    int numberOfWalls = 9;
    const int wallLength = 9;

    const char enemySymbol = '@';
    const int numberOfEnemies = 5;
    int enemyPositions[numberOfEnemies][2];

    const char gateSymbol = 'X';
    const char gateSymbolOpen = '\'';
    int gateLength = 5;
    bool gateIsOpen = false;

#endif