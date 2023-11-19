#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define KEY_FILENAME "id_rsa"
#define EXPONENT_MAX 1000

typedef long long int lli;
typedef uint8_t BYTE;

lli mod_pow(lli a, lli b, lli c);
lli gcd(lli a, lli b);
lli inverse(lli r1, lli r2);
lli rand_exponent(lli phi);
void generate_key(char *key_filename);
void encrypt(char *input_filename, char *output_filename, char *public_key_filename);
void decrypt(char *input_filename, char *output_filename, char *public_key_filename, char *private_key_filename);
