#include "SimSearcher.h"

using namespace std;

SimSearcher::SimSearcher()
{
    qid = 0;
}

SimSearcher::~SimSearcher()
{
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
    this->qq = q;
    ifstream fin(filename);
    string line;
    if(!fin){cout<<"could not open file!";return FAILURE;}
    int id = 0;
    while(getline(fin,line)){
        records.push_back(line);
    }
    for(int  i =0;i<records.size();i++){
        updateList(records[i],i);
    }
    recordSize = records.size();
    sort(inverList.begin(),inverList.end());
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
    //int T = len1-qq+1-thrs*qq;
    //vector<int>candidate;
    //vector<int>gramIndex;
    //genGramList(q,gramIndex);
    //scanCount(gramIndex,candidate,T,qid++);
    int size = 1;
    for(int id = 0;id<recordSize;id++){
        int len2 = records[id].length();
        if(abs(len1-len2)>thrs) continue;
        int distance = 0;
        distance = len1 < len2 ? calED(q, records[id], thrs) : calED(records[id], q, thrs);
        if(distance == -1) continue;
        if(distance<=thrs) result.push_back(pair<unsigned,unsigned>(id,distance));
    }
    return SUCCESS;
}

int SimSearcher::calED(const string& s1,const string& s2,int threshold) {   //s2 must be longer thant s1!
    int row = s1.length();
    int col = s2.length();
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
int SimSearcher::splitIntoGram(const string& s,int q,vector<string>& result){
    int len = s.length();
    if(len<q) return FAILURE;
    for(int i = 0;i<=len-q;i++){
        result.push_back(string(s,i,q));
    }
    return SUCCESS;
}
void SimSearcher::updateList(const string& s,const int id){
    vector<string>grams;
    if(splitIntoGram(s,qq,grams)==SUCCESS){
        int gsize = grams.size();
        for(int i = 0;i<gsize;i++){
            bool appeared = false;
            for(int j = 0;j<inverList.size();j++){

                if(grams[i] == inverList[j].gram){
                    inverList[j].list.push_back(id);
                    appeared = true;
                    break;
                }
            }
            if(!appeared){
                Entry tmp;
                tmp.gram = grams[i];
                tmp.list.push_back(id);
                inverList.push_back(tmp);
            }
        }
    }

}

void SimSearcher::genGramList(const string& q,vector<int>& gramIndex){
    vector<string> grams;
    splitIntoGram(q,qq,grams);

    for(int i  =0;i<grams.size();i++){
        for(int j = 0;j<inverList.size();j++){
            if(grams[i] == inverList[j].gram){
                gramIndex.push_back(j);
                break;
            }
        }
    }

}

void SimSearcher::scanCount(vector<int>&gramIndex,vector<int>&candidate,int T,int qid){
    for(int i = 0;i<recordSize;i++) counters[i] = 0;
    for(int i = 0;i<gramIndex.size();i++){
        int index = gramIndex[i];
        int size = inverList[index].list.size();
        for(int j = 0;j<size;j++){
            int id = inverList[index].list[j];
            counters[id]++;
        }
    }
//    sort(candidate.begin(),candidate.end());
//    candidate.erase(unique(candidate.begin(),candidate.end()),candidate.end());
}
