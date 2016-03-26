#include "SimSearcher.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    SimSearcher searcher;
    vector<vector<string>>QGramTable;
    vector<pair<unsigned, unsigned> > resultED;
    vector<pair<unsigned, double> > resultJaccard;

    unsigned q = 3, edThreshold = 2;
    double jaccardThreshold = 0.85;

    searcher.createIndex(argv[1], q);
   // searcher.searchJaccard("query", jaccardThreshold, resultJaccard);
    searcher.searchED("sittingSunday",3, resultED);

    return 0;

}

