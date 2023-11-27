#include "file_ops.h"

// ================================
//      GENERAL PURPOSE
// ================================
void AppendLine(const char* filePath, pstring line){
    pofstream file(filePath, std::ios::app);
    file << line;
}

void ReplaceLine(const char* filePath, pstring line, int replaceNum){
    pofstream write("./write.txt");
    pifstream file(filePath);
    if(!file){ std::wcerr << "[ ERR ] ReplaceLine() FAILED. FILE COULD NOT BE OPENED\n"; }
    pstring readingLine;

    int counter = 0;
    while(counter != replaceNum - 1){
        getline(file, readingLine);
        write << readingLine << '\n';
        counter++;
    };

    getline(file, readingLine);
    write << line << '\n';

    while(getline(file, readingLine)){
        write << readingLine << '\n';
    }

    // Close the file and the writeFile. Then delete the file and       //
    // rename the writeFile to the file.                                //
    file.close();
    write.close();

    remove(filePath);
    rename("./write.txt", filePath);
}

int GetLastLineFirstNum(const char* filePath){
    pstring openExc = openExcMsg;
    pifstream file(filePath);
    if(!file){ throw openExc; }

    file.seekg(0, std::ios_base::end);              // Move to the EOF.
    std::ifstream::pos_type pos = file.tellg();    // Get the curr pos and assign to a variable.
    pos = int(pos) - 2;             // Reduce the pos by two to
    file.seekg(pos);                // Go back two chars.

    pchar ch = ' ';
    // Executes while a newline isn't found. //
    while(ch != '\n'){
        pos = int(pos) - 1;         // Reduce the pos by one to
        file.seekg(pos);            // go back one char.
        file.get(ch);               // Get the curr char.
    }

    pstring lastLine;
    getline(file, lastLine);
    
    return stoi(lastLine);          // Return the first number in the last line.
}

// ==================================
//      FILE-SPECIFIC
// ==================================
void CheckMoviesCsv(const char* movFilePath){
    pifstream movFile(movFilePath);
    pstring readingLine;
    getline(movFile, readingLine);      // Get the first line in the movies.csv file.
    int delimNum = 0;

    // Get the number of semicolons in the line. //
    while(true){
        std::size_t nextDelim = readingLine.find(';');
        if(nextDelim == std::string::npos) break;
        readingLine = readingLine.substr(nextDelim + 1);
        delimNum++;
    }

    // The movies.csv file should have 11 fields, and thus, the first line should have
    // 10 semicolons. If 9 were found, add the missing field to the first line, and add a semicolon
    // at the end of every other line.
    if(delimNum == 9){
        movFile.seekg(0, std::ios::beg);
        getline(movFile, readingLine);
        #ifdef _WIN32
        readingLine.append(L";expiry\n");
        #else
        readingLine.append(";expiry\n");
        #endif
        pofstream write("./write.csv");
        write << readingLine;

        while(getline(movFile, readingLine)){
            #ifdef _WIN32
            readingLine.append(L";\n");
            #else
            readingLine.append(";\n");
            #endif
            write << readingLine;
        }

        movFile.close();
        write.close();
        remove(movFilePath);
        rename("./write.csv", movFilePath);
    }
}

void PopulateMovieList(List<Movie> &baseList, const char* movFilePath){
    pstring openExc = openExcMsg;
    pifstream movFile(movFilePath);
    if(!movFile){ throw openExc; }

    int nextDelim;              // Stores the pos of the next semicolon in the curr line.
    pstring readingLine;        // Stores the curr line.

    // Vars for setting the movie genres. //
    int nextPipe = 0;           // Stores the pos of the next pipe in the tempReadingLine.
    bool exitSuccess = false;   // True if no more than 7 genres were found, false otherwise.
    bool getNextLine = false;
    pstring tempReadingLine;    // Temp copy of the curr line.
    pstring genreExc = genreExcMsg;  // Exc thrown when exitSuccess is false.
    pstring statusExc = statusExcMsg;

    getline(movFile, readingLine);          // Ignore the first line in the inFile.
    for(int i = 1; getline(movFile, readingLine); i++){
        // Store each of the 11 data fields.
        for(int j = 0; j < 11; j++){
            nextDelim = readingLine.find(';');
            switch(j){
                // ID. //
                case 0: 
                    baseList.data[i].ID = stoi(readingLine);       // Store the first number of the curr line.
                    break;
                // Title. //
                case 1:
                    baseList.data[i].title = readingLine.substr(0, nextDelim);
                    break;
                // Genres. //
                case 2:
                    // Create a temp copy of the curr line with only the movie genres in it. //
                    tempReadingLine = readingLine.substr(0, nextDelim);
                    // Store a max of 7 genres. //
                    for(int k = 0; k < 7; k++){         
                        nextPipe = tempReadingLine.find('|');
                        // If no pipe character was found on the temp line, assign the last genre and stop storing any more. //
                        if(nextPipe == -1){ baseList.data[i].genres[k] = tempReadingLine; exitSuccess = true; break; }
                        // Otherwise, store the curr genre and remove it from the temp line. //
                        else{
                            baseList.data[i].genres[k] = tempReadingLine.substr(0, nextPipe);
                            tempReadingLine = tempReadingLine.substr(nextPipe + 1);
                        }
                    }
                    // =====================
                    // Throw an exception if the loop never encountered a break statement, meaning there were
                    // more than 7 genres in the readingLine.
                    // =====================
                    if(exitSuccess){ exitSuccess = false; break; }
                    else{ genreExc.append(to_pstring(i)); throw genreExc; }
                // Duration. //
                case 3:
                    baseList.data[i].duration = stoi(readingLine); 
                    break;
                // Director. //
                case 4:
                    baseList.data[i].director = readingLine.substr(0, nextDelim);
                    break;
                // Price. //
                case 5:
                    baseList.data[i].price = stof(readingLine.substr(0, nextDelim));
                    break;
                // Release. //
                case 6:
                    /**
                     * WARNING: Assumes that the release date is in Y-M-d format, the year
                     * is 4 characters long, and the month and day are two characters long each.
                     */
                    baseList.data[i].release.year  = stoi(readingLine.substr(0, 4));
                    baseList.data[i].release.month = stoi(readingLine.substr(5, 2));
                    baseList.data[i].release.day   = stoi(readingLine.substr(8, 2));
                    break;
                // Rent to. //
                case 7:
                    #ifdef _WIN32
                        if(readingLine == L";;;"){ getNextLine = true; break; }
                    #else
                        if(readingLine == ";;;"){ getNextLine = true; break; }
                    #endif
                    
                    baseList.data[i].rentedTo = readingLine.substr(0, nextDelim);
                    break;
                // Rent date. //
                case 8:
                    /**
                     * WARNING: Same considerations as case 6.
                     */
                    baseList.data[i].rentedOn.year  = stoi(readingLine.substr(0, 4));
                    baseList.data[i].rentedOn.month = stoi(readingLine.substr(5, 2));
                    baseList.data[i].rentedOn.day   = stoi(readingLine.substr(8, 2));
                    break;
                // Status. //
                case 9:
                    #ifdef _WIN32
                    tempReadingLine = readingLine.substr(0, nextDelim);
                    if(tempReadingLine == L"returned"){
                        baseList[i].status = MOV_STATUS_RETURNED;
                    } else if(tempReadingLine == L"rented"){
                        baseList[i].status = MOV_STATUS_RENTED;
                    } else if(tempReadingLine == L"expired"){
                        baseList[i].status = MOV_STATUS_EXPIRED;
                    } else{
                        statusExc.append(to_pstring(i));
                        throw statusExc;
                    }
                    #else
                    tempReadingLine = readingLine.substr(0, nextDelim);
                    if(tempReadingLine == "returned"){
                        baseList.data[i].status = MOV_STATUS_RETURNED;
                    } else if(tempReadingLine == "rented"){
                        baseList.data[i].status = MOV_STATUS_RENTED;
                    } else if(tempReadingLine == "expired"){
                        baseList.data[i].status = MOV_STATUS_EXPIRED;
                    } else{
                        statusExc.append(to_pstring(i));
                        throw statusExc;
                    }
                    #endif
                    break;
                // Expiry. //
                case 10:
                    /**
                     * WARNING: Same considerations as case 6.
                     */ 
                    baseList.data[i].expiry.year  = stoi(readingLine.substr(0, 4));
                    baseList.data[i].expiry.month = stoi(readingLine.substr(5, 2));
                    baseList.data[i].expiry.day   = stoi(readingLine.substr(8, 2));
                    break;
            }
            if(getNextLine){ getNextLine = false; break; }
            readingLine = readingLine.substr(nextDelim + 1);  // Remove the stored data field from the curr line.
        }
    }
}

void PopulateUserList(List<User> &userList, const char* userDataFilePath){
    pstring openExc = openExcMsg;
    pifstream userDataFile(userDataFilePath);
    if(!userDataFile){ throw openExc; }

    int nextDelim;
    pstring readingLine;
    getline(userDataFile, readingLine);

    for(int i = 1; getline(userDataFile, readingLine); i++){
        for(int j = 0; j < 3; j++){
            nextDelim = readingLine.find(';');
            switch(j){
                // ID. //
                case 0:
                    userList.data[i].ID = stoi(readingLine);
                    break;
                // Name. //
                case 1:
                    userList.data[i].name = readingLine.substr(0, nextDelim);
                    break;
                // Rented movies. //
                case 2:
                    userList.data[i].movies = readingLine;
                    break;
            }
            readingLine = readingLine.substr(nextDelim + 1);
        }
    }
}