#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "ciphers.h"

int main(int argc, char *argv[]) {
    // Ensure proper usage
    char *ciphers = "apv";
    char cipher = getopt(argc, argv, ciphers);
    if (cipher == '?')
    {
        fprintf(stderr, "Invalid cipher.\n");
        return 1;
    }
    if (getopt(argc, argv, ciphers) != -1)
    {
        fprintf(stderr, "Only one cipher allowed.\n");
        return 2;
    }
    if (argc != optind + 2)
    {
        fprintf(stderr, USAGE);
        return 3;
    }

    FILE *input_file = fopen(argv[optind], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Could not open file %s\n", argv[optind]);
        return 2;
    }

    FILE *output_file = fopen("./files/output", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Could not open file\n");
        return 2;
    }

    switch (cipher)
    {
    case 'a':
        int key = atoi(argv[optind+1]);
        if (key <= 0) {
            fprintf(stderr, "Key must be a positive integer\n");
            return 2;
        }
        autokey_cipher(input_file, output_file, key);
        break;
    case 'p':
        FILE *key_file = fopen(argv[optind+1], "r");
        if (key_file == NULL) {
            fprintf(stderr, "Could not open key file %s\n", argv[optind+1]);
            return 2;
        }
        playfair_cipher(input_file, output_file, key_file);
        break;
    case 'v':
        char *key_str = argv[optind+1];
        for (int i = 0; key_str[i] != '\0'; i++) {
            if (!isalpha(key_str[i])) {
                fprintf(stderr, "Key must be alphabetic\n");
                return 2;
            }
        }
        vigenere_cipher(input_file, output_file, key_str);
        break;
    default:
        break;
    }
    return 0;
}