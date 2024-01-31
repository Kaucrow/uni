#include "test.h"

void TestGetDataDir(){
    assert(GetDataDir() != "");
}

void TestGetFileNumOfLines(const char* filePath){
    try{ assert(GetFileNumOfLines(filePath) > 0); }
    catch(string exc){
        cout << "[ TEST ] " << exc << '\n' << std::endl;
        assert(0 == 1);
    }
}

void TestMergeSort(List<IntFrag> &arr, int l, int r){
    MergeSort(arr, l, r);
    for(++l; l < r; l++)
        assert(arr.data[l].data >= arr.data[l - 1].data);
}

void TestBinSearch(List<IntFrag> &arr, int l, int r, int search){
    assert(BinSearch(arr, l, r, search) != -1);
}

void ExecAllTests(){
    TestGetDataDir();

    string dataDir = GetDataDir();
    char CLTFILE_PATH[dataDir.length() + 13];
    strcpy(CLTFILE_PATH, (dataDir + "/clients.csv").c_str());

    TestGetFileNumOfLines(CLTFILE_PATH);

    List<IntFrag> testList(100);
    for(int i = 0; i < 100; i++)
        testList.data[i].data = Random(0, 99);

    TestMergeSort(testList, 0, 99);

    testList.data[0].data = 0;

    TestBinSearch(testList, 0, 99, 0);
}