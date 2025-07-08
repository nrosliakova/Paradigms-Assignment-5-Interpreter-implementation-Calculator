
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <queue>

using namespace std;

vector<string> tokenization(string equation);
queue<string> convert_to_postfix(vector<string> tokens);
double calculate(queue<string> operands);

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
    cout << "result = " << calculate(operands);
}

vector<string> tokenization(string equation) {
    string numbers = "1234567890";
    string operators = "!()+-*/^";
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
        //if (string("*/%").find(tokens[i]) != string::npos) {
        if (string("*/").find(tokens[i]) != string::npos) {
            //if (oper.size() != 0 && string("!^*/%").find(oper.top()) != string::npos) {
            if (oper.size() != 0 && string("!^*/").find(oper.top()) != string::npos) {
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
double calculate(queue<string> operands) {
    stack<string> calculation_stack;
    while (operands.size() > 0) {
        while (string("!()+-*/^").find(operands.front()) == string::npos) {
            calculation_stack.push(operands.front());
            operands.pop();
        }
        string operation = operands.front();
        operands.pop();
        double num1, num2;
        if (operation == "!") {
            num1 = stod(calculation_stack.top());
            calculation_stack.pop();
            double res = 1;
            for (int i = 1; i <= res;i++) 
                res *= i;
            calculation_stack.push(to_string(res));

        }
        else {
            num1 = stod(calculation_stack.top());
            calculation_stack.pop();
            num2 = stod(calculation_stack.top());
            calculation_stack.pop();
            
            if (operation == "+")
                calculation_stack.push(to_string(num2 + num1));
            else if (operation == "-")
                calculation_stack.push(to_string(num2 - num1));
            else if (operation == "*")
                calculation_stack.push(to_string(num2 * num1));
            else if (operation == "/")
                calculation_stack.push(to_string(num2 / num1));
            else if (operation == "^")
                calculation_stack.push(to_string(pow(num2, num1)));
        }

    }
    return stod(calculation_stack.top());
}
