#include <cstdio>
void print(){
    //printf("hello world \n");
}
int main(){

    if( print == print){
        printf(" print == print"); printf("\n");
    }

    if( print == &print){ // !!! print == &print 成立
        printf(" print == &print"); printf("\n");
    }
    return 0;
}
