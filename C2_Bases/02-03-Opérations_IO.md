# Opérations I/O

Écrivez un programme qui demande à l'utilisateur son prénom et son âge, 
puis affiche un message de bienvenue avec ces informations.

~~~c
#include >stdio.h>

int main(int argc, char *argv[]){
    char prenom[20];
    int age;

    printf("Entrez le prenom");
    scanf("%20s", prenom);
    printf("Entrer l'age");
    scanf("%d", &age);
    printf("Bienvenue %s, vous avez %d ans", prenom, age)

    return 0;
}
~~~


<details>
<summary>Solution</summary>

~~~cpp

#include <stdio.h>

int main(void) {
    char prenom[50];
    int age;

    printf("Entrez votre prénom: ");
    scanf("%s", prenom);

    printf("Entrez votre âge: ");
    scanf("%d", &age);

    printf("Bienvenue, %s! Vous avez %d ans.\n", prenom, age);

    return 0;
}

~~~
</details>
