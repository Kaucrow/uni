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