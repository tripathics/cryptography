#include "rsa.h"

#define BUF_SIZE 1024

FILE *input_file;
FILE *output_file;
FILE *public_key_file;
FILE *private_key_file;

// Fast exponentiation a ^ b % c
lli mod_pow(lli a, lli b, lli c)
{
    int res = 1;
    while (b > 0)
    {
        /* Need long multiplication else this will overflow... */
        if (b & 1)
            res = (res * a) % c;
        b = b >> 1;
        a = (a * a) % c; /* Same deal here */
    }
    return res;
}

lli gcd(lli a, lli b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// find multiplicative inverse of r1 under mod r2
lli inverse(lli x, lli n)
{
    lli r1 = x, r2 = n, t1 = 0, t2 = 1;
    if (r1 < r2)
    {
        lli temp = r1;
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

/**
 * Return random exponent between 3 and phi-1
*/
lli rand_exponent(lli phi)
{
    srand(time(NULL));
    lli e = rand() % phi;
    while (1)
    {
        if (gcd(e, phi) == 1)
            return e;
        e = (e + 1) % phi;
        if (e <= 2)
            e = 3;
    }
}

void generate_key(char *key_filename)
{
    // public_key_filename = key_filename + ".pub";
    char *public_key_filename = malloc(strlen(key_filename) + 5);
    strcpy(public_key_filename, key_filename);
    strcat(public_key_filename, ".pub");

    FILE *pub_key_file = fopen(public_key_filename, "w");
    FILE *private_key_file = fopen(key_filename, "w");
    if (!pub_key_file || !private_key_file)
    {
        fprintf(stderr, "Error opening key file\n");
        exit(1);
    }
    lli p, q;
    printf("Enter coprimes p, q: ");
    scanf("%lli %lli", &p, &q);
    lli n = p * q;
    if (n < 256)
    {
        fprintf(stderr, "Cannot encode single byte\n");
        fclose(pub_key_file);
        fclose(private_key_file);
        exit(1);
    }
    printf("Generating key pair...\n");
    lli phi = (p - 1) * (q - 1);
    lli e = rand_exponent(phi);
    lli d = inverse(e, phi);

    fprintf(pub_key_file, "%lli,%lli", e, n);
    fprintf(private_key_file, "%lli", d);
    printf("Key pair generated in %s and %s\n", public_key_filename, key_filename);
    free(public_key_filename);
    fclose(pub_key_file);
    fclose(private_key_file);
    return;
}

void encrypt(char *input_filename, char *output_filename, char *public_key_filename)
{
    input_file = fopen(input_filename, "rb");
    output_file = fopen(output_filename, "wb");
    if (!input_file || !output_file)
    {
        fprintf(stderr, "Error opening input/output file\n");
        exit(1);
    }
    public_key_file = fopen(public_key_filename, "r");
    if (!public_key_file)
    {
        fprintf(stderr, "Error opening public key file\n");
        exit(1);
    }
    lli e, n;
    fscanf(public_key_file, "%lli,%lli", &e, &n);
    fclose(public_key_file);

    // byte stream to be encoded in chunks of bytes;
    int bytes;
    if (n >> 21) bytes = 3;
    else if (n >> 14) bytes = 2;
    else bytes = 1;

    // read bytes chunks from input file, encode and write to output file
    size_t r;
    BYTE *buffer = malloc(bytes);
    while ((r = fread(buffer, sizeof(BYTE), bytes, input_file)) > 0)
    {
        int encoded = 0;
        for (int i = 0; i < bytes; i++)
        {
            encoded = encoded << 8;
            encoded += buffer[i];
        }
        unsigned long long int encoded_block = mod_pow(encoded, e, n);
        BYTE encoded_bytes[sizeof(unsigned long long int)];
        memcpy(encoded_bytes, &encoded_block, sizeof(unsigned long long int));
        for (int i = 0; i < sizeof(unsigned long long int); i++)
            fwrite(&encoded_bytes[i], sizeof(BYTE), 1, output_file);
    }
    fclose(input_file);
    fclose(output_file);
    printf("Encrypted file written to %s\n", output_filename);
    return;
}

void decrypt(char *input_filename, char *output_filename, char *public_key_filename, char *private_key_filename)
{
    input_file = fopen(input_filename, "rb");
    output_file = fopen(output_filename, "wb");
    if (!input_file || !output_file)
    {
        fprintf(stderr, "Error opening input/output file\n");
        exit(1);
    }
    private_key_file = fopen(private_key_filename, "r");
    public_key_file = fopen(public_key_filename, "r");
    if (!private_key_file || !public_key_file)
    {
        fprintf(stderr, "Error opening key files\n");
        exit(1);
    }
    lli d, n, e;
    fscanf(private_key_file, "%lli", &d);
    fscanf(public_key_file, "%lli,%lli", &e, &n);
    fclose(private_key_file);
    fclose(public_key_file);

    // byte stream has been encoded in chunks of bytes;
    int bytes;
    if (n >> 21) bytes = 3;
    else if (n >> 14) bytes = 2;
    else bytes = 1;

    // read ciphertext chunks from input file, decode and write to output file
    size_t r;
    BYTE buffer[sizeof(unsigned long long int)];
    while ((r = fread(buffer, sizeof(unsigned long long int), 1, input_file)) > 0)
    {
        unsigned long long int encoded_block;
        memcpy(&encoded_block, buffer, sizeof(unsigned long long int));
        int decoded = mod_pow(encoded_block, d, n);
        BYTE decoded_bytes[bytes];
        for (int i = bytes - 1; i >= 0; i--)
        {
            decoded_bytes[i] = decoded & 0xFF;
            decoded = decoded >> 8;
        }
        fwrite(decoded_bytes, sizeof(BYTE), bytes, output_file);
    }
    fclose(input_file);
    fclose(output_file);
    printf("Decrypted file written to %s\n", output_filename);
    return;
}