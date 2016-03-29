#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
using namespace std;
const int SUCCESS = 0;
const int FAILURE = 1;
class SimSearcher
{
public:
	vector<string> records;
	map<string,vector<int> > invertList;
	//vector<vector<int> >orderList;  //sort the invertList by size of eachlist
	int recordSize;
	int qq;
	int d[256][256];
	int counters[100000];

	int calED(const string& s1,const string& s2,int threshold);
	int min3(int a,int b,int c){int t = a<b?a:b;return t<c?t:c;}
	void scanCount(const string& q,int T,vector<int>& candidate);
	void updateList(const string& s,int id);
	int splitIntoGram(const string& s,int q,vector<string>& result);
public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);

};

