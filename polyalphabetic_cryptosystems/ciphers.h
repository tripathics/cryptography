#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define KEYLEN ('z' - 'a' + 1)
#define USAGE "Usage: ./polyalphabetic [cipher] infile keyfile\ncipher:\n\t-c:\tAutokey\n\t-m:\tPlayfair\n\t-v:\tVigenere\n"

void search_pair(int *, int *, int *, int *, char *, char [5][5][2]);
int get_additive_inverse(int);
void playfair_cipher(FILE *, FILE *, FILE *);
void autokey_cipher(FILE *, FILE *, int);
void vigenere_cipher(FILE *, FILE *, char *);