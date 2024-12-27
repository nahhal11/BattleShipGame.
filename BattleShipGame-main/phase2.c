#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define SIZE 10
#define EMPTY '-'
#define SHIP 'S'
#define HIT '*'
#define MISS 'O'

typedef struct {
    int x, y;
} Shot;

// Function prototypes
void setupBoard(char board[SIZE][SIZE]);
void printBoard(char board[SIZE][SIZE], int hideShips);
void placeShipsManual(char board[SIZE][SIZE]);
void placeShipsEasy(char board[SIZE][SIZE]);
void placeShipsMedium(char board[SIZE][SIZE]);
void placeShipsHard(char board[SIZE][SIZE]);
int isMoveValid(char board[SIZE][SIZE], int row, int col);
int makeMove(char board[SIZE][SIZE], int row, int col);
Shot getShotEasy();
Shot getShotMedium(char board[SIZE][SIZE]);
Shot getShotHard(char board[SIZE][SIZE]);

int gameFinished(char board[SIZE][SIZE]);
void placeShip(char board[SIZE][SIZE], int x, int y, int size, int horizontal);
int canPlaceShip(char board[SIZE][SIZE], int x, int y, int size, int horizontal);

// Setup an empty board
void setupBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
}

// Print the board with optional hidden ships
void printBoard(char board[SIZE][SIZE], int hideShips) {
    printf("  ");
    for (int col = 1; col <= SIZE; col++) {
        printf("%d ", col);
    }
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%c ", 'A' + i);
        for (int j = 0; j < SIZE; j++) {
            if (hideShips && board[i][j] == SHIP) {
                printf("%c ", EMPTY);
            } else {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
}

// Place ships manually
void placeShipsManual(char board[SIZE][SIZE]) {
    int shipSizes[] = {5, 4, 3, 2};
    int numShips = 4;

    printf("Place your ships on the board.\n");
    printBoard(board, 0);

    for (int i = 0; i < numShips; i++) {
        int size = shipSizes[i];
        int x, y, horizontal, placed = 0;

        while (!placed) {
            printf("Place ship of size %d (enter row [A-J], column [1-10], and orientation [0: vertical, 1: horizontal]): ", size);
            char rowChar;
            scanf(" %c %d %d", &rowChar, &x, &horizontal);
            rowChar = toupper(rowChar); // Ensure row is uppercase
            y = rowChar - 'A';          // Convert row letter to index
            x -= 1;                     // Convert column number to index

            if (canPlaceShip(board, x, y, size, horizontal)) {
                placeShip(board, x, y, size, horizontal);
                placed = 1;
                printf("Ship placed successfully.\n");
                printBoard(board, 0);
            } else {
                printf("Invalid placement. Try again.\n");
            }
        }
    }
}
// Place ships for Easy difficulty
void placeShipsEasy(char board[SIZE][SIZE]) {
    int shipSizes[] = {5, 4, 3, 3, 2};
    int numShips = 5;

    for (int i = 0; i < numShips; i++) {
        int size = shipSizes[i];
        int placed = 0;

        while (!placed) {
            int x = rand() % SIZE;
            int y = rand() % SIZE;
            int horizontal = rand() % 2;

            if (canPlaceShip(board, x, y, size, horizontal)) {
                placeShip(board, x, y, size, horizontal);
                placed = 1;
            }
        }
    }
}
// Place ships for Medium difficulty
void placeShipsMedium(char board[SIZE][SIZE]) {
    placeShipsEasy(board);
    if (canPlaceShip(board, 0, 0, 3, 1)) {
        placeShip(board, 0, 0, 3, 1);
    }
}

// Not Randomly place ships for hard difficulty
void placeShipsHard(char board[SIZE][SIZE]) {
    int shipSizes[] = {5, 4, 3, 3, 2}; // Sizes of ships
    int numShips = 5;

    for (int i = 0; i < numShips; i++) {
        int placed = 0;

        while (!placed) {
            // Divide the board into quadrants and target each quadrant progressively
            int quadrant = rand() % 4; // Randomly select a quadrant
            int startX, startY;
            switch (quadrant) {
                case 0: // Top-left quadrant
                    startX = rand() % (SIZE / 2);
                    startY = rand() % (SIZE / 2);
                    break;
                case 1: // Top-right quadrant
                    startX = rand() % (SIZE / 2);
                    startY = SIZE / 2 + rand() % (SIZE / 2);
                    break;
                case 2: // Bottom-left quadrant
                    startX = SIZE / 2 + rand() % (SIZE / 2);
                    startY = rand() % (SIZE / 2);
                    break;
                case 3: // Bottom-right quadrant
                    startX = SIZE / 2 + rand() % (SIZE / 2);
                    startY = SIZE / 2 + rand() % (SIZE / 2);
                    break;
            }

            int horizontal = rand() % 2; // Randomly choose orientation
            if (canPlaceShip(board, startX, startY, shipSizes[i], horizontal)) {
                placeShip(board, startX, startY, shipSizes[i], horizontal);
                placed = 1;
            }
        }
    }
}

// Check if a move is valid
int isMoveValid(char board[SIZE][SIZE], int row, int col) {
    return row >= 0 && row < SIZE && col >= 0 && col < SIZE &&
           (board[row][col] == EMPTY || board[row][col] == SHIP);
}

// Make a move
int makeMove(char board[SIZE][SIZE], int row, int col) {
    if (board[row][col] == SHIP) {
        board[row][col] = HIT;
        return 1;
    } else {
        board[row][col] = MISS;
        return 0;
    }
}

// Check if the game is finished
int gameFinished(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == SHIP) {
                return 0;
            }
        }
    }
    return 1;
}

// Helper to place a ship
void placeShip(char board[SIZE][SIZE], int x, int y, int size, int horizontal) {
    for (int i = 0; i < size; i++) {
        if (horizontal) {
            board[y][x + i] = SHIP;
        } else {
            board[y + i][x] = SHIP;
        }
    }
}

// Check if a ship can be placed
int canPlaceShip(char board[SIZE][SIZE], int x, int y, int size, int horizontal) {
    for (int i = 0; i < size; i++) {
        if (horizontal) {
            if (x + i >= SIZE || board[y][x + i] != EMPTY) {
                return 0;
            }
        } else {
            if (y + i >= SIZE || board[y + i][x] != EMPTY) {
                return 0;
            }
        }
    }
    return 1;
}

// Easy bot logic
Shot getShotEasy() {
    Shot shot;
    shot.x = rand() % SIZE;
    shot.y = rand() % SIZE;
    return shot;
}
// Medium bot logic
Shot getShotMedium(char board[SIZE][SIZE]) {
    Shot shot;
    static int turn = 0;

    if (turn % 2 == 0) {
        shot.x = rand() % SIZE;
        shot.y = rand() % SIZE;
    } else {
        shot.x = rand() % SIZE;
        shot.y = rand() % SIZE;
    }
    turn++;
    return shot;
}
// Hard bot logic
Shot getShotHard(char board[SIZE][SIZE]) {
    static int lastHitX = -1, lastHitY = -1; // Stores the last successful hit
    static int targetMode = 0; // 0 = hunting, 1 = targeting
    Shot shot;

    if (targetMode == 1) {
        // Target mode: Attack adjacent cells
        if (lastHitX > 0 && board[lastHitX - 1][lastHitY] == EMPTY) {
            shot.x = lastHitX - 1;
            shot.y = lastHitY;
        } else if (lastHitX < SIZE - 1 && board[lastHitX + 1][lastHitY] == EMPTY) {
            shot.x = lastHitX + 1;
            shot.y = lastHitY;
        } else if (lastHitY > 0 && board[lastHitX][lastHitY - 1] == EMPTY) {
            shot.x = lastHitX;
            shot.y = lastHitY - 1;
        } else if (lastHitY < SIZE - 1 && board[lastHitX][lastHitY + 1] == EMPTY) {
            shot.x = lastHitX;
            shot.y = lastHitY + 1;
        } else {
            // If all neighbors are explored, switch back to hunting
            targetMode = 0;
        }
    }

    if (targetMode == 0) {
        // Hunting mode: Random selection
        do {
            shot.x = rand() % SIZE;
            shot.y = rand() % SIZE;
        } while (board[shot.x][shot.y] != EMPTY); // Avoid previously attacked cells
    }

    // Update last hit if a successful shot is made
    if (board[shot.x][shot.y] == SHIP) {
        lastHitX = shot.x;
        lastHitY = shot.y;
        targetMode = 1; // Switch to target mode
    }

    return shot;
}

// Main game loop
void play_battleship() {
    char humanGrid[SIZE][SIZE], botGrid[SIZE][SIZE];
    int playerTurn = 1; // 1 = player, 0 = bot
    srand(time(NULL));

    setupBoard(humanGrid);
    setupBoard(botGrid);

    placeShipsManual(humanGrid);
    placeShipsHard(botGrid);

    printf("Your board:\n");
    printBoard(humanGrid, 0);

    while (1) {
        char rowChar;
        int col, row;
        Shot botShot;

        if (playerTurn) {
            printf("Your turn! Enter row (A-J) and column (1-10): ");
            scanf(" %c %d", &rowChar, &col);
            rowChar = toupper(rowChar);
            row = rowChar - 'A';
            col -= 1;

            if (!isMoveValid(botGrid, row, col)) {
                printf("Invalid move, try again.\n");
                continue;
            }

            if (makeMove(botGrid, row, col)) {
                printf("You hit a ship!\n");
            } else {
                printf("You missed.\n");
            }
        } else {
            botShot = getShotEasy();
            printf("Bot attacks (%c, %d).\n", 'A' + botShot.y, botShot.x + 1);

            if (makeMove(humanGrid, botShot.y, botShot.x)) {
                printf("The bot hit your ship!\n");
            } else {
                printf("The bot missed.\n");
            }
        }

        printf("\nBot's Board:\n");
        printBoard(botGrid, 1);
        printf("\nYour Board:\n");
        printBoard(humanGrid, 0);

        if (gameFinished(botGrid)) {
            printf("You win!\n");
            break;
        } else if (gameFinished(humanGrid)) {
            printf("The bot wins!\n");
            break;
        }

        playerTurn = !playerTurn;
    }
}

int main() {
    play_battleship();
    return 0;
}
