#include "file_ops.h"

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