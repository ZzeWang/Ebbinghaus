//
// Created by 39389_000 on 2019/1/25.
//
#include "..\include\Project.h"
#include "..\include\symbol.h"

#include <direct.h>
#include <fstream>
#include <algorithm>
#include <iostream>

extern string msg_io_path;
extern string msg_pjk_name;
extern string msg_job_name;
extern string msg_box;
extern project_table_item dpe;

void Project::createProj(const string &name)
{
    msg_box = "in creating project";
    string tmp;
    if (m_path.back() == '\\')
        tmp = m_path + name;
    else
        tmp = m_path + "\\" + name;

    for (auto &it: projects_table)
    {
        if (name == it.pjt_name)
        {
            msg_pjk_name = dpe.pjt_name;
            pjkCreateError();
            pjkExistError(name);
            return;
        }
    }

    try
    {
#if defined(_WIN32)
        _mkdir(tmp.c_str());
#else
        mkdir(tmp.c_str(), 0777);
#endif
        std::fstream o;
        o.open(tmp + "\\jobTable", std::ios::out);
        o.close();
        o.open(tmp + "\\jobTable_info", std::ios::out);
        o.close();
        o.open(tmp + "\\zeroTable", std::ios::out);
        o.close();
        o.open(tmp + "\\freezeTable", std::ios::out);
        o.close();
        o.open(tmp + "\\lookTable", std::ios::out);
        o.close();
        project_table_item e{name, tmp, generateId(), 0, 0};
        projects_table.push_back(e);
        setConfig();
        msg_pjk_name = name;
        pjkCreateSuccess();
    }
    catch (...)
    {
        throw;
    }
}
void Project::get()
{
    // cause the project's table entry is
    // defined, so there is not changes in table scheme
    // Therefore, we do not set interfaces which is different
    // from what Job class does

    // the get() function is DFA
    io_buf.pjk_read_pjks();
    string tmp = io_buf.get_pjk_buf();
    project_table_item e{"", "", 0, 0, 0};

    int cnt = 0;
    for (int idx = 0; idx < tmp.size();)
    {
        int status = 0;
        while (status == 0 && isalpha(tmp[idx]))
        {
            e.pjt_name += tmp[idx];
            idx++;
        }
        if (status == 0 && tmp[idx] == ':')
        {
            status = 2;
            idx += 2;
        }
        while (status == 2 && isdigit(tmp[idx]))
        {
            e.pjk_id *= 10;
            e.pjk_id += tmp[idx] - '0';
            idx++;
        }
        if (status == 2 && tmp[idx] == ',')
        {
            status = 4;
            idx++;
        }
        while (status == 4 && (isalpha(tmp[idx]) || tmp[idx] == '\\' || tmp[idx] == ':'))
        {
            e.pjt_path += tmp[idx];
            idx++;
        }
        if (status == 4 && tmp[idx] == ',')
        {
            status = 5;
            idx++;
        }
        while (status == 5 && isdigit(tmp[idx]))
        {
            e.cnt *= 10;
            e.cnt += tmp[idx] - '0';
            idx++;
        }
        if (status == 5 && tmp[idx] == ',')
        {
            status = 6;
            idx++;
        }
        while (status == 6 && isdigit(tmp[idx]))
        {
            e.status *= 10;
            e.status += tmp[idx] - '0';
            idx++;
        }
        if (status == 6 && tmp[idx] == '\n')
        {
            status = 7;
            idx++;
        }
        if (status == 7 && tmp[idx] == 'e')
        {
            projects_table.push_back(e);
            cnt++;
            break;
        } else if (isalpha(tmp[idx]))
        {
            status = 0;
            projects_table.push_back(e);
            cnt++;
            e.clear();
            continue;
        }
    }

}
void Project::put()
{
    string tmp;
    std::replace_if(projects_table.begin(), projects_table.end(),
                    [&](const project_table_item &x) { return x.pjt_name == dpe.pjt_name; }, dpe);
    std::for_each(projects_table.begin(), projects_table.end(),
                  [&tmp](const project_table_item &x) { tmp += x.format(); });
    io_buf.pjk_write(io_buf.get_pjk_table_path(), tmp);
}
void Project::changeProj(const string &name)
{
    msg_box = "in changing project.";
    io_buf.read_block_board();
    if (dpe.pjt_name == name)
    {
        msg_pjk_name = dpe.pjt_name;
        defaultPjkWaring();
    }

    auto res_selected = std::find_if(projects_table.begin(), projects_table.end(),
                                     [&](project_table_item &x)
                                     {
                                         if (x.pjt_name == name)
                                         {
                                             x.status == 0 ? x.status = 1 : x.status;
                                             return x.pjt_name == name;
                                         }
                                         else
                                             return x.pjt_name == name;
                                     });

    if (res_selected == projects_table.end())
    {
        msg_pjk_name = name;
        pjkDoesNotExist();
        return;
    }

    dpe = *res_selected;
    io_buf.set_block_board(); // update block board, using the user selected project


    msg_pjk_name = (*res_selected).pjt_name;
    pjkChangeSuc();

}
void Project::deleteProj(const int id)
{
    auto re = std::find_if(projects_table.begin(), projects_table.end(),
                           [id](const project_table_item &x) { return x.pjk_id == id; });
    if (re != projects_table.end())
    {
        deleteProj((*re).pjt_name);
    }
}
void Project::deleteProj(const string &name)
{
    msg_box = "in deleting project.";
    auto size = projects_table.size();
    projects_table.remove_if([&name](const project_table_item &x) { return name == x.pjt_name; });
    if (size - projects_table.size() != 0)
    {
        pjk_id--;
        setConfig();

        string tmp;
        if (m_path.back() == '\\')
            tmp = m_path + name;
        else
            tmp = m_path + "\\" + name;

        msg_pjk_name = name;
        delSuccess();
    } else
    {
        msg_pjk_name = name;
        msg_box += "\nAll projects are follow:";
        std::for_each(projects_table.begin(), projects_table.end(),
                      [&](const project_table_item &x) { msg_box += x.format(); });
        delError();
    }
}
void Project::readConfig()
{
    // DFA
    string tmp = io_buf.get_pjk_buf();

    int id = tmp.find("id=", 0) + 3;
    while (isdigit(tmp[id]) && tmp[id] != ';')
    {
        pjk_id *= 10;
        pjk_id += tmp[id] - '0';
        id++;
    }
}
void Project::setConfig()
{
    string tmp = "id=" + std::to_string(pjk_id);
    io_buf.pjk_write(io_buf.get_config_path(), tmp);
}