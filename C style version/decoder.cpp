#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
using namespace std;
#define NOT_EXIST -1
#define MAX_CURRENT 10000

typedef struct{
    char **seq;
    unsigned short * code;
    int now_size;
    int max_size;
}Dictionary;

void insert_seq(Dictionary * dict, char *seq){
    int i = dict->now_size;
    dict->seq[i] = (char *)malloc(sizeof(char) * (strlen(seq) +1));//×îºóÎª'\0'
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
    putchar('\n');
}

char * get_code(Dictionary *dict , int code){
    if(code < 0 || code>=dict->now_size){
        return NULL;
    }
    int i = code;
    return dict->seq[i];
}

void lzw_decode(unsigned short codes[], int n, Dictionary * dict, string filename){
    ofstream ofile(filename.c_str());
    char prev[MAX_CURRENT];
    char *output;
    output = get_code(dict, codes[0]);
    //printf("%s", output);
    ofile<<output;
    int i;
    for(int i=1;i<n;++i){
        strcpy(prev, output);
        output = get_code(dict, codes[i]);
        if(output==NULL){
            sprintf(prev,"%s%c", prev, prev[0]);
            output = prev;
            //printf("%s", output);
            ofile<<output;
            insert_seq(dict, prev);
        }
        else{
            //printf("%s", output);
            ofile<<output;
            sprintf(prev,"%s%c", prev, output[0]);
            insert_seq(dict, prev);
        }
    }
    //ofile.close();
}

int main(){
    unsigned short codes[100000];
    int id=0;
    string filename = "text_encoded.xiang";
    FILE * fp;
    if((fp=fopen(filename.c_str(),"rb"))==NULL)
    {
        printf("cant open the file");
        exit(0);
    }
    //printf("FILE p offset:%d\n", (int)ftell(fp));
    unsigned short * buffer;
    buffer = (unsigned short *)malloc(sizeof(unsigned short));
    while(fread(buffer, sizeof(unsigned short), 1,fp)){
        if(feof(fp)) break;
        //cout<<* buffer<<endl;
        codes[id++] = * buffer;
    }
    fclose(fp);

    Dictionary D1;
    init_dictionary(&D1, MAX_CURRENT);
    //print_dictionary(&D1);

    lzw_decode(codes, id, &D1, "text_decoded.txt");
    return 0;
}

