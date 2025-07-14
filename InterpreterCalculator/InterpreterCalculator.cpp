
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
auto solve = [=](string equation) {return calculate(convert_to_postfix(tokenization(equation)));};
vector<string> get_parameters(string s);

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
    equation.erase(remove(equation.begin(), equation.end(), ' '), equation.end());
    string numbers = "1234567890.";
    string operators = "!()+-*/^";
    stringstream num;
    vector<string> tokens;
    for (int i = 0; i < equation.length(); i++) {
        if (numbers.find(equation[i]) != string::npos) {
            if (equation[i] == '.') {
                if (num.str().length() != 0) num << equation[i];
                else {
                    cerr << "Error in the input at symbol " << i << "!";
                    exit(0);
                }
            }
            else num << equation[i];
        }
        else if (operators.find(equation[i]) != string::npos) {
            if (i == 0 || (string("!+-*/^").find(equation[i - 1]) != string::npos && string("!+-*/^").find(equation[i]) != string::npos)) {
                cerr << "Error in the input at symbol " << i << "!";
                exit(0);
            }
            if (num.str().length() != 0) {
                tokens.push_back(num.str());
                num.str("");
            }
            tokens.push_back(string{ equation[i] });
        }
        else if (equation[i] == 'm') {
            if (!((equation[i + 1] == 'a' && equation[i + 2] == 'x') || (equation[i + 1] == 'i' && equation[i + 2] == 'n'))) {
                cout << "Input error, index: " << i;
                exit(0);
            }
            string s = equation.substr(i);
            string parameters_str = s.substr(s.find('(') + 1, s.find(')') - s.find('(') - 1);
            vector<string> parameters = get_parameters(parameters_str);
            if (parameters.size() != 2) {
                cout << "Error in func min/max parameters";
                exit(0);
            }
            double num1 = solve(parameters[0]);
            double num2 = solve(parameters[1]);
            if (equation[i + 1] == 'a' && equation[i + 2] == 'x') tokens.push_back(to_string(max(num1, num2)));
            else if (equation[i + 1] == 'i' && equation[i + 2] == 'n') tokens.push_back(to_string(min(num1, num2)));
            i += s.find(')');
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
        if (string("*/").find(tokens[i]) != string::npos) {
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
        else if (oper.size() != 0 && tokens[i] == ")") {
            while (oper.size() > 0 && oper.top() != "(") {
                operands.push(oper.top());
                oper.pop();
            }
            oper.pop();
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
    if (operands.size() == 1) return stod(operands.front());
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
vector<string> get_parameters(string s) {
    vector<string> parameters;
    stringstream ss(s);
    while (ss.good()) {
        string param;
        getline(ss, param, ',');
        parameters.push_back(param);
    }
    return parameters;
}