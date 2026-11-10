#include <stdio.h>
#include <ctype.h> 
#include <string.h>

int main() {
    char texto[] = "Whatman";
    
    for (int i = 0; texto[i] != '\0'; i++) {
        texto[i] = toupper(texto[i]);
    }
    
    printf("String em maiusculo: %s\n", texto);
    return 0;
}