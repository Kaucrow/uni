#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <limits>           // for flushing the stdin
#include <conio.h>          // for getch()

using   std::cout, std::cin, std::endl, std::setw,
        std::begin, std::end;

int GetCard(int deckCards[], int &cardCount, int &currHandSum, bool showCard, bool onlyGetDeck);
void PrintCard(int &drawnCard);
void DrawHands(int playerHands[], int &availHands);
void DrawResults(int playerHands[], int flagHands[], int &dealerSum);
void ClrScr();

int main(){
    // MULTI-USE VARIABLES
    int nextCard, cardCount = 0, playerMoney = 100,
        minBet;
    int playerBet[4] = {};
    int playerHands[4] = {};        // sum of the cards in each of the player's hands. One element for each hand
    int deckCards[52] = {};
    
    // STARTING HAND SETUP VARIABLES
    int placedCardCount = 1; 
    char flagSplit;
    
    // USER INPUT AND GAME VARIABLES
    int availHands = 4, handSelect = 0, actSelect = 0, dealerSum = 0, 
        currKey;
    int flagHitOrStay = 99;         // [0 : stay] [1 : hit] [99 : no action]
    int flagHands[4] = {0,9,9,9};   // [0 : normal] [1 : won] [2 : over] [3 : stayed] [4 : surrendered] [9 : not playing] 
    int dealerHand[13] = {};
    int flagExit = 0;               // [0 : stay in the hand play screen] [1 : exit the hand play screen]
                                    // [4 : exit the hand select screen]
    
    srand(time(NULL));
    GetCard(deckCards, cardCount, playerHands[handSelect], 0, 1);       // get the starting deck

    /*cout << "Cuanto dinero tiene?: ";
    cin >> playerMoney;
    cout << "Cuanto es la apuesta minima?: ";
    cin >> minBet;
    if(minBet > playerMoney){ cout  << "Lo siento, no tiene suficiente"
                                    << "\ndinero para jugar :("; return 0; }
    */
    while(playerMoney > 0){
        ClrScr(); 
        // ===================================
        // *** SET UP THE STARTING HAND(s) ***
        // ===================================
        playerHands[0] = (rand()%1+1);
        for(int splitCount = 0; splitCount <= 3; splitCount++){
            availHands = splitCount + 1;
            flagHands[splitCount] = 0;
            // the value of temp is a displacement added to the loop 
            // for avoiding putting more than two cards on any hand
            for(int temp = (placedCardCount - splitCount - 1); temp < 4; temp++){
                ClrScr();
                DrawHands(playerHands, availHands);
                cout << endl;
                // if a 0 is reached in the playerHands, stop the setup
                if(!playerHands[temp]){ splitCount = 4; break; }
                nextCard = ((rand() % 1) + 1);      // REPLACE BY GetCard();
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
                //DrawHands(playerHands, availHands);
                cout << endl;
            }
        }
        
        // get the dealer's initial hand
        cout << "\n* El crupier coloca una carta frente a el."; getchar();
        dealerHand[0] = GetCard(deckCards, cardCount, dealerSum, 1, 0);
        cout << "\n* El crupier coloca una carta boca abajo."; getchar();
        dealerHand[1] = GetCard(deckCards, cardCount, dealerSum, 0, 0);

        // ==================
        // *** USER INPUT ***
        // ==================
        // Key ASCII Codes (Windows) [<- : 75] [-> : 77] [Enter : 13]
        do{
            ClrScr();
            DrawHands(playerHands, availHands);
            cout << endl << setw((handSelect * 4) + (27 - ((availHands - 1)*2))) << '*' << endl;
            cout << setw(40) << "Cual mano? (<-/->/Enter): ";
            do{
                currKey = getch();
            }while(currKey != 75 && currKey != 77 && currKey != 13);

            // if the left arrow key was pressed, move left
            if(currKey == 75){
                if(handSelect > 0) handSelect--;
                cout << "handSelect: " << handSelect << endl;
            }
            // if the right arrow key was pressed, move right
            else if(currKey == 77){
                if(handSelect < availHands - 1) handSelect++;
                cout << "handSelect: " << handSelect << endl;
            }
            // if the Enter key was pressed, select the current hand
            else{
                flagExit = 0;
                // ===========================
                // *** PLAYING HAND SCREEN ***
                // ===========================
                do{
                    ClrScr();
                    cout    << setw(28) << "*** MANO [" << handSelect + 1 << "]: " 
                            << playerHands[handSelect] << " ***\n\n"
                            << "[PLANTARSE]"
                            << setw((actSelect * 10) + 2) << "*" 
                            << setw(39 - (10 * actSelect)) << "[PEDIR]\n"
                            << setw(26) << "[ATRAS]"
                            << setw(13) << "[RENDIR]\n\n";
                    
                    // HAND STATUS HANDLER
                    switch(flagHands[handSelect]){
                        case 0:
                            // on hand hit
                            if(flagHitOrStay == 1){
                                cout << "Recibes una carta."; getchar();
                                GetCard(deckCards, cardCount, playerHands[handSelect], 1, 0);
                                if(playerHands[handSelect] > 21) flagHands[handSelect] = 2;
                                flagHitOrStay = 99;
                                continue;
                            }
                            // on hand stay
                            else if(!flagHitOrStay){
                                flagHands[handSelect] = 3;
                                continue;
                            }
                            // on neither hit nor stay
                            else cout << "Utilice las flechas para moverse y\nEnter para aceptar.\n"; break;
                        case 2: cout << setw(21) << "La mano se paso.\n"; break;
                        case 3: cout << setw(20) << "La mano se quedo.\n"; break;
                        case 4: cout << setw(20) << "La mano fue rendida.\n";
                    }
                    
                    do{
                        currKey = getch();
                    }while(currKey != 75 && currKey != 77 && currKey != 13); 
                    
                    // SWITCH THE ACTION
                    if(currKey == 75 && actSelect > 0){ actSelect--; }
                    if(currKey == 77 && actSelect < 3){ actSelect++; }
                    
                    // EXECUTE THE ACTION
                    if(currKey == 13){
                        switch(actSelect){
                        case 0 /* stay */:
                            flagHitOrStay = 0; 
                            break;
                        case 1 /* back */:
                            flagExit = 1; 
                            break;
                        case 2 /* surrender */:
                            if(!flagHands[handSelect]) flagHands[handSelect] = 4;
                            break;
                        case 3 /* hit */:
                            flagHitOrStay = 1;
                        }
                    } 
                }while(!flagExit);
                // EXECUTES ON PLAYING HAND SCREEN EXIT
                flagExit = 0; actSelect = 0; flagHitOrStay = 99;
                // if there are no more hands left, stop the user input phase
                for (int i = 0; i < 4; i++){ if(flagHands[i] != 0) flagExit++;} 
            }
        }while(flagExit != 4);
        
        ClrScr();
        // ============================
        // *** DEALER'S TURN SCREEN ***
        // ============================
        cout << "* El crupier voltea su carta boca abajo."; getchar();
        PrintCard(dealerHand[1]);
        cout << "dealerSum: " << dealerSum; getchar();
        while(dealerSum <= 16){
            cout << "* El crupier toma una carta."; getchar();
            GetCard(deckCards, cardCount, dealerSum, 1, 0);
            cout << "dealerSum: " << dealerSum; getchar();
        }
        DrawResults(playerHands, flagHands, dealerSum); getchar();

        cout << "\n*** RESET ***\n"; getchar();
        for(int i = 0; i < 4; i++) playerHands[i] = 0;
        placedCardCount = 1;
        handSelect = 0;
        playerMoney--;
    }
}

//  *** GET A CARD FROM THE CURRENT DECK AND GET A NEW  ***
//  *** DECK IF THE CURRENT ONE IS EMPTY                ***
int GetCard(int deckCards[], int &cardCount, int &currHandSum, bool showCard, bool onlyGetDeck){
    int pos, drawnCard;
    // check if the deck is empty, and if so, get a new one
    if(!cardCount){ 
        for(int i = 1; i <= 13; i++){
            for(int j = 4*(i-1); j < 4*i; j++){ deckCards[j] = i; }
        }
        cardCount = 52; 
        if(onlyGetDeck) return 0;
    } 
    // get a random non-empty position on the deck
    do{
        pos = (rand() % 52);
    }while(!deckCards[pos]);
    drawnCard = deckCards[pos];
    deckCards[pos] = 0;
    cardCount--;
    if(showCard) PrintCard(drawnCard);
    if(drawnCard > 10) currHandSum += (drawnCard - (drawnCard%10));     // for mapping J, Q and K
    else if(drawnCard == 1 && currHandSum < 11) currHandSum += 11;      // for switching the ace value
    else currHandSum += drawnCard;
    return drawnCard;
}

//  *** PRINT THE DRAWN CARD'S NAME ***
void PrintCard(int &drawnCard){
    cout << "Es un";
    switch(drawnCard){
        case 1:
            cout << " As."; break;
        case 11: 
            cout << "a Jota."; break;
        case 12:
            cout << "a Reina."; break;
        case 13:
            cout << " Rey."; break;
        default:
            cout << " " << drawnCard << "."; 
    }
    getchar();
}

//  *** PRINT THE CURRENT PLAYER'S HANDS  ***
void DrawHands(int playerHands[], int &availHands){
    cout << setw(40) << " *** MANOS ACTUALES ***\n\n";
    cout << setw(23 - ((availHands - 1)*2)) << " ";
    for(int i = 0; i < 4; i++){ if(!playerHands[i]) break; cout << setw(4) << playerHands[i]; }
}

//  *** PRINT THE GAME RESULTS ***
void DrawResults(int playerHands[], int flagHands[], int &dealerSum){
    ClrScr();
    cout << setw(39) << " *** RESULTADOS ***\n\n";
    for(int i = 0; i < 4; i++){ 
        if(!playerHands[i]) break; 
        cout << setw(25) << "-> Mano " << i+1 << ": ";
        if(playerHands[i] > dealerSum){
            if(dealerSum <= 21){
                if(playerHands[i] <= 21) cout << "Gano ";
                else cout << "Perdio ";
            }
            else cout << "Empate ";
        }
        else if(playerHands[i] < dealerSum){
            if(dealerSum <= 21) cout << "Perdio ";
            else if(playerHands[i] <= 21) cout << "Gano ";
            else cout << "Empate ";
        }
        else cout << "Empate ";
        cout << "(" << playerHands[i] << ")\n";
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