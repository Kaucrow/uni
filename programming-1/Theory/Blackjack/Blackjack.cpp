#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <limits>               // for flushing the stdin
#include <conio.h>              // for getch()

using   std::cout, std::cin, std::setw;

int GetCard(int deckCards[], int &cardCount, int &currHandSum, bool showCard, bool onlyGetDeck);
void PrintCard(int &drawnCard);
void DrawHands(int playerHands[], int playerBet[], int &availHands);
void DrawStats(int &playerMoney, int &minBet, int &roundNum);
void ClrScr();

int main(){
    // MULTI-USE VARIABLES
    int cardCount = 0, playerMoney, addMoney, minBet, roundNum = 0;
    int dealerHidden;               // stores the dealer's hidden card
    int playerBet[4] = {};
    int playerHands[4] = {};        // sum of the cards in each of the player's hands. One element for each hand
    int deckCards[52] = {};
    
    // STARTING HAND SETUP VARIABLES
    int placedCardCount, nextCard; 
    char flagSplit;
    
    // USER INPUT AND GAME VARIABLES
    int availHands, handSelect = 0, actSelect = 0, dealerSum = 0, 
        currKey;
    int flagHitOrStay = 99;         // [0 : stay] [1 : hit] [99 : no action]
    int flagHands[4] = {9,9,9,9};   // [0 : normal] [1 : won] [2 : over] [3 : stayed] [4 : surrendered] [5 : lost] [9 : not playing] 
    int flagExit = 0;               // [0 : stay in the hand play screen] [1 : exit the hand play screen]
                                    // [4 : exit the hand select screen]
    
    srand(time(NULL));
    GetCard(deckCards, cardCount, playerHands[handSelect], 0, 1);       // get the starting deck

    ClrScr();
    cout << "Cuanto dinero tiene?: ";
    cin >> playerMoney;
    cout << "Cuanto es la apuesta minima?: ";
    cin >> minBet;
    if(minBet > playerMoney){ cout  << "\nCRUPIER: Lo siento, no tiene\n"
                                    << "suficiente dinero para jugar.\n"; return 0; }

    do{
        roundNum++;
        // ===================================
        // *** SET UP THE STARTING HAND(s) ***
        // ===================================
        do{
            ClrScr(); DrawStats(playerMoney, minBet, roundNum);
            cout << "\nCuanto desea apostar esta ronda?: ";
            cin >> playerBet[0];
        }while(playerBet[0] < minBet || playerBet[0] > playerMoney);
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        ClrScr(); cout << "* Recibes la primera carta "; getchar();
        GetCard(deckCards, cardCount, playerHands[handSelect], 1, 0); placedCardCount = 1;
        for(int splitCount = 0; splitCount <= 3; splitCount++){
            availHands = splitCount + 1;
            flagHands[splitCount] = 0;
            // the value of card is a displacement added to the loop
            // for avoiding putting more than two cards on any hand
            for(int card = (placedCardCount - splitCount - 1); card < 4; card++){
                ClrScr(); DrawHands(playerHands, playerBet, availHands);
                // if a 0 is reached in the playerHands, stop the setup
                if(!playerHands[card]){ splitCount = 4; break; }
                cout << "\n\n* Recibes una carta.\n";
                nextCard = 0; GetCard(deckCards, cardCount, nextCard, 1, 0);
                placedCardCount++;
                // executes if the card on the current hand is equal to the drawn card
                if(playerHands[card] == nextCard && splitCount < 3){
                    do{
                        cout << "\nDesea dividir la mano? (y/n): ";
                        flagSplit = getchar();
                        // to remove the "\n" left after getchar()
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                    }while(flagSplit != 'y' && flagSplit != 'n');
                    // if the player wants to split, put the drawn card in the nearest empty
                    // playerHands[] element, divide the bet, and exit this loop to increase the splitCount
                    if(flagSplit == 'y'){ 
                        playerHands[splitCount+1] = nextCard;
                        playerBet[splitCount+1] = playerBet[card]/2;
                        playerBet[card] /= 2;
                        break; 
                    }
                }
                // executes if the player can't split or doesn't want to
                playerHands[card] += nextCard;
            }
        }
        
        ClrScr(); DrawHands(playerHands, playerBet, availHands);
        // get the dealer's initial hand
        cout << "\n\n* El crupier coloca una carta frente a el."; getchar();
        GetCard(deckCards, cardCount, dealerSum, 1, 0);
        cout << "\n* El crupier coloca una carta boca abajo."; getchar();
        dealerHidden = GetCard(deckCards, cardCount, dealerSum, 0, 0);

        // ==================
        // *** USER INPUT ***
        // ==================
        // Key ASCII Codes (Windows) [<- : 75] [-> : 77] [Enter : 13]
        do{
            ClrScr();
            DrawHands(playerHands, playerBet, availHands);
            cout << '\n' << setw((handSelect * 5) + (28 - ((availHands - 1)*2))) << '*' << '\n';
            cout << setw(42) << "Cual mano? (<-/->/Enter): ";
            do{
                currKey = getch();
            }while(currKey != 75 && currKey != 77 && currKey != 13);

            // if the left arrow key was pressed, move left
            if(currKey == 75){
                if(handSelect > 0) handSelect--;
                cout << "handSelect: " << handSelect << '\n';
            }
            // if the right arrow key was pressed, move right
            else if(currKey == 77){
                if(handSelect < availHands - 1) handSelect++;
                cout << "handSelect: " << handSelect << '\n';
            }
            // if the Enter key was pressed, select the current hand
            else{
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
                                cout << setw(38) << "* Recibes una carta." << setw(28); getchar();
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
                            else cout   << setw(45) << "Utilice las flechas para moverse\n"
                                        << setw(40) << "y Enter para aceptar.\n"; break;
                        case 2: cout << setw(37) << "La mano se paso.\n"; break;
                        case 3: cout << setw(38) << "La mano se quedo.\n"; break;
                        case 4: cout << setw(39) << "La mano fue rendida.\n";
                    }
                    
                    // if there are no more hands left, stop the user input phase
                    for(int i = 0; i < 4; i++){ 
                        if(flagHands[i] == 0){ flagExit = 0; break; }
                        else flagExit = 2;
                    } 
                    if(flagExit == 2){ getch(); break; }

                    do{
                        currKey = getch();
                    }while(currKey != 75 && currKey != 77 && currKey != 13); 
                    
                    // SWITCH THE ACTION
                    if(currKey == 75 && actSelect > 0){ actSelect--; }
                    else if(currKey == 77 && actSelect < 3){ actSelect++; }
                    
                    // EXECUTE THE ACTION
                    else if(currKey == 13){
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
                flagExit--; actSelect = 0; flagHitOrStay = 99;
            }
        }while(!flagExit);
        flagExit = 0; handSelect = 0;

        // ============================
        // *** DEALER'S TURN SCREEN ***
        // ============================
        ClrScr();
        cout << "* El crupier voltea su carta boca abajo."; getchar();
        PrintCard(dealerHidden);
        cout << "La mano del crupier suma " << dealerSum << ".\n"; getchar();
        while(dealerSum <= 16){
            cout << "* El crupier toma una carta."; getchar();
            GetCard(deckCards, cardCount, dealerSum, 1, 0);
            cout << "La mano del crupier suma " << dealerSum << ".\n"; getchar();
        }

        // ======================
        // *** RESULTS SCREEN ***
        // ====================== 
        // this will also update the flagHands for later updating the
        // playerMoney according to hand win, lose or tie
        ClrScr();
        cout << setw(39) << " *** RESULTADOS ***\n\n";
        for(int i = 0; i < 4; i++){ 
            if(!playerHands[i]) break; 
            cout << setw(21) << "-> Mano " << i+1 << " (" << playerBet[i] << "$): ";
            if(flagHands[i] == 4) cout << "Rendida";
            else if(playerHands[i] > dealerSum){
                if(dealerSum <= 21){
                    if(playerHands[i] <= 21){ cout << "  Gano"; flagHands[i] = 1; }
                    else{ cout << "Perdio"; flagHands[i] = 5; }
                }
                else{ cout << "Empate"; flagHands[i] = 0; }
            }
            else if(playerHands[i] < dealerSum){
                if(dealerSum <= 21){ cout << "Perdio"; flagHands[i] = 5; }
                else if(playerHands[i] <= 21){ cout << "  Gano"; flagHands[i] = 1; }
                else{ cout << "Empate"; flagHands[i] = 0; }
            }
            else{ cout << "Empate"; flagHands[i] = 0; }
            cout << " (" << playerHands[i] << ")\n";
        }
        cout << '\n' << setw(34) << "-> Mano del crupier: " << dealerSum << '\n';
        
        DrawStats(playerMoney, minBet, roundNum); getchar();

        // UPDATE THE PLAYER'S MONEY
        for(int i = 0; i < 4; i++){
            if(flagHands[i] == 4) addMoney -= (playerBet[i] / 2);               // on hand surrender
            else if(flagHands[i] == 1) addMoney += playerBet[i];                // on hand win
            else if(flagHands[i] == 5) addMoney -= playerBet[i];                // on hand lose
        }

        playerMoney += addMoney;
        cout << "* El jugador " << "1";
        if(addMoney >= 0) cout << " gano ";
        else cout << " perdio ";
        cout << abs(addMoney) << "$.\n"; getchar();
        
        ClrScr(); DrawStats(playerMoney, minBet, roundNum); getchar();
        
        for(int i = 0; i < 4; i++){ playerHands[i] = 0; flagHands[i] = 9; } addMoney = 0; dealerSum = 0; 
    }while(playerMoney >= minBet);
    
    ClrScr();
    cout    << "* Te has quedado sin dinero para apostar...\n"; getchar();
    cout    << "Durante unos instantes, contemplas la mesa frente\n" 
            << "a ti con una mirada melancolica, y poco despues,\n"
            << "te levantas, dispuesto a abandonar el casino."; getchar();
    cout    << "Caminas lentamente hacia la salida, desanimado\n"
            << "y abatido por la derrota."; getchar();
    cout    << "..."; getchar();
    cout    << "Repentinamente, un audaz pensamiento cruza tu mente";
    getch(); cout << "."; getch(); cout << "."; getch(); cout << "."; getchar();
    cout    << "\"Y si apuesto la casa?\"\n"; getchar();
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
    if(drawnCard > 10) currHandSum += (drawnCard - (drawnCard % 10));   // for mapping J, Q and K
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
void DrawHands(int playerHands[], int playerBet[], int &availHands){
    cout << setw(42) << "*** MANOS ACTUALES ***\n\n";
    cout << "Suma -->";
    cout << setw(15 - ((availHands - 1)*2)) << " ";
    cout << setw(6) << playerHands[0];
    for(int i = 1; i < 4; i++){ if(!playerHands[i]) break; cout << setw(5) << playerHands[i]; }
    cout << "\nApuesta -->";
    cout << setw(12 - ((availHands - 1)*2)) << " ";
    cout << setw(5) << playerBet[0] << "$";
    for(int i = 1; i < 4; i++){ if(!playerHands[i]) break; cout << setw(4) << playerBet[i] << "$"; }
}

void DrawStats(int &playerMoney, int &minBet, int &roundNum){
    cout    << "_______________________________\n"
            << "| Jugador |  Estado  | Dinero |" << setw(20) << "Apuesta minima: " << minBet << "$\n"
            << "|_____________________________|" << setw(20) << "Ronda: " << roundNum << '\n'
            << "|" << setw(5) << "1" << setw(5) << "|" << setw(9); 
    if(playerMoney >= minBet) cout << "Jugando"; else cout << "Quebrado"; 
    cout    << setw(2) << "|"
            << setw(6) << playerMoney << "$" << setw(3) << "|\n"
            << "|_____________________________|\n";
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