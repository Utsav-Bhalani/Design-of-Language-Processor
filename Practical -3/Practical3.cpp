#include <bits/stdc++.h>
using namespace std;

vector<string> keywords = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
vector<string> operators = {"+", "-", ",", "", ";", "|", "&", "^", "%", "<", ">", "=", "~", "!"};

bool isKeyword(const string &word)
{
    return find(keywords.begin(), keywords.end(), word) != keywords.end();
}

void addToken(vector<pair<string, string>> &tokens, const string &word)
{
    if (isKeyword(word))
    {
        tokens.push_back({"keyword", word});
    }
    else if (isdigit(word[0]))
    {
        // Check if the word is a valid number
        bool isNumber = all_of(word.begin(), word.end(), ::isdigit);
        if (isNumber)
        {
            tokens.push_back({"number", word});
        }
        else
        {
            cerr << "Error: Invalid token '" << word << "' starting with a digit." << endl;
        }
    }
    else
    {
        tokens.push_back({"identifier", word});
    }
}


int main()
{
    ifstream f("C:\\Users\\utsav\\OneDrive\\Desktop\\B. Tech\\6th semester\\DLP\\Practical -3\\Testcases\\testcase1.c");

    if (!f.is_open())
    {
        cerr << "error opening file";
        return 1;
    }

    string s;
    vector<pair<string, string>> tokens;
    bool inMultiLineComment = false;
    while (getline(f, s))
    {
        string word = "";
        bool inStringS = false;
        bool inStringD = false;

        for (int i = 0; i < s.length(); i++)
        {
            char c = s[i];

            if (c == '/' && i + 1 < s.length() && s[i + 1] == '/')
                break;

            if (c == '/' && i + 1 < s.length() && s[i + 1] == '*')
            {
                inMultiLineComment = true;
                i++;
                continue;
            }
            if (inMultiLineComment)
            {
                if (c == '*' && i + 1 < s.length() && s[i + 1] == '/')
                {
                    inMultiLineComment = false;
                    i++;
                }
                continue;
            }

            if (!inMultiLineComment)
            {
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                    word += c;
                else if (c >= '0' && c <= '9')
                {
                    if (word.empty())
                        word += c;
                    else
                    {
                        addToken(tokens, word);
                        word = "";
                    }
                }
                else if (c == ',' || c == ';' || c == '.' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']')
                {
                    if (!word.empty())
                        addToken(tokens, word);
                    word = "";
                    tokens.push_back({"punctuation", string(1, c)});
                }
                else if (find(operators.begin(), operators.end(), string(1, c)) != operators.end())
                {
                    if (!word.empty())
                        addToken(tokens, word);
                    word = "";
                    tokens.push_back({"operator", string(1, c)});
                }
                else if (c == '\'')
                {
                    if (!inStringS)
                        word += c;
                    else
                    {
                        word += c;
                        tokens.push_back({"string", word});
                        word = "";
                    }
                    inStringS = !inStringS;
                }
                else if (c == '\"')
                {
                    if (!inStringD)
                        word += c;
                    else
                    {
                        word += c;
                        tokens.push_back({"string", word});
                        word = "";
                    }
                    inStringD = !inStringD;
                }
                else
                {
                    if (!word.empty())
                        addToken(tokens, word);
                    word = "";
                }
            }
        }
        if (!inMultiLineComment && !word.empty())
        {
            addToken(tokens, word);
        }
    }

    for (auto str : tokens)
        cout << str.first << " : " << str.second << endl;

    f.close();
    return 0;
}