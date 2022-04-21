#include <iostream>
#include <string>
#include <vector>

/*
    Finite Automaton Pattern Matching Algorithm
    1. Start with state 0.
    2. Iterate each charactor (i) from text string and get the next state.
    3. If next state is final state (m i.e. m is size of pattern), found the pattern at i-m shift.

    Compute stateTransition using pattern.
    1. m is the size of pattern.
    2. Intialize stateTransition matrix with total sates (i.e m+1) and number of charaters(256).
    3. Iterate each state q from 0 to m.
    4. Interate for each charactor i.
    5. Compute max next state k i.e is min of final state m and next state i.e q+1.
    6. Compute k prefix of pattern and k suffix of text (i.e pattern for current state q + next char i).
    7. if it matches then update stateTransition[q][i] = k.
    8. else decrease k by 1 and contune steps #6 until found the next state.
*/

class FiniteAutomataPatternMatcher {
public:
    FiniteAutomataPatternMatcher(const std::string& T, const std::string& P):T(T), P(P) {
        buildStateTransition();
    }

    void match() {
        int n = T.size();
        int m = P.size();
        int q = 0;
        for (int i = 1; i <= n; ++i) {
            auto ch = T[i-1];
            q = nextState(q, ch);
            if (q == m) {
                std::cout << "Pattern matched at shift: " << i-m << std::endl;
            }
        }
    }
    
private:
    int nextState(int currentState, int ch) {
        return stateTransitions[currentState][ch];
    }

    void buildStateTransition() {
        int m = P.size();
        stateTransitions.resize(m+1, std::vector<int>(UNIQUE_CHAR_MAX_COUNT, 0));

        for (int q = 0; q <= m; ++q) {
            for (int i = 0; i<= UNIQUE_CHAR_MAX_COUNT-1; ++i) {
                int k = std::min(m, q+1);
                while(k>=0) {

                    if (checkPatternPrefixTextSuffix(q, k, i)) {
                        stateTransitions[q][i] = k;
                        break;
                    }
                    --k;
                }
            }
        }
    }

    bool checkPatternPrefixTextSuffix(int q, int k, int i) {

        auto a = P.substr(0,k);
        auto b = T.substr(0,q) + char(i);
        b = b.substr(b.size()-k, k);

        return a == b;
    }

    const int32_t UNIQUE_CHAR_MAX_COUNT = 256;
    const std::string &T;
    const std::string &P;

    std::vector<std::vector<int>> stateTransitions;

};

int main() {
    std::cout << "FinteAutomataPatternMatchingAlgorithm" << std::endl;

    std::string T = "AABAACAADAABAAABAA";
    std::string P = "AABA";

    FiniteAutomataPatternMatcher(T, P).match();

    /*
        Output:
        Pattern matched at shift: 0
        Pattern matched at shift: 9
        Pattern matched at shift: 13

    */
    return 0;
}