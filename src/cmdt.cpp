#include <map>
#include <queue>
#include <iostream>
#include <cstring>
#include "../head/Job.h"
#include "../head/cmdt.h"

using namespace std;

int __G__stat = 0;
int a_idx = 0;
int c_idx = 0;
char cmd[100];
char arg[100];

// jump table
map<char, int> s1 = {
        {'-', 2},
        {'s', 11},
        {'a', 25},
        {'A', 27},
        {'d', 31},
        {'f', 44},
        {'F', 46},
        {'r', 50},
};


map<char, int> s2 = {
        {'p', 3},
        {'s', 11},
        {'a', 21},
        {'d', 31},
        {'h', 61},
        {'f', 41},
        {'r', 71},
};

map<char, int> s3 = {
        {'a', 4},
        {'c', 5},
        {'d', 6},
};

map<char, int> q3 = {
        {'+', 25},
        {'s', 27},
};

map<char, int> n2 = {
        {'o', 44},
        {'e', 80},
};

map<char, int> n3 = {
        {'+', 46},
        {'s', 48},
};

/*
 *  head : word parse branch node
 *  nextJump: look up hash table and find word entry
 *  dispatch: get into entry
 *  sub-function: check word pattern and collect argement
 *  ->command enqueue
 *  ->command enqueue
 *  ->command enqueue
 *  ... repeat until no commands
 *
 *
 *
 *
 * */


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
void nextJump(int stat, char _nex)
{
    map<char, int>::iterator look;
    if (stat == 1)
        look = s1.find(_nex);
    else if (stat == 3)
        look = s3.find(_nex);
    else if (stat == 2)
        look = s2.find(_nex);
    else if (stat == 24)
        look = q3.find(_nex);
    else if (stat == 43)
        look = n2.find(_nex);
    else if (stat == 45)
        look = n3.find(_nex);
    else if (stat == 0)
    {
        if (_nex == '-')
        {
            __G__stat = 1;
            return;
        } else
        {
            cout << "Entry error" << endl;
            exit(0);
        }
    } else
        return;

    // miss table
    if (stat == 1 && look == s1.end())
    {
        __G__stat = -1;
        throw_parse_error(__G__stat);
    } else if (stat == 2 && look == s2.end())
    {
        __G__stat = -2;
        throw_parse_error(__G__stat);
    } else if (stat == 3 && look == s3.end())
    {
        __G__stat = -3;
        throw_parse_error(__G__stat);
    } else if (stat == 24 && look == q3.end())
    {
        __G__stat = -24;
        throw_parse_error(__G__stat);
    } else if (stat == 43 && look == n2.end())
    {
        __G__stat = -43;
        throw_parse_error(__G__stat);
    } else if (stat == 43 && look == n3.end())
    {
        __G__stat = -43;
        throw_parse_error(__G__stat);
    }
    else if (stat == 45 && look == n3.end())
    {
        __G__stat = -45;
        throw_parse_error(__G__stat);
    }
    else
    {
        __G__stat = look->second;
    }
}
void head()
{
    for (char x = getChar();; x = getChar())
    {
        if (__G__stat == 0 && x == '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 1 && x == '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 1 && isalpha(x))
            nextJump(__G__stat, x);
        else if (__G__stat == 2 && x != '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 3 && x != '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 24 && x != '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 43 && x != '-')
            nextJump(__G__stat, x);
        else if (__G__stat == 45 && x != '-')
            nextJump(__G__stat, x);
        else
        {
            ungetChar(2);
            break;
        }
    }
};
void dispatch()
{
    switch (__G__stat)
    {
        case 11:
            pstart();
            break;
        case 31:
            pdone();
            break;
        case 4:
            pa();
            break;
        case 5:
            pc();
            break;
        case 6:
            pd();
            break;
        case 41:
            f_head();
            break;
        case 44:
            fro_head();
            break;
        case 46:
            fro();
            break;
        case 48:
            fros();
            break;
        case 80:
            jfree();
            break;
        case 21:
            add_head();
            break;
        case 25:
            add();
            break;
        case 27:
            adds();
            break;
        case 61:
            help();
            break;
        case 71:
            remove();
            break;
    }
}
inline void set_error_code_print(int code, const char *info, const char *withcmd = nullptr)
{
    cout << info;
    if (withcmd)
        cout << withcmd << endl;
}

void throw_parse_error(int stat)
{
    if (stat == -16)
        set_error_code_print(-15, "'start' expected not :", cmd);
    if (stat == -35)
        set_error_code_print(-34, "'done' expected not :", cmd);
    if (stat == -1)
        set_error_code_print(-1, "'-' expected, not :", cmd);
    if (stat == -2)
        set_error_code_print(-2, "--pa|d|c|--start|--f..|--a.. expected not :", cmd);
    if (stat == -3)
        set_error_code_print(-3, "--pa|pc|pd expected not :", cmd);
    if (stat == -4)
        set_error_code_print(-4, "--pd need arg");
    if (stat == -5)
        set_error_code_print(-5, "--pc need arg");
    if (stat == -6)
        set_error_code_print(-6, "--pd need arg");
    if (stat == -24)
        set_error_code_print(-24, "--add.. expected, not :", cmd);
    if (stat == -25)
        set_error_code_print(-25, "--add+ need arg");
    if (stat == -26)
        set_error_code_print(-26, "--add+ expected, not:", cmd);
    if (stat == -27)
        set_error_code_print(-27, "--adds need arg");
    if (stat == -28)
        set_error_code_print(-28, "--adds expected, not: ", cmd);
    if (stat == -48)
        set_error_code_print(-28, "--fros need arg");
    if (stat == -43)
        set_error_code_print(-43, "--free|fro|fros expected, not :", cmd);
    if (stat == -47)
        set_error_code_print(-47, "--fro+ expected, not: ", cmd);
    if (stat == -49)
        set_error_code_print(-49, "--fros expected, not :", cmd);
    if (stat == -50)
        set_error_code_print(-50, "--free expected ,not :", cmd);
    if (stat == -44)
        set_error_code_print(-44, "--frozen need arg");
    if (stat == -45)
        set_error_code_print(-44, "--fro... expected, not :", cmd);
    if (stat == -46)
        set_error_code_print(-46, "--fro+ need arg");
    if (stat == -64)
        set_error_code_print(-64, "--help expected, not", cmd);
    if (stat == -71)
        set_error_code_print(-71, "--remove need arg");
    if (stat== -77)
        set_error_code_print(-77, "--remove expected, not", cmd);
    if (stat == -82)
        set_error_code_print(-77, "--free expected, not", cmd);
    if (stat == -80)
        set_error_code_print(-77, "--free need arg");
}



inline char getChar()
{
    return cmd[c_idx++];
}

inline void ungetChar()
{
    c_idx--;
}
inline void ungetChar(int m)
{c_idx-=m;}

Job<> jp("F:\\cppProject\\AAebb\\project");
Project pjk("F:\\cppProject\\AAebb\\dir", "F:\\cppProject\\AAebb\\project");

void call(int stat)
{

    switch (stat)
    {
        case 64:
            help_msg();
            break;
        case 16:
            jp.startup();
            jp.join();
            break;
        case 35:
            jp.finishJob();
            jp.put();
            break;
        case 5:
            pjk.createProj(arg);
            pjk.put();
            break;
        case 6:
            pjk.changeProj(arg);
            jp.pre_load();
            break;
        case 7:
            pjk.deleteProj(arg);
            pjk.put();
            break;
        case 26:
            jp.addJob(job_info_table_item(0, string(arg), 0));
            jp.put();
            break;
        case 28:
            jp.addJobs(string(arg));
            jp.put();
            break;
        case 82:
            jp.unfreezeJob(string(arg));
            jp.put();
            break;
        case 47:
            jp.freezeJob(string(arg));
            jp.put();
            break;
        case 49:
            cout << "call job.frozens() with arg=" << arg << endl;
            break;
        case 77:
            jp.deleteJobByName(string(arg));
            jp.put();
            break;
    }
    arg[0] = 0;
    a_idx = 0;
}

void run()
{
    if (__G__stat < 0)
    {
        cout << "roll back" << endl;
        return;
    }

    while (!exec_queue.empty())
    {
        pair<int, string> x = exec_queue.front();
        exec_queue.pop();
        strcpy(arg, x.second.c_str());
        call(x.first);
    }
}


inline void entry_check(int stat)
{
    if (__G__stat != stat)
    {
        cout << "Entry error" << endl;
        exit(0);
    }
}

inline void template_set_G_stat(int start_stat, int end_stat, const char *stat_nex, int idx_nex = 0)
{
    if (start_stat != end_stat)
    {
        char x = getChar();
        if (*(stat_nex + idx_nex) == x)
        {
            __G__stat = ++start_stat;
            template_set_G_stat(start_stat, end_stat, stat_nex, ++idx_nex);
        } else
        {
            return;
        }
    }
}

inline void without_arg_check_exec(int error_code)
{
    if (__G__stat != error_code)
    {
        __G__stat = -error_code;
        throw_parse_error(__G__stat);
    } else
    {
        pair<int, string> exe(__G__stat, "");
        exec_queue.push(exe);
        __G__stat = 0;
    }
}

inline void with_arg_check_exec(int error_code)
{
    if (a_idx == 0)
    {
        __G__stat = error_code;
        throw_parse_error(__G__stat);
    } else
    {
        arg[a_idx] = 0;
        exec_queue.push(make_pair(__G__stat, string(arg)));
        __G__stat = 0;
    }
}

inline int eat_arg(int error_code)
{
    a_idx = 0; //eat char from the beginning
    for (char x = getChar(); x != 0 && x != '-'; x = getChar())
    {
        x != '"' ? arg[a_idx++] = x : x;
    }
    ungetChar();
    with_arg_check_exec(error_code);
    return a_idx;
}
void help()
{
    entry_check(61);
    template_set_G_stat(61,64,"elp");
    without_arg_check_exec(64);
}
void pstart()
{
    entry_check(11);
    template_set_G_stat(11, 16, "start", 0);
    without_arg_check_exec(16);
}

void pdone()
{
    entry_check(31);
    template_set_G_stat(31, 35, "done");
    without_arg_check_exec(35);
}

void add_head()
{
    entry_check(21);
    template_set_G_stat(21, 24, "add", 0);
    if (__G__stat != 24)
    {
        __G__stat = -24;
        throw_parse_error(__G__stat);
    }
}

void f_head()
{
    entry_check(41);
    template_set_G_stat(41, 43, "fr", 0);
    if (__G__stat != 43)
    {
        __G__stat = -43;
        throw_parse_error(__G__stat);
    }

}

void adds()
{
    entry_check(27);
    template_set_G_stat(27,28,"s");
    if (__G__stat != 28)
    {
        __G__stat = -28;
        throw_parse_error(__G__stat);
    }
    eat_arg(-27);
}

void add()
{
    entry_check(25);
    template_set_G_stat(25,26,"+");
    if (__G__stat != 26)
    {
        __G__stat = -26;
        throw_parse_error(__G__stat);
    }
    eat_arg(-25);
}

void remove()
{
    entry_check(71);
    template_set_G_stat(71, 77,"remove");
    if (__G__stat != 77)
    {
        __G__stat = -77;
        throw_parse_error(__G__stat);
    }
    eat_arg(-71);
}

void jfree()
{
    entry_check(80);
    template_set_G_stat(80, 82, "ee");
    if (__G__stat != 82)
    {
        __G__stat = -82;
        throw_parse_error(__G__stat);
    }
    eat_arg(-80);
}

void fro()
{ // 46
    entry_check(46);
    template_set_G_stat(46,47, "+");
    if (__G__stat != 47)
    {
        __G__stat = -47;
        throw_parse_error(__G__stat);
    }
    eat_arg(-46);
}

void fros()
{
    entry_check(48);
    template_set_G_stat(48,49,"s");
    if (__G__stat != 49)
    {
        __G__stat = -49;
        throw_parse_error(__G__stat);
    }
    eat_arg(-48);
}

void fro_head()
{
    entry_check(44);
    template_set_G_stat(44, 45, "o", 0);
    if (__G__stat != 45)
    {
        __G__stat = -45;
        throw_parse_error(__G__stat);
    }
}

inline void template_p_(int stat, const char* pat)
{
    entry_check(stat);
    template_set_G_stat(stat, stat+1, pat);
    eat_arg(-stat);
}

void pc()
{
    entry_check(5);
    template_set_G_stat(5, 6, "c");
    if (__G__stat != 6)
    {
        __G__stat = -6;
        throw_parse_error(__G__stat);
    }
    eat_arg(-5);
}

void pa()
{
    entry_check(4);
    template_set_G_stat(4, 5, "a");
    if (__G__stat != 5)
    {
        __G__stat = -5;
        throw_parse_error(__G__stat);
    }
    eat_arg(-4);
}

void pd()
{
    entry_check(6);
    template_set_G_stat(6, 7, "d");
    if (__G__stat != 7)
    {
        __G__stat = -7;
        throw_parse_error(__G__stat);
    }
    eat_arg(-6);
}



