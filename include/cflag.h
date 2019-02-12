//
// Created by 39389_000 on 2019/2/11.
//

#ifndef CFLAG_FLAG_H
#define CFLAG_FLAG_H
#pragma once

#include <vector>
#include <string>
#include <map>
#include <queue>

extern std::map<char, int> eat_para;

struct __cmd
{
    size_t begin, end;
    std::string name, para;
};

struct __cmd_compose
{
    __cmd _long_name;
    __cmd _short_name;
};

class Cflag
{
    std::vector<std::map<char, int>> jmp;
    std::vector<__cmd_compose> etab;
    bool is_collected;
    size_t m_scope;
    size_t arrow;
    void __init__(){
        std::map<char, int> tmp;
        tmp['-'] = 1; // ready for short cmd -
        jmp[0] = tmp;
        tmp.clear();
        tmp['-'] = 2;
        jmp[1] = tmp; // ready for long cmd --
    }

public:
    std::queue<__cmd_compose> exec_q;

    explicit
    Cflag(size_t scope) : m_scope(scope), jmp(scope), arrow(2), exec_q(), is_collected(false)
    {
        __init__();
        jmp[m_scope - 1] = eat_para;
    };

    void push(const std::string &, const std::string &, bool);

    bool is_valid(const std::string &);
    __cmd __push(const std::string &name, bool have_para, int);
    size_t collect_cmds(const std::string &);
    void test();
    bool  get(__cmd_compose&);
};

#endif //CFLAG_FLAG_H
