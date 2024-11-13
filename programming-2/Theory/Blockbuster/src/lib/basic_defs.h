#include <string>

#ifdef _WIN32
    #define pcout std::wcout
    #define pcerr std::wcerr
    #define pcin std::wcin
    #define pfstream std::wfstream
    #define pifstream std::wifstream
    #define pofstream std::wofstream
    #define pstring std::wstring
    #define to_pstring(str) (std::to_wstring(str))
    #define pstringstream std::wstringstream
    #define pchar wchar_t

    #define openExcMsg L"[ ERR ] movies.csv FILE DOES NOT EXIST IN THE PROVIDED PATH."
    #define genreExcMsg L"[ ERR ] FOUND TOO MANY GENRES ON MOVIE NUMBER "
    #define statusExcMsg L"[ ERR ] UNEXPECTED STATUS FOUND ON MOVIE NUMBER "
#else
    #define pcout std::cout
    #define pcerr std::cerr
    #define pcin std::cin
    #define pfstream std::fstream
    #define pifstream std::ifstream
    #define pofstream std::ofstream
    #define pstring std::string
    #define to_pstring(str) (std::to_string(str))
    #define pstringstream std::stringstream
    #define pchar char

    #define openExcMsg "[ ERR ] movies.csv FILE DOES NOT EXIST IN THE PROVIDED PATH"
    #define genreExcMsg "[ ERR ] FOUND TOO MANY GENRES ON MOVIE NUMBER "
    #define statusExcMsg "[ ERR ] UNEXPECTED STATUS FOUND ON MOVIE NUMBER "
#endif

using std::getline;