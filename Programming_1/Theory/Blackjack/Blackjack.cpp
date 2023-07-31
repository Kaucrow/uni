#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <limits>
#include <conio.h>

using   std::cout, std::cin, std::endl, std::copy,
        std::begin, std::end, std::setw;

int GetCard(int deckCards[], int deckNew[], int &cardCount);
void DrawHands(int playerHands[]);
void ClrScr();

int main(){
    int playerHands[4] = {};                       // sum of the cards in each of the player's hands. One element for each hand
    int deckCards[52] = {};
    const int deckNew[52] = {   1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,
                                9,9,9,9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13 };
    int nextCard, cardCount = 52, availHands = 4, placedCardCount = 1, playerMoney = 100,
        minBet, playerBet, dealerSum = 0, handSelect = 0, currKey;
    char flagSplit;
    
    srand(time(NULL));
    copy(deckNew, deckNew + 52, deckCards);     // get a new deck
    
    /*cout << "Cuanto dinero tiene?: ";
    cin >> playerMoney;
    cout << "Cuanto es la apuesta minima?: ";
    cin >> minBet;
    if(minBet > playerMoney){ cout  << "Lo siento, no tiene suficiente"
                                    << "\ndinero para jugar :("; return 0; }
    */
    while(playerMoney > 0){
        ClrScr();
     
        // *** SET UP THE STARTING HAND(s) ***
        playerHands[0] = (rand()%1+1);                                 // replace by: GetCard(deckCards, deckNew, cardCount);
        for(int splitCount = 0; splitCount <= 3; splitCount++){
            // the value of temp is a displacement added to the loop 
            // for avoiding putting more than two cards on any hand
            for(int temp = (placedCardCount - splitCount - 1); temp < 4; temp++){
                // if a 0 is reached in the playerHands, stop the setup
                if(!playerHands[temp]){ availHands = splitCount + 1; splitCount = 4; break; }
                nextCard = (rand() % 1) + 1;
                placedCardCount++;
                cout << "Salio un: " << nextCard << endl;
                // executes if the card on the current hand is equal to the drawn card
                if(playerHands[temp] == nextCard && splitCount < 3){
                    do{
                        cout << "Desea dividir? (y/n): ";
                        flagSplit = getchar();
                        // to remove the "\n" left after getchar()
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                        cout << "flagSplit: " << flagSplit << endl;
                    }while(flagSplit != 'y' && flagSplit != 'n');
                    // if the player wants to split, put the drawn card in the nearest empty
                    // playerHands[] element and exit this loop to increase the splitCount
                    if(flagSplit == 'y'){ playerHands[splitCount+1] = nextCard; break; }
                }
                // executes if the player can't split or doesn't want to
                playerHands[temp] += nextCard;
                DrawHands(playerHands);
                cout << endl;    
            }
        }
        
        // *** USER INPUT ***
        // [<- : 75] [-> : 77] [Enter : 13]
        do{
            ClrScr();
            if(handSelect != 99){
                DrawHands(playerHands);
                cout << setw(33 - 4*availHands) << "[SURRENDER]" << endl;
                cout << setw((handSelect*4) + 4) << '*' << endl;
            }
            cout << "Cual mano? (<-/->/Enter): ";
            do{
                currKey = getch();
            }while(currKey != 75 && currKey != 77 && currKey != 13);
            cout << '\n' << currKey;
            if(currKey == 75){
                if(handSelect > 0) handSelect--;
                if(handSelect == 5) handSelect = availHands - 1;
                cout << "handSelect: " << handSelect << endl;
            }
            else if(currKey == 77){
                if(handSelect == availHands - 1) handSelect = 6;
                if(handSelect < availHands - 1) handSelect++;
                cout << "handSelect: " << handSelect << endl;
            }
            else{
                cout << "handSelect: " << handSelect << endl;
                if(handSelect == 6) break;          // replace by actions on surrender
                do{
                    cout << "Current hand sum: " << playerHands[handSelect] << endl;
                    cout << setw(10);
                }while(currKey != 80);
            }
        cout << "\n*** RESET ***\n";
        }while(availHands > 0);

        getchar();
        cout << "\n*** RESET ***\n";
        for(int i = 0; i < 4; i++) playerHands[i] = 0;
        placedCardCount = 1;
        playerMoney--;
    }
}

//  *** GET A CARD FROM THE CURRENT DECK AND GET A NEW  ***
//  *** DECK IF THE CURRENT ONE IS EMPTY                ***
int GetCard(int deckCards[], int deckNew[], int &cardCount){
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

void DrawHands(int playerHands[]){
    cout << " *** PLAYER HANDS ***\n";
    for(int i = 0; i < 4; i++){ if(!playerHands[i]) break; cout << setw(4) << playerHands[i]; }
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