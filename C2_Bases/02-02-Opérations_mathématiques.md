# Opérations mathématiques

Écrivez un programme qui effectue les opérations suivantes

Indications
-----------
Déclarez les nombres dans des variables locales et stockez le résultat dans une variable globale.

- Addition de deux nombres entiers.
- Soustraction de deux nombres réels.
- Multiplication de deux nombres entiers.
- Division de deux nombres réels.

~~~cpp
unsigned int resultatInt;
float resultatSub;
float resultatMult;
float resultatDiv;
// on peut se contenter d'un float pour les trois 
// mais j'assume 3 variables différentes.

int main(){
    resultatInt = 2 + 3;
    resultatSub = 2.5 - 3;
    resultatMult = 2.5 * 3;
    resultatDiv = 2.5/3;

}

~~~

<details>
<summary>Solution</summary>

~~~cpp

int res_int;
float res_float;

int main() {

    int a = 5, b = 3;
    float c = 7.5, d = 2.5;

    res_int = a + b;
    res_int = c - d;

    res_float = a * b;
    res_float = c / d;

    return 0;
}

~~~
</details>
