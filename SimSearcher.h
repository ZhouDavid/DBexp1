#pragma once
#include <vector>
#include <list>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
using namespace std;
const int SUCCESS = 0;
const int FAILURE = 1;

struct Entry{
	string gram;
	vector<int> list;
	bool operator < (const Entry& b)const {
		return (this->gram < b.gram);
	}
	bool operator > (const Entry& b)const {
		return (this->gram > b.gram);
	}
};
class SimSearcher
{
public:
	vector<string> records;
	vector<Entry> inverList;
	int counters[100000][2];
	int recordSize;
	int qq;
	int qid;

	int calED(const string& s1,const string& s2,int threshold);
	int min3(int a,int b,int c){int t = a<b?a:b;return t<c?t:c;}
	void genGramList(const string& q,vector<int>& gramIndex);
	void scanCount(vector<int>& gramIndex,vector<int>&candidate,int T,int qid);

public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);

	void updateList(const string& s,int id);
	int splitIntoGram(const string& s,int q,vector<string>& result);
};

