#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <limits>

using   std::cout, std::cin, std::endl, std::copy,
        std::begin, std::end;

int DrawCard(int deckCards[], int deckNew[], int &cardCount);
void ClrScr();

int main(){
    int cardsSum[4] = {};
    int deckCards[52] = {};
    const int deckNew[52] = {   1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,
                                9,9,9,9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13 };
    int nextCard, cardCount = 52, placedCardCount = 1, playerMoney = 100, minBet, playerBet;
    char flagSplit;
    
    srand(time(NULL));
    copy(deckNew, deckNew + 52, deckCards);         // get a new deck
    
    /*cout << "Cuanto dinero tiene?: ";
    cin >> playerMoney;
    cout << "Cuanto es la apuesta minima?: ";
    cin >> minBet;
    if(minBet > playerMoney){ cout  << "Lo siento, no tiene suficiente"
                                    << "\ndinero para jugar :("; return 0; }
    */
    while(playerMoney > 0){
        ClrScr();
        cardsSum[0] = (rand()%3+1);     //DrawCard(deckCards, deckNew, cardCount);
     
        // *** SET UP THE STARTING HAND(s) ***
        for(int splitCount = 0; splitCount <= 3; splitCount++){
            cout << "outer iteration: " << splitCount << endl;
            for(int i = 0; i < 4; i++) cout << cardsSum[i] << " ";
            cout << endl;
            for(int temp = (placedCardCount - splitCount - 1); temp < 4; temp++){
                if(!cardsSum[temp]){ splitCount = 4; break; }
                nextCard = (rand()%3)+1;
                placedCardCount++;
                cout << "nextCard: " << nextCard << endl;
                if(cardsSum[temp] == nextCard){
                    do{
                        cout << "Desea dividir? (y/n): ";
                        flagSplit = getchar();
                        // to remove the "\n" left after getchar()
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        cout << "flagSplit: " << flagSplit << endl;
                    }while(flagSplit != 'y' && flagSplit != 'n'); 
                    if(flagSplit == 'y'){ cardsSum[splitCount+1] = nextCard; break; }
                }
                cardsSum[temp] += nextCard;
                cout << "inner iteration: " << temp << endl;
                for(int i = 0; i < 4; i++) cout << cardsSum[i] << " ";
                cout << endl;
            }
        }
        cout << "\nEXIT\n";

        // *** PRINT THE CURRENT CARDS SUM
        for(int i = 0; i < 4; i++) cout << cardsSum[i] << " ";
        // cout << "amount: " << cardCount << endl;
        getchar();
        cout << "\n*** RESET ***\n";
        for(int i = 0; i < 4; i++) cardsSum[i] = 0;
        placedCardCount = 1;
        playerMoney--;
    }
}

//  *** DRAW A CARD FROM THE CURRENT DECK AND GET A NEW ***
//  *** DECK IF THE CURRENT ONE IS EMPTY                ***
int DrawCard(int deckCards[], int deckNew[], int &cardCount){
    int pos, drawnCard;
    
    // check if the deck is empty, and if so, grab a new one
    if(!cardCount){ cout << "\nCOPIED\n"; copy(deckNew, deckNew + 52, deckCards); cardCount = 52; }
    
    // get a random non-empty position on the deck
    do{
        pos = (rand() % 52);
    }while(!deckCards[pos]);
    drawnCard = deckCards[pos];
    deckCards[pos] = 0;
    cardCount--;
    return drawnCard;
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