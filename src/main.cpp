#include <cstring>
#include <iostream>
#include "..\include\cflag.h"
using namespace std;

extern char cmd[100];
extern char arg[100];

int main(int args, char *argv[])
{

    int tmp_idx = 0;
    for (int i=1; i<args; i++)
        strcat(cmd, argv[i]);
    string scmd(cmd);
    init_cmds();
    run(scmd);

}