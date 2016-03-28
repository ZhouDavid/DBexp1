#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
using namespace std;
const int SUCCESS = 0;
const int FAILURE = 1;

class SimSearcher
{
public:
	vector<pair<int,string>> records;

	int calED(const string& s1,const string& s2,int threshold);
	int min3(int a,int b,int c){int t = a<b?a:b;return t<c?t:c;}
public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
};

