#include <stdio.h>

int main(){
    int a = 26;
    if(a%3 == 0){
        if(a%13==0){
            printf("%d est divisible par 3 et 13", a);
        }else{
        printf("%d est divisible par 3", a);
        }
    }else{
        if(a%13==0){
            printf("%d est divisible par 13",a);
        }
    }
    return 0;
}