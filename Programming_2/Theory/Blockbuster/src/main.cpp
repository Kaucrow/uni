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

#define USRDATA_PATH "./data/users_data.csv"
#define CSV_PATH "./data/movies.csv"

using   std::wcout, std::wcerr, std::wcin, std::getline, std::wfstream,
        std::wifstream, std::wofstream, std::wstring;

enum { FILTER = 1, GETMOVDATA = 2, ADD = 3, RENT = 4, EXIT = 5 };
enum { DUR, YEA, MON, DAY };
enum { TTL, DIR };

/**
 * @brief OS agnostic clear screen function.
 */
void ClrScr();

wstring GetDateTime(bool add14Days = false);

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
    _setmode(_fileno(stdin), _O_U8TEXT);        // Change the STDIN mode to use UTF-8 characters.

    GetDateTime();

    //wstring csvFileName = L"./data/movies.csv";
    //wfstream csvFile(CSV_PATH);
    //if(!csvFile){ wcerr << "ERR: FILE \"" << CSV_PATH << "\" COULD NOT BE OPENED."; return 1; }
    //csvFile.imbue(loc);                          // Apply the locale to the movies.csv stream object.

    CheckMoviesCsv(CSV_PATH);

    int totalMovies = 0;
    try{ totalMovies = GetLastLineFirstNum(CSV_PATH); }     // Get the number of movies in the movies.csv file.
    catch(wstring exc){ wcerr << exc << '\n'; return 1; }

    /*  Create lists of movies which hold 4000 movies each, where the first index of the array is unused.
        each movie list is sorted according to a different property
        (duration, title, director, release year, release month, release day). */
    Movie baseList[totalMovies + 3001];
    IntFrag durList[totalMovies + 3001];
    IntFrag yeaList[totalMovies + 3001];
    IntFrag monList[totalMovies + 3001];
    IntFrag dayList[totalMovies + 3001];
    WstrFrag ttlList[totalMovies + 3001];
    WstrFrag dirList[totalMovies + 3001];
    IntFrag* intFrags[4] = { durList, yeaList, monList, dayList };
    WstrFrag* wstrFrags[2] = { ttlList, dirList };

    wstring username;
    wfstream openTest(USRDATA_PATH);
    wstring appendLine;
    int userNum = 0;
    if(!openTest){
        wcout   << "[ INFO ] No users_data.csv file was found. Please input the name\n"
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

    wcout << userList[1].name << '\n';      // debug
    wcin.get();     // debug
    wcin.get();     // debug

    /* Populate the base movie list. */
    try{ PopulateMovieList(baseList, totalMovies, CSV_PATH); }
    catch(wstring exc){ wcerr << exc << '\n'; return 1; }

    /* Copy the base movie list elements data to each frag list. */
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
                    << "(2) Get movie info\n"
                    << "(3) Add a movie\n"
                    << "(4) Rent a movie\n"
                    << "(5) Exit\n"
                    << "Select option: ";
            wcin >> action;
            while(action < FILTER || action > EXIT){
                wcout << "INVALID OPTION.\nSelect option: ";
                wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                wcin >> action;
            }
            wcin.ignore(1);

            /***************************
            /*  Search with filters.
            **************************/
            if(action == FILTER){
                ClrScr();
                int idMatches[totalMovies + 3001];      // Movie list array to stores the search matches.

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

                wstring wstrSearch;
                int intSearch;
                /**
                 * Generate the "toMatch" movie according to the selected filter,
                 * perform the search, and store the matching movies in the "matches" movie list array.
                 */
                ClrScr();
                switch(action){
                    case 1:
                        wcout << "Duration to search for: ";
                        wcin >> intSearch;
                        BinSearchStoreMatches(intFrags[DUR], idMatches, 1, totalMovies, intSearch);
                        break;
                    case 2:
                        wcout << "Title to search for: ";
                        getline(wcin, wstrSearch);
                        BinSearchStoreMatches(wstrFrags[TTL], idMatches, 1, totalMovies, wstrSearch);
                        break;
                    case 3:
                        wcout << "Director to search for: ";
                        getline(wcin, wstrSearch);
                        BinSearchStoreMatches(wstrFrags[DIR], idMatches, 1, totalMovies, wstrSearch);
                        break;
                    case 4:
                        wcout << "Year to search for: ";
                        wcin >> intSearch;
                        BinSearchStoreMatches(intFrags[YEA], idMatches, 1, totalMovies, intSearch);
                        break;
                    case 5:
                        wcout << "Month to search for: ";
                        wcin >> intSearch;
                        while(intSearch < 1 || intSearch > 12){
                            wcout << "Please input a valid month.\n";
                            wcin >> intSearch;
                        }
                        BinSearchStoreMatches(intFrags[MON], idMatches, 1, totalMovies, intSearch);
                        break;
                    case 6:
                        wcout << "Day to search for: ";
                        wcin >> intSearch;
                        while(intSearch < 1 || intSearch > 31){
                            wcout << "Please input a valid day.\n";
                            wcin >> intSearch;
                        }
                        BinSearchStoreMatches(intFrags[DAY], idMatches, 1, totalMovies, intSearch);
                        break;
                    default:
                        wcerr << "[ ERR ] THIS SHOULD NEVER EXECUTE. IT'S ONLY HERE FOR DEBUG PURPOSES.\n";     // debug
                        break;
                }

                ClrScr();
                /* Print the matching movies from the "matches" movie list array. */
                for(int i = 0; idMatches[i] != 0; i++){
                    wcout << baseList[idMatches[i]].title << '\n';
                }
                wcin.get(); 
            }
            /***************************
            *  Get movie info.
            **************************/
            else if(action == GETMOVDATA){
                ClrScr();
                wstring search;
                wcout << "-> Movie title: ";
                wcin >> search;
                int ttlPos = BinSearch(wstrFrags[TTL], 1, totalMovies, search);
                if(ttlPos == -1){
                    wcerr << L"[ ERR ] THE MOVIE DOES NOT EXIST.\n";
                    wcin.ignore(1);
                    wcin.get();
                    continue;
                }
                int moviePos = wstrFrags[TTL][ttlPos].ID;
                wcout   << L"[ INFO ] Found movie \"" << search << L"\".\n\n"
                        << L"-> Title: " << baseList[moviePos].title << L'\n'
                        << L"-> Duration: " << baseList[moviePos].duration << L" min.\n"
                        << L"-> Director: " << baseList[moviePos].director << L'\n'
                        << L"-> Release date: " << baseList[moviePos].release.year << L'-' << baseList[moviePos].release.month << L'-' << baseList[moviePos].release.day << L'\n'
                        << L"-> Genres:\n";
                for(int i = 0; baseList[moviePos].genres[i] != L""; i++)
                    wcout << L"  * " << baseList[moviePos].genres[i] << L'\n';
                if(baseList[moviePos].rentedTo != L""){
                    wcout   << L"-> Rented to: " << baseList[moviePos].rentedTo << L'\n'
                            << L"-> Rented on: " << baseList[moviePos].rentedOn.year << L'-' << baseList[moviePos].rentedOn.month << L'-' << baseList[moviePos].rentedOn.day << L'\n'
                            << L"-> Expiry: " << baseList[moviePos].expiry.year << L'-' << baseList[moviePos].expiry.month << L'-' << baseList[moviePos].expiry.day << L'\n';
                }
                wcin.get();
                wcin.get();
            }
            /***************************
            *  Add a movie.
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
                
                baseList[totalMovies] = toStore;
                /**
                 * Update each of the frag lists with the added movie data,
                 * while preserving the sorting order in each of them.
                 */
                StoreNewFrag(intFrags[DUR], 1, totalMovies, toStore.duration);
                StoreNewFrag(intFrags[YEA], 1, totalMovies, toStore.release.year);
                StoreNewFrag(intFrags[MON], 1, totalMovies, toStore.release.month);
                StoreNewFrag(intFrags[DAY], 1, totalMovies, toStore.release.day);
                StoreNewFrag(wstrFrags[TTL], 1, totalMovies, toStore.title);
                StoreNewFrag(wstrFrags[DIR], 1, totalMovies, toStore.director);

                wcout << "[ INFO ] THE MOVIE WAS ADDED SUCCESSFULLY.\n";
            }
            /***************************
            /*  Rent a movie.
            **************************/
            else if(action == RENT){
                ClrScr();
                wstring rentName;
                wcout << "*** MOVIE RENT ***\n";
                wcout << "Input the name of the movie: ";
                wcin >> rentName;

                // Search for the movie to rent, and throw an error if it doesn't exist in the database. //
                // If it exists, it will get stored in the toRent[] array.                               //
                int ttlPos = BinSearch(wstrFrags[TTL], 1, totalMovies, rentName);
                if(ttlPos == -1){
                    wcerr << L"[ ERR ] THE MOVIE DOES NOT EXIST.\n";
                    wcin.ignore(1);
                    wcin.get();
                    continue;
                }
                int rentPos = wstrFrags[TTL][ttlPos].ID;

                wstring currDate = GetDateTime();           // Get the current date.
                wstring expiryDate = GetDateTime(true);     // Get the expiry date.

                // Update the movies.csv file with the rent information.
                UpdateMoviesCsv(CSV_PATH, rentPos, username, currDate, expiryDate);
                
                // Update the rented movie data with the rent information.
                UpdateMovieData(baseList, rentPos, username, currDate, expiryDate);

                wcin.get();
            }
            // Executes if the user selects the "Exit" action. //
            else break;
            wcin.get();
        }
    }
    return 0;
}

wstring GetDateTime(bool add14Days){
    time_t rawtime;
    struct tm timeinfo;
    wchar_t buffer[20];

    time(&rawtime);
    if(add14Days) rawtime += 1209600;

    localtime_s(&timeinfo, &rawtime);

    wcsftime(buffer, 20, L"%Y-%m-%d", &timeinfo);

    std::wcout << buffer << '\n';
    std::wcin.get();

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