
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <queue>

using namespace std;

vector<string> tokenization(string equation);
queue<string> convert_to_postfix(vector<string> tokens);

int main()
{
    string equation;
    getline(cin, equation);

    vector<string> tokens = tokenization(equation);
    cout << "Tokens: ";
    for (int i = 0; i < tokens.size(); i++)
        cout << tokens[i] << ", ";
    cout << endl;

    cout << "Postfix: ";
    queue<string> operands = convert_to_postfix(tokens);
    queue<string> operands_print(operands);
    for (int i = 0; i < operands.size(); i++) {
        cout << operands_print.front() << " ";
        operands_print.pop();
    }
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
queue<string> convert_to_postfix(vector<string> tokens) {
    stack<string> oper;
    queue<string> operands;
    for (int i = 0; i < tokens.size(); i++) {
        if (string("*/%").find(tokens[i]) != string::npos) {
            if (oper.size() != 0 && string("!^*/%").find(oper.top()) != string::npos) {
                operands.push(oper.top());
                oper.pop();
            }
            oper.push(tokens[i]);
        }
        else if (string("+-").find(tokens[i]) != string::npos) {
            if (oper.size() != 0) {
                while (oper.size() != 0 && oper.top() != "(") {
                    operands.push(oper.top());
                    oper.pop();
                }
            }
            oper.push(tokens[i]);
        }
        else if (string("(^!").find(tokens[i]) != string::npos) {
            oper.push(tokens[i]);
        }
        else if (oper.size() != 0 && oper.top() == ")") {
            //if (oper.size() != 0) {
            while (oper.size() > 0 && oper.top() != "(") {
                operands.push(oper.top());
                oper.pop();
            }
            oper.pop();
            //}
        }
        else if (isdigit(tokens[i][0])) {
            operands.push(tokens[i]);
        }
    }
    while (oper.size() != 0) {
        operands.push(oper.top());
        oper.pop();
    }
    return operands;
}
