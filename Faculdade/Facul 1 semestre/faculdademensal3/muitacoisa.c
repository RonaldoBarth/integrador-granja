#include <stdio.h>
#include <string.h>

int main() {
    char x[] = "Hello world";
    char y[20], z[6];

    printf("%s%s\n%s%s\n", "x: ", x, "y: ", strcpy(y, x));

    strncpy(z, x, 5);
    z[5] = '\0';

    printf("z: %s\n", z);

    return 0;
}

/*strcat(): concatena s2 a s1 (desafio utilizar a funcao)
strncat(): concatena no maximo n caracteres de s2 a s1 (desafio utilizar a funcao)5 */

