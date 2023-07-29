#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
using std::cout, std::endl, std::setw, std::fill_n;

int main(){
    //      B       I       N       G       O
    //    1-15    16-30   31-45   46-60   61-75    < -- number range for each row

    // SET UP VARIABLES, ARRAYS, AND RAND() SEED
    srand(time(NULL));
    int currBall, currRow, currCol;
    int playerCard[5][5] = {};
    bool markedCard[5][5] = {};
    char title[5] = {'B','I','N','G','O'};
    bool usedNum[75] = {};   // array to keep track of already used numbers/cards
    //fill_n(usedNum, 75, false);
 
    // PRINT "BINGO"
    cout << setw(3) << title[0];
    for(int i = 1; i < 5; i++){
        cout << setw(4) << title[i];
    }
    cout << endl;
    // ASSIGN PLAYER CARD NUMBERS
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            do{    
                // based on the current row, set a number in the row range
                playerCard[i][j] = ((rand() % 15)) + (1) + (15*j); 
            }while(usedNum[playerCard[i][j] - 1] == true);
            usedNum[playerCard[i][j] - 1] = true;
            cout << setw(3) << playerCard[i][j] << " ";
        }
        cout << endl;
    }
    /*for(int i = 0; i < 75; i++){
        cout << usedNum[i];
    }*/
    
    // RESET usedNum AND START THE GAME
    fill_n(usedNum, 75, false);
    for(int i = 0; i < 25; i++){ 
        do{
            currRow = rand() % 5; currCol = rand() % 5;
            currBall = playerCard[currRow][currCol];
        }while(usedNum[currBall - 1] == true);
        usedNum[currBall - 1] = true;
        markedCard[currRow][currCol] = 1;
    }
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            cout << markedCard[i][j] << " ";
        }
        cout << endl;
    }

    /*for(int i = 0; i < 75; i++){
        cout << usedNum[i];
    }*/
}