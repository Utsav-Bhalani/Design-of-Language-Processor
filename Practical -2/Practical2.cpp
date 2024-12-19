#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

int main()
{
    int inSymbol, state, currState, finalState;
    
    cout << "Enter the number of Input Symbols: ";
    cin >> inSymbol;
    vector<char> symbols(inSymbol);
    cout << "Enter the Symbols: ";
    for (char &symbol : symbols)
        cin >> symbol;

    cout << "Enter the number of States: ";
    cin >> state;
    vector<char> states(state);
    cout << "Enter the States: ";
    for (char &stateChar : states)
        cin >> stateChar;

    cout << "Enter the Initial State: ";
    cin >> currState;
    cout << "Enter the Final State: ";
    cin >> finalState;

    unordered_map<string, int> table;
    for (const char &stateChar : states)
    {
        for (const char &symbol : symbols)
        {
            int nextState;
            cout << stateChar << " -> " << symbol << " ";
            cin >> nextState;
            table[string(1, stateChar) + symbol] = nextState;
        }
    }

    string inpString;
    cout << "Enter input String: ";
    cin >> inpString;

    for (char inputChar : inpString)
    {
        currState = table[string(1, currState) + inputChar];
    }

    cout << (currState == finalState ? "String is Valid" : "String is Invalid");
    return 0;
}
// Output:  Enter the number of Input Symbols: 2
//          Enter the Symbols: a b
//          Enter the number of States: 2
//          Enter the States: A B
//          Enter the Initial State: A  
//          Enter the Final State: B
//          A -> a B
//          A -> b A 