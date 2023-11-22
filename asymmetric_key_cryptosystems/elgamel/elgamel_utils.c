#include "elgamel.h"

FILE *input_file;
FILE *output_file;
FILE *public_key_file;
FILE *private_key_file;

/**
 * Calculate a^b mod c using fast exponentiation
 */
int mod_pow(int a, int b, int c)
{
    int res = 1;
    while (b > 0)
    {
        if (b & 1)
            res = (res * a) % c;
        b = b >> 1;
        a = (a * a) % c;
    }
    return res;
}

int inverse(int x, int n)
{
    int r1 = x, r2 = n, t1 = 0, t2 = 1;
    if (r1 < r2)
    {
        int temp = r1;
        r1 = r2;
        r2 = temp;
    }

    while (r2 > 0)
    {
        int q = r1 / r2;
        int r = r1 % r2;
        int t = t1 - q * t2;
        r1 = r2;
        r2 = r;
        t1 = t2;
        t2 = t;
    }

    if (r1 != 1)
    {
        fprintf(stderr, "Inverse doesn't exist\n");
        exit(1);
    }
    if (t1 < 0)
        t1 += n;
    return t1;
}

bool is_primitive_root(int p, int r)
{
    int result[p];
    memset(result, 0, sizeof(result));
    for (int i = 1; i < p; i++)
    {
        int x = mod_pow(r, i, p);
        if (result[x] == 1)
            return false;
        result[x] = 1;
    }
    return true;
}

int primitive_root(int p)
{
    for (int r = 2; r < p; r++)
    {
        if (is_primitive_root(p, r))
            return r;
    }
    fprintf(stderr, "Cannot find primitive root\n");
    exit(1);
}

void generate_key(char *key_filename)
{
    char *public_key_filename = malloc(strlen(key_filename) + 5);
    strcpy(public_key_filename, key_filename);
    strcat(public_key_filename, ".pub");
    char *private_key_filename = key_filename;
    fprintf(stdout, "Generating key pair...\n");

    public_key_file = fopen(public_key_filename, "w");
    private_key_file = fopen(private_key_filename, "w");
    if (public_key_file == NULL || private_key_file == NULL)
    {
        fprintf(stderr, "Error opening keyfile\n");
        exit(1);
    }

    // large prime number
    int p;
    do
    {
        printf("Enter a large positive prime number: ");
        scanf("%i", &p);
    } while (p < 1);

    int e1 = primitive_root(p);
    srand(time(NULL));
    int d = rand() % (p - 1) + 1;
    int e2 = mod_pow(e1, d, p);

    fprintf(public_key_file, "%i,%i,%i", e1, e2, p);
    fprintf(private_key_file, "%i", d);
    printf("Key pair generated in %s and %s\n", public_key_filename, key_filename);
    fclose(public_key_file);
    fclose(private_key_file);
    return;
}

void encrypt(char *key_filename, int inp)
{
    srand(time(NULL));
    char *public_key_filename = malloc(strlen(key_filename) + 5);
    strcpy(public_key_filename, key_filename);
    strcat(public_key_filename, ".pub");
    char *private_key_filename = key_filename;
    public_key_file = fopen(public_key_filename, "r");

    int e1, e2, p;
    int r = rand() % (p - 1) + 1;
    fscanf(public_key_file, "%i,%i,%i", &e1, &e2, &p);
    fclose(public_key_file);

    int c1 = mod_pow(e1, r, p);
    int c2 = (inp * mod_pow(e2, r, p)) % p;
    printf("Encrypted message: (%i, %i)\n", c1, c2);
    return;
}

void decrypt(char *key_filename, int c1, int c2)
{
    char *public_key_filename = malloc(strlen(key_filename) + 5);
    strcpy(public_key_filename, key_filename);
    strcat(public_key_filename, ".pub");
    char *private_key_filename = key_filename;
    public_key_file = fopen(public_key_filename, "r");
    private_key_file = fopen(private_key_filename, "r");

    int e1, e2, p;
    int d;
    fscanf(public_key_file, "%i,%i,%i", &e1, &e2, &p);
    fscanf(private_key_file, "%i", &d);
    fclose(public_key_file);
    fclose(private_key_file);

    // plain text
    int P = (c2 * inverse(mod_pow(c1, d, p), p)) % p;
    printf("Decrypted message: %i\n", P);
    return;
}