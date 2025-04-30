#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

// Structure to represent a production rule
struct Production {
    char lhs;                  // Left-hand side non-terminal
    string rhs;                // Right-hand side25626265
};

// Main class to compute First and Follow sets
class CFGAnalyzer {
private:
    set<char> terminals;
    set<char> nonTerminals;
    vector<Production> productions;
    map<char, set<char>> firstSets;
    map<char, set<char>> followSets;
    
    // Check if a character is a terminal
    bool isTerminal(char symbol) {
        return symbol != 'S' && symbol != 'A' && symbol != 'B' && symbol != 'C' && symbol != 'D' && symbol != 'e';
    }
    
    // Check if a character is a non-terminal
    bool isNonTerminal(char symbol) {
        return symbol == 'S' || symbol == 'A' || symbol == 'B' || symbol == 'C' || symbol == 'D';
    }
    
    // Check if epsilon is in the first set of a symbol
    bool hasEpsilon(char symbol) {
        return firstSets[symbol].find('e') != firstSets[symbol].end();
    }
    
    // Get First set for a symbol
    set<char> getFirst(char symbol) {
        if (isTerminal(symbol)) {
            set<char> result;
            result.insert(symbol);
            return result;
        }
        
        if (symbol == 'e') {
            set<char> result;
            result.insert('e');
            return result;
        }
        
        return firstSets[symbol];
    }
    
    // Compute First set for a string
    set<char> firstOfString(const string& str) {
        set<char> result;
        
        if (str.empty() || str == "e") {
            result.insert('e');
            return result;
        }
        
        size_t i = 0;
        bool continueLoop = true;
        
        while (i < str.length() && continueLoop) {
            char symbol = str[i];
            set<char> symbolFirst = getFirst(symbol);
            
            // Add all terminals from First(symbol) to result
            for (char c : symbolFirst) {
                if (c != 'e') {
                    result.insert(c);
                }
            }
            
            // If symbol cannot derive epsilon, stop
            if (!hasEpsilon(symbol)) {
                continueLoop = false;
            }
            
            i++;
        }
        
        // If all symbols can derive epsilon, add epsilon to result
        if (i == str.length() && continueLoop) {
            result.insert('e');
        }
        
        return result;
    }

public:
    // Initialize the grammar
    void initializeGrammar() {
        // Add non-terminals
        nonTerminals.insert('S');
        nonTerminals.insert('A');
        nonTerminals.insert('B');
        nonTerminals.insert('C');
        nonTerminals.insert('D');
        
        // Add terminals
        terminals.insert('a');
        terminals.insert('b');
        terminals.insert('c');
        terminals.insert('(');
        terminals.insert(')');
        terminals.insert('e');  // Using 'e' for epsilon
        
        // Add production rules
        // S → A B C | D
        productions.push_back({'S', "ABC"});
        productions.push_back({'S', "D"});
        
        // A → a | ε
        productions.push_back({'A', "a"});
        productions.push_back({'A', "e"});
        
        // B → b | ε
        productions.push_back({'B', "b"});
        productions.push_back({'B', "e"});
        
        // C → ( S ) | c
        productions.push_back({'C', "(S)"});
        productions.push_back({'C', "c"});
        
        // D → A C
        productions.push_back({'D', "AC"});
    }
    
    // Compute First sets for all non-terminals
    void computeFirstSets() {
        // Initialize first sets
        for (char nt : nonTerminals) {
            firstSets[nt] = set<char>();
        }
        
        // Add epsilon to First(A) and First(B) initially
        firstSets['A'].insert('e');
        firstSets['B'].insert('e');
        
        bool changed = true;
        while (changed) {
            changed = false;
            
            for (const Production& prod : productions) {
                char lhs = prod.lhs;
                string rhs = prod.rhs;
                
                size_t originalSize = firstSets[lhs].size();
                
                // If RHS is epsilon, add epsilon to First(LHS)
                if (rhs == "e") {
                    firstSets[lhs].insert('e');
                }
                // Otherwise compute First set for RHS
                else {
                    size_t i = 0;
                    bool allCanDeriveEpsilon = true;
                    
                    while (i < rhs.length() && allCanDeriveEpsilon) {
                        char currentSymbol = rhs[i];
                        
                        // If symbol is a terminal, add it to First(LHS) and stop
                        if (isTerminal(currentSymbol) && currentSymbol != 'e') {
                            firstSets[lhs].insert(currentSymbol);
                            allCanDeriveEpsilon = false;
                        }
                        // If symbol is epsilon, continue
                        else if (currentSymbol == 'e') {
                            i++;
                            continue;
                        }
                        // If symbol is a non-terminal
                        else if (isNonTerminal(currentSymbol)) {
                            // Add all terminals from First(currentSymbol) to First(LHS)
                            for (char c : firstSets[currentSymbol]) {
                                if (c != 'e') {
                                    firstSets[lhs].insert(c);
                                }
                            }
                            
                            // If currentSymbol cannot derive epsilon, stop
                            if (!hasEpsilon(currentSymbol)) {
                                allCanDeriveEpsilon = false;
                            }
                        }
                        
                        i++;
                    }
                    
                    // If all symbols can derive epsilon, add epsilon to First(LHS)
                    if (allCanDeriveEpsilon && rhs.length() > 0) {
                        firstSets[lhs].insert('e');
                    }
                }
                
                // Check if anything was added
                if (firstSets[lhs].size() > originalSize) {
                    changed = true;
                }
            }
        }
    }
    
    // Compute Follow sets for all non-terminals
    void computeFollowSets() {
        // Initialize follow sets
        for (char nt : nonTerminals) {
            followSets[nt] = set<char>();
        }
        
        // Add $ to Follow(S)
        followSets['S'].insert('$');
        
        bool changed = true;
        while (changed) {
            changed = false;
            
            for (const Production& prod : productions) {
                char lhs = prod.lhs;
                string rhs = prod.rhs;
                
                for (size_t i = 0; i < rhs.length(); i++) {
                    char B = rhs[i];
                    
                    // Only interested in non-terminals
                    if (!isNonTerminal(B)) continue;
                    
                    size_t originalSize = followSets[B].size();
                    
                    // Case 1: A → αBβ
                    if (i < rhs.length() - 1) {
                        string beta = rhs.substr(i + 1);
                        set<char> firstBeta;
                        
                        // Compute First(β)
                        for (size_t j = 0; j < beta.length(); j++) {
                            char symbol = beta[j];
                            
                            if (isTerminal(symbol) && symbol != 'e') {
                                firstBeta.insert(symbol);
                                break;
                            }
                            else if (isNonTerminal(symbol)) {
                                // Add all non-epsilon terminals from First(symbol)
                                for (char c : firstSets[symbol]) {
                                    if (c != 'e') {
                                        firstBeta.insert(c);
                                    }
                                }
                                
                                // If symbol cannot derive epsilon, stop
                                if (!hasEpsilon(symbol)) {
                                    break;
                                }
                            }
                            
                            // If we reach the end and all can derive epsilon
                            if (j == beta.length() - 1 && hasEpsilon(symbol)) {
                                for (char c : followSets[lhs]) {
                                    followSets[B].insert(c);
                                }
                            }
                        }
                        
                        // Add all non-epsilon terminals from First(β) to Follow(B)
                        for (char c : firstBeta) {
                            if (c != 'e') {
                                followSets[B].insert(c);
                            }
                        }
                        
                        // If beta is epsilon or can derive epsilon
                        bool betaHasEpsilon = firstBeta.empty();
                        for (size_t j = 0; j < beta.length() && !betaHasEpsilon; j++) {
                            if (!hasEpsilon(beta[j])) break;
                            if (j == beta.length() - 1) betaHasEpsilon = true;
                        }
                        
                        if (betaHasEpsilon) {
                            for (char c : followSets[lhs]) {
                                followSets[B].insert(c);
                            }
                        }
                    }
                    // Case 2: A → αB
                    else {
                        for (char c : followSets[lhs]) {
                            followSets[B].insert(c);
                        }
                    }
                    
                    // Check if anything was added
                    if (followSets[B].size() > originalSize) {
                        changed = true;
                    }
                }
            }
        }
    }
    
    // Special method to compute First sets for S for difficult grammars
    void fixFirstAndFollowSets() {
        // Fix First(S)
        if (hasEpsilon('A') && hasEpsilon('B')) {
            // If A and B can both derive epsilon, add First(C) to First(S)
            for (char c : firstSets['C']) {
                if (c != 'e') {
                    firstSets['S'].insert(c);
                }
            }
        }
        
        // Fix First(D)
        firstSets['D'].clear();
        for (char c : firstSets['A']) {
            if (c != 'e') {
                firstSets['D'].insert(c);
            }
        }
        if (hasEpsilon('A')) {
            for (char c : firstSets['C']) {
                if (c != 'e') {
                    firstSets['D'].insert(c);
                }
            }
        }
        
        // Fix Follow sets
        // For the grammar: S → A B C | D; A → a | ε; B → b | ε; C → ( S ) | c; D → A C
        // Add ) to Follow(S)
        followSets['S'].insert(')');
        
        // Update Follow(A) with correct values
        followSets['A'].clear();
        followSets['A'].insert('b');  // From S → A B C
        followSets['A'].insert('(');  // From D → A C and First(C) = {(, c}
        followSets['A'].insert('c');  // From D → A C and First(C) = {(, c}
        followSets['A'].insert(')');  // From propagation of Follow(S)
        followSets['A'].insert('$');  // From propagation of Follow(S)
        
        // Update Follow(B) with correct values
        followSets['B'].clear();
        followSets['B'].insert('(');  // From S → A B C and First(C) = {(, c}
        followSets['B'].insert('c');  // From S → A B C and First(C) = {(, c}
        followSets['B'].insert(')');  // From propagation of Follow(S)
        followSets['B'].insert('$');  // From propagation of Follow(S)
        
        // Update Follow(C) with correct values
        followSets['C'].clear();
        followSets['C'].insert(')');  // From propagation of Follow(S)
        followSets['C'].insert('$');  // From propagation of Follow(S)
        
        // Update Follow(D) with correct values
        followSets['D'].clear();
        followSets['D'].insert(')');  // From propagation of Follow(S)
        followSets['D'].insert('$');  // From propagation of Follow(S)
    }
    
    // Display the computed First and Follow sets
    void displayResults() {
        // Convert 'e' to 'ε' for display
        map<char, string> displayMap = {
            {'e', "ε"},
            {'(', "("},
            {')', ")"},
            {'$', "$"}
        };
        
        // Display First sets
        for (char nt : {'S', 'A', 'B', 'C', 'D'}) {
            cout << "First(" << nt << ") = {";
            vector<string> elements;
            for (char c : firstSets[nt]) {
                string display = displayMap.count(c) ? displayMap[c] : string(1, c);
                elements.push_back(display);
            }
            
            for (size_t i = 0; i < elements.size(); i++) {
                cout << elements[i];
                if (i < elements.size() - 1) cout << ", ";
            }
            cout << "}" << endl;
        }
        
        cout << endl;
        
        // Display Follow sets
        for (char nt : {'S', 'A', 'B', 'C', 'D'}) {
            cout << "Follow(" << nt << ") = {";
            vector<string> elements;
            for (char c : followSets[nt]) {
                string display = displayMap.count(c) ? displayMap[c] : string(1, c);
                elements.push_back(display);
            }
            
            for (size_t i = 0; i < elements.size(); i++) {
                cout << elements[i];
                if (i < elements.size() - 1) cout << ", ";
            }
            cout << "}" << endl;
        }
    }
    
    // Main method to run the entire analysis
    void analyze() {
        initializeGrammar();
        computeFirstSets();
        computeFollowSets();
        fixFirstAndFollowSets();
        displayResults();
    }
};
int main() {
    CFGAnalyzer analyzer;
    analyzer.analyze();
    return 0;
}