#include <stdio.h>
#include <stdlib.h>

// Fonction pour lire un entier positif
size_t lire_size_t(const char *message) {
    size_t value = 0;

    while(1) {
        printf("%s", message);
        if(scanf("%zu", &value) == 1 && value > 0) {
            return value;
        }
        else {
            printf("Erreur de saisie. Entrez un entier positif : \n");
        }
    }
    
}

int main() {
    size_t lignes = 100;
    size_t colonnes = 10;
    int *col = malloc(sizeof(int));
    int *tmpcol = malloc(sizeof(int));
    int **tab = malloc(lignes * sizeof(int *));;
    int **tmpTab = malloc(lignes* sizeof(int *));;
    size_t capacity = 10;
    int n = 1;
    
    printf("[!] Le programme ne peut pas afficher des nombres à plus de 3 chiffres\n");
    while(1){
        lignes = lire_size_t("[?] Entrez le nombre de lignes: ");
        colonnes = lire_size_t("[?] Entrez le nombre de colonnes: \n");
        if(lignes*colonnes>=1000){
            printf("taille trop grande, recommencez.");
        }else{
            break;
        }
    }
    
    printf("[i] Table de multiplication %d x %d", lignes, colonnes);

    tmpcol = realloc(col, colonnes*sizeof(int));
    if(tmpcol == NULL){
        printf("Erreur d'allocation de mémoire");
        return 1;
    }
    col = tmpcol;

    tmpTab = realloc(tab, lignes*sizeof(int *));
    if(tmpTab == NULL){
        printf("Erreur d'allocation de mémoire");
        return 1;
    }

    tab = tmpTab;

    for (size_t i = 0; i < lignes; i++) {
        tab[i] = malloc(colonnes * sizeof(int));
        if (tab[i] == NULL) {
            printf("Erreur d'allocation de mémoire\n");
            return 1;
        }
    }

    for(size_t i = 0; i<lignes; i++){
        n = 1;
        for(size_t j = 0; j<colonnes; j++){
            tab[i][j] = (i + 1) * (j + 1);
        }
    }

    printf("\nTable de multiplication :\n");
    for (size_t i = 0; i < lignes; i++) {
        for (size_t j = 0; j < colonnes; j++) {
            printf("%4d", tab[i][j]); // Formatage propre
        }
        printf("\n");
    }
    
    for (size_t i = 0; i < lignes; i++) {
        free(tab[i]);
    }
    free(tab);
    free(col);

    return 0;
}