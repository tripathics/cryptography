#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define KEY_FILENAME "id_elgamel"
#define USAGE "Usage: ./elgamel [option] inp[,inp2] key_file\n\noption:\n\tGenerate keys\t-g [key_file]\n\tEncrypt file\t-e key_file Plaintext\n\tDecrypt file\t-d key_file Ciphertext1 Ciphertext2\n"

void generate_key(char *key_filename);
void encrypt(char *key_filename, int inp);
void decrypt(char *key_filename, int inp1, int inp2);