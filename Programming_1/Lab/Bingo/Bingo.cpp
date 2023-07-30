#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <algorithm>
using   std::cout, std::endl, std::setw, std::any_of, 
        std::begin, std::end;

void DrawTables(int playerCard[][5], bool markedCard[][5]);
void ClrScr();

int main(){
    //      B       I       N       G       O
    //    1-15    16-30   31-45   46-60   61-75    < -- number range for each column

    // *** SET UP VARIABLES, ARRAYS, AND RAND() SEED ***
    srand(time(NULL));
    int currBall;
    char skipFlag;
    bool currSkip;
    int playerCard[5][5] = {};
    bool markedCard[5][5] = {}; markedCard[2][2] = 1;
    bool usedNumPlayer[75] = {};                            // to keep track of already used numbers in player card
    bool usedNumBall[75] = {};                              // to keep track of already used balls
    int countCol[5] = {0, 0, 1, 0, 0};                      // to keep the sum of the '1's in each column
    int countRow[5] = {0, 0, 1, 0, 0};                      // to keep the sum of the '1's in each row
    int countDiagP = 0, countDiagS = 0;

    // *** ASSIGN PLAYER CARD NUMBERS ***
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            do{
                // based on the current row, set a number in the row range
                playerCard[i][j] = ((rand() % 15)) + (1) + (15*j); 
            }while(usedNumPlayer[playerCard[i][j] - 1]);
            usedNumPlayer[playerCard[i][j] - 1] = true;     // set the current number as used
        }
    }
    playerCard[2][2] = 0;

    // *** RESET usedNum AND START THE GAME ***
    do{
        cout.flush();                                       // need to flush the stdout on POSIX
        ClrScr();
        DrawTables(playerCard, markedCard);
        cout << setw(30)    << "--- Juego de Bingo ---\n"
             << setw(34)    << "Tabla izquierda: Numeros del jugador\n"
             << setw(34)    << "Tabla derecha: Numeros marcados\n"
             << setw(31)    << "Desea saltar los turnos\n"
             << setw(31)    << "en donde no marque? (y/n): ";
        skipFlag = getchar();
    }while(skipFlag != 'y' && skipFlag != 'n');
    cout << setw(32) << "Presione Enter para jugar...";
    getchar();
    cout.flush();
    ClrScr();
    for(int i = 1; i <= 71; i++){ 
        currSkip = 1;
        
        // get the ball for the current iteration
        do{
            // keep generating the ball until the current ball hasn't been used yet
            currBall = (rand() % 75) + 1;
        }while(usedNumBall[currBall - 1]);
        usedNumBall[currBall - 1] = 1;                      // set the ball as used
        
        // check if the ball is in the player card
        if(usedNumPlayer[(currBall - 1)]){
            // if it is, find its position in the player card
            for(int j = 0; j < 5; j++){
                for(int k = 0; k < 5; k++){
                    if(playerCard[j][k] == currBall){
                        markedCard[j][k] = 1;
                        countRow[j] += 1; countCol[k] += 1;
                        if(j == k) countDiagP++;                // element in 1st Diagonal: if currRow == currCol
                        if((5 - 1) == (j + k)) countDiagS++;    // element in 2nd Diagonal: if number of col - 1 == currRow + currCol
                        // check if the player won
                        if( any_of(begin(countRow), end(countRow), [](int n){return n == 5;}) ||
                            any_of(begin(countCol), end(countCol), [](int n){return n == 5;}) ||
                            countDiagP == 4 || countDiagS == 4){
                            DrawTables(playerCard, markedCard);
                            cout << setw(24) << "BINGO! :)\n";
                            cout << setw(24) << "Turnos totales: " << i << endl;
                            return 0;
                        }
                        currSkip = 0;                           // when currSkip is 0, the draw phase won't be skipped
                        j = 5;                                  // stop the player card search after the pos has been found
                        break;
                    }
                }
            }
        }

        if(skipFlag == 'y' && currSkip == 1) continue;

        // print the current tables, turn, and ball number. Then wait for keypress
        DrawTables(playerCard, markedCard);
        cout << setw(20) << "Turno " << i << endl;
        cout << setw(26) << "La bola actual es: " << currBall << endl;
        cout << setw(26) << "Presione Enter..."; getchar();
        ClrScr();
    }
}

void DrawTables(int playerCard[5][5], bool markedCard[5][5]){
        // print "BINGO"
        cout << "  B   I   N   G   O\n"; 
        // print the player card & marked card
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 5; k++){
                cout << setw(3);
                cout << playerCard[j][k] << " ";
            }
            cout << "|";
            for(int k = 0; k < 5; k++){
                cout << " " << markedCard[j][k] << " ";
            }
            cout << endl;
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