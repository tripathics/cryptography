#include "ciphers.h"

int get_additive_inverse(int k)
{
    int inverse = (KEYLEN - k) % KEYLEN;
    if (inverse < 0) inverse += KEYLEN;
    return inverse;
}

int get_multiplicative_inverse(int x)
{
    int r1 = x, r2 = KEYLEN, t1 = 0, t2 = 1;
    if (r1 < r2)
    {
        int temp = r1;
        r1 = r2;
        r2 = temp;
    }

    while (r2 > 0)
    {
        int q = r1 / r2, r = r1 % r2;
        int t = t1 - q * t2;
        r1 = r2; r2 = r;
        t1 = t2; t2 = t;
    }

    if (r1 != 1)
    {
        fprintf(stderr, "Inverse doesn't exist\n");
        exit(1);
    }
    if (t1 < 0)
        t1 += KEYLEN;
    return t1;
}

void additive_cipher(FILE *input_file, FILE *output_file, int key)
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

    fprintf(stdout, "Ciphertext written to files/output\n");
    fprintf(stdout, "Decrypt with %i\n", key_inverse);
    return;
}

void multiplicative_cipher(FILE *input_file, FILE *output_file, int key)
{
    int key_inverse = get_multiplicative_inverse(key);
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

    fprintf(stdout, "Ciphertext written to files/output\n");
    fprintf(stdout, "Decrypt with: %i\n", key_inverse);
    return;
}

void affine_cipher_encrypt(FILE *input_file, FILE *output_file, int prod_key, int sum_key)
{
    char p;
    while ((p = fgetc(input_file)) != EOF)
    {
        if (!isalpha(p)) continue;
        char C = ((tolower(p) - 'a') * prod_key) % KEYLEN + 'a';
        C = (C - 'a' + sum_key) % KEYLEN + 'a';
        fputc(C, output_file);
    }
    fprintf(stdout, "Ciphertext written to files/output\n");
    return;
}

void affine_cipher_decrypt(FILE *input_file, FILE *output_file, int prod_key, int sum_key)
{
    char c;
    while ((c = fgetc(input_file)) != EOF)
    {
        if (!isalpha(c)) continue;
        char P = (tolower(c) - 'a' + sum_key) % KEYLEN + 'a';
        if (P < 'a')
            P += KEYLEN;
        P = (P - 'a') * prod_key % KEYLEN + 'a';
        fputc(P, output_file);
    }
    fprintf(stdout, "Plaintext written to files/output\n");
    return;
}