#include <stdio.h>

void copy1(char *, const char *);
void copy2(char *, const char *);

int main() {
    char string1[10], *string2 = "Hello!", string3[10], string4[] = "Good bye!";
}
    copy1(string1, string2);
    printf("string1 = %s\n", string1);

    copy1(string3, string4);
    printf("string3 = %s\n", string4);

    return 0;
}

void copy1(char *str)