#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
int main()
{
    string path = "./folder1/file1.txt";
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) == 0)
    {
        if (!S_ISREG(fileStat.st_mode))
            throw -1;
    }
    else
        throw -1;
}