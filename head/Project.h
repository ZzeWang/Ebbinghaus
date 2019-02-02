//
// Created by 39389_000 on 2019/1/24.
//

#ifndef AAEBB_PROJECT_H
#define AAEBB_PROJECT_H

#include<list>
#include <string>
#include "IO.h"

using std::list;
using std::string;

struct project_table_item
{
public:
    string pjt_name;
    string pjt_path;
    int pjk_id;
    int cnt;    // set every job's id
    int status; // 0 created, 1 started, 2 finished
public:
    project_table_item(const project_table_item &other) = default;

    project_table_item &operator=(const project_table_item &other) = default;

    inline void clear() { pjt_name = "", pjt_path = "", pjk_id = 0, cnt = 0, status = 0; };

    inline void add_id() { cnt++; };

    inline string format() const
    {
        return pjt_name + "::" + std::to_string(pjk_id) + "," + pjt_path + "," + std::to_string(cnt) + "," + std::to_string(status) + "\n" ;
    }
};


class Project
{
    list<project_table_item> projects_table;
    IO io_buf;
    string m_path;
    int pjk_id;
public:

    Project(const string &_m_path, const string &project)
            : projects_table(), io_buf(project), m_path(_m_path), pjk_id(0)
    {
        io_buf.pjk_read_config();
        readConfig();
        io_buf.pjk_read_pjks();
        get();
    };

    inline int generateId() { return pjk_id++; };

    void init() {};

    void changeProj(const string &name);

    void createProj(const string &name);

    void deleteProj(const string &name);

    void deleteProj(const int id);

    void readConfig();

    void setConfig();

    void get();

    void put();
};


#endif //AAEBB_PROJECT_H
