#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define KEYLEN ('z' - 'a' + 1)

int get_additive_inverse(int);
int get_multiplicative_inverse(int);
void additive_cipher(FILE *, FILE *, int);
void multiplicative_cipher(FILE *, FILE *, int);
void affine_cipher(FILE *, FILE *, int, int);