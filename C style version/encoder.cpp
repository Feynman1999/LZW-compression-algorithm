#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
using namespace std;
#define NOT_EXIST -1
#define MAX_CURRENT 100000

typedef struct{
    char **seq;
    unsigned short * code;
    int now_size;
    int max_size;
}Dictionary;

void insert_seq(Dictionary * dict, char *seq){
    int i = dict->now_size;
    dict->seq[i] = (char *)malloc(sizeof(char) * (strlen(seq) +1));//最后为'\0'
    strcpy(dict->seq[i], seq);
    assert(i<=(1<<16-1));//limit 16 digit
    dict->code[i] = i;
    dict->now_size++;
}

void init_dictionary(Dictionary * dict, int max_size){
    dict->max_size = max_size;
    dict->now_size = 0;
    dict->seq = (char **)malloc(sizeof(char *) * max_size);
    dict->code = (unsigned short *)malloc(sizeof(unsigned short) * max_size);
    char seq[2];
    memset(seq,0,sizeof(seq));
    for(int i=1;i<128;++i){
        seq[0]=i;
        insert_seq(dict, seq);
    }
}

void print_dictionary(Dictionary * dict){
    for(int i=0;i<40;++i) printf("*");
    putchar('\n');
    printf(" Code          Sequence\n");
    for(int i=0;i<40;++i) printf("*");
    putchar('\n');
    for(int i=0;i<dict->now_size;++i){
        printf("%5d%13c", dict->code[i], ' ');
        printf("%s\n", dict->seq[i]);
    }
    for(int i=0;i<40;++i) printf("*");
    putchar('\n');
}

int find_seq(Dictionary * dict, char *seq){
    for(int i=0;i<dict->now_size;++i){
        if(strcmp(seq, dict->seq[i])==0) return dict->code[i];
    }
    return NOT_EXIST;
}

void lzw_encode(char *text, Dictionary * dict, string filename){
    unsigned short buffer[100000];
    int buffer_id=0;
    int len = strlen(text);
    char current[MAX_CURRENT];
    memset(current,0,sizeof(current));
    char next;
    int code;
    int i=0;
    int j=0;//current id
    while(i<len){
        current[j++] = text[i++];
        while(find_seq(dict, current)!=NOT_EXIST  && i<len){
            current[j++] = text[i++];
        }
        //可能是current找不到 或者 i==len
        if(find_seq(dict, current)==NOT_EXIST){//没找到  也有可能i=len
            //cout<<"插入: "<<current<<" "<<dict->now_size<<endl;
            insert_seq(dict, current);//插入字典
            assert(i>0);
            next = text[--i];
            current[j-1]=0;
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
        while(j>0){
            j--;
            current[j]=0;
        }
    }
    FILE * pFile;
    if((pFile = fopen (filename.c_str(), "wb"))==NULL)
    {
        printf("can't open the file");
        exit(0);
    }
    fwrite(buffer, sizeof(unsigned short), buffer_id, pFile);
    fclose (pFile);
}
char text[1000000];
int main(){
    freopen("text.txt","r",stdin);
    string text2;
    char tp;
    while(tp=getchar()){
        if(tp==EOF) break;
        text2+=tp;
    }
    strcpy(text, text2.c_str());
    Dictionary D1;
    init_dictionary(&D1, MAX_CURRENT);
    //print_dictionary(&D1);
    string filename = "text_encoded.xiang";
    lzw_encode(text, &D1 , filename);
    return 0;
}

