#include <stdio.h>
#include <stdlib.h>

int get_gcd(int, int);
int get_extended_gcd(int, int);

int main(int argc, char *argv[])
{
    if (argc != 3 && argc != 4)
    {
        fprintf(stderr, "Usage: ./gcd num1 num2 [type]\n\ntype:\n\tn:normal\n\te:extended\n");
        return 1;
    }

    int a = atoi(argv[1]), b = atoi(argv[2]);
    char type = 'n';
    if (argc == 4)
        type = argv[3][0];

    int gcd;

    if (type == 'e')
        gcd = get_extended_gcd(a, b);
    else
        gcd = get_gcd(a, b);

    printf("%d\n", gcd);
    return 0;
}

int get_gcd(int r1, int r2)
{
    if (r2 == 0)
        return r1;
    return get_gcd(r2, r1 % r2);
}

int get_extended_gcd(int r1, int r2)
{
    int x = r1, n = r2, t1 = 0, t2 = 1;
    if (r1 < r2) {
        int temp = r1;
        r1 = r2; r2 = temp;
    }

    while (r2 > 0) {
        int q = r1 / r2;
        int r = r1 % r2;
        int t = t1 - q * t2;
        r1 = r2; r2 = r;
        t1 = t2; t2 = t;
    }
    if (r1 == 1)
        printf("Inverse of %i under mod %i is %i\n", x, n, (t1 < 0) ? t1 + n : t1);
    return r1;
}