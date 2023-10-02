#include <stdio.h>
#include <stdlib.h>

int get_gcd(int, int);
int get_extended_gcd(int, int);

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: ./gcd num1 num2 [type]\n\ntype:\n\tn:normal\n\te:extended\n");
        return 1;
    }

    int a = atoi(argv[1]), b = atoi(argv[2]);
    char type = 'n';
    if (argc == 4) type = argv[3][0];

    int gcd;

    if (type == 'e') {
        gcd = get_extended_gcd(a, b);
    } 
    else {
        gcd = get_gcd(a,b);
    }

    printf("%d\n", gcd);
    return 0;
}

int get_gcd(int r1, int r2) {
    while (r2 > 0) {
        int q = r1 / r2;
        int r = r1 % r2;

        r1 = r2;
        r2 = r;
    }
    return r1;
}

int get_extended_gcd(int r1, int r2) {
    int t1 = 0, t2 = 1, s1 = 1, s2 = 0, t, s;
    if (r1 < r2) {
        int temp = r1;
        r1 = r2;
        r2 = temp;
    }
    while (r2 > 0) {
        int q = r1 / r2;
        int r = r1 % r2;
        int s, t;

        r1 = r2;
        r2 = r;
        t = t1 - q * t2;
        s = s1 - q * s2;
        t1 = t2;
        t2 = t;
        s1 = s2;
        s2 = s;
    }

    printf("s: %d  t: %d\n", s1, t1);
    return r1;
}