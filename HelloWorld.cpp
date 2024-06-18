#include <iostream>
#include <fstream>
#include <string>
#include "Sales_data.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;


int main()
{
    string s("C:\\Users\\吴品江\Desktop\\新建 文本文档.txt");
    string fileResult;
    ifstream fileIn(s);
    fileIn >> fileResult;
    cout << fileResult;
    

    return 0;
}
