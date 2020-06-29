#include "ParameterReader.h"

int getNextChar(const char *para) {
    int pos = 0;
    while (para[pos] == ' ') {
        ++pos;
    }
    return pos;
}

void getNextWord(const char *para, char *word) {
    int pos = 0;
    while (para[pos] != '\0') {
        word[pos] = para[pos];
        ++pos;
    }
    word[pos] = '\0';
}

void getFloat(const char *para, int &a, int &b) {
    a = 0;
    b = 1;
    int i = 0;
    while (para[i] != '.' && para[i] != '\0') {
        a *= 10;
        a += para[i] - '0';
        i++;
    }
    i++;
    while (para[i] != '\0') {
        a *= 10;
        a += para[i] - '0';
        b *= 10;
        i++;
    }
}

void getLongFloat(const char *para, long long &a, long long &b) {
    a = 0;
    b = 1;
    int i = 0;
    while (para[i] != '.' && para[i] != '\0') {
        a *= 10;
        a += para[i] - '0';
        i++;
    }
    i++;
    while (para[i] != '\0') {
        a *= 10;
        a += para[i] - '0';
        b *= 10;
        i++;
    }
}