#include "rsa.h"
#include <getopt.h>

#define USAGE "Usage: ./rsa [option] [files]\n\noption:\n\tGenerate keys\t-g [key_file]\n\tEncrypt file\t-e [inp, out, pub_key_file]\n\tDecrypt file\t-d [inp, out, pub_key_file, prv_key_file]\n"

int main(int argc, char *argv[])
{
    char *options = "ged";
    char option = getopt(argc, argv, options);
    if (option == '?')
    {
        fprintf(stderr, "Invalid option.\n");
        return 1;
    }

    // Ensure only one option
    if (getopt(argc, argv, options) != -1)
    {
        fprintf(stderr, "Only one option allowed.\n");
        return 2;
    }

    // Ensure proper usage
    if (argc < 2)
    {
        fprintf(stderr, USAGE);
        return 3;
    }

    switch (option)
    {
    case 'g':
        if (argc != optind + 1)
            generate_key(KEY_FILENAME);
        else
            generate_key(argv[2]);
        break;
    case 'e':
        if (argc != optind + 3)
        {
            fprintf(stderr, USAGE);
            return 3;
        }
        encrypt(argv[2], argv[3], argv[4]);
        break;
    case 'd':
        if (argc != optind + 4)
        {
            fprintf(stderr, USAGE);
            return 3;
        }
        decrypt(argv[2], argv[3], argv[4], argv[5]);
        break;
    default:
        fprintf(stderr, USAGE);
        return 4;
    }
    return 0;
}