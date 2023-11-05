#include <iostream>
#include <string>
#include <fstream>
using std::wstring, std::wofstream, std::wifstream, std::getline;

void AppendLine(const char* filePath, wstring line);
void ReplaceLine(const char* filePath, wstring line, int lineNum);