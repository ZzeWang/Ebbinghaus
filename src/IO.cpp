//
// Created by 39389_000 on 2019/1/24.
//

#include "../include/Project.h"
#include "../include/symbol.h"
#include <fstream>

using std::string;
using std::fstream;

extern string msg_io_path;
extern string msg_pjk_name;
extern string msg_job_name;
extern string msg_box;
extern project_table_item dpe;

void IO::set_block_board()
{
    // msg
    fstream wout(block_board, std::ios::out);
    wout << dpe.format() << 'e';
    wout.clear();

    msg_io_path = dpe.format();
    bbUpdate();
}
void IO::read_block_board()
{
    // read the default project's table entry
    fstream bb(block_board.c_str(), bb.in);
    if (bb.is_open())
    {
        char temps[1024];
        bb.read(temps, 1024);
        job_buf = temps;
    }
    else
    {
        msg_io_path = block_board;
        openError();
    }
    string tmp = job_buf;
    dpe.clear();
    int idx = 0;
    int status = 0;
    while (status == 0 && isalpha(tmp[idx]))
    {
        dpe.pjt_name += tmp[idx];
        idx++;
    }
    if (status == 0 && tmp[idx] == ':')
    {
        status = 2;
        idx += 2;
    }
    while (status == 2 && isdigit(tmp[idx]))
    {
        dpe.pjk_id *= 10;
        dpe.pjk_id += tmp[idx] - '0';
        idx++;
    }
    if (status == 2 && tmp[idx] == ',')
    {
        status = 4;
        idx++;
    }
    while (status == 4 && (isalpha(tmp[idx]) || tmp[idx] == '\\' || tmp[idx] == ':'))
    {
        dpe.pjt_path += tmp[idx];
        idx++;
    }
    if (status == 4 && tmp[idx] == ',')
    {
        status = 5;
        idx++;
    }
    while (status == 5 && isdigit(tmp[idx]))
    {
        dpe.cnt *= 10;
        dpe.cnt += tmp[idx] - '0';
        idx++;
    }
    if (status == 5 && tmp[idx] == ',')
    {
        status = 6;
        idx++;
    }
    while (status == 6 && isdigit(tmp[idx]))
    {
        dpe.status *= 10;
        dpe.status += tmp[idx] - '0';
        idx++;
    }
}
void IO::__read(const string& path, string& buf)
{
    msg_io_path = path;
    buf.clear();
    fstream rin(path, rin.in);
    if (rin.is_open())
    {
        rin.seekg(0, std::ios::end);
        long long size = rin.tellg();
        rin.seekg(0, std::ios::beg);
        auto *tmp = new char[size + 1];
        tmp[size] = '\0';
        rin.read(tmp, size);
        buf = tmp;
        delete[] tmp;
        rin.close();
    }
    else
        openError();
}
void IO::job_read(const string& path)
{
    /*
     this function supports the physical store independence for
     a specific physical store device

     read default project's information from block board
     then, in term of its information, reading more details
     from file or database (it depends on your impl)

     read the detail info of the block board, which will be used to locate
     the project's content in file system (we use file rather then database)
     */
    __read(path, job_buf);
}
void IO::__write(const string &path, const string& content)
{
    fstream wout(path, std::ios::out);
    if (wout.is_open())
    {
        if (content.back() == 'e')
            wout << content;
        else
            wout << content << "e";
    }
    else
    {
        msg_io_path = path;
        openError();
    } //msg
}
void IO::job_write(const string &path, const string& content)
{
    // content is the formatted job information table
    // job_write is to write content and job table into device.

    __write(path, content);

}
void IO::pjk_write(const string &path, const string& content)
{
    __write(path, content);
}
void IO::pjk_read_pjks()
{
    // this function supports the physical store independence for
    // a specific physical store device
    __read(m_path+"\\projects", pjk_buf);
}
void IO::pjk_read_config()
{
    __read(config, pjk_buf);
}