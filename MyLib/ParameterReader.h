#ifndef PARAMETERREADER_H
#define PARAMETERREADER_H

int getNextChar(const char *para);

void getNextWord(const char *para, char *word);

void getFloat(const char *para, int &a, int &b);

void getLongFloat(const char *para, long long &a, long long &b);

#endif
