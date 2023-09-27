#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "ciphers.h"

#define KEYLEN ('z' - 'a' + 1)

int get_inverse(int);

int main(int argc, char *argv[]) {
    // Define allowable ciphers
    char *ciphers = "cma";

    // Get filter flag and check validity
    char cipher = getopt(argc, argv, ciphers);
    if (cipher == '?')
    {
        fprintf(stderr, "Invalid cipher.\n");
        return 1;
    }

    // Ensure only one filter
    if (getopt(argc, argv, ciphers) != -1)
    {
        fprintf(stderr, "Only one cipher allowed.\n");
        return 2;
    }

    // Ensure proper usage
    if (argc != optind + 2 && argc != optind + 3)
    {
        fprintf(stderr, "Usage: ./monoalphabetic [cipher] infile key[,key2]\ncipher: -c -m -a\n-c:\tadditive\n-m:\tmultiplicative\n-a:\taphine\n");
        return 3;
    }

    int key = atoi(argv[optind + 1]), key2 = 1;
    if (argc == optind + 3) key2 = atoi(argv[optind + 2]);
    if (key <= 0 && key2 <= 0) {
        fprintf(stderr, "Enter a valid key (greater than 0)\n");
        return 2;
    }

    FILE *input_file = fopen(argv[optind], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Could not open input file %s\n", argv[1]);
        return 2;
    }

    FILE *output_file = fopen("./files/output", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Could not open output file\n");
        return 2;
    }

    int return_value = 0;

    switch (cipher)
    {
    case 'c':
        return_value = additive_cipher(input_file, output_file, key);
        break;
    case 'm':
        return_value = multiplicative_cipher(input_file, output_file, key);
        break;
    case 'a':
        return_value = affine_cipher(input_file, output_file, key, key2);
        break;
    default:
        break;
    }

    return return_value;
}