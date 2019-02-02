//
// Created by 39389_000 on 2019/2/2.
//

#ifndef AAEBB_CMDT_H
#define AAEBB_CMDT_H

#include <queue>
// global vars



static std::queue<std::pair<int, std::string>> exec_queue;

// local functions declaration
static void pstart();
static void pdone();
static void pa();
static void pc();
static void pd();
static void add_head();
static void adds();
static void add();
static void f_head();
static void jfree();
static void fro();
static void fro_head();
static void fros();
void throw_parse_error(int stat);
void nextJump(int stat, char _nex);
void dispatch();
void run();
void head();

#endif //AAEBB_CMDT_H
