#include "file_ops.h"

// ================================
//      GENERAL PURPOSE
// ================================
void AppendLine(const char* filePath, string line){
    ofstream file(filePath, std::ios::app);
    file << line;
}

void ReplaceLine(const char* filePath, string line, int replaceNum){
    ofstream write("./write.txt");
    ifstream file(filePath);
    if(!file){ std::wcerr << "[ ERR ] ReplaceLine() FAILED. FILE COULD NOT BE OPENED\n"; }
    string readingLine;

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

/*int GetLastLineFirstNum(const char* filePath){
    wstring openExc = L"[ ERR ] movies.csv FILE DOES NOT EXIST IN THE PROVIDED PATH";
    wifstream file(filePath);
    if(!file){ throw openExc; }

    file.seekg(0, std::ios_base::end);              // Move to the EOF.
    std::wifstream::pos_type pos = file.tellg();    // Get the curr pos and assign to a variable.
    pos = int(pos) - 2;             // Reduce the pos by two to
    file.seekg(pos);                // Go back two chars.

    wchar_t ch = ' ';
    // Executes while a newline isn't found. //
    while(ch != '\n'){
        pos = int(pos) - 1;         // Reduce the pos by one to
        file.seekg(pos);            // go back one char.
        file.get(ch);               // Get the curr char.
    }

    wstring lastLine;
    getline(file, lastLine);
    
    return stoi(lastLine);          // Return the first number in the last line.
}*/

int GetFileNumOfLines(const char* filePath, bool ignoreBlankLines){
    string filePathStr(filePath);
    string openExc = "[ ERR ] FILE \"" + filePathStr + "\" DOES NOT EXIST IN THE PROVIDED PATH.\n";
    ifstream file(filePath);
    if(!file){ throw openExc; }

    int linesCount = 0;
    string readingLine;

    if(ignoreBlankLines){
        while(getline(file, readingLine)){
            if(readingLine != "") linesCount++;
        }
    }
    else{
        while(getline(file, readingLine)){
            linesCount++;
        }
    }

    return linesCount;
}

/**
 * @brief Gets the position of the Nth semicolon in a line.
 * @param line - wstring object containing the line to search.
 * @param commaNum - Number of semicolon in the line to get the position of.
 * @return Position of the Nth semicolon in the line (zero-based).
 */
int GetNthDelimPos(string line, int delimNum){
    int delimCounter = 0, totalPos = 0, currPos = 0;
    while(delimCounter != delimNum){
        currPos = (line.find_first_of(','));
        totalPos += currPos;
        if(delimCounter > 0) totalPos += 1;
        line = line.substr(currPos + 1);
        delimCounter++;
    }
    return totalPos;
}

// ==================================
//      FILE-SPECIFIC
// ==================================
/*void CheckMoviesCsv(const char* movFilePath){
    wifstream movFile(movFilePath);
    wstring readingLine;
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
        readingLine.append(L";expiry\n");
        wofstream write("./write.csv");
        write << readingLine;

        while(getline(movFile, readingLine)){
            readingLine.append(L";\n");
            write << readingLine;
        }

        movFile.close();
        write.close();
        remove(movFilePath);
        rename("./write.csv", movFilePath);
    }
}*/

void PopulateClientList(Client baseList[], const char* movFilePath){
    string openExc = "[ ERR ] \"clients.csv\" FILE DOES NOT EXIST IN THE PROVIDED PATH.";
    ifstream movFile(movFilePath);
    if(!movFile){ throw openExc; }

    int nextDelim;              // Stores the pos of the next comma in the curr line.
    string readingLine;         // Stores the curr line.

    movFile.seekg(0);
    getline(movFile, readingLine);          // Ignore the first line in the inFile.

    for(int i = 1; getline(movFile, readingLine); i++){
        baseList[i].ID = i;
        for(int j = 0; j < 5; j++){         // And store each of the 5 data fields.
            nextDelim = readingLine.find(',');
            switch(j){
                // CI. //
                case 0: 
                    baseList[i].CI = stoi(readingLine);       // Store the first number of the curr line.
                    break;
                // Client name. //
                case 1:
                    baseList[i].name = readingLine.substr(0, nextDelim);
                    break;
                // Account number. //
                case 2:
                    baseList[i].accNum = stoll(readingLine);
                    break;
                // Account type. //
                case 3:
                    baseList[i].accType = readingLine.substr(0, nextDelim);
                    break;
                // Suspend status. //
                case 4:
                    readingLine == "true" ? baseList[i].suspend = true : baseList[i].suspend = false;
                    break;
            }
            readingLine = readingLine.substr(nextDelim + 1);  // Remove the stored data field from the curr line.
        }
    }
}