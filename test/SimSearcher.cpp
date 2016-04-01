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
    this->qq = q;
    ifstream fin(filename);
    string line;
    int id = 0;
    if(!fin){cout<<"could not open file!";return FAILURE;}
    //invertList.rehash(BUCKET_COUNT);
    smin = 1023;
    while(getline(fin,line)){
        records.push_back(line);
        updateList(line,id);
        int word_size = updateWordList(line,id);
        word_counter.push_back(word_size);
        if(smin > word_size)smin = word_size;
        id++;
    }

    recordSize = records.size();
//    for(unordered_map<string,vector<int> >::iterator it=invertList.begin();it!=invertList.end();it++){
//        listSize.push_back(pair<string,int>(it->first,it->second.size()));
//    }
//
//    sort(listSize.begin(),listSize.end(),asdf);

//    for(int i = 0;i<listSize.size();i++){
//        position[listSize[i].first] = i;
//    }


    fin.close();
    return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
    result.clear();
    string q = query;
    int len1 = q.length();
    vector<string> words;
    split(q,' ',words);
    //words 去重
    sort(words.begin(),words.end());
    words.erase(unique(words.begin(),words.end()),words.end());

    vector<int> candidate;
    int word_size = words.size();
    int T = my_max(threshold*word_size,(smin+word_size)*threshold/(threshold+1));
    scanCount(this->word_invertList,words,T,candidate);
    sort(candidate.begin(),candidate.end());
    int size = candidate.size();
    for(int i = 0;i<size;i++){
        int id = candidate[i];
        double jaccard = counters[id]/(word_counter[id]+word_size);
        if(jaccard>=threshold) result.push_back(pair<unsigned,double>(id,jaccard));
    }
    return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
    result.clear();
    int thrs = threshold;
    string q = query;

    vector<string> grams;
    splitIntoGram(q,qq,grams);
    //grams 去重
    sort(grams.begin(),grams.end());
    grams.erase(unique(grams.begin(),grams.end()),grams.end());

    int len1 = q.length();
    int T = len1-qq+1-thrs*qq;
    if(T>=1){
        vector<int>candidate;
        scanCount(this->invertList,grams,T,candidate);
        //divideSkip(grams,T,candidate);
        int size = candidate.size();
        sort(candidate.begin(),candidate.end());
        for(int i = 0;i<size;i++){
            int id = candidate[i];
            int len2 = records[id].length();
            if(abs(len1-len2)>thrs) continue;
            int distance = 0;
            if(len1<len2){
                distance = calED(q,records[id],thrs);
            }
            else{
                distance = calED(records[id],q,thrs);
            }
            if(distance == -1) continue;
            if(distance<=thrs) result.push_back(pair<unsigned,unsigned>(id,distance));
        }
    }
    else{
        for(int i = 0;i<recordSize;i++){
            int len2 = records[i].length();
            if(abs(len1-len2)>thrs) continue;
            int distance = 0;
            if(len1<len2){
                distance = calED(q,records[i],thrs);
            }
            else{
                distance = calED(records[i],q,thrs);
            }
            if(distance == -1) continue;
            if(distance<=thrs) result.push_back(pair<unsigned,unsigned>(i,distance));
        }
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
        if(count == end-start+1) return -1;
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
//        sort(grams.begin(),grams.end());
//        grams.erase(unique(grams.begin(),grams.end()),grams.end());
        int gsize = grams.size();
        for(int i = 0;i<gsize;i++){
            invertList[grams[i]].push_back(id);
        }
    }
}

int SimSearcher::updateWordList(const string& s,int id){
    vector<string> words;
    split(s,' ',words);
    sort(words.begin(),words.end());
    words.erase(unique(words.begin(),words.end()),words.end());
    int size = words.size();
    for(int i = 0;i<size;i++){
        word_invertList[words[i]].push_back(id);
    }
    return size;
}


void SimSearcher::scanCount(unordered_map<string,vector<int> >&invertList,const vector<string>& grams,int T,vector<int>& candidate){
    int gsize = grams.size();
    for(int i = 0;i<recordSize;i++) counters[i] = 0;
    unordered_map<string,vector<int> >::iterator it;
    for(int i = 0;i<gsize;i++){
        it = invertList.find(grams[i]);
        if(it!=invertList.end()){
            for(int j =0;j<it->second.size();j++){
                counters[it->second[j]]++;
                if(counters[it->second[j]] == T) candidate.push_back(it->second[j]);
            }
        }
    }
}

void SimSearcher::divideSkip(const vector<string>& grams,int T,vector<int>& candidate){
    vector<vector<int> > selected;
    for(int i = 0;i<grams.size();i++){
        if(invertList.find(grams[i])!=invertList.end())
            selected.push_back(invertList[grams[i]]);
    }
    sort(selected.begin(),selected.end(),comp);
    vector<vector<int> >::iterator tmp =selected.begin();
    int L = T/2;
    tmp=tmp+L;
    vector<vector<int> >::iterator border = tmp;
    vector<vector<int> >shortLists(border,selected.end());
    unordered_map<int,int> records;  //第一个int 是id，第二个是出现的次数
    mergeSkip(T-L,shortLists,records);
    for(unordered_map<int,int>::iterator it = records.begin();it!=records.end();it++){
        int count = 0;
        for(tmp = selected.begin();tmp!=border;tmp++){
            if(binary_search(tmp->begin(),tmp->end(),it->first)){
                count++;
            }
        }
        if((it->second+count)>=T){candidate.push_back(it->first);}
    }

}

void SimSearcher::mergeSkip(int T,vector<vector<int>>&lists,unordered_map<int,int>&result){
    cout<<"start"<<endl;
    int len = lists.size();
    vector<vector<int>::iterator >pointers;
    vector<int>popedList;
    vector<pair<int,int> > heap;  // 第一个int代表list的编号，第二个int代表当前首元素id
    for(int i  =0;i<len;i++){
        heap.push_back(pair<int,int>(i,lists[i][0]));
        pointers.push_back(lists[i].begin());    // 初始化 pointer
    }
    make_heap(heap.begin(),heap.end(),heap_comp);//建立最小堆
    while(!heap.empty()){
        popedList.clear();
        int n = 0;
        int t = heap.front().second;
        while(t == heap.front().second){
            n++;
            popedList.push_back(heap.front().first);
            pop_heap(heap.begin(),heap.end(),heap_comp);
            heap.pop_back();
        }
        if(n>=T){
            result[t] = n;
            for(int j = 0;j<popedList.size();j++){
                int listId = popedList[j];
                pointers[listId]++;
                if(pointers[listId]!=lists[listId].end()){
                    heap.push_back(pair<int,int>(listId,*pointers[listId]));
                    push_heap(heap.begin(),heap.end(),heap_comp);
                }
            }
        }
        else{
            for(int j = 0;j<T-1-n;j++){
                popedList.push_back(heap.front().first);
                pop_heap(heap.begin(),heap.end(),heap_comp);
                heap.pop_back();
            }
            int tp = heap.front().second;
            for(int i = 0;i<popedList.size();i++){
                int listId = popedList[i];
                pointers[listId] = lower_bound(lists[listId].begin(),lists[listId].end(),tp);
                if(pointers[listId]!=lists[listId].end()){
                    heap.push_back(pair<int,int>(listId,*pointers[listId]));
                    push_heap(heap.begin(),heap.end(),heap_comp);
                }
            }
        }
    }
}

int SimSearcher::split(const string& str,char s,vector<string>&words){
    int len = str.length();
    int start = 0;
    for(int i = 0;i<len;i++){
        if(str[i] == s){
            words.push_back(str.substr(start,i-start));
            while(str[i+1] == s){
                i++;
            }
        }
    }
    words.push_back(str.substr(start,len-start));
}

