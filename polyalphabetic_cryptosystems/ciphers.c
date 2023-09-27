#include "ciphers.h"

int get_additive_inverse(int k)
{
    return KEYLEN - k % KEYLEN;
}

int autokey_cipher(FILE *input_file, FILE *output_file, int key)
{
    char p, p_prev = key + 'a';
    while ((p = fgetc(input_file)) != EOF)
    {
        if (!isalpha(p))
            continue;
        p = tolower(p);
        char c = (p + p_prev - 2 * 'a') % 26 + 'a';
        fwrite(&c, sizeof(char), 1, output_file);
        p_prev = p;
    }

    fclose(input_file);
    fclose(output_file);
    fprintf(stdout, "Ciphertext written to files/output\n");
    return 0;
}

int playfair_cipher(FILE *input_file, FILE *output_file, FILE *key_file)
{
    // read from keyfile into 5x5 array in memory
    char key[5][5][2];

    char curr[2] = "\0\0";
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            char c;
            while ((c = fgetc(key_file)) != EOF && !isalpha(c))
                ;
            fseek(key_file, -1, SEEK_CUR);

            fread(curr, sizeof(char), 2, key_file);

            key[i][j][0] = tolower(curr[0]);
            if (isalpha(curr[1]))
                key[i][j][1] = tolower(curr[1]);
            else
                key[i][j][1] = '\0';
        }
    }

    char p;
    int char_frequency[KEYLEN];
    for (int i = 0; i < KEYLEN; i++)
        char_frequency[i] = 0;
    char **pairs = NULL;
    int pair_ptr = 0;
    while ((p = fgetc(input_file)) != EOF)
    {
        if (!isalpha(p))
            continue;
        // increment character count
        char_frequency[tolower(p) - 'a']++;

        if (pairs == NULL)
            pairs = malloc(sizeof(char *));
        if (pairs == NULL)
        {
            fprintf(stderr, "Error allocating pairs\n");
            return 1;
        }

        if (pairs[pair_ptr] == NULL)
        {
            pairs[pair_ptr] = calloc(2, sizeof(char));
            pairs[pair_ptr][0] = tolower(p);
        }
        else
        {
            pairs[pair_ptr][1] = tolower(p);
            pair_ptr++;
            pairs = realloc(pairs, (pair_ptr + 1) * sizeof(char *));
            pairs[pair_ptr] = NULL;
        }
    }
    // find bogus character
    char bogus = 'x';
    for (int i = 0; i < KEYLEN; i++)
    {
        if (char_frequency[i] == 0)
        {
            bogus = i + 'a';
            break;
        }
    }
    // if odd number of characters, or same characters in a pair, add bogus character
    for (int i = 0; i <= pair_ptr; i++)
    {
        if (pairs[i][1] == 0)
            pairs[i][1] = bogus;
        else if (pairs[i][0] == pairs[i][1])
            pairs[i][1] = bogus;
    }

    for (int i = 0; i <= pair_ptr; i++)
    {
        // find the pairs indices
        int p1x = 0, p1y = 0, p2x = 0, p2y = 0;
        search_pair(&p1x, &p1y, &p2x, &p2y, pairs[i], key);
        int c1x, c1y, c2x, c2y;

        if (p1x == p2x)
        {
            c1x = p1x + 1 % 5;
            c2x = p2x + 1 % 5;
            c1y = p1y;
            c2y = p2y;
        }
        else if (p1y == p2y)
        {
            c1y = p1y + 1 % 5;
            c2y = p2y + 1 % 5;
            c1x = p1x;
            c2y = p2y;
        }
        else
        {
            c1x = p1x;
            c1y = p2y;
            c2x = p2x;
            c2y = p1y;
        }

        char C[] = {(char)key[c1x][c1y][0], (char)key[c2x][c2y][0]};
        fwrite(C, sizeof(char), 2, output_file);
    }

    // free allocated memories
    for (int i = 0; i <= pair_ptr; i++)
        free(pairs[i]);
    free(pairs);
    fclose(input_file);
    fclose(output_file);
    fclose(key_file);
    fprintf(stdout, "Ciphertext written to files/output\n");
    return 0;
}

void search_pair(int *p1x, int *p1y, int *p2x, int *p2y, char *pair, char key[5][5][2])
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (pair[0] == key[i][j][0] || pair[0] == key[i][j][1])
            {
                *p1x = i;
                *p1y = j;
            }

            if (pair[1] == key[i][j][0] || pair[1] == key[i][j][1])
            {
                *p2x = i;
                *p2y = j;
            }
        }
    }
}

int vigenere_cipher(FILE *input_file, FILE *output_file, char *key)
{
    size_t keylen = strlen(key), key_pointer = 0;
    char decrypt_key[keylen + 1];
    for (int i = 0; i < keylen; i++)
        decrypt_key[i] = get_additive_inverse(tolower(key[i]) - 'a') + 'a';
    decrypt_key[keylen] = '\0';

    char p;
    while ((p = fgetc(input_file)) != EOF)
    {
        if (!isalpha(p))
            continue;
        char C = (tolower(p) + tolower(key[key_pointer++ % keylen]) - 2 * 'a') % KEYLEN + 'a';
        fputc(C, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    fprintf(stdout, "Ciphertext written to files/output\n");
    fprintf(stdout, "Decrypt with: %s\n", decrypt_key);
    return 0;
}