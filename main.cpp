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
#include <chrono>

typedef unsigned int ui;

using namespace std;

enum strategy {
    huge,
    uniform,
    peak
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
    ui n1;
    ui n2;
    ui k;
    ui large;
    double prob;
    strategy s;
};

struct Param parseArgs(int argc, char **argv) {
    Param rtn;
    ui n1 = 0;
    ui n2 = 0;
    ui large = 2;
    char *arg;
    char para[15] = "";
    int i = 1;
    int cnt = 1;
    double prob = 0.0;
    ui k = 10;
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
        if (strcmp(para, "upper") == 0) {
            ss << arg;
            ss >> n1;
            ss.clear();
            cout << n1 << endl;
        } else if (strcmp(para, "lower") == 0) {
            ss << arg;
            ss >> n2;
            ss.clear();
            cout << n2 << endl;
        } else if (strcmp(para, "k") == 0) {
            ss << arg;
            ss >> k;
            ss.clear();
            cout << k << endl;
        } else if (strcmp(para, "prob") == 0) {
            ss << fixed << setprecision(2) << arg;
            ss >> prob;
            ss.clear();
            cout << prob << endl;
        } else if (strcmp(para, "large") == 0) {
            ss << arg;
            ss >> large;
            cout << large << endl;
        } else if (strcmp(para, "strategy") == 0) {
            if (strcmp(arg, "uniform") == 0) {
                s = strategy::uniform;
                cout << "uniform" << endl;
            } else if (strcmp(arg, "huge") == 0) {
                s = strategy::huge;
                cout << "huge" << endl;
            } else if (strcmp(arg, "peak") == 0) {
                s = strategy::peak;
                cout << "peak" << endl;
            }
        } 
                
    }
    rtn.n1 = n1;
    rtn.n2 = n2;
    rtn.k = k;
    rtn.prob = prob;
    rtn.large = large;
    rtn.s = s;
    return rtn;
}

int main(int argc, char **argv) {
    chrono::high_resolution_clock::time_point beginning = chrono::high_resolution_clock::now();
    struct Param paras = parseArgs(argc, argv); 
    ui m = 0;
    // unordered_set<four_tuple> butterflies; 
    // unordered_set<ui> edges;
    vector<pair<ui, ui> > edges;
    stringstream ss;
    ss << fixed;
    ss.precision(2);
    ss << paras.prob;
    string pr;
    ss >> pr;
    chrono::high_resolution_clock::duration d = chrono::high_resolution_clock::now() - beginning;
    unsigned seed = d.count();
    if (paras.s == strategy::uniform) { 
        string filename = "uniform_" + to_string(paras.n1) + "_" + to_string(paras.n2) + "_" + pr;
        ofstream fout;
        default_random_engine generator;
        generator.seed(seed);
        uniform_real_distribution<double> distribution(0.0, 1.0);
        fout.open(filename.c_str(), ios::out);
        for (ui i = 1; i <= paras.n1; i++) {
            for (ui j = 1; j <= paras.n2; j++) {
                double p = distribution(generator);
                if (p <= paras.prob) {
                    edges.push_back(make_pair(i, j));
                    m++;
                } else {
                    continue;
                }
            }
        }
        fout << "% bip" << endl;
        fout << "% " << m << " " << paras.n1 << " " << paras.n2 << endl;
        for (ui i = 0; i < m; i++) {
            fout << edges[i].first << " " << edges[i].second << endl;
        }
        fout.close();

    } else if (paras.s == strategy::huge) {
        string filename = "huge_" + to_string(paras.n1) + "_" + to_string(paras.n2) + "_" + pr;
        ofstream fout;
        default_random_engine generator;
        generator.seed(seed);
        uniform_real_distribution<double> distribution(0.0, 1.0);
        fout.open(filename.c_str(), ios::out);
        for (ui i = 1; i <= paras.k; i++) {
            edges.push_back(make_pair(1, i));
            edges.push_back(make_pair(2, i));
            m += 2;
        }
        for (ui i = paras.k + 1; i <= paras.n2; i++) {
            double p = distribution(generator);
            if (p <= paras.prob) {
                edges.push_back(make_pair(1, i));
                m++;
            }
            p = distribution(generator);
            if (p <= paras.prob) {
                edges.push_back(make_pair(2, i));
                m++;
            }
        }
        for (ui i = 3; i <= paras.n1; i++) {
            for (ui j = 1; j <= paras.n2; j++) {
                double p = distribution(generator);
                if (p <= paras.prob) {
                    edges.push_back(make_pair(i, j));
                    m++;
                } else {
                    continue;
                }
            }
        }
        fout << "% bip" << endl;
        fout << "% " << m << " " << paras.n1 << " " << paras.n2 << endl;
        for (ui i = 0; i < m; i++) {
            fout << edges[i].first << " " << edges[i].second << endl;
        }
        fout.close();
    } else if (paras.s == strategy::peak) {
        string filename = "peak_" + to_string(paras.n1) + "_" + to_string(paras.n2) + "_" + to_string(paras.large) + "_" + pr;
        ofstream fout;
        default_random_engine generator;
        generator.seed(seed);
        uniform_real_distribution<double> distribution(0.0, 1.0);
        fout.open(filename.c_str(), ios::out);
        for (ui i = 1; i <= paras.large; i++) {
            for (ui j = 1; j <= paras.k; j++) {
                edges.push_back(make_pair(i, j));
                m++;
            }
            for (ui j = paras.k + 1; j <= paras.n2; j++) {
                double p = distribution(generator);
                if (p <= paras.prob) {
                    edges.push_back(make_pair(i, j));
                    m++;
                } else {
                    continue;
                }
            }
        }
        for (ui i = paras.large + 1; i <= paras.n1; i++) {
            for (ui j = 1; j <= paras.n2; j++) {
                double p = distribution(generator);
                if (p <= paras.prob) {
                    edges.push_back(make_pair(i, j));
                    m++;
                } else {
                    continue;
                }
            }
        }
        fout << "% bip" << endl;
        fout << "% " << m << " " << paras.n1 << " " << paras.n2 << endl;
        for (ui i = 0; i < m; i++) {
            fout << edges[i].first << " " << edges[i].second << endl;
        }
        fout.close();
    }
    
    return 0;

}
