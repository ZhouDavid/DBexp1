#include "SimSearcher.h"

using namespace std;

int main(int argc, char **argv)
{
    SimSearcher searcher;

    vector<pair<unsigned, unsigned> > resultED;
    vector<pair<unsigned, double> > resultJaccard;

    unsigned q = 2, edThreshold = 2;
    double jaccardThreshold = 0.85;

    searcher.createIndex(argv[1], q);
   // searcher.searchJaccard("query", jaccardThreshold, resultJaccard);
    searcher.searchED("shtick", edThreshold, resultED);
    for(int i = 0;i<resultED.size();i++){
        cout<<resultED[i].first<<' '<<resultED[i].second<<endl;
    }
    return 0;
}

