#include <cstring>
#include <iostream>
#include "..\include\cmdt.h"
using namespace std;

extern int __G__stat;
extern int a_idx;
extern int c_idx;
extern char cmd[100];
extern char arg[100];

int main(int args, char *argv[])
{
    __G__stat = 0;
    a_idx = 0;
    c_idx = 0;
    int tmp_idx = 0;
    for (int i=1; i<args; i++)
        strcat(cmd, argv[i]);
    int size = strlen(cmd);
    for( ;c_idx<size;)
    {
        head();
        dispatch();
        if (__G__stat < 0)
            break;
    }
    run();

}