//
// Created by 39389_000 on 2019/2/2.
//

#ifndef AAEBB_SYMBOL_H
#define AAEBB_SYMBOL_H

#include <string>
using std::string;

void openError();
void bbUpdate();


void analyzeResult(int);
void jobDoesNotExist();
void synSuccess();
void jobAddError();
void jobAddSuccess();
void jobDelError();
void jobDelSuccess();
void unfreezeSuc();
void freezeSuc();
void unfreezeError();
void freezeError();
void doesExist();
void parseError();
void getSuccess();
void addition(const string &, int);

void pjkExistError(const string &dupName);
void pjkChangeSuc();
void defaultPjkWaring();
void pjkDoesNotExist();
void delSuccess();
void delError();
void pjkCreateSuccess();
void pjkCreateError();
void addition_(const string &, int);
#endif //AAEBB_SYMBOL_H
