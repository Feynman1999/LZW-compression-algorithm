#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<vector>
#include<map>
#include<time.h>
#include<fstream>
using namespace std;

int n = 4e4;

int main()
{
    srand(time(0));
    freopen("text.txt","w",stdout);
    for(int i=0;i<n;++i){
        char c = rand()%127+1;
        while(c==0x1A){
            c = rand()%127+1;
        }
        cout<<c;
    }
    return 0;
}
