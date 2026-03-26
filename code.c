#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

char *catFrames[] =  {
  "\033[3A\033[K  A_A \n>('w')<\n (j l)j\n",  // first
  "\033[3A\033[K  A_A \n>(-w-)<\n (j l)j\n",  // blink
  "\033[3A\033[K  A_A \n>('w')<\n (j l)j\n",  // tail 1
  "\033[3A\033[K  A_A \n>('w')<\n (j l)/\n",  // tail 2
  "\033[3A\033[K  A_A \n>('w')<\n (j l)-\n",  // tail 3
  "\033[3A\033[K  A_A \n>('w')<\n (j l)/\n",  // tail 4
  "\033[3A\033[K  A_A \n>('w')<\n (j l)j\n",  // tail 5
  "\033[3A\033[K  A_A .\n>(-w-)<\n (j l)j\n",  // meow 1
  "\033[3A\033[K  A_A .<\n>(-v-)<\n (j l)j\n",  // meow 2
  "\033[3A\033[K  A_A .<meow!>\n>('O')<\n (j l)j\n",  // meow 3
  "\033[3A\033[K  A_A .<meow!>\n>('o')<\n (j l)j\n",  // meow 4
  "\033[3A\033[K  A_A .\n>('w')<\n (j l)j\n",  // meow 5
};

void sleep_float(float seconds) {
    struct timespec ts;
    ts.tv_sec = (time_t)seconds;
    ts.tv_nsec = (long)((seconds - ts.tv_sec) * 1e9); // convert decimal to nanoseconds
    nanosleep(&ts, NULL);
}

// print cat gif
void printCat () {
    // get the amount of frames
    int count = 0;
    while (catFrames[count] != NULL) {
        count++;
    }

    for (int i = 0; i < count; i++) {
        printf("%s", catFrames[i]);
        sleep_float(0.356);
    }
}

// all the slots, edited by user
char slots[9] = "123456789";

// characters for players
char playerChars[2] = "xo";

// error message edited when handled
const char *error = "\033[31mPlease input a valid cell!\033[0m";
char errorStr[35] = "\033[K";

int board[9] = {0};

int usedCells[9] = {0};

// contains the colors for the
// edited slots
char slotColors[9][9] = {
    "", "", "",
    "", "", "",
    "", "", "",
};

// the colors used to edit slotColors
const char *playerColors[2] = {
    "\033[31m", // player 1
    "\033[34m" // player 2
};

const int winCombinations[8][3] = {
    {0,1,2}, {3,4,5}, {6,7,8}, // horizontal
    {0,3,6}, {1,4,7}, {2,5,8}, // vertical
    {0,4,8}, {2,4,6} // diagonal
};

int checkWin(int player) {
    for (int w = 0; w < 8; w++) {
        if (board[winCombinations[w][0]] == player &&
            board[winCombinations[w][1]] == player &&
            board[winCombinations[w][2]] == player) {
            return 1;
            }
    }
    return 0;
}

// print grid
void printGrid() {
    printf("\033[8A%s\n%s%c \033[0m| %s%c \033[0m| %s%c\n\033[0m─────────\n%s%c \033[0m| %s%c \033[0m| %s%c\n\033[0m─────────\n%s%c \033[0m| %s%c \033[0m| %s%c\033[0m\n\n",
    errorStr,
    slotColors[6], slots[6], slotColors[7], slots[7], slotColors[8], slots[8],
    slotColors[3], slots[3], slotColors[4], slots[4], slotColors[5], slots[5],
    slotColors[0], slots[0], slotColors[1], slots[1], slotColors[2], slots[2]);
}

int main () {

    // so that printing a new grid doesnt eat up your prompt
    // the solution of doom #1
    puts("\n\n\n\n\n\n\n");

    // current player, always at
    // player 1 by default
    int currentPlr = 1;
    int prevPlr = 2;

    char input;

    for (int i = 0; i < 9; i++) {
        printGrid();

        // if player won
        if (checkWin(prevPlr)) {
            printf("\033[B");
            printGrid();
            printf("\033[2K\033[33mPlayer %d has won!\033[0m\n\n", prevPlr);
            return prevPlr;
        }

        // the question
        printf("Player %d, please, pick a valid grid:\033[1C \033[K\033[1D", currentPlr);
        // the solution of doom #2
        scanf(" %c%*", &input);
        while (getchar() != '\n' && getchar() != EOF);

        // check input and use it
        int cnt = 0;
        int j;
        int validity = 0;
        for (j = 1; j < 10; j++) {

            if (usedCells[j - 1] == 1) {
                continue;
            }

            if ((input - '0') == j) {
                slots[j - 1] = playerChars[currentPlr - 1];
                board[j - 1] = currentPlr;
                strcpy(slotColors[j - 1], playerColors[currentPlr - 1]);
                usedCells[j - 1] = 1;

                validity = 1;

                break;
            } else {
                continue;
            }
        }

        // (in)validity handle
        if (!validity) {
            strcpy(errorStr, error);
            if (i != 0) {
                i--;
            }
            continue;
        }

        // toggle current player
        switch (currentPlr) {
            case 1:
            currentPlr = 2;
            prevPlr = 1;
            break;
            case 2:
            currentPlr = 1;
            prevPlr = 2;
            break;
        }

        // set error message to blank
        strcpy(errorStr, "\033[K");
    }

    // check for incase two winning combinations
    // are made. we really dont know why it didnt
    // work in the for loop...
    // cat is only here cus easter egg >:3
    // ... sorry player 2!
    if (checkWin(prevPlr)) {
        printf("\033[B");
        printGrid();
        printf("\033[2K\033[33mPlayer %d has won!\033[0m\033[K\n\n", prevPlr);
        // print cat
        puts("\n\n");
        printCat();
        puts("\33[2K\r"); // clear weird err msg that appears
        return prevPlr;
    }

    // print final result
    printf("\033[B");
    printGrid();
    printf("\033[34mNobody won, what a bummer!\033[K\n\n\033[0m");
    return 0;
}
