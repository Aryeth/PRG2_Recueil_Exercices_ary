#include <stdio.h>
#include <stdlib.h>

int main(){
    int tmp = 0;
    int count = 1;
    int* tablo = malloc(sizeof(int));

    while(1){

        printf("Entrez un entier positif (une lettre pour finir): ");
        //cond de sortie
        if (scanf("%d", &tmp) != 1) {
            break;
        }
        //scanf flush        
        while (getchar() != '\n');
        ++count;

        //reallocation ++
        int* temp = realloc(tablo,count * sizeof(int));
        //verif qu'il reste de la mémoire
        if(temp != NULL){
            tablo = temp;
        }else{
            printf("memory allocation error");
            free(tablo);
            return 1;
        }

        tablo[count - 1] = tmp; // Store the new value
        
    }
    //retour
    printf("Vous avez entré:");
    for (int i = 0; i < count; i++) {
        printf("%d ", tablo[i]);
    }
    printf("\n");
    
    free(tablo);

    return 0;
}