#include "ciphers.h"
#include <getopt.h>

#define USAGE "Usage: ./monoalphabetic [cipher] infile key[,key2]\ncipher:\n\t-c:\tadditive\n\t-m:\tmultiplicative\n\t-a:\taffine encrypt\n\t-d:\taffine decrypt\n"

int main(int argc, char *argv[])
{
    // Ensure proper usage
    char *ciphers = "cmad";
    char cipher = getopt(argc, argv, ciphers);
    if (cipher == '?')
    {
        fprintf(stderr, "Invalid cipher.\n");
        return 1;
    }
    if (getopt(argc, argv, ciphers) != -1)
    {
        fprintf(stderr, "Only one cipher allowed.\n");
        return 1;
    }
    if (argc != optind + 2 && argc != optind + 3)
    {
        fprintf(stderr, USAGE);
        return 1;
    }

    int key = atoi(argv[optind + 1]), key2 = 1;
    if (argc == optind + 3)
        key2 = atoi(argv[optind + 2]);
    if (key <= 0 && key2 <= 0)
    {
        fprintf(stderr, "Enter a valid key (greater than 0)\n");
        return 2;
    }

    FILE *input_file = fopen(argv[optind], "r");
    FILE *output_file = fopen("./files/output", "w");
    if (input_file == NULL || output_file == NULL)
    {
        fprintf(stderr, "Could not open input/output file(s)\n");
        return 2;
    }
    
    switch (cipher)
    {
    case 'c':
        additive_cipher(input_file, output_file, key);
        break;
    case 'm':
        multiplicative_cipher(input_file, output_file, key);
        break;
    case 'a':
        affine_cipher_encrypt(input_file, output_file, key, key2);
        break;
    case 'd':
        affine_cipher_decrypt(input_file, output_file, key, key2);
        break;
    default:
        break;
    }
    fclose(input_file);
    fclose(output_file);

    return 0;
}