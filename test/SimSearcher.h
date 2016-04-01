#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <unordered_map>
using namespace std;
using namespace __gnu_cxx;
const int SUCCESS = 0;
const int FAILURE = 1;
//const int BUCKET_COUNT = 100000000;
//const int GRAM_COUNT = 10000;
class SimSearcher
{
public:
	vector<string> records;
	unordered_map<string,vector<int> > invertList;
	unordered_map<string,vector<int> > word_invertList;
	//vector<pair<string,int> > listSize;  // 记录每个qgram-list的 size,并排序
	//unordered_map<string,int> position; // 查询一个gram,给出它在listSize中的下标
	static bool comp(vector<int>a,vector<int> b){
		return (a.size() > b.size());
	}
	static bool heap_comp(pair<int,int> a,pair<int,int>b){return (a.second > b.second);}
	double my_max(double a,double b){return a>b?a:b;}
	int recordSize;
	int qq;
	int d[256][256];
	int counters[100000];
	vector<int> word_counter;
	int smin;  //记录中最小串的长度

	int calED(const string& s1,const string& s2,int threshold);
	int min3(int a,int b,int c){int t = a<b?a:b;return t<c?t:c;}
	void scanCount(unordered_map<string,vector<int> >&invertList,const vector<string>& grams,int T,vector<int>& candidate);
	void updateList(const string& s,int id);
	int updateWordList(const string& s,int id);   //返回unique words 数量
	int splitIntoGram(const string& s,int q,vector<string>& result);
	void divideSkip(const vector<string>& grams,int T,vector<int>& candidate);
	void mergeSkip(int T,vector<vector<int> >&,unordered_map<int,int>&result);
	int split(const string& q,char s,vector<string>&words);

public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);

};

