
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector<string> tokenization(string equation);

int main()
{
    string equation;
    getline(cin, equation);

    vector<string> tokens = tokenization(equation);
    cout << "Tokens: ";
    for (int i = 0; i < tokens.size(); i++)
        cout << tokens[i] << ", ";
    cout << endl;
}

vector<string> tokenization(string equation) {
    string numbers = "1234567890";
    string operators = "!()+-*/^%";
    stringstream num;
    vector<string> tokens;
    for (auto symb : equation) {
        if (numbers.find(symb) != string::npos) {
            num << symb;
        }
        else if (operators.find(symb) != string::npos) {
            if (num.str().length() != 0) {
                tokens.push_back(num.str());
                num.str("");
            }
            tokens.push_back(string{ symb });
        }
        else if (symb == ' ') {
            if (num.str().length() != 0) {
                tokens.push_back(num.str());
                num.str("");
            }
        }
    }
    if (num.str().length() != 0) {
        tokens.push_back(num.str());
        num.str("");
    }
    return tokens;
}
