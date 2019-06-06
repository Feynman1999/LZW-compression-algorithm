#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<vector>
#include<map>
#include<fstream>
#include<time.h>
using namespace std;
#define NOT_EXIST -1

typedef struct{
    map<string, unsigned short> mp;
    vector<string> hash_back;
}Dictionary;

void insert_seq(Dictionary * dict, string seq){
    int now_size = dict->mp.size();
    assert(now_size<=(1<<16-1));//limit 16 digit
    dict->mp[seq] = now_size;
    dict->hash_back.push_back(seq);
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

string get_code(Dictionary *dict , int code){
    if(code < 0 || code>=dict->mp.size()){
        return "";
    }
    return dict->hash_back[code];
}

void lzw_decode(vector<unsigned short> codes, Dictionary * dict, string filename){
    ofstream ofile(filename.c_str());
    double decode_time;
    clock_t start = clock();
    string prev;
    string output;
    output = get_code(dict, codes[0]);
    //cout<<output;
    ofile<<output;
    int i;
    for(int i=1;i<codes.size();++i){
        prev = output;
        output = get_code(dict, codes[i]);
        if(output==""){
            output = prev + prev[0];
            //cout<<output;
            ofile<<output;
            insert_seq(dict, output);
        }
        else{
            //cout<<output;
            ofile<<output;
            prev = prev + output[0];
            insert_seq(dict, prev);
        }
    }
    clock_t finish = clock();
    decode_time = (double)(finish-start)/CLOCKS_PER_SEC;
    cout<<"decode_time: "<<decode_time<<endl;
    ofile.close();
}

int main(){
    vector<unsigned short> codes;
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
        codes.push_back(* buffer);
    }
    fclose(fp);

    Dictionary D1;
    init_dictionary(&D1);
    //print_dictionary(&D1);

    lzw_decode(codes, &D1, "text_decoded.txt");
    return 0;
}

