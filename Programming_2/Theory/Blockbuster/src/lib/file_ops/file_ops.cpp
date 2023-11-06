#include "file_ops.h"

// ================================
//      GENERAL PURPOSE
// ================================
void AppendLine(const char* filePath, wstring line){
    wofstream file(filePath, std::ios::app);
    file << line;
    file.close();
}

void ReplaceLine(const char* filePath, wstring line, int lineNum){
    wofstream write("./write.txt");
    wifstream file(filePath);
    if(!file){ std::wcerr << "[ ERR ] ReplaceLine() FAILED. FILE COULD NOT BE OPENED\n"; }
    wstring readingLine;

    int counter = 0;
    while(counter != lineNum - 1){
        getline(file, readingLine);
        write << readingLine << '\n';
        counter++;
    };

    getline(file, readingLine);
    write << line << '\n';

    while(getline(file, readingLine)){
        write << readingLine << '\n';
    }

    // Close the file and the writeFile. Then delete the file and //
    // rename the writeFile to the file.                                 //
    file.close();
    write.close();

    remove(filePath);
    rename("./write.txt", filePath);
}

/**
 * @name GetNumMoviesImplem
 * Sets the inFile pos to the start of the last line, gets the line,
 * and returns the first number in it.
 */
int GetLastLineFirstNum(const char* filePath){
    wstring openExc = L"[ ERR ] movies.csv FILE DOES NOT EXIST IN THE PROVIDED PATH";
    wifstream file(filePath);
    if(!file){ throw openExc; }

    file.seekg(0, std::ios_base::end);            // Move to the EOF.
    std::wifstream::pos_type pos = file.tellg();  // Get the curr pos and assign to a variable.
    pos = int(pos) - 2;             // Reduce the pos by two to
    file.seekg(pos);              // Go back two chars.

    wchar_t ch = ' ';
    /* Executes while a newline isn't found. */
    while(ch != '\n'){
        pos = int(pos) - 1;         // Reduce the pos by one to
        file.seekg(pos);          // go back one char.
        file.get(ch);             // Get the curr char.
    }

    wstring lastLine;
    getline(file, lastLine);
    
    //file.seekg(0, std::ios_base::beg);    // Put the inFile position back at the beginning.
    file.close();
    return stoi(lastLine);          // Return the first number in the last line.
}

// ==================================
//      FILE-SPECIFIC
// ==================================
void CheckMoviesCsv(const char* movFilePath){
    wifstream movFile(movFilePath);
    wstring readingLine;
    getline(movFile, readingLine);
    int commaNum = 0;
    while(true){
        std::size_t nextComma = readingLine.find(',');
        if(nextComma == std::string::npos) break;
        readingLine = readingLine.substr(nextComma + 1);
        commaNum++;
    }
    if(commaNum == 8){
        movFile.seekg(0, std::ios::beg);
        getline(movFile, readingLine);
        readingLine.append(L",expiry\n");
        wofstream write("./write.csv");
        write << readingLine;
        while(getline(movFile, readingLine)){
            readingLine.append(L",\n");
            write << readingLine;
        }
        movFile.close();
        write.close();
        remove(movFilePath);
        rename("./write.csv", movFilePath);
    }
    movFile.close();
}

void PopulateMovieList(Movie movList[], int movieNum, const char* movFilePath){
    wstring openExc = L"[ ERR ] movies.csv FILE DOES NOT EXIST IN THE PROVIDED PATH";
    wifstream movFile(movFilePath);
    if(!movFile){ throw openExc; }

    int nextComma;              // Stores the pos of the next comma in the curr line.
    wstring readingLine;       // Stores the curr line.

    /* Vars for setting the movie genres. */
    int nextPipe = 0;           // Stores the pos of the next pipe in the tempReadingLine.
    bool exitSuccess = false;   // True if no more than 6 genres were found, false otherwise.
    bool getNextLine = false;
    wstring tempReadingLine;    // Temp copy of the curr line.
    wstring genreExc = L"[ ERR ] FOUND TOO MANY GENRES ON MOVIE NUMBER ";  // Exc thrown when exitSuccess is false.

    movFile.seekg(0);
    getline(movFile, readingLine);      // Ignore the first line in the inFile.

    for(int i = 1; i <= movieNum; i++){
        getline(movFile, readingLine);  // Get the next line
        for(int j = 0; j < 8; j++){     // And store each of the 6 data fields.
            nextComma = readingLine.find(',');
            switch(j){
                /* ID */
                case 0: 
                    movList[i].ID = stoi(readingLine);       // Store the first number of the curr line.
                    break;
                /* Title */
                case 1:
                    /** 
                     * If the first character is a double quote, adjust the value of nextComma to actually
                     * contain the pos of the comma after the title.
                     * WARNING: Assumes the last double quote char is within the title, and nowhere else.
                     */
                    if(readingLine[0] == '"') nextComma = (readingLine.substr(1)).find_last_of('"') + 2;
                    movList[i].title = readingLine.substr(0, nextComma);
                    break;
                /* Genres */
                case 2:
                    /* Create a temp copy of the curr line with only the movie genres in it. */
                    tempReadingLine = readingLine.substr(0, nextComma);
                    /* Store a max of 6 genres. */
                    for(int k = 0; k < 6; k++){         
                        nextPipe = tempReadingLine.find('|');
                        /* If no pipe character was found on the temp line, assign the last genre and stop storing any more. */
                        if(nextPipe == -1){ movList[i].genres[k] = tempReadingLine; exitSuccess = true; break; }
                        /* Otherwise, store the curr genre and remove it from the temp line. */
                        else{
                            movList[i].genres[k] = tempReadingLine.substr(0, nextPipe);
                            tempReadingLine = tempReadingLine.substr(nextPipe + 1);
                        }
                    }
                    /**
                     * Throw an exception if the loop never encountered a break statement, meaning there were
                     * more than 6 genres in the readingLine.
                     */
                    if(exitSuccess){ exitSuccess = false; break; }
                    else{ genreExc.append(std::to_wstring(i)); throw genreExc; }
                /* Duration */
                case 3:
                    movList[i].duration = stoi(readingLine); 
                    break;
                /* Director */
                case 4:
                    movList[i].director = readingLine.substr(0, nextComma);
                    break;
                /* Release */
                case 5:
                    /**
                     * WARNING: Assumes that the release date is in the year-month-day format,
                     * the year is 4 characters long, and the month and day are two characters long each.
                     */
                    movList[i].release.year  = stoi(readingLine.substr(0, 4));
                    movList[i].release.month = stoi(readingLine.substr(5, 2));
                    movList[i].release.day   = stoi(readingLine.substr(8, 2));
                    break;
                case 6:
                    if(readingLine == L",,,"){ getNextLine = true; break; }
                    movList[i].rentedTo = readingLine.substr(0, nextComma);
                    break;
                case 7:
                    movList[i].rentedOn.year  = stoi(readingLine.substr(0, 4));
                    movList[i].rentedOn.month = stoi(readingLine.substr(5, 2));
                    movList[i].rentedOn.day   = stoi(readingLine.substr(8, 2));
                    break;
                case 8:
                    break;
                case 9:
                    movList[i].expiry.year  = stoi(readingLine.substr(0, 4));
                    movList[i].expiry.month = stoi(readingLine.substr(5, 2));
                    movList[i].expiry.day   = stoi(readingLine.substr(8, 2));
                    break;
            }
            if(getNextLine){ getNextLine = false; break; }
            readingLine = readingLine.substr(nextComma + 1);  // Remove the stored data field from the curr line.
        }
    }
    movFile.close();
}

void PopulateUserList(User userList[], int userNum, const char* userDataFilePath){
    wstring openExc = L"[ ERR ] user_data.csv FILE DOES NOT EXIST IN THE PROVIDED PATH";
    wifstream userDataFile(userDataFilePath);
    if(!userDataFile){ throw openExc; }

    int nextComma;
    wstring readingLine;
    getline(userDataFile, readingLine);

    for(int i = 1; getline(userDataFile, readingLine); i++){
        for(int j = 0; j < 3; j++){
            nextComma = readingLine.find(',');
            switch(j){
                case 0:
                    userList[i].ID = stoi(readingLine);
                    break;
                case 1:
                    userList[i].name = readingLine.substr(0, nextComma);
                    break;
                case 2:
                    userList[i].movies = readingLine;
                    break;
            }
            readingLine = readingLine.substr(nextComma + 1);
        }
    }
}