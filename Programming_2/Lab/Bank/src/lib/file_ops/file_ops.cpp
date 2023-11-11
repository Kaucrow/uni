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
 * @brief Gets the position of the Nth comma in a line.
 * @param line - string object containing the line to search.
 * @param commaNum - Number of comma in the line to get the position of.
 * @return Position of the Nth comma in the line (zero-based).
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

void PopulateClientList(Client baseList[], const char* clientFilePath){
    string openExc = "[ ERR ] \"clients.csv\" FILE DOES NOT EXIST IN THE PROVIDED PATH.\n";
    ifstream clientFile(clientFilePath);
    if(!clientFile){ throw openExc; }

    int nextDelim;              // Stores the pos of the next comma in the curr line.
    string readingLine;         // Stores the curr line.

    getline(clientFile, readingLine);           // Ignore the first line in the clientFile.

    for(int i = 1; getline(clientFile, readingLine); i++){
        // Store the client's ID. //
        baseList[i].ID = i;

        // Store each of the 5 basic data fields in each Client struct element. //
        for(int j = 0; j < 5; j++){
            nextDelim = readingLine.find(',');      // Get the pos of the next comma in the readingLine.
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
                    (readingLine.substr(0, nextDelim) == "current") ?
                        baseList[i].accType = ACC_CURRENT : baseList[i].accType = ACC_DEBIT;
                    break;
                // Suspend status. //
                case 4:
                    readingLine == "true" ? baseList[i].suspended = true : baseList[i].suspended = false;
                    break;
            }
            readingLine = readingLine.substr(nextDelim + 1);    // Remove the stored data field from the curr line.
        }
    }
}

void PopulateClientListBalance(Client baseList[], const char* clientOpsPath){
    string openExc = "[ ERR ] \"client_ops.csv\" FILE DOES NOT EXIST IN THE PROVIDED PATH.";
    ifstream clientOps(clientOpsPath);
    if(!clientOps){ throw openExc; }

    string readingLine;         // Stores the curr line.

    getline(clientOps, readingLine);          // Ignore the first line in the clientOps.

    // Store the balance for every client. //
    for(int i = 1; getline(clientOps, readingLine); i++){
        readingLine = readingLine.substr(GetNthDelimPos(readingLine, 2) + 1);
        baseList[i].balance.dollars = stoi(readingLine);
        baseList[i].balance.cents = stoi(readingLine.substr(readingLine.find_first_of('.') + 1));
    }
}