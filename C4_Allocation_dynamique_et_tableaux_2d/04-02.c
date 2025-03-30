#include <stdio.h>
#include <stdlib.h>

#define WIDTH 7
#define HEIGHT 6

int main(){
    //1D
    int tab1D[WIDTH*HEIGHT];

    printf("Solution 1D :\n");
    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH;j++){
            tab1D[i*WIDTH+j] = (j+1)*(i+1);
        }
    }

    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH;j++){
            printf("%2d ", tab1D[i * WIDTH + j]);
        }
        printf("\n");
    }

    
    //2D
    int tab2D[HEIGHT][WIDTH];

    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH;j++){
            tab2D[i][j] = (j+1)*(i+1);
        }
    }
    printf("Solution 2D :\n");
    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH;j++){
            printf("%2d ", tab2D[i][j]);
        }
        printf("\n");
    }

    //1D to 2D
    int tab1to2D[WIDTH*HEIGHT];

    printf("Solution 1D to 2D :\n");
    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH;j++){
            tab1to2D[i*WIDTH+j] = (j+1)*(i+1);
        }
    }

    int(*tb1to2)[WIDTH] = (int(*)[WIDTH]) tab1to2D;

    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH;j++){
            printf("%2d ", tb1to2[i][j]);
        }
        printf("\n");
    }

    //2D
    int tab2to1D[HEIGHT][WIDTH];

    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH;j++){
            tab2to1D[i][j] = (j+1)*(i+1);
        }
    }

    int *tb2to1 = (int *)tab2to1D;

    printf("Solution 2D to 1D :\n");
    for(int i = 0; i<HEIGHT; i++){
        for(int j = 0; j<WIDTH;j++){
            printf("%2d ", tb2to1[i*WIDTH + j]);
        }
        printf("\n");
    }



    return 0;
}