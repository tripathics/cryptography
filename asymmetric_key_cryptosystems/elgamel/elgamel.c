#include "elgamel.h"
#include <getopt.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    char *options = "ged";
    char option = getopt(argc, argv, options);
    if (option == '?')
    {
        fprintf(stderr, "Invalid option.\n");
        return 1;
    }
    if (getopt(argc, argv, options) != -1)
    {
        fprintf(stderr, "Only one option allowed.\n");
        return 1;
    }
    if (argc < 2)
    {
        fprintf(stderr, USAGE);
        return 1;
    }

    switch (option)
    {
    case 'g':
        if (argc < optind + 1)
            generate_key(KEY_FILENAME);
        else
            generate_key(argv[optind]);
        break;
    case 'e':
        if (argc != optind + 2) {
            fprintf(stderr, USAGE);
            return 1;
        }
        int inp = atoi(argv[optind + 1]);
        encrypt(argv[optind], inp);
        break;
    case 'd':
        if (argc != optind + 3) {
            fprintf(stderr, USAGE);
            return 1;
        }
        int inp1 = atoi(argv[optind + 1]);
        int inp2 = atoi(argv[optind + 2]);
        decrypt(argv[optind], inp1, inp2);
        break;
    default:
        break;
    }
}