// test lambda capture
//
/* author: Rainboy email: rainboylvx@qq.com  time: 2021年 06月 29日 星期二 15:55:05 CST */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int maxn = 1e6+5,maxe = 1e6+5; //点与边的数量

int n,m;

void DO(int * p,int s = 0){
    if(s >= 2 ) return;
    printf("in functon do \n");
    printf("%x\n",p);
    auto f = [&](){
        printf("%x\n",p);
        DO(p,s+1);
    };
    f();
}

int main(){
    int cnt = 1;
    int * pc = &cnt;
    printf("%x\n",pc);
    auto f = [&](){
        printf("%x\n",&cnt);
        printf("%x\n",pc);
    };
    f();
    DO(pc);
    printf("============\n");
    return 0;
}
