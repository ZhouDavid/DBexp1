#include "SimSearcher.h"
#include <fstream>
#include <string>
#include <time.h>
#include <hash_map>
using namespace std;

int main(int argc, char **argv)
{
    clock_t start,end;
    SimSearcher searcher;
    vector<pair<unsigned, unsigned> > resultED;
    vector<pair<unsigned, double> > resultJaccard;

    unsigned q = 3, edThreshold = 3;
    double jaccardThreshold = 0.85;
    ifstream fin("E:\\semester\\dabase-training\\hw1\\test1\\query.txt");

    if(!fin) {cout<<"could not open query file"<<endl;return 0;}
    searcher.createIndex(argv[1], q);
    start= clock();
   // searcher.searchJaccard("query", jaccardThreshold, resultJaccard);
    string query;
    ofstream fout("E:\\semester\\dabase-training\\hw1\\test1\\cur-eiditon\\result.txt");
    while(getline(fin,query)){
        searcher.searchED(query.c_str(), edThreshold, resultED);
            for(int i = 0;i<resultED.size();i++){
            fout<<resultED[i].first<<' '<<resultED[i].second<<endl;
        }
        fout<<"--------"<<endl;
    }
    fin.close();
    fout.close();
    end = clock();
    cout<<"run time:"<<(double)(end-start)/CLOCKS_PER_SEC<<'s'<<endl;
    return 0;
}

