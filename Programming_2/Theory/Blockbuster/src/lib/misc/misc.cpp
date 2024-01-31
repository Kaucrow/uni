#include "misc.h"

void ClrScr(){
    #ifdef _WIN32
        // If on Windows OS
        std::system("cls");
    #else
        // Assuming POSIX OS
        std::system("clear");
    #endif
}

#ifdef _WIN32
string GetDataDir(){
    auto ValidDataDir = [](string path) -> bool{
        struct stat s;
        if((stat(path.c_str(), &s) == 0)){
            if(s.st_mode & S_IFDIR)
                return true;
        }
        return false;
    };

    char pBuf[256];
    size_t len = sizeof(pBuf);
    GetModuleFileName(NULL, pBuf, len);

    string path(pBuf);
    path.append("\\data");
    if(ValidDataDir(path)) return path;
    path.erase(path.find_last_of('\\'));

    for(int i = 0; i < 3; i++){
        path.erase(path.find_last_of('\\'));
        path.append("\\data");
        if(ValidDataDir(path)) return path;
        path.erase(path.find_last_of('\\'));
    }
    return "";
}
#else
string GetDataDir(){
    using std::filesystem::is_directory;

    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    string path;
    if(count != -1) path = dirname(result);
    path.append("/data");
    if(is_directory(path)) return path;
    path.erase(path.find_last_of('/'));
    
    for(int i = 0; i < 3; i++){
        path.erase(path.find_last_of('/'));
        path.append("/data");
        if(is_directory(path)) return path;
        path.erase(path.find_last_of('/'));
    }
    return "";
}
#endif

void GetAction(int& action, int lowBound, int highBound){
    pcin >> action;
    while(action < lowBound || action > highBound){
        pcout << "INVALID OPTION.\nSelect option: ";
        pcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        pcin >> action;
    }
    pcin.ignore(1);
}

pstring GetDate(bool add14Days){
    time_t rawtime;
    struct tm timeinfo;
    pchar buffer[20];

    time(&rawtime);
    if(add14Days) rawtime += 1209600;

    #ifdef _WIN32
        localtime_s(&timeinfo, &rawtime);
        wcsftime(buffer, 20, L"%Y-%m-%d", &timeinfo);
    #else
        localtime_r(&rawtime, &timeinfo);
        strftime(buffer, 20, "%Y-%m-%d", &timeinfo);
    #endif

    return buffer;
}