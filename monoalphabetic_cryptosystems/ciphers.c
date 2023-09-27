#include "ciphers.h"

int get_additive_inverse(int k)
{
    return KEYLEN - k % KEYLEN;
}

int get_multiplicative_inverse(int k)
{
    for (int i = 1; i < KEYLEN; i++)
    {
        if (k * i % KEYLEN == 1)
            return i;
    }
    return -1;
}

int additive_cipher(FILE *input_file, FILE *output_file, int key)
{
    char p;
    int key_inverse = get_additive_inverse(key);
    while ((p = fgetc(input_file)) != EOF)
    {
        if (!isalpha(p))
            continue;

        // make sure plain text is small
        p = tolower(p);
        char C = (p + key - 'a') % KEYLEN + 'a';
        fputc(C, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    fprintf(stdout, "Ciphertext written to files/output\n");
    fprintf(stdout, "Decrypt with %i\n", key_inverse);

    return 0;
}

int multiplicative_cipher(FILE *input_file, FILE *output_file, int key)
{
    int key_inverse = get_multiplicative_inverse(key);
    if (key_inverse == -1)
    {
        fprintf(stderr, "The provided key inverse doesn't exist under mod %i!\n", KEYLEN);
        return 3;
    }

    char p;
    while ((p = fgetc(input_file)) != EOF)
    {
        if (!isalpha(p))
            continue;

        // make sure plain text is small
        p = tolower(p);
        char C = ((p - 'a') * key) % KEYLEN + 'a';
        fputc(C, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    fprintf(stdout, "Ciphertext written to files/output\n");
    fprintf(stdout, "Decrypt with: %i\n", key_inverse);
    return 0;
}

int affine_cipher(FILE *input_file, FILE *output_file, int prod_key, int sum_key)
{
    int prod_key_inverse = get_multiplicative_inverse(prod_key);
    if (prod_key_inverse == -1)
    {
        fprintf(stderr, "The provided key inverse doesn't exist under mod %i!\n", KEYLEN);
        return 3;
    }
    int sum_key_inverse = get_additive_inverse(sum_key);
    char p;
    char *mult_cipher_text = malloc(sizeof(char));
    int i = 0;
    while ((p = fgetc(input_file)) != EOF)
    {
        if (!isalpha(p))
            continue;
        mult_cipher_text[i++] = (tolower(p) - 'a') * prod_key % KEYLEN + 'a';
        mult_cipher_text = realloc(mult_cipher_text, (i + 1) * sizeof(char));
    }

    for (int j = 0; j < i; j++)
    {
        char C = (mult_cipher_text[j] - 'a' + sum_key) % KEYLEN + 'a';
        fputc(C, output_file);
    }

    free(mult_cipher_text);
    fclose(input_file);
    fclose(output_file);
    fprintf(stdout, "Ciphertext written to files/output\n");
    fprintf(stdout, "Decrypt with: (*) key1: %i & (+) key2: %i\n", prod_key_inverse, sum_key_inverse);
    return 0;
}