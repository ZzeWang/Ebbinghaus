//
// Created by 39389_000 on 2019/2/12.
//

//

#include <algorithm>
#include <iostream>
#include "..\include\cflag.h"

// 从a-z的全域映射,还需要添加A-Z的
std::map<char, int> eat_para
        {
                {'a', 99},
                {'b', 99},
                {'c', 99},
                {'d', 99},
                {'e', 99},
                {'f', 99},
                {'g', 99},
                {'h', 99},
                {'i', 99},
                {'j', 99},
                {'k', 99},
                {'l', 99},
                {'m', 99},
                {'n', 99},
                {'o', 99},
                {'p', 99},
                {'q', 99},
                {'r', 99},
                {'s', 99},
                {'t', 99},
                {'u', 99},
                {'v', 99},
                {'w', 99},
                {'x', 99},
                {'y', 99},
                {'z', 99},
                {'-', 0},
                {'A', 99},
                {'B', 99},
                {'C', 99},
                {'D', 99},
                {'E', 99},
                {'F', 99},
                {'G', 99},
                {'H', 99},
                {'I', 99},
                {'J', 99},
                {'K', 99},
                {'L', 99},
                {'M', 99},
                {'N', 99},
                {'O', 99},
                {'P', 99},
                {'Q', 99},
                {'R', 99},
                {'S', 99},
                {'T', 99},
                {'U', 99},
                {'V', 99},
                {'X', 99},
                {'Y', 99},
                {'Z', 99}
        };

size_t Cflag::collect_cmds(const std::string &cmd)
{
    if (!is_valid(cmd))
        return 0;

    int idx = 0;
    std::string tmp_buf_cmd;
    for (size_t _i = 0; _i < cmd.size();)
    {
        do
        {
            if (jmp[idx].find(cmd[_i]) != jmp[idx].end())
            {
                if (cmd[_i] != '-')
                    tmp_buf_cmd += cmd[_i];
                idx = jmp[idx][cmd[_i++]];
            }
        } while ((idx != 0 || cmd[_i] != '-') && idx != m_scope - 1 && _i < cmd.size());

        if (idx == m_scope - 1)
        {

            __cmd_compose _new_cmd;
            auto res = std::find_if(etab.begin(), etab.end(),
                                    [&](const __cmd_compose &the)
                                    {
                                        return tmp_buf_cmd == the._long_name.name ||
                                               tmp_buf_cmd == the._short_name.name;
                                    });
            _new_cmd._long_name = res->_long_name;
            _new_cmd._short_name = res->_short_name;
            while (cmd[_i] != '-' && _i < cmd.size())
            {
                _new_cmd._long_name.para += cmd[_i];
                _new_cmd._short_name.para += cmd[_i];
                _i++;
            }

            exec_q.push(_new_cmd);
            tmp_buf_cmd.clear();

            idx = 0;
            continue;
        }

        if (idx == 0 && (cmd[_i] == '-' || _i == cmd.size()))
        {
            // it isn't necessary to look for res in etab.
            auto res = std::find_if(etab.begin(), etab.end(),
                                    [&](const __cmd_compose &the)
                                    {
                                        return tmp_buf_cmd == the._long_name.name ||
                                               tmp_buf_cmd == the._short_name.name;
                                    });
            exec_q.push(*res);
            tmp_buf_cmd.clear();
        }
    }
    is_collected = true;
    return exec_q.size();
}

bool Cflag::is_valid(const std::string &cmd)
{

    int idx = 0;
    size_t collect_para = m_scope - 1;
    for (size_t _i = 0;;)
    {
        if (jmp[idx].find(cmd[_i]) != jmp[idx].end())
            idx = jmp[idx][cmd[_i++]];
        else
            return false;

        if (idx == collect_para)
        {
            // collect para
            int bf = _i;
            while (cmd[_i] != '-' && _i < cmd.size()) _i++;
            if (bf == _i) // 没有收集到任何字符, 即没有输入参数
                return false;
            else if (cmd[_i] == '-')
                idx = 0;
            else
                break;
        }

        if (_i >= cmd.size())
            break;
    }
    return true;
}

__cmd Cflag::__push(const std::string &name, bool have_para, int idx)
{
    __cmd new_cmd;

    char last_alpha = 0;
    bool is_short = (idx == 1);


    is_short ? new_cmd.begin = 1 : new_cmd.begin = arrow;
    if (is_short && name.empty())
        return __cmd{0,0,"",""};
    new_cmd.name = name;

    for (const char &x : name)
    {
        if (jmp[idx].find(x) != jmp[idx].end())
        {
            idx = jmp[idx][x];
            continue;
        } else
        {
            last_alpha = x;
            if (is_short) // little different with long cmd
            {
                idx++;
                break;
            }
            if (jmp[idx].empty())
                ++arrow;
            jmp[idx][x] = arrow;
            idx = arrow;

        }
    }

    new_cmd.end = (size_t) idx;
    if (last_alpha == 0) // error
    { ; // TODO
    }
    size_t cmd_last_alpha_position = (size_t) idx - 1;

    if (have_para)
        jmp[cmd_last_alpha_position][last_alpha] = m_scope - 1;
    else
        jmp[cmd_last_alpha_position][last_alpha] = 0;

    return new_cmd;
}
bool Cflag::get(__cmd_compose& ret)
{
    if (!is_collected)
        return false;
    if (!exec_q.empty())
    {
        ret = exec_q.front();
        exec_q.pop();
        return true;
    } else
        return false;
}
void Cflag::test()
{
    size_t cnt = 0;
    std::cout << "jmp table" << "\n";
    std::for_each(jmp.begin(), jmp.end(), [&cnt](const std::map<char, int> &_mp)
    {
        auto b = _mp.begin();
        std::cout << cnt << ":";
        while (b != _mp.end())
        {
            std::cout << b->first << " " << b->second << "\t";
            b++;
        }
        std::cout << std::endl;
        cnt++;
    });
    std::cout << std::endl;


    std::for_each(etab.begin(), etab.end(),
                  [](const __cmd_compose &x)
                  {
                      std::cout << x._long_name.begin << " " << x._long_name.end << " " << x._long_name.name
                                << "\n";
                      std::cout << x._short_name.begin << " " << x._short_name.end << " " << x._short_name.name
                                << std::endl;
                  });
    std::cout << std::endl;
    while (!exec_q.empty())
    {
        std::cout << exec_q.front()._long_name.name << " " << exec_q.front()._long_name.para << "\n";
        std::cout << exec_q.front()._short_name.name << " " << exec_q.front()._short_name.para << "\n";
        exec_q.pop();
    }
}

void Cflag::push(const std::string &ln, const std::string &sn, bool hp)
{
    __cmd_compose new_cmd;

    if (sn.empty())
    {
        __cmd short_cmd {0,0,"",""};
        new_cmd._long_name = __push(ln, hp, 2);
        new_cmd._short_name = short_cmd;
    }
    else
        new_cmd = {__push(ln, hp, 2), __push(sn, hp, 1)};
    etab.push_back(new_cmd);
}