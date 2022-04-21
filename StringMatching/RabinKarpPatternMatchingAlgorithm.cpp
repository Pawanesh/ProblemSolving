/*

*/

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class RabinKarpPatternMatcher {
    public:
    RabinKarpPatternMatcher(const std::string &T, const std::string &P) :T(T), P(P) {

    }

    

    void match() const {
        auto n = T.size();
        auto m = P.size();

        int pHash = 0;
        int tHash = 0;

        for (int i = 0; i <= m-1; ++i) {
            pHash = (pHash*d + P[i]) % q;
            tHash = (tHash*d + T[i]) % q;
        }
        
        tHash = tHash % q;
        pHash = pHash % q;
        int h = powerModule(d, m-1, q);

        for (int s = 0; s <= n-m; ++s) {
            if (pHash == tHash) {
                if (isSame(T.substr(s, m), P)) {
                    std::cout << "Pattern found at shift: " << s << std::endl;
                }
            }

            if (s < n-m) {
                tHash = ( (d*(tHash - (T[s] * h )) )  + T[s+m]  ) %q ;

                if (tHash < 0)
                    tHash = (tHash + q);
            }
        }
        
    }

    int powerModule(int a, int b, int q) const {
        int h = 1;
        for (int i = 1; i <= b; ++i) {
            h = (h*a) % q;
        }
        return h;
    }
    bool isSame(const std::string &a, const std::string &b) const {
        return a == b;
    }

    const std::string &T;
    const std::string &P;

    const int d = 256; //Base 256 for ascii character
    const int q = 101; //Prime for modulo operation

};


int main() {
    std::cout << "RabinKarpPatternMatchingAlgorithm" << std::endl;

    std::string T = "AABAACAADAABAAABAA";
    std::string P = "AABA";

    RabinKarpPatternMatcher(T, P).match();

    /*
        Output:
        Pattern matched at shift: 0
        Pattern matched at shift: 9
        Pattern matched at shift: 13

    */
    return 0;
}