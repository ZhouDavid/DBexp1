//
// Created by Zhou Jianyu on 2016/3/21.
//

#ifndef LAB1_SIMSEARCH_H
#define LAB1_SIMSEARCH_H
#pragma once
#include <vector>
#include <set>
#include <utility>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <string.h>
#include <cmath>
using namespace std;
const int SUCCESS = 0;
const int FAILURE = 1;
struct ListNode{
    unsigned count;
    ListNode* next;
};
class SimSearcher
{
private:
    unsigned q;

    void print(const vector<string>& lines);
    void print(const vector<vector<string>> table);
    void print(vector<pair<unsigned, double> > resultJaccard);
    void print(set<string>&s);
    vector<string> split(const string s,unsigned q);   //split a string into q grams
    bool exist(vector<string>& set,const string s);
    bool appeared(string& q,vector<string>&set,int end);
    bool appeared(const string q,vector<pair<unsigned,string> >& s);
    double calCommon(vector<string>& s1,vector<string>&s2);   //calculate the common elements' amount
    double calTotal(double common,unsigned long long size1,unsigned long long size2);
    unsigned min(unsigned a,unsigned b,unsigned c);
    int assign(unsigned *matrix[],unsigned row,unsigned col,unsigned value);
    void split(string& str,char a,set<string>& set);
    void unique(string& str,vector<string>& set);
    vector<pair<unsigned,vector<string> > >wordTable;
    int baoliCalED(string& query,string& s,int threshold);
    int genGram(const string& query,vector<string>& result);
    int calCandidate(vector<string>& grams,int threshold,vector<int>& candidateList);
    void addToGramList(const string s,int id);

public:
    SimSearcher();
    ~SimSearcher();
    vector<string> lines;   //input lines
    vector<int> line_lengths;
    vector<vector<string> > QTable;//two dimension string table, every line stands for a q-gram set for one input line
    vector<pair<string,vector<int>>> gramList;   //store all q-grams in lines
    int createQTable(vector<string>&lines ,unsigned q);
    int createList(vector<string>& lines,unsigned q);    //create inverted list for q-gram
    double calSimilarity(string s1,string s2,unsigned q);   //calculate  s1 s2 Jaccard similarity
    double calSimilarity(const string s1, int index, unsigned q);   //calculate  s1 s2 Jaccard similarity(using index in QTable)
    bool calED(const string s1,int index,unsigned threshold,unsigned& distance);
    int createIndex(const char *filename, unsigned q);
    int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
    int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);

};


#endif //LAB1_SIMSEARCH_H
