#include <stdio.h>
#include <stdlib.h>

int main() {
    int tmp = 0;
    int count = 0;
    int* tablo = NULL; // Start with NULL

    while (1) {
        printf("Entrez un entier positif (une lettre pour finir): ");

        if (scanf("%d", &tmp) != 1) {
            while (getchar() != '\n');
            break; // Exit on non-integer input
        }

        while (getchar() != '\n'); // Clear input buffer

        

        // Reallocate memory
        int* temp = realloc(tablo, count+1 * sizeof(int));
        if (temp != NULL) {
            tablo = temp;
        } else {
            printf("Erreur d'allocation mémoire\n");
            free(tablo);
            return 1;
        }

        tablo[count] = tmp; // Store the value correctly
        // Increase count before realloc
        count++;
    }

    // Print the stored numbers correctly
    printf("Vous avez entre: ");
    for (int i = 0; i < count; i++) {
        printf("%d ", tablo[i]);
    }
    printf("\n");

    free(tablo);
    return 0;
}
