#include "SimSearcher.h"
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

int main(int argc, char **argv)
{
    clock_t start,end;
    SimSearcher searcher;
    vector<pair<unsigned, unsigned> > resultED;
    vector<pair<unsigned, double> > resultJaccard;

    unsigned q = 3, edThreshold = 3;
    double jaccardThreshold = 0.5;
    ifstream fin("E:\\semester\\dabase-training\\hw1\\test1\\query.txt");

    if(!fin) {cout<<"could not open query file"<<endl;return 0;}
    searcher.createIndex(argv[1], q);
    start= clock();
    //searcher.searchJaccard("hello world", jaccardThreshold, resultJaccard);
    string query;
    ofstream fout("E:\\semester\\dabase-training\\hw1\\test1\\cur-eiditon\\result.txt");
    while(getline(fin,query)){
        searcher.searchJaccard(query.c_str(), jaccardThreshold, resultJaccard);
        for(int i = 0;i<resultJaccard.size();i++){
            fout<<resultJaccard[i].first<<' '<<resultJaccard[i].second<<endl;
        }
        fout<<"--------"<<endl;
    }
    fin.close();
    fout.close();
    end = clock();
    cout<<"run time:"<<(double)(end-start)/CLOCKS_PER_SEC<<'s'<<endl;

    return 0;
}

