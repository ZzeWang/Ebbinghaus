//
// Created by 39389_000 on 2019/1/24.
//

#ifndef AAEBB_IO_H
#define AAEBB_IO_H

#include<string>
#include <iostream>

using std::string;

class IO
{
    string m_path;
    string block_board;
    string config;
    string pjk_buf;
    string job_buf;

public:

    explicit
    IO(const string &_m) : m_path(_m), pjk_buf(""), job_buf(""), block_board(_m + "\\bb"), config(_m + "\\config") {};

    void read_block_board();
    void set_block_board();

    inline string get_pjk_table_path() const { return m_path+"\\projects"; };
    inline string get_config_path() const { return m_path+"\\config"; };
    inline string get_job_buf() const { return job_buf; };
    inline string get_pjk_buf() const { return pjk_buf; };
    inline void __read(const string&, string&);
    inline void __write(const string &, const string &);

    void pjk_read_pjks();
    void pjk_read_config();
    void pjk_write(const string &, const string&);
    void job_read(const string &);
    void job_write(const string &, const string &);

};

#endif //AAEBB_IO_H
