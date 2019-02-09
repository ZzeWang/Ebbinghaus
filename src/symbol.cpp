//
// Created by 39389_000 on 2019/1/29.
//
#include <cstdio>
#include <string>
#include "..\include\symbol.h"

using namespace std;

string msg_io_path;
string msg_pjk_name;
string msg_job_name;
string msg_box;

void openError()
{
    printf("!!!%s  file open fail.\n", msg_io_path.c_str());
}
void bbUpdate()
{
    printf("---block board update.\n");
}
void addition(const string &x, int stat = 0)
{
    switch (stat)
    {
        case 0:
            printf("---[%s]:[%s], %s.\n", msg_job_name.c_str(), x.c_str(), msg_box.c_str());
            break;
        case 1:
            printf("!!![%s]:[%s], %s.\n", msg_job_name.c_str(), x.c_str(), msg_box.c_str());
            break;
        case 2:
            printf("~~~[%s]:[%s], %s.\n", msg_job_name.c_str(), x.c_str(), msg_box.c_str());
            break;
        default:
            break;
    }

}
void getSuccess()
{
    addition("get successfully");
}
void parseError()
{
    addition("parse error", 1);
}
void jobDelSuccess()
{
    addition("job delete successfully");
}
void jobDelError()
{
    addition("job delete error", 1);
}
void doesExist()
{
    addition("job exists", 2);
}
void jobAddError()
{
    addition("job add error", 1);
}
void jobAddSuccess()
{
    addition("job add successfully");
}
void synSuccess()
{
    addition("zero table syn job table, finish successfully");
}
void jobDoesNotExist()
{
    addition("job does not exist", 2);
}
void freezeError()
{
    addition("job frozen error", 1);
}
void freezeSuc()
{
    addition("ob freezes successfully");
}
void unfreezeError()
{
    addition("job didn't be frozen", 1);
}
void unfreezeSuc()
{
    addition("job free");
}
void analyzeResult(int stat)
{
    printf("-!-![%s]:[%s], ", msg_job_name.c_str(), msg_box.c_str());
    switch (stat)
    {
        case 1:
            printf("start job or update look table.\n");
            break;
        case 2:
            printf("job start.\n");
            break;
        case 3:
            printf("job already started.\n");
            break;
        case 4:
            printf("error , trying to finish a finished job.\n");
            break;
        default:
            break;
    }
}




void addition_(const string &x, int stat = 0)
{
    switch (stat)
    {
        case 0:
            printf("---[%s]:[%s], %s.\n", msg_pjk_name.c_str(), x.c_str(), msg_box.c_str());
            break;
        case 1:
            printf("!!![%s]:[%s], %s.\n", msg_pjk_name.c_str(), x.c_str(), msg_box.c_str());
            break;
        case 2:
            printf("~~~[%s]:[%s], %s.\n", msg_pjk_name.c_str(), x.c_str(), msg_box.c_str());
            break;
        default:
            break;
    }
}
void pjkCreateSuccess()
{
    addition_("project create successfully");
}
void pjkCreateError()
{
    addition_("project create error", 1);
}
void pjkExistError(const string &dupName)
{
    printf("!!![%s]:[%s], project [%s] exist.\n", msg_pjk_name.c_str(), msg_box.c_str(), dupName.c_str());
}
void defaultPjkWaring()
{
    addition_("project is default", 2);
}
void pjkDoesNotExist()
{
    addition_("project not exist", 2);
}
void pjkChangeSuc()
{
    addition_("project change successfully");
}
void delSuccess()
{
    addition_("project delete successfully");
}
void delError()
{
    addition_("project delete error", 1);
}
