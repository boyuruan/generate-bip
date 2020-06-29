#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cstring>
#include <sstream>
#include "MyLib/ParameterReader.h"
#include <cmath>
#include <unordered_map>
#include <unordered_set>

typedef unsigned int ui;

using namespace std;

enum strategy {
    huge,
    uniform,
    toss
};

struct four_tuple {
    ui A{0};
    ui B{0};
    ui C{0};
    ui D{0};

    four_tuple() {}

    four_tuple(ui a, ui b, ui c, ui d) {
        A = a;
        B = b;
        C = c;
        D = d;
    }
};

namespace std {
    template<>
    struct hash<four_tuple> {
        size_t operator()(const four_tuple &t) const {
            auto hash1 = hash<ui>()(t.A);
            auto hash2 = hash<ui>()(t.B);
            auto hash3 = hash<ui>()(t.C);
            auto hash4 = hash<ui>()(t.D);
            return hash1 ^ hash2 ^ hash3 ^ hash4;
        }
    };
}

struct Param {
    ui b;
    double avg_k;
    strategy s;
};

struct Param parseArgs(int argc, char **argv) {
    Param rtn;
    ui b = 0;
    char *arg;
    char para[15] = "";
    int i = 1;
    int cnt = 1;
    double avg_k = 0.0;
    strategy s = strategy::uniform;
    stringstream ss;
    while (cnt < argc) {
        arg = argv[cnt++];
        if (cnt == argc) {
            break;
        }
        i = getNextChar(arg);
        getNextWord(arg + i + 1, para);
        cout << para << '\t';
        arg = argv[cnt++];
        if (strcmp(para, "b") == 0) {
            ss << arg;
            ss >> b;
            ss.clear();
            cout << b << endl;
        } else if (strcmp(para, "k") == 0) {
            ss << fixed << setprecision(1) << arg;
            ss >> avg_k;
            ss.clear();
            cout << avg_k << endl;
        } else if (strcmp(para, "strategy") == 0) {
            if (strcmp(arg, "uniform") == 0) {
                s = strategy::uniform;
            } else if (strcmp(arg, "huge") == 0) {
                s = strategy::huge;
            } else if (strcmp(arg, "toss") == 0) {
                s = strategy::toss;
            }
        } 
                
    }
    rtn.b = b;
    rtn.avg_k = avg_k;
    rtn.s = s;
    return rtn;
}

int main(int argc, char **argv) {
    struct Param paras = parseArgs(argc, argv); 
    string _k, _b;
    stringstream ss;
    ss << fixed << setprecision(1) << paras.avg_k;
    ss >> _k;
    ss.clear();
    ss << paras.b;
    ss >> _b;
    ss.clear();
    ui m, n1, n2;
    m = 0;
    n1 = 0;
    n2 = 0;
    // unordered_set<four_tuple> butterflies; 
    // unordered_set<ui> edges;
    
    if (paras.s == strategy::uniform) {
        string filename = "uniform_" + _b + "_" + _k + ".bloom";
    // cout << filename << endl;
        ofstream fout;
        ui k = (ui) paras.avg_k;
        fout.open(filename.c_str(), ios::out | ios::binary);
        fout.write((char *) &m, sizeof(ui));
        fout.write((char *) &n1, sizeof(ui));
        fout.write((char *) &n2, sizeof(ui));
        ui i = 1;
        while (i <= paras.b) {
            if (m == 0) {
                fout.write((char *) &i, sizeof(ui));
                fout.write((char *) &k, sizeof(ui));
                for (ui j = 1; j <= 2 * k; j++) {
                    fout.write((char *) &j, sizeof(ui));
                    // edges.insert(j);
                    m++; 
                }
                i++;
            } else {
                ui t = m / k;
                for (ui x = 0; x < t; x++) {
                    fout.write((char *) &i, sizeof(ui));
                    fout.write((char *) &k, sizeof(ui));
                    for (ui j = x * k + 1; j <= (x + 1) * k; j++) {
                        fout.write((char *) &j, sizeof(ui));
                        m++;
                    }
                    for (ui j = t * k + 1; j <= (t + 1) * k; j++) {
                        fout.write((char *) &j, sizeof(ui));
                        m++;
                    }
                    i++;
                }
            } 
        }
        fout.seekp(0);
        fout.write((char *) &m, sizeof(ui));

    } else if (paras.s == strategy::huge) {

    }
    
    return 0;

}
