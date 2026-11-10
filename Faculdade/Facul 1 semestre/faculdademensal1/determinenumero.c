#include <stdio.h>

int main(){
    int x;

    printf("Coloque seu numero:");
    scanf("%i", &x);

    if (x == 0)
        printf("Seu numero eh 0."); 
    if (x >= 0) 
        printf("Seu numero e positivo"); 
    if  (x <= 0)   
        printf("Seu numero e negativo"); 

    return 0;

}