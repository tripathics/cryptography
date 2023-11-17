#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "ciphers.h"

#define USAGE "Usage: ./monoalphabetic [cipher] infile key[,key2]\ncipher:\n\t-c:\tadditive\n\t-m:\tmultiplicative\n\t-a:\taphine\n"
#define KEYLEN ('z' - 'a' + 1)

int main(int argc, char *argv[])
{
    // Ensure proper usage
    char *ciphers = "cma";
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
    if (input_file == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", argv[1]);
        return 2;
    }

    FILE *output_file = fopen("./files/output", "w");
    if (output_file == NULL)
    {
        fprintf(stderr, "Could not open file\n");
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
        affine_cipher(input_file, output_file, key, key2);
        break;
    default:
        break;
    }

    return 0;
}