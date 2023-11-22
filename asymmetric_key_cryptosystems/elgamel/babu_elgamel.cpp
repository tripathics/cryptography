#include <bits/stdc++.h>
using namespace std;
vector<int> elgamalEncr(int e1, int e2, int P, int plaintext)
{
    int r = rand() % (plaintext - 1) + 1;
    int r1 = r, c1 = 1, c2 = 1;
    while (r1--)
    {
        c1 = (c1 * e1) % P;
        c2 = (c2 * e2) % P;
    }
    c2 = (plaintext * c2) % P;
    return {c1, c2};
}

int elgamalDecr(int d, int P, int c1, int c2)
{
    int p = 1;
    for (int i = 0; i < P - 1 - d; i++)
    {
        p = (p * c1) % P;
    }
    p = (p * c2) % P;
    return p;
}
int main()
{
    std::cout << "Enter a large prime number : ";
    int P; cin >> P;
    int d = rand() % (P - 2) + 1;
    // primitive root
    int e1 = 0;
    for (int i = 2; i < P; i++)
    {
        bool checker = true;
        for (int j = 1; j < P - 1; j++)
        {
            int mod = 1, k = j;
            while (k--)
            {
                mod = (mod * i) % P;
            }
            if (mod == 1)
            {
                checker = false;break;
            }
        }
        if (checker)
        {
            e1 = i;break;
        }
    }
    int it = d, mod = 1;
    while (it--)
    {
        mod = (mod * e1) % P;
    }
    int e2 = mod;
    cout << endl << "Enter message : ";
    int plaintext; cin >> plaintext;
    vector<int> ans = elgamalEncr(e1, e2, P, plaintext);
    cout << "Encripted message : " << ans[0] << " " << ans[1] << endl;
    cout << "Decripted message : " << elgamalDecr(d, P, ans[0], ans[1]);
    return 0;
}