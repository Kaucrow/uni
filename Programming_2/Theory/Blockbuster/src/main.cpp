#include <iostream>
#include <fstream>
#include <fcntl.h>              // for _setmode().
#include <ctime>
#include <iomanip>
#include <boost/locale.hpp>
#include <structs.h>
#include <merge_sort.h>
#include <bin_search.h>
#include <store_movie.h>
#include <rent_movie.h>
#include <file_ops.h>

#define USRDATA_PATH "./data/user_data.csv"
#define CSV_PATH "./data/movies.csv"

using   std::wcout, std::wcerr, std::wcin, std::getline, std::wfstream,
        std::wifstream, std::wofstream, std::wstring;

//enum { DURATION, TITLE, DIRECTOR, YEAR, MONTH, DAY, BASE };
enum { SEARCH = 1, ADD = 2, RENT = 3, EXIT = 4 };
enum { DUR, YEA, MON, DAY };
enum { TTL, DIR };

/**
 * @brief OS agnostic clear screen function.
 */
void ClrScr();

wstring GetDateTime();

/**
 * @brief Gets the number of movies in the movies.csv file.
 * @param inFile - wifstream object that contains the movies.csv file.
 * @return Number of movies in the movies.csv file.
 * @see GetNumMoviesImplem "GetNumMovies() implementation details".
 */

int main(){
    boost::locale::generator gen;               // Create the locale generator.
    std::locale loc = gen("en_US");             // Create an "en_US" locale
    std::locale::global(loc);                   // and set it as the global locale.
    _setmode(_fileno(stdout), _O_U8TEXT);       // Change the STDOUT mode to use UTF-8 characters.
    _setmode(_fileno(stdin), _O_U8TEXT);       // Change the STDOUT mode to use UTF-8 characters.

    //wstring csvFileName = L"./data/movies.csv";
    //wfstream csvFile(CSV_PATH);
    //if(!csvFile){ wcerr << "ERR: FILE \"" << CSV_PATH << "\" COULD NOT BE OPENED."; return 1; }
    //csvFile.imbue(loc);                          // Apply the locale to the movies.csv stream object.

    int totalMovies = 0;
    try{ totalMovies = GetLastLineFirstNum(CSV_PATH); }     // Get the number of movies in the movies.csv file.
    catch(wstring exc){ wcerr << exc << '\n'; return 1; }

    /*  Create lists of movies which hold 4000 movies each, where the first index of the array is unused.
        each movie list is sorted according to a different property
        (duration, title, director, release year, release month, release day). */
    /*Movie ttlList[totalMovies + 3001];
    Movie durList[totalMovies + 3001];
    Movie dirList[totalMovies + 3001];
    Movie yeaList[totalMovies + 3001];
    Movie monList[totalMovies + 3001];
    Movie dayList[totalMovies + 3001];*/
    Movie baseList[totalMovies + 3001];
    IntFrag durList[totalMovies + 3001];
    IntFrag yeaList[totalMovies + 3001];
    IntFrag monList[totalMovies + 3001];
    IntFrag dayList[totalMovies + 3001];
    WstrFrag ttlList[totalMovies + 3001];
    WstrFrag dirList[totalMovies + 3001];
    IntFrag* intFrags[4] = { durList, yeaList, monList, dayList };
    WstrFrag* wstrFrags[2] = { ttlList, dirList };
    //Movie* movList[6] = { durList, ttlList, dirList, yeaList, monList, dayList };

    wstring username;
    wfstream openTest(USRDATA_PATH);
    wstring appendLine;
    int userNum = 0;
    if(!openTest){
        wcout   << "[ INFO ] No user_data.csv file was found. Please input the name\n"
                << "         of the first user, so the file may be created: ";
        wcin >> username;
        appendLine = L"1," + username + L",\n";
        AppendLine(USRDATA_PATH, L"id,name,movies\n" + appendLine);
    }
    else openTest.close();

    try{ userNum = GetLastLineFirstNum(USRDATA_PATH); }
    catch(wstring exc){ wcerr << exc << '\n'; return 1; }

    User userList[userNum + 101];
    try{ PopulateUserList(userList, userNum, USRDATA_PATH); }
    catch(wstring exc){ wcerr << exc << '\n'; return 1; }
    //appendLine = std::to_wstring(userNum + 1) + L',' + username + L",\n";
    //AppendLine(USRDATA_PATH, appendLine);
    wcout << userList[1].name << '\n';
    wcin.get();
    wcin.get();

    /* Populate the DURATION movie list. */
    try{ PopulateMovieList(baseList, totalMovies, CSV_PATH); }
    catch(wstring exc){ wcerr << exc << '\n'; return 1; }

    /* Copy the duration movie list elements to all of the other lists. */

    for(int i = 1; i <= totalMovies; i++){
        for(int j = 0; j < 4; j++){ intFrags[j][i].ID = baseList[i].ID; }
        for(int j = 0; j < 2; j++){ wstrFrags[j][i].ID = baseList[i].ID; }
        intFrags[DUR][i].data = baseList[i].duration;
        intFrags[YEA][i].data = baseList[i].release.year;
        intFrags[MON][i].data = baseList[i].release.month;
        intFrags[DAY][i].data = baseList[i].release.day;
        wstrFrags[TTL][i].data = baseList[i].title;
        wstrFrags[DIR][i].data = baseList[i].director;
    }
    /*    movList[DURATION][i] = baseList[i];
        movList[TITLE][i]    = baseList[i];
        movList[DIRECTOR][i] = baseList[i];
        movList[YEAR][i]     = baseList[i];
        movList[MONTH][i]    = baseList[i];
        movList[DAY][i]      = baseList[i];
    }*/

    //for(int i = 0; i < 6; i++)
     //   MergeSort(movList[i], 1, totalMovies, i);
    
    /* Sort each list. */
    wcout << intFrags[DUR][1].data << '\n';       // debug
    wcout << wstrFrags[TTL][1].data << '\n';
    for(int i = 0; i < 4; i++)
        MergeSort(intFrags[i], 1, totalMovies);
    for(int i = 0; i < 2; i++)
        MergeSort(wstrFrags[i], 1, totalMovies);

    wcout << intFrags[DUR][1].data << '\n';       // debug
    wcout << wstrFrags[TTL][1].data << '\n';

    wcin.get();
    wcin.get();
    /*for(int i = 1; i <= totalMovies; i++){
        durFrag[i].data = std::to_wstring(baseList[i].duration);
    }
    wcout << durFrag[1].data << '\n';
    wcin.get();
    wcin.get();

    MergeSort(durFrag, 1, totalMovies);
    wcout << durFrag[1].data << '\n';
    wcin.get();
    wcin.get();*/

    /***************************
    /*  Main loop.
     **************************/
    int action, currUser;
    while(true){
        ClrScr();
        wcout   << "*** MENU ***\n"
                << "(1) Login\n"
                << "(2) Exit\n"
                << "Select option: ";
        wcin >> action;
        switch(action){
            case 1: break;
            case 2:
                wcout << "\nTerminating execution...\n";
                return 0;
        }

        ClrScr(); 
        currUser = 0;
        wcout   << "*** LOGIN ***\n"
                << "-> User: ";
        wcin >> username;

        for(int i = 1; i <= userNum; i++){
            if(userList[i].name == username){
                currUser = userList[i].ID;
            }
        }
        if(currUser == 0){
            userNum++;
            AppendLine(USRDATA_PATH, std::to_wstring(userNum) + L',' + username + L",\n");
            currUser = userNum;
        }

        while(true){
            ClrScr();
            wcout   << "*** CHOOSE AN ACTION ***\n"
                    << "(1) Search with filters\n"
                    << "(2) Add a movie\n"
                    << "(3) Rent a movie\n"
                    << "(4) Exit\n"
                    << "Select option: ";
            wcin >> action;
            while(action < SEARCH || action > EXIT){
                wcout << "INVALID OPTION.\nSelect option: ";
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                wcin >> action;
            }
            wcin.ignore(1);

            /***************************
            /*  Search with filters.
            **************************/
            if(action == SEARCH){
                ClrScr();
                Movie matches[totalMovies + 3001];      // Movie list array to stores the search matches.
                Movie toMatch;                          // Movie that holds the parameter to be matched in the corresponding movie list.
                wcout   << "*** FILTERS ***\n"
                        << "(1) Duration\n"
                        << "(2) Title\n"
                        << "(3) Director\n"
                        << "(4) Release year\n"
                        << "(5) Release month\n"
                        << "(6) Release day\n"
                        << "Select option: ";

                wcin >> action;
                while(action < 1 || action > 6){
                    wcout << "INVALID OPTION.\nSelect option: ";
                    wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    wcin >> action;
                }
                wcin.ignore(1);

                wstring search;
                ClrScr();

                /**
                 * Generate the "toMatch" movie according to the selected filter,
                 * perform the search, and store the matching movies in the "matches" movie list array.
                 */
                switch(action){
                    case 1:
                        wcout << "Duration to search for: ";
                        getline(wcin, search);
                        toMatch.duration = stoi(search);
                        //BinSearch(movList[DURATION], matches, 1, totalMovies, toMatch, DURATION);
                        break;
                    case 2:
                        wcout << "Title to search for: ";
                        getline(wcin, search);
                        toMatch.title = search;
                        //BinSearch(movList[TITLE], matches, 1, totalMovies, toMatch, TITLE);
                        break;
                    case 3:
                        wcout << "Director to search for: ";
                        getline(wcin, search);
                        toMatch.director = search;
                        //BinSearch(movList[DIRECTOR], matches, 1, totalMovies, toMatch, DIRECTOR);
                        break;
                    case 4:
                        wcout << "Year to search for: ";
                        getline(wcin, search);
                        toMatch.release.year = stoi(search);
                        //BinSearch(movList[YEAR], matches, 1, totalMovies, toMatch, YEAR);
                        break;
                    case 5:
                        wcout << "Month to search for: ";
                        getline(wcin, search);
                        while(stoi(search) < 1 || stoi(search) > 12){
                            wcout << "Please input a valid month.\n";
                            getline(wcin, search);
                        }
                        toMatch.release.month = stoi(search);
                        //BinSearch(movList[MONTH], matches, 1, totalMovies, toMatch, MONTH);
                        break;
                    case 6:
                        wcout << "Day to search for: ";
                        getline(wcin, search);
                        while(stoi(search) < 1 || stoi(search) > 31){
                            wcout << "Please input a valid day.\n";
                            getline(wcin, search);
                        }
                        toMatch.release.day = stoi(search);
                        //BinSearch(movList[DAY], matches, 1, totalMovies, toMatch, DAY);
                        break;
                    default:
                        wcerr << "[ ERR ] THIS SHOULD NEVER EXECUTE. IT'S ONLY HERE FOR DEBUG PURPOSES.\n";     // debug
                        break;
                }

                ClrScr();
                /* Print the matching movies from the "matches" movie list array. */
                for(int i = 0; matches[i].duration != 0; i++){
                    wcout << matches[i].title << '\n';
                }
            
            }
            /***************************
            /*  Add a movie.
            **************************/
            else if(action == ADD){
                ClrScr();
                Movie toStore;
                wstring storeDat;
                totalMovies++;              // Increase the count of movies.

                wcout << "*** NEW MOVIE DATA ***\n";

                /* Get each of the 6 data fields of the movie that will be added.*/
                wcout << "-> Duration: ";
                getline(wcin, storeDat);
                while(stoi(storeDat) <= 0){
                    wcerr << "[ ERR ] The movie cannot have a duration less than or equal to 0.\n-> Duration:";
                    getline(wcin, storeDat);
                }
                toStore.duration = stoi(storeDat);

                wcout << "-> Title: ";
                getline(wcin, storeDat);
                storeDat[0] = toupper(storeDat[0]);
                toStore.title = storeDat;

                wcout << "-> Genres\n";
                for(int i = 0; i < 6; i++){
                    wcout << "   * " << i + 1 << ": ";
                    getline(wcin, storeDat);
                    toStore.genres[i] = storeDat;
                }

                wcout << "-> Director: ";
                getline(wcin, storeDat);
                toStore.director = storeDat;

                wcout << "-> Release year: ";
                getline(wcin, storeDat);
                toStore.release.year = stoi(storeDat);
                
                wcout << "-> Release month: ";
                getline(wcin, storeDat);
                toStore.release.month = stoi(storeDat);
                
                wcout << "-> Release day: ";
                getline(wcin, storeDat);
                toStore.release.day = stoi(storeDat);

                /**
                 * Update each of the movie lists with the added movie, while preserving the
                 * sorting order in each of them.
                 */
                /*StoreNewMovie(movList[DURATION], 1, totalMovies, toStore, DURATION);
                StoreNewMovie(movList[TITLE], 1, totalMovies, toStore, TITLE);
                StoreNewMovie(movList[DIRECTOR], 1, totalMovies, toStore, DIRECTOR);
                StoreNewMovie(movList[YEAR], 1, totalMovies, toStore, YEAR);
                StoreNewMovie(movList[MONTH], 1, totalMovies, toStore, MONTH);
                StoreNewMovie(movList[DAY], 1, totalMovies, toStore, DAY);*/

                wcout << "[ INFO ] THE MOVIE WAS ADDED SUCCESSFULLY.\n";
            }
            /***************************
            /*  Rent a movie.
            **************************/
            else if(action == RENT){
                ClrScr();
                // An array of one movie is declared here, since the StoreMatches() function //
                // in bin_search.cpp only accepts an array of movies.                        //
                Movie toRent[1];
                wcout << "*** MOVIE RENT ***\n";
                wcout << "Input the name of the movie: ";
                wcin >> toRent[0].title;

                // Search for the movie to rent, and throw an error if it doesn't exist in the database. //
                // If it exists, it will get stored in the toRent[] array.                               //
                /*if(BinSearch(ttlList, toRent, 1, totalMovies, toRent[0], TITLE) == -1)
                    wcerr << L"[ ERR ] THE MOVIE DOES NOT EXIST.\n";*/

                wstring currDate = GetDateTime().substr(0, 10);     // Get the current date.

                // Update the movies.csv file with the rent information.
                RentMovie(CSV_PATH, toRent[0].ID, username, currDate);

                wcin.get();
            }
            // Executes if the user selects the "Exit" action. //
            else break;
            wcin.get();
        }
    }
    return 0;
}

wstring GetDateTime(){
    time_t rawtime;
    struct tm timeinfo;
    wchar_t buffer[20];

    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    wcsftime(buffer, 20, L"%Y-%m-%d %H:%M:%S", &timeinfo);

    return buffer;
}

void ClrScr(){
    #ifdef _WIN32
        // If on Windows OS
        std::system("cls");
    #else
        // Assuming POSIX OS
        std::system("clear");
    #endif
}