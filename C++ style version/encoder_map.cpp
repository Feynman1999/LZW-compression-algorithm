#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include<map>
using namespace std;
#define NOT_EXIST -1

typedef struct{
    map<string, unsigned short> mp;
}Dictionary;

void insert_seq(Dictionary * dict, string seq){
    int now_size = dict->mp.size();
    assert(now_size<=(1<<16-1));//limit 16 digit
    dict->mp[seq] = now_size;
}

void init_dictionary(Dictionary * dict){
    for(int i=1;i<128;++i){
        insert_seq(dict, string(1,char(i)));
    }
}

void print_dictionary(Dictionary * dict){
    for(int i=0;i<40;++i) printf("*");
    putchar('\n');
    printf(" Code          Sequence\n");
    for(int i=0;i<40;++i) printf("*");
    putchar('\n');
    for(auto k:dict->mp){
        printf("%5d%13c", k.second, ' ');
        printf("%s\n", k.first.c_str());
    }
    for(int i=0;i<40;++i) printf("*");
    putchar('\n');
}

int find_seq(Dictionary * dict, string seq){
    if(dict->mp.count(seq)) return dict->mp[seq];
    return NOT_EXIST;
}

void lzw_encode(string text, Dictionary * dict, string filename){
    unsigned short buffer[100000];
    double encode_time;
    clock_t start = clock();
    string current;
    int buffer_id=0;
    int len = text.length();
    char next;
    int code;
    int i=0;
    while(i<len){
        current+=text[i++];
        while(find_seq(dict, current)!=NOT_EXIST  && i<len){
            current+=text[i++];
        }
        //可能是current找不到 或者 i==len
        if(find_seq(dict, current)==NOT_EXIST){//没找到  也有可能i=len
            //cout<<"插入: "<<current<<" "<<dict->now_size<<endl;
            insert_seq(dict, current);//插入字典
            assert(i>0);
            next = text[--i];
            current = current.substr(0, current.length()-1);
            //cout<<current<<" ";
            code = find_seq(dict, current);
            //cout<<code<<endl;
            buffer[buffer_id++]=code;
            assert(code!=NOT_EXIST);//一定存在
        }
        else{//找到了  但到最后了    next 没了
            code = find_seq(dict, current);
            //cout<<code<<endl;
            buffer[buffer_id++]=code;
            assert(code!=NOT_EXIST);//一定存在
            break;
        }
        current="";
    }
    clock_t finish = clock();
    encode_time = (double)(finish-start)/CLOCKS_PER_SEC;
    cout<<"encode_time: "<<encode_time<<endl;
    FILE * pFile;
    if((pFile = fopen (filename.c_str(), "wb"))==NULL)
    {
        printf("can't open the file");
        exit(0);
    }
    fwrite(buffer, sizeof(unsigned short), buffer_id, pFile);
    fclose (pFile);
}

int main(){
    freopen("text.txt","r",stdin);
    string text;
    char tp;
    while(tp=getchar()){
        if(tp==EOF) break;
        text+=tp;
    }
    //cout<<text.length()<<endl;
    Dictionary D1;
    init_dictionary(&D1);
    //print_dictionary(&D1);
    string filename = "text_encoded.xiang";
    lzw_encode(text, &D1 , filename);
    return 0;
}

