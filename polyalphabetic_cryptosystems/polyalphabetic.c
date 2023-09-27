#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "ciphers.h"

#define KEYLEN ('z' - 'a' + 1)

int get_inverse(int);

int main(int argc, char *argv[]) {
    // Define allowable ciphers
    char *ciphers = "apv";

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
    if (argc != optind + 2)
    {
        fprintf(stderr, "Usage: ./polyalphabetic [cipher] infile keyfile\ncipher: -a -p -v\n-c:\tAutokey\n-m:\tPlayfair\n-a:\tVigenere\n");
        return 3;
    }

    FILE *input_file = fopen(argv[optind], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Could not open input file %s\n", argv[optind]);
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
    case 'a':
        int key = atoi(argv[optind+1]);
        if (key <= 0) {
            fprintf(stderr, "Key must be a positive integer\n");
            return 2;
        }
        return_value = autokey_cipher(input_file, output_file, key);
        break;
    case 'p':
        FILE *key_file = fopen(argv[optind+1], "r");
        if (key_file == NULL) {
            fprintf(stderr, "Could not open key file %s\n", argv[optind+1]);
            return 2;
        }
        return_value = playfair_cipher(input_file, output_file, key_file);
        break;
    case 'v':
        char *key_str = argv[optind+1];
        for (int i = 0; key_str[i] != '\0'; i++) {
            if (!isalpha(key_str[i])) {
                fprintf(stderr, "Key must be alphabetic\n");
                return 2;
            }
        }
        return_value = vigenere_cipher(input_file, output_file, key_str);
        break;
    default:
        break;
    }
    return return_value;
}