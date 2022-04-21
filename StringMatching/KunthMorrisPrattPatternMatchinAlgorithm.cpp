/*
    Kunth Morris Pratt Pattern Matching Algorithm
    1. Start with state q = 0.
    2. Compute prefix function  using pattern prefixFunction.
    3. Iterate for each character i of text.
    4. Find the charactor index of text that does not match with pattern (q+1).
    5. Get the prefix index for current state.
    6. If next pattern charactor matches with text charactor, increase the state q to q+1.
    7. If state matches with final state m( i.e m = Pattern.size()), print found pattern at shift i-m index.
    8. Get the next state using prefix function to find next match for pattern.

    Compute prefix function using pattern.
    1. Let m = pattern.size().
    2. Initialize state prefix finction prefixFunction of size m+1 and intialize 0.
    3. Initialize prefixFunction[0] = 0, prefixFunction[1] = 0.
    4. Iterate state q from 2 to m.
    5. Compute k < q such that k prefix of pattern q is suffix of pattern q.
    6. Update prefixFunction[q] = k.
    7. return prefixFunction. 
*/

#include <string>
#include <vector>
#include <iostream>

class KunthMorrisPrattPatternMatcher {
    public:
    KunthMorrisPrattPatternMatcher(const std::string &T, const std::string &P) :T(T), P(P) {
        computePrefixFunction();
    }

    void computePrefixFunction() {
        auto m = P.size();
        prefixFunction.resize(m+1, 0);
        prefixFunction[0] = 0;
        prefixFunction[1] = 0;

        int k = 0;
        for (int q = 2; q <= m; ++q) {
            auto qChar = P[q-1];
            while( k>0 && P[k] != qChar) {
                k = prefixFunction[k];
            }

            if (P[k] == qChar) {
                k = k+1;
            }
            prefixFunction[q] = k;
            std::cout << "prefixFunction: " << q << ", " << prefixFunction[q] << std::endl;
        }
    }

    void match() const {
        auto n = T.size();
        auto m = P.size();

        int q = 0;
        for (int i = 1; i <= n; ++i) {
            auto ch = T[i-1];
            while(q > 0 && P[q] != ch) {
                q = prefixFunction[q];
            }

            if ( P[q] == ch ) {
                q = q + 1;
            }

            if (q == m) {
                std::cout << "Pattern found at shift: " << i-m << std::endl;
            }
        }
    }

    const std::string &T;
    const std::string &P;
    std::vector<int> prefixFunction;
};


int main() {
    std::cout << "KunthMorrisPrattPatternMatchinAlgorithm" << std::endl;

    std::string T = "AABAACAADAABAAABAA";
    std::string P = "AABA";

    KunthMorrisPrattPatternMatcher(T, P).match();

    /*
        Output:
        Pattern matched at shift: 0
        Pattern matched at shift: 9
        Pattern matched at shift: 13

    */
    return 0;
}