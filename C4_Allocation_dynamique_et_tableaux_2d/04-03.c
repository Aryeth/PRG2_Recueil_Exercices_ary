#include <stdio.h>
#include <stdlib.h>

int main(){
    int width, height;
    int** tab2d;

    while (1)
    {
        printf("entrez le nb de lignes :");
        if(scanf("%d",&height)!= 1){
            while (getchar() != "\n");
            printf("Erreur de saisie.");
            continue;
        }
        printf("\nentrez le nb de colonnes :");
        if(scanf("%d",&width)!= 1){
            while (getchar() != "\n");
            printf("Erreur de saisie.");
            continue;
        }
        break;
    }
    
    int** tmp1 = calloc(height, sizeof(int *));
    if(tmp1 ==NULL){
        printf("Erreur d'allocation mémoire");
        return 1;
    }
    tab2d = tmp1;

    for(size_t i = 0; i<height; i++){
        int* tmp2 = calloc(width, sizeof(int));
        if(tmp2 == NULL){
            printf("Erreur d'allocation mémoire");
            return 1;
        }
        tab2d[i] = tmp2;
    }

    for(int i = 0; i<height; i++){
        for(int j = 0; j<width;j++){
            tab2d[i][j] = (i+1)*(j+1);
        }
    }

    printf("Solution :\n");
    for(int i = 0; i<height; i++){
        for(int j = 0; j<width;j++){
            printf("%2d ", tab2d[i][j]);
        }
        printf("\n");
    }

    free(tab2d);

    for(size_t i = 0; i<width; i++){
        free(tab2d[i]);
    }

    return 0;
}