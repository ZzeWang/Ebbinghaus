//
// Created by 39389_000 on 2019/1/24.
//
#include <list>
#include <iostream>
#include <algorithm>
#include "fstream"

#ifndef AAEBB_JOB_H
#define AAEBB_JOB_H

#include "Project.h"
#include "symbol.h"

project_table_item dpe{};

using std::cout;
using std::endl;
using std::list;
extern string msg_io_path;
extern string msg_pjk_name;
extern string msg_job_name;
extern string msg_box;

// struct job table item
struct job_table_item
{
public:
    int job_id, c_down, exec_times, stat;

    // stat : status for a job, 0 unfrozen 1 frozen 2 beingCreated 3 normal
    job_table_item(const job_table_item &other) = default;
    job_table_item(int _id, int _cd, int et, int st) : job_id(_id), c_down(_cd), exec_times(et), stat(st) {};
    job_table_item &operator=(const job_table_item &other) = default;
    inline void clear() { job_id = 0, c_down = 0, exec_times = 0, stat = 0; };
    inline string format() const
    {
        return std::to_string(job_id) + "," + std::to_string(c_down) + "," + std::to_string(exec_times) + "," +
               std::to_string(stat) + "\n";
    };
};

struct job_info_table_item_base
{
    int job_id;
    int status;
public :
    job_info_table_item_base(const int id, const int stat) : job_id(id), status(stat) {};
    virtual bool operator==(const job_info_table_item_base &other){return job_id == other.job_id;}
    virtual string format(){return std::to_string(job_id) + "," + std::to_string(status) + "\n";}
    virtual ~job_info_table_item_base() {};
};

struct job_info_table_item : public job_info_table_item_base
{
    string word;
public:
    job_info_table_item(const int &_j, const string &_w, const int &_s) : job_info_table_item_base{_j, _s}, word(_w) {};
    virtual string format() const final { return std::to_string(job_id) + "," + word + "," + std::to_string(status) + "\n"; }
    virtual bool operator==(const job_info_table_item &other) const final { return word == other.word; };
    virtual bool operator==(const int other) const final { return job_id == other; };
    virtual bool operator==(const string& other) const final { return word == other; };
};


template<class Job_Info_Table_Item = job_info_table_item>
class Job
{
public:
    typedef Job_Info_Table_Item T;
private:
    list<job_table_item> job_table;
    list<T> job_info_table;
    list<job_table_item> zero_table;
    list<job_table_item> frozen_table;
    IO io_buf;
    void __syn(const list<job_table_item> &_T, list<job_table_item> &_M);

public:
    explicit
    Job(const string &project) : io_buf(project), job_table(), job_info_table(), zero_table(){pre_load();};//make sure that project is the default project.
    void pre_load();
    int generate_cdown(int);
    void addJob(const T);
    void addJobs();
    void addJobs(const string &_p);
    void load_job_table_for(list<job_table_item> &which, const string &path);
    void deleteJob(const T &it);
    void deleteJobById(const int it);
    void deleteJobByName(const string &);
    void freezeJobs();
    void freezeJob(const string &name);
    void freezeJob(const job_table_item it);
    void unfreezeJob(const T it);
    void unfreezeJob(const string &name);
    void startup();
    void finishJob();
    static int analyze(int input_stp, int stat)
    {
        msg_job_name = dpe.pjt_name;
        msg_box = "in analyzing job";
        // integer check
        // input_stp 由使用的成员函数提供
        // =1 startup
        // =2 finishJob
        //
        // stat 由dpe提供
        // =1 started
        // =2 finished
        if (input_stp == 1 && stat == 1)
        {
            analyzeResult(1);
            return 1;
        }
        if (input_stp == 1 && stat == 2)
        {
            analyzeResult(2);
            return 2;
        }
        if (input_stp == 2 && stat == 1)
        {
            analyzeResult(3);
            return 3;
        }
        if (input_stp == 2 && stat == 2)
        {
            analyzeResult(4);
            return 4;
        }
        return -1;
    }
    void join();// join job_table and job_info_table using job_id;
    void put();
    virtual void get();
    inline void syn_zero_job_table() { __syn(zero_table, job_table); };
};

static list<Job<>::T> mul_add_table;
static list<job_table_item> mul_frozen_table;

#include <cstdio>

template<class T>
void Job<T>::join()
{
    list<job_table_item> look;
    load_job_table_for(look, dpe.pjt_path + "\\lookTable");

    printf("---------------------------\n");
    printf("id\ttimes\tstat\tcontent\n");
    for (auto &lk: look)
    {
        for (auto &info: job_info_table)
        {
            if (lk.job_id == info.job_id)
            {
                printf("%d\t%d\t%d\t%s\n", lk.job_id, lk.exec_times, lk.stat, info.word.c_str());
                break;
            }
        }
    }
}

template<class T>
void Job<T>::pre_load()
{
    job_table.clear();
    job_info_table.clear();
    frozen_table.clear();

    io_buf.read_block_board();
    load_job_table_for(job_table, dpe.pjt_path + "\\jobTable");
    load_job_table_for(frozen_table, dpe.pjt_path + "\\freezeTable");
    get();
}

template<class T>
void Job<T>::unfreezeJob(const string &name)
{
    auto it = std::find_if(job_info_table.begin(), job_info_table.end(),
                           [&name](const T &x) { return x == name; });
    if (it == job_info_table.end())
        unfreezeError();
    else
        unfreezeJob(*it);
}


template<class T>
void Job<T>::unfreezeJob(const T it)
{
    msg_job_name = std::to_string(it.job_id);
    msg_box = "in unfreeze job.";
    std::for_each(job_table.begin(), job_table.end(),
                  [&it](job_table_item &x) { if (it.job_id == x.job_id) x.stat = 3; });
    auto size = frozen_table.size();
    frozen_table.remove_if([&it](const job_table_item &x) { return it.job_id == x.job_id; });

    if (size - frozen_table.size() == 0)
        unfreezeError();
    else
        unfreezeSuc();
}

template<class T>
void Job<T>::freezeJobs()
{
    std::for_each(mul_frozen_table.begin(), mul_frozen_table.end(), [&](const job_table_item it) { freezeJob(it); });
    mul_frozen_table.clear();
}

template<class T>
void Job<T>::freezeJob(const string &name)
{
    msg_box = "in freeze job.";

    auto it = std::find_if(job_info_table.begin(), job_info_table.end(),
                           [&name](const T &x) {return x == name; });

    if (it != job_info_table.end())
    {
        auto x = std::find_if(job_table.begin(), job_table.end(),
                              [&it](const job_table_item &x) { return x.job_id == it->job_id; });
        if (x != job_table.end())
        {
            msg_job_name = it->word;
            freezeJob(*x);
        } else
            jobDoesNotExist();
    } else
        jobDoesNotExist();
}

template<class T>
void Job<T>::freezeJob(const job_table_item it)
{
    msg_box = "in freeze job.";

    auto res_in = std::find_if(zero_table.begin(), zero_table.end(),
                               [&it](job_table_item &x) { return x.job_id == it.job_id; });

    if (res_in != zero_table.end())
    {
        // modify the job's status
        (*res_in).stat = 1;
        frozen_table.push_back(*res_in);

        job_table_item tmp_cpy = *res_in;
        // remove the frozen job from zero table
        zero_table.remove_if([res_in](const job_table_item &x) { return x.job_id == (*res_in).job_id; });

        std::replace_if(job_table.begin(), job_table.end(),
                        [tmp_cpy](const job_table_item &x) { return x.job_id == tmp_cpy.job_id; }, tmp_cpy);
        freezeSuc();
    } else
        jobDoesNotExist();

};

template<class T>
void Job<T>::startup()
{
    int s = analyze(1, dpe.status); // s is ok
    dpe.status = 1;
    io_buf.set_block_board();


    zero_table.clear();
    std::for_each(job_table.begin(), job_table.end(),
                  [&](const job_table_item &x)
                  {
                      if (x.c_down == 0 && x.stat != 1) // no frozen
                          zero_table.push_back(x);
                  });

    //combinate zero and frozen
    std::for_each(frozen_table.begin(), frozen_table.end(),
                  [&](const job_table_item &x) { zero_table.push_back(x); });

    string tmp;
    std::for_each(zero_table.begin(), zero_table.end(), [&tmp](const job_table_item &x) { tmp += x.format(); });
    io_buf.job_write(dpe.pjt_path + "\\lookTable", tmp); //msg

};

template<class T>
void Job<T>::finishJob()
{
    msg_job_name = "sys:job";
    msg_box = "in finish job.";

    int s = analyze(2, dpe.status);
    if (s == 4)
    {
        analyzeResult(s);
        return;
    }
    dpe.status = 2;
    io_buf.set_block_board();
    std::for_each(zero_table.begin(), zero_table.end(), [](job_table_item &x) { if (x.stat != 1) x.exec_times++; });
    std::for_each(job_table.begin(), job_table.end(), [](job_table_item &x) { if (x.c_down != 0) x.c_down--; });
    std::for_each(zero_table.begin(), zero_table.end(),
                  [&](job_table_item &x) { x.c_down = generate_cdown(x.exec_times); });

    syn_zero_job_table();
    synSuccess();
}

template<class T>
void Job<T>::addJobs(const string &_p)
{
    size_t per = 0, suf = 0, size = _p.size();
    for (; suf < size; suf++)
    {
        if (_p[suf] != ',')
            continue;
        else
        {
            mul_add_table.push_back(T(0, _p.substr(per, suf - per), 0));
            per = suf + 1;
        }
    }
    mul_add_table.push_back(T(0, _p.substr(per, suf - per), 0));
    addJobs();
}

template<class T>
void Job<T>::addJobs()
{
    std::for_each(mul_add_table.begin(), mul_add_table.end(), [&](const T &x) { addJob(x); });
    mul_add_table.clear();
}

template<class T>
void Job<T>::addJob(const T it)
{
    msg_job_name = std::to_string(it.job_id);
    msg_box = "in adding job.";
    auto res = std::find_if(job_info_table.begin(), job_info_table.end(),
                            [&](const T &x) -> bool { return x==it; });
    if (res == job_info_table.end())
    {
        dpe.add_id();
        io_buf.set_block_board();
        job_table.emplace_back(dpe.cnt, 0, 0, 2);// stat = 2
        job_info_table.push_back(T(dpe.cnt, it.word, it.status));
        jobAddSuccess();
    } else
    {
        jobAddError();
        doesExist();
        return;
    }
}

template<class T>
void Job<T>::deleteJobById(const int it)
{
    msg_job_name = std::to_string(it);
    msg_box = "in delete job by id.";
    auto size = job_table.size();
    job_info_table.remove_if([&it](const T &item) -> bool { return item==it; });
    job_table.remove_if([&it](job_table_item &item) { return it == item.job_id; });
    if (size - job_table.size() != 0)
    {
        jobDelSuccess();
    } else
    {
        jobDoesNotExist();
    }
}

template<class T>
void Job<T>::deleteJob(const T& it)
{
    msg_job_name = std::to_string(it.job_id);
    msg_box = "in delete job.";
    auto size = job_table.size();
    job_info_table.remove_if([&it](const T &item) { return item==it; });
    job_table.remove_if([&it](job_table_item &item) { return it.job_id == item.job_id; });
    if (size - job_table.size() != 0)
    {
        jobDelSuccess();
    } else
    {
        jobDelError();
        jobDoesNotExist();
    }
}

bool fuckTobool(const string &name)
{
    for (const char &x: name)
        if (!isdigit(x))
            return false;
    return true;
}

template<class T>
void Job<T>::deleteJobByName(const string &name)
{
    if (name == "")
        return;
    if (fuckTobool(name))
    {
        deleteJobById(atoi(name.c_str()));
        return;
    }
    auto res = std::find_if(job_info_table.begin(), job_info_table.end(), [&name](const T &x)
    {
        return x==name;
    });
    if (res != job_info_table.end())
        deleteJob(*res);
    else
    {
        jobDelError();
        jobDoesNotExist();
    }
    return;
}

template<class T>
void Job<T>::load_job_table_for(list<job_table_item> &which, const string &path)
{
    msg_box = "in loading table.";
    io_buf.job_read(path); // read desk , and load info into io_buf.job_buf
    string tmp = io_buf.get_job_buf();
    job_table_item e(0, 0, 0, 0);
    int n = 0;
    for (int idx = 0; idx < tmp.size();)
    {
        int status = 0;
        if (tmp[idx] == 'e')
            break;
        while (status == 0 && isdigit(tmp[idx]))
        {
            n *= 10;
            n += tmp[idx] - '0';
            idx++;
        }
        if (tmp[idx] == ',' && status == 0)
        {
            status = 1;
            e.job_id = n;
            idx++;
            n = 0;
        } else
            status = -1;
        while (status == 1 && isdigit(tmp[idx]))
        {
            n *= 10;
            n += tmp[idx] - '0';
            idx++;
        }
        if (tmp[idx] == ',' && status == 1)
        {
            e.c_down = n;
            status = 2;
            idx++;
            n = 0;
        } else
            status = -1;

        while (status == 2 && isdigit(tmp[idx]))
        {
            n *= 10;
            n += tmp[idx] - '0';
            idx++;
        }
        if (tmp[idx] == ',' && status == 2)
        {
            e.exec_times = n;
            status = 3;
            idx++;
            n = 0;
        } else
            status = -1;

        while (status == 3 && isdigit(tmp[idx]))
        {
            n *= 10;
            n += tmp[idx] - '0';
            idx++;
        }
        if (tmp[idx] == '\n' && status == 3)
        {
            e.stat = n;
            status = 4;
            idx++;
        } else
            status = -1;

        if (status == 4 && isdigit(tmp[idx]))
        {
            which.push_back(e);
            status = 0;
            n = 0;
            e.clear();
            continue;
        } else if (tmp[idx] == 'e')
        {
            which.push_back(e);
            break;
        } else
            status = -1;
        if (status == -1)
        {
            msg_job_name = "sys:job";
            parseError();
        }
    }

}


template<class T>
void Job<T>::put()
{
    // this function supports the logical independence of each type T
    // T must impl virtual function format() that tells put() how to
    // format job_info_table entries.

    // Cause we use file system to store table, we just use a fstream instance to
    // handle it.

    // Cause job_table's format is defined, we do not need to change our
    // put() function to adjust for it.
    // In function IO::job_write,

    string tmp;
    std::for_each(job_info_table.begin(), job_info_table.end(), [&tmp](const T &x) { tmp += x.format(); });
    io_buf.job_write(dpe.pjt_path + "\\jobTable_info", tmp);

    tmp.clear();
    std::for_each(job_table.begin(), job_table.end(), [&tmp](const job_table_item &x) { tmp += x.format(); });
    io_buf.job_write(dpe.pjt_path + "\\jobTable", tmp);

    tmp.clear();
    std::for_each(zero_table.begin(), zero_table.end(), [&tmp](const job_table_item &x) { tmp += x.format(); });
    io_buf.job_write(dpe.pjt_path + "\\zeroTable", tmp);

    tmp.clear();
    std::for_each(frozen_table.begin(), frozen_table.end(), [&tmp](const job_table_item &x) { tmp += x.format(); });
    io_buf.job_write(dpe.pjt_path + "\\freezeTable", tmp);
}

template<class T>
int Job<T>::generate_cdown(int exec)
{
    static int hash[11] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5};
    return exec >= 11 ? 6 : hash[exec];
}

template<class T>
void Job<T>::get()
{
    msg_job_name = "sys:job->get";
    // every T must impl get() function,
    // the function supports the job's logical independence
    io_buf.job_read(dpe.pjt_path + "\\jobTable_info");
    T e(0, "", 0);
    //each different T has different finite automation that
    // can parse the pattern

    // for a default job information table items, there are
    // a word field and a status field
    string tmp = io_buf.get_job_buf();
    int status = 0;

    for (int idx = 0; idx < tmp.size();)
    {
        while (status == 0 && isdigit(tmp[idx]))
        {
            e.job_id *= 10;
            e.job_id += tmp[idx] - '0';
            idx++;
        }
        if (tmp[idx] == ',' && status == 0)
        {
            idx++;
            status = 2;
        }
        while (status == 2 && isalpha(tmp[idx]))
        {
            e.word += tmp[idx];
            idx++;
        }
        if (status == 2 && tmp[idx] == ',')
        {
            idx++;
            status = 3;
        }
        while (status == 3 && isdigit(tmp[idx]))
        {
            e.status *= 10;
            e.status += tmp[idx] - '0';
            idx++;
        }
        if (tmp[idx] == 'e')
            break;
        else if (status == 3 && tmp[idx] == '\n')
        {
            idx++;
            status = 0;
            job_info_table.push_back(e);
            e = job_info_table_item(0, "", 0);
            continue;
        }
    }
    msg_box = std::to_string(job_info_table.size());
    getSuccess();
}

template<class T>
void Job<T>::__syn(const list<job_table_item> &_T, list<job_table_item> &_M)
{
    for (auto &t: _M)
    {
        auto res_in_T = std::find_if(_T.begin(), _T.end(),
                                     [&t](const job_table_item &x) { return x.job_id == t.job_id; });
        if (res_in_T != _T.end())
            std::replace_if(_M.begin(), _M.end(),
                            [&res_in_T](const job_table_item &x) { return x.job_id == (*res_in_T).job_id; }, *res_in_T);
    }
}

#endif //AAEBB_JOB_H
