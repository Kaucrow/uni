#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <algorithm>
using   std::cout, std::endl, std::setw, std::fill_n,
        std::any_of, std::begin, std::end;

void ClrScr();

int main(){
    //      B       I       N       G       O
    //    1-15    16-30   31-45   46-60   61-75    < -- number range for each row
    // testmsg
    // SET UP VARIABLES, ARRAYS, AND RAND() SEED
    srand(time(NULL));
    int currBall;
    int playerCard[5][5] = {};
    int playerrCard[4];
    bool markedCard[5][5] = {}; markedCard[2][2] = 1;
    char title[5] = {'B','I','N','G','O'};
    bool usedNum[75] = {};                      // array to keep track of already used numbers/balls
    int countCol[5] = {0, 0, 1, 0, 0};          // array to keep the sum of the '1's in each column
    int countRow[5] = {0, 0, 1, 0, 0};          // array to keep the sum of the '1's in each row
    int countDiagP = 0, countDiagS = 0;

    // ASSIGN PLAYER CARD NUMBERS
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            do{
                // based on the current row, set a number in the row range
                playerCard[i][j] = ((rand() % 15)) + (1) + (15*j); 
            }while(usedNum[playerCard[i][j] - 1]);
            usedNum[playerCard[i][j] - 1] = true;           // set the current number as used
        }
    }
    playerCard[2][2] = 0;

    // RESET usedNum AND START THE GAME
    fill_n(usedNum, 75, false);
    ClrScr();
    for(int i = 1; i <= 71; i++){ 
        // get the ball for the current iteration
        do{
            // keep generating the ball until the current ball hasn't been used yet
            currBall = (rand() % 75) + 1;
        }while(usedNum[currBall - 1]);
        usedNum[currBall - 1] = 1;                          // set the ball as used
        
        // check if the ball is in the player card
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                if(playerCard[j][k] == currBall){
                    markedCard[j][k] = 1;
                    countRow[j] += 1; countCol[k] += 1;
                    if(j == k) countDiagP++;                // element in 1st Diagonal: if currRow == currCol
                    if((5 - 1) == (j + k)) countDiagS++;    // element in 2nd Diagonal: if number of col - 1 == currRow + currCol
                    goto EndCheck;                          // stop the search after the ball has been found
                }
            }
        }
        EndCheck:

        // print "BINGO"
        cout << setw(3) << title[0];
        for(int i = 1; i < 5; i++){
            cout << setw(4) << title[i];
        }
        cout << endl;
        
        // print the player card & marked card
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                cout << setw(3);
                cout << playerCard[j][k] << " ";
            }
            cout << "|";
            for(int k = 0; k < 5; k++){
                cout << setw(2);
                cout << markedCard[j][k] << " ";
            }
            cout << endl;
        }
        
        // check if the player won
        if( any_of(begin(countRow), end(countRow), [](int n){return n == 5;}) ||
            any_of(begin(countCol), end(countCol), [](int n){return n == 5;}) ||
            countDiagP == 4 || countDiagS == 4){
                cout << setw(24) << "BINGO! :)\n";
                cout << setw(24) << "Turnos totales: " << i << endl;
                return 0;
        }

        // otherwise, print the current ball and wait for keypress
        cout << setw(26) << "La bola actual es: " << currBall << endl;
        cout << setw(26) << "Presione Enter..."; getchar();
        ClrScr();
    }
}

void ClrScr(){
    #ifdef _WIN32
        // if on Windows OS
        std::system("cls");
    #else
        // assuming POSIX OS
        std::system("clear");
    #endif
}