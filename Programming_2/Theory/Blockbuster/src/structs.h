#include <string>
using std::string;

struct Movie{
    int ID = 0, duration = 0;
    string title = "NULL";
    string genres[7];
    struct{
        int year = 0, month = 0, day = 0;
    } release;
};

/*struct MovieList{
    public:
        MovieList(int setSize) : size(setSize + 3000){}
        int GetSize(){ return size; }
    private:
        int size;
};*/