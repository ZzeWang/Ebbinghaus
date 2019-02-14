#include <map>
#include <queue>
#include <iostream>
#include <cstring>
#include <cmake_config.h>
#include "../include/Job.h"
#include "../include/cflag.h"

using namespace std;

char cmd[100];
char arg[100];
#include "..\include\cflag.h"

void help_msg()
{
    cout <<
    "Usage of commands:\n" \
    "Management Of Project:\n" \
    "   --pa [name]     create a project that named after name\n" \
    "   --pc [name]     change the default project to name\n"\
    "   --pd [name]     delete project named after name\n"
    "Management Of Job:\n" \
    "   --start     start up the default project\n" \
    "               if the project have already started, this command will\n"\
    "               upgrade the job tables\n" \
    "   --done      finish the project\n"
    "   --add+ [job]\n"\
    "       -a [job]        add a single job into project\n"
    "   --adds [job,...]\n" \
    "       -A [job,...]    add a list of jobs into project\n"
    "                       note:you can use \"\" to brace job list\n"
    "   --fro  [job]\n"\
    "      -f  [job]        stop a job, and making the job working around\n"
    "   --fros [job,...]\n"\
    "       -F [job,...]    stop a list of job, and making them working\n"
    "   --free [job]        free a stopped job\n" \
    "   --remove [job]      remove a job from project"
    << endl;
}


static const string dir(INSTALL_DIR);
Project pjk(dir+"\\dir", dir+"\\project");
Job<JOB_INFO_TYPE> jp(dir+"\\project");
Cflag cflag(100);


void call(int stat)
{
    switch (stat)
    {
        case 1:
            jp.pre_load();
            jp.join();
            break;
        case 2:
            help_msg();
            break;
        case 16:
            jp.pre_load();
            jp.startup();
            jp.join();
            break;
        case 35:
            jp.pre_load();
            jp.finishJob();
            jp.put();
            break;
        case 5:
            pjk.createProj(arg);
            pjk.put();
            break;
        case 6:
            pjk.changeProj(arg);
            break;
        case 7:
            pjk.deleteProj(arg);
            pjk.put();
            break;
        case 26:
            jp.pre_load();
            jp.addJobByParam(string(arg));
            jp.put();
            break;
        case 28:
            jp.pre_load();
            jp.addJobs(string(arg));
            jp.put();
            break;
        case 82:
            jp.pre_load();
            jp.unfreezeJob(string(arg));
            jp.put();
            break;
        case 47:
            jp.pre_load();
            jp.freezeJob(string(arg));
            jp.put();
            break;
        case 49:
            cout << "call job.frozens() with arg=" << arg << endl;
            break;
        case 77:
            jp.pre_load();
            jp.deleteJobByName(string(arg));
            jp.put();
            break;
        default:
            // ;
            break;
    }
    arg[0] = 0;
}

static size_t see(const __cmd_compose& cmd)
{
    if (cmd._long_name.name ==  "start" || cmd._short_name.name =="s")
        return 16;
    if (cmd._long_name.name ==  "done" || cmd._short_name.name =="D")
        return 35;
    if (cmd._long_name.name == "help")
        return 2;
    if (cmd._long_name.name == "look")
        return 1;
    if (cmd._long_name.name ==  "pa")
        return 5;
    if (cmd._long_name.name ==  "pc")
        return 6;
    if (cmd._long_name.name ==  "pd")
        return 7;
    if (cmd._long_name.name ==  "add")
        return 26;
    if (cmd._long_name.name ==  "muladd")
        return 28;
    if (cmd._long_name.name ==  "free")
        return 82;
    if (cmd._long_name.name ==  "frozen")
        return 47;
    if (cmd._long_name.name ==  "remove")
        return 77;

    return 0;
}

void init_cmds()
{
    cflag.push("pc", "", true);
    cflag.push("pa", "", true);
    cflag.push("pd", "", true);
    cflag.push("start", "s", false);
    cflag.push("done", "D", false);
    cflag.push("free", "", true);
    cflag.push("frozen", "f", true);
    cflag.push("mulfrozen", "F", true);
    cflag.push("add", "a", true);
    cflag.push("muladd", "A", true);
    cflag.push("remove", "r", true);
    cflag.push("look", "l", false);
    cflag.push("help", "h", false);
}

void run(const string& cmd)
{
    cflag.collect_cmds(cmd);
    __cmd_compose ret;
    while(cflag.get(ret))
    {
        size_t c_id  = see(ret);
        strcpy(arg, ret._long_name.para.c_str());
        call(c_id);
    }

}



