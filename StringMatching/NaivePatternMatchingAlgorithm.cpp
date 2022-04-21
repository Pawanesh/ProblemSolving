/*

*/

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class PatternMatcher {
    public:
    PatternMatcher(const std::string &T, const std::string &P) :T(T), P(P) {

    }

    void match() const {
        auto n = T.size();
        auto m = P.size();


        for (int s = 0; s <= n-m; ++s) {
            if (isSame(T.substr(s, m), P)) {
                std::cout << "Pattern found at shift: " << s << std::endl;
            }
        }
        
    }


    bool isSame(const std::string &a, const std::string &b) const {
        return a == b;
    }

    const std::string &T;
    const std::string &P;


};


int main() {
    std::cout << "RabinKarpPatternMatchingAlgorithm" << std::endl;

    std::string T = "AABAACAADAABAAABAA";
    std::string P = "AABA";

    PatternMatcher(T, P).match();

    /*
        Output:
        Pattern matched at shift: 0
        Pattern matched at shift: 9
        Pattern matched at shift: 13

    */
    return 0;
}