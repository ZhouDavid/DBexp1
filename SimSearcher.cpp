#include "SimSearcher.h"

using namespace std;

SimSearcher::SimSearcher()
{
}

SimSearcher::~SimSearcher()
{
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
    ifstream fin(filename);
    string line;
    vector<string> lines;
    if(!fin){cout<<"could not open file!";return FAILURE;}
    int id = -1;
    bool appeared = false;
    while(getline(fin,line)){
        id++;
        //appeared = false;
//        for(int i = 0;i<lines.size();i++){
//            if(line == lines[i]){
//                appeared = true;
//                break;
//            }
//        }
       // if(!appeared){
            lines.push_back(line);
            records.push_back(pair<int,string>(id,line));
        //}
    }
//    for(int i = 0;i<records.size();i++){
//        cout<<records[i].first<<' '<<records[i].second<<endl;
//    }
    fin.close();
    return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
    result.clear();
    return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
    result.clear();
    int thrs = threshold;
    string q = query;
    int len1 = q.length();
    int size = records.size();
    for(int i = 0;i<size;i++){
        int len2 = records[i].second.length();

        if(abs(len1-len2)>thrs) continue;

        int distance = 0;
        if(len1<len2){
            distance = calED(q,records[i].second,thrs);
        }
        else{
            distance = calED(records[i].second,q,thrs);
        }
        if(distance == -1) continue;
        if(distance<=thrs) result.push_back(pair<unsigned,unsigned>(records[i].first,distance));
    }
    return SUCCESS;
}

int SimSearcher::calED(const string& s1,const string& s2,int threshold) {   //s2 must be longer thant s1!
    int row = s1.length();
    int col = s2.length();
    int d[row+1][col+1];

    for(int i =0;i<=threshold&&i<=col;i++)d[0][i]=i;
    for(int i = 0;i<=threshold&&i<=row;i++)d[i][0]=i;
    int start;int end;int last_end;
    int count = 0;
    for(int i = 1;i<=row;i++){
        count = 0;
        last_end = end;
        start = (i-threshold>1)?(i-threshold):1;
        end = (i+threshold)<col?(i+threshold):col;
        for(int j = start;j<=end;j++){
            if(j == start){
                int cost = (s1[i-1]==s2[j-1])? 0:1;
                int insertion = d[i-1][j]+1;
                int match = d[i-1][j-1]+cost;
                d[i][j] = min(insertion,match);
                if(d[i][j]>threshold) count++;
            }
            else if(j == end){
                if(last_end == col){
                    int cost = s1[i-1]==s2[j-1]? 0:1;
                    int insertion = d[i-1][j]+1;
                    int deletion = d[i][j-1]+1;
                    int match = d[i-1][j-1]+cost;
                    d[i][j] = min3(deletion,insertion,match);
                    if(d[i][j]>threshold) count++;
                }
                else{
                    int cost = s1[i-1]==s2[j-1]? 0:1;
                    int deletion = d[i][j-1]+1;
                    int match = d[i-1][j-1]+cost;
                    d[i][j] = min(deletion,match);
                    if(d[i][j]>threshold) count++;
                }

            }
            else{
                int cost = s1[i-1]==s2[j-1]? 0:1;
                int insertion = d[i-1][j]+1;
                int deletion = d[i][j-1]+1;
                int match = d[i-1][j-1]+cost;
                d[i][j] = min3(deletion,insertion,match);
                if(d[i][j]>threshold) count++;
            }
        }
        if(count == 2*threshold+1) return -1;
    }
    return d[row][col];
}
