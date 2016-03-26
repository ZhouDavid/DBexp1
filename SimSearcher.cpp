//
// Created by Zhou Jianyu on 2016/3/21.
//

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
    if(q<0){cout<<"q is below zero,create index failed.";return FAILURE;}
    this->q = q;
    ifstream fin(filename);
    if(!fin){cout<<"cannot open file!"<<endl; return FAILURE;}
    string line;
   // unsigned int i =0;
    while(getline(fin,line)){
        //if(!appeared(line,lines)){
            lines.push_back(line);  //store in lines
            line_lengths.push_back(line.length());
            //vector<string> tmp;
           // unique(line,tmp);//store in wordTable
           // wordTable.push_back(pair<unsigned,vector<string> >(i,tmp));
       // }
      //  i++;
    }

   // print(lines);
   // cout<<"--------------------------------"<<endl;
    //createQTable(lines,q);
    return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
    result.clear();
    string qquery=query;
    //naive edition
    for(unsigned i=0;i<lines.size();i++){
       // if(appeared(lines[i].second,lines,i))continue;
        double jac = calSimilarity(qquery,i,q);
        if(jac<threshold)continue;
        else{
            result.push_back(pair<unsigned,double>(i,jac));
        }
    }

    return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
    result.clear();
//    for(unsigned i=0;i<lines.size();i++){
//        unsigned distance;
//        string q = query;
//        //unique(q);
//
//        bool ans = calED(q,i,threshold,distance);
//        if(!ans) continue;
//        result.push_back(pair<unsigned,unsigned>(lines[i].first,distance));
//    }
    string q = query;
    for(int i = 0;i<lines.size();i++){
        int r = int(q.length()-line_lengths[i]);
        r = abs(r);
        if(r>threshold)continue;
        int distance = baoliCalED(q,lines[i],threshold);
        if(distance<=threshold) result.push_back(pair<unsigned,unsigned>(i,distance));

    }
    return SUCCESS;
}
int SimSearcher::createQTable(vector<string> &lines, unsigned q) {
    for(unsigned i = 0;i<lines.size();i++){
        string tmp = lines[i];
        vector<string>row;
        if(tmp.length()<=q){
            row.push_back(tmp);
            QTable.push_back(row);
            gramList.insert(tmp);
        }
        else{
            for(unsigned j = 0;j<tmp.length()-q;j++){
                string gram(tmp,j,q);
                row.push_back(gram);
                gramList.insert(gram);
            }
            row.push_back(string(tmp,tmp.length()-q,q));
            gramList.insert(string(tmp,tmp.length()-q,q));
            QTable.push_back(row);
        }
    }
   // print(gramList);
    return SUCCESS;
}

double SimSearcher::calSimilarity(const string s1,const string s2,unsigned q){
    double result=0;
    vector<string> ss1=split(s1,q);
    vector<string> ss2=split(s2,q);
    double d1 = calCommon(ss1,ss2);
    double d2 = calTotal(d1,ss1.size(),ss2.size());
    result=d1/d2;
    return result;
}

double SimSearcher::calSimilarity(const string s1, int index, unsigned q) {
    vector<string> ss1 = split(s1,q);
    double d1 = calCommon(ss1,QTable[index]);
    double d2 = calTotal(d1,ss1.size(),QTable[index].size());
    return d1/d2;
}


bool SimSearcher::calED(const string s1,int index,unsigned threshold,unsigned &distance){
    int row = s1.length()+1;
    int col = lines[index].length()+1;
    int d[row][col];
    //init matrix with 999
    for(int i = 0;i<row;i++){
        for(int j = 0;j<col;j++)
            d[i][j] = 999;
    };

    for(unsigned i = 0;i<=threshold&&i<row;i++) d[i][0]=i;
    for(unsigned i = 1;i<=threshold&&i<col;i++) d[0][i]=i;
    unsigned i,j;
    for(i = 1;i<row;i++){
        for(j= i>threshold>0 ? i-threshold:1;j<col&&j<=i+threshold;j++){
            unsigned cost = lines[index][j-1] == s1[i-1]?0:1;
            unsigned deletion = d[i-1][j]+1;
            unsigned insertion = d[i][j-1]+1;
            unsigned substitution = d[i-1][j-1]+cost;
            d[i][j]=min(deletion,insertion,substitution);
        }
    }
    unsigned delta = col-j;
    distance = d[i-1][j-1]+delta;

    for(int m = 0;m<row;m++){
        for(int n = 0;n<col;n++)
            cout<<d[m][n]<<' ';
        cout<<endl;
    }

    if(distance>threshold) return false;
    return true;
}

void SimSearcher::print(const vector<string>& lines){
    for(int i = 0;i<lines.size();i++)
        cout<<lines[i]<<endl;
}
void SimSearcher::print(const vector<vector<string>> table){
    for(int i= 0;i<table.size();i++){
        for(int j = 0;j<table[i].size();j++){
            cout<<table[i][j]<<' ';
        }
        cout<<endl;
    }
}

void SimSearcher::print(vector<pair<unsigned, double> > resultJaccard){
    for(int i = 0;i<resultJaccard.size();i++){
        cout<<resultJaccard[i].first<<' '<<resultJaccard[i].second<<endl;
    }
}

void SimSearcher::print(set<string>&s){
    for(set<string>::iterator i =s.begin() ;i!=s.end();i++){cout<<*i<<endl;}
}

vector<string> SimSearcher::split(const string s,unsigned q){   //split a string into q grams
    vector<string> result;
    if(s.length()<=q){result.push_back(s);return result;}
    for(int i = 0;i<s.length()-q;i++){
        result.push_back(string(s,i,q));
    }
    result.push_back(string(s,s.length()-q,q));
    return result;
}
bool SimSearcher::exist(vector<string>& set,const string s){
    if(set.empty())return false;
    for(int i = 0;i<set.size();i++){
        if(set[i]==s) return true;
    }
    return false;
}

bool SimSearcher::appeared(string& q,vector<string>&set,int end){
    if(end>set.size())end = set.size();
    for(int i = 0;i<end;i++){
        if(set[i] == q) return true;
    }
    return false;
}

bool SimSearcher::appeared(const string q,vector<pair<unsigned,string> >& s){
    for(int i =0;i<s.size();i++){
        string tmp = s[i].second;
        if(q == tmp) return true;

    }
    return false;
}


double SimSearcher::calCommon(vector<string>& s1,vector<string>&s2){   //calculate the common elements' amount
    double count=0;
    vector<string> appeared;
    for(int i = 0;i<s1.size();i++){
        if(exist(appeared,s1[i])) continue;
        appeared.push_back(s1[i]);
        for(int j=0;j<s2.size();j++){
            if(s1[i] == s2[j])count++;
        }
    }
    return count;
}

double SimSearcher::calTotal(double common,unsigned long long size1,unsigned long long size2){
    return size1+size2-common;
}

unsigned SimSearcher::min(unsigned a,unsigned b,unsigned c){
    unsigned t = a < b ? a : b;
    return t < c ? t : c;
}

int SimSearcher::assign(unsigned *matrix[],unsigned row,unsigned col,unsigned value){
    for(int i = 0;i<row;i++){
        for(int j = 0;j<col;j++)
            matrix[i][j] = value;
    }
    return SUCCESS;
}

void SimSearcher::split(string& str,char a,set<string>& set){
    int start = 0;
    for(int i = 0;i<str.length();i++){
        if(str[i] == a){
            set.insert(string(str,start,i-start));
            start=i+1;
        }
    }
    set.insert(string(str,start));
}

void SimSearcher::unique(string& str,vector<string>&vset){
    vset.clear();
    set<string>uset;
    split(str,' ',uset);
    for(set<string>::iterator it = uset.begin();it!=uset.end();it++){
        vset.push_back(*it);
    }
}

int SimSearcher::baoliCalED(string& q,string& s,int threshold){
    int row = q.length()+1;
    int col = s.length()+1;
    int d[row][col];
    for(int i = 0;i<row;i++)
        for(int j = 0;j<col;j++)
            d[i][j]=3000;
//    for(int i = 0;i<=threshold&&i<row;i++) d[i][0] = i;
//    for(int j = 0;j<=threshold&&j<col;j++) d[0][j] = j;
//    for(int i = 1;i<row;i++){
//        for(int j= i>threshold>0 ? i-threshold:1;j<col&&j<=i+threshold;j++){
//            int a1 = d[i-1][j]+1;
//            int a2 = d[i][j-1]+1;
//            int add = (q[i-1]==s[j-1])?0:1;
//            int a3 = add+d[i-1][j-1];
//            d[i][j] = min(a1,a2,a3);
//        }
//    }

    for(int i = 0;i<=threshold&&i<row;i++) d[i][0] = i;
    for(int j  =0;j<=threshold&&j<col;j++) d[0][j] = j;
    for(int i  =1;i<row;i++){
        for(int j = 1;j<col;j++){
            if(abs(i-j)>threshold) continue;
            int a1 = d[i-1][j]+1;
            int a2 = d[i][j-1]+1;
            int add = (q[i-1]==s[j-1])?0:1;
            int a3 = add+d[i-1][j-1];
            d[i][j] = min(a1,a2,a3);
        }
    }
//    for(int i = 0;i<row;i++){
//        for(int j = 0;j<col;j++){
//            cout<<d[i][j]<<' ';
//        }
//        cout<<endl;
//    }
    return d[row-1][col-1];
}