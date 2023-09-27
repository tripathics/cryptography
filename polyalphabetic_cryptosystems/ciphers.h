#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define KEYLEN ('z' - 'a' + 1)

int playfair_cipher(FILE *, FILE *, FILE *);
void search_pair(int *, int *, int *, int *, char *, char [5][5][2]);

int autokey_cipher(FILE *, FILE *, int);
int vigenere_cipher(FILE *, FILE *, char *);