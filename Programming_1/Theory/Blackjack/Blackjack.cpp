#include <iostream>
#include <stdlib.h>
#include <time.h>

using   std::cout, std::cin, std::endl, std::copy,
        std::begin, std::end;

int DrawCard(int deckCards[], int deckNew[], int &cardCount);
//int DrawCard();

int main(){
    int playerCards[24] = {};
    int deckCards[52] = {};
    int deckNew[52] = { 1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,
                        9,9,9,9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13 };
    int currCard, cardCount = 52, playerMoney, minBet, playerBet;
    int temp = 100;
    
    srand(time(NULL));
    copy(deckNew, deckNew + 52, deckCards);         // get a new deck
    
    /*cout << "Cuanto dinero tiene?: ";
    cin >> playerMoney;
    cout << "Cuanto es la apuesta minima?: ";
    cin >> minBet;
    if(minBet > playerMoney){ cout  << "Lo siento, no tiene suficiente"
                                    << "\ndinero para jugar :("; return 0; }
    */
    while(temp > 0){
        playerCards[0] = DrawCard(deckCards, deckNew, cardCount);
        cout << playerCards[0] << endl;
        cout << "amount: " << cardCount << endl;
        temp--;
    }
}

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