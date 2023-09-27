#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define KEYLEN ('z' - 'a' + 1)

int get_additive_inverse(int);
int get_multiplicative_inverse(int);
int additive_cipher(FILE *, FILE *, int);
int multiplicative_cipher(FILE *, FILE *, int);
int affine_cipher(FILE *, FILE *, int, int);