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
void PopulateMovieList(Movie movList[], int movieNum, const char* movFilePath){
    wstring openExc = L"[ ERR ] movies.csv FILE DOES NOT EXIST IN THE PROVIDED PATH";
    wifstream movFile(movFilePath);
    if(!movFile){ throw openExc; }

    int nextComma;              // Stores the pos of the next comma in the curr line.
    wstring wReadingLine;       // Stores the curr line.

    /* Vars for setting the movie genres. */
    int nextPipe = 0;           // Stores the pos of the next pipe in the wTempReadingLine.
    bool exitSuccess = false;   // True if no more than 6 genres were found, false otherwise.
    wstring wTempReadingLine;   // Temp copy of the curr line.
    wstring genreExc = L"[ ERR ] FOUND TOO MANY GENRES ON MOVIE NUMBER ";  // Exc thrown when exitSuccess is false.

    movFile.seekg(0);
    getline(movFile, wReadingLine);      // Ignore the first line in the inFile.

    for(int i = 1; i <= movieNum; i++){
        getline(movFile, wReadingLine);  // Get the next line
        for(int j = 0; j < 6; j++){     // And store each of the 6 data fields.
            nextComma = wReadingLine.find(',');
            switch(j){
                /* ID */
                case 0: 
                    movList[i].ID = stoi(wReadingLine);       // Store the first number of the curr line.
                    break;
                /* Title */
                case 1:
                    /** 
                     * If the first character is a double quote, adjust the value of nextComma to actually
                     * contain the pos of the comma after the title.
                     * WARNING: Assumes the last double quote char is within the title, and nowhere else.
                     */
                    if(wReadingLine[0] == '"') nextComma = (wReadingLine.substr(1)).find_last_of('"') + 2;
                    movList[i].title = wReadingLine.substr(0, nextComma);
                    break;
                /* Genres */
                case 2:
                    /* Create a temp copy of the curr line with only the movie genres in it. */
                    wTempReadingLine = wReadingLine.substr(0, nextComma);
                    /* Store a max of 6 genres. */
                    for(int k = 0; k < 6; k++){         
                        nextPipe = wTempReadingLine.find('|');
                        /* If no pipe character was found on the temp line, assign the last genre and stop storing any more. */
                        if(nextPipe == -1){ movList[i].genres[k] = wTempReadingLine; exitSuccess = true; break; }
                        /* Otherwise, store the curr genre and remove it from the temp line. */
                        else{
                            movList[i].genres[k] = wTempReadingLine.substr(0, nextPipe);
                            wTempReadingLine = wTempReadingLine.substr(nextPipe + 1);
                        }
                    }
                    /**
                     * Throw an exception if the loop never encountered a break statement, meaning there were
                     * more than 6 genres in the wReadingLine.
                     */
                    if(exitSuccess){ exitSuccess = false; break; }
                    else{ genreExc.append(std::to_wstring(i)); throw genreExc; }
                /* Duration */
                case 3:
                    movList[i].duration = stoi(wReadingLine); 
                    break;
                /* Director */
                case 4:
                    movList[i].director = wReadingLine.substr(0, nextComma);
                    break;
                /* Release */
                case 5:
                    /**
                     * WARNING: Assumes that the release date is in the year-month-day format,
                     * the year is 4 characters long, and the month and day are two characters long each.
                     */
                    movList[i].release.year  = stoi(wReadingLine.substr(0, 4));
                    movList[i].release.month = stoi(wReadingLine.substr(5, 2));
                    movList[i].release.day   = stoi(wReadingLine.substr(8, 2));
                    break;
            } 
            wReadingLine = wReadingLine.substr(nextComma + 1);  // Remove the stored data field from the curr line.
        }
    }
    movFile.close();
}

void PopulateUserDataList(User userList[], int userNum, const char* userDataFilePath){
    
}