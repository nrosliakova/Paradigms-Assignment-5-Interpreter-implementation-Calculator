
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include <queue>
#include <map>

using namespace std;

vector<string> tokenization(string equation);
queue<string> convert_to_postfix(vector<string> tokens);
double calculate(queue<string> operands);
vector<string> get_parameters(string s, int& last_par_pos);
map<string, string> variables;
auto solve = [=](string equation) {return calculate(convert_to_postfix(tokenization(equation)));};

int main()
{
    string equation;
    getline(cin, equation);
    equation.erase(remove(equation.begin(), equation.end(), ' '), equation.end());
    while (equation.find("var") == 0) {
        string v = equation.substr(3, equation.find('=') - 3);
        string value = to_string(solve(equation.substr(equation.find('=') + 1)));
        variables[v] = value;
        getline(cin, equation);
        equation.erase(remove(equation.begin(), equation.end(), ' '), equation.end());
    }
    cout << "result = " << solve(equation);
    //vector<string> tokens = tokenization(equation);
    //cout << "Tokens: ";
    //for (int i = 0; i < tokens.size(); i++)
    //    cout << tokens[i] << ", ";
    //cout << endl;

    //cout << "Postfix: ";
    //queue<string> operands = convert_to_postfix(tokens);
    //queue<string> operands_print(operands);
    //for (int i = 0; i < operands.size(); i++) {
    //    cout << operands_print.front() << " ";
    //    operands_print.pop();
    //}
    //cout << endl;
    //cout << "result = " << calculate(operands);
}

vector<string> tokenization(string equation) {
    equation.erase(remove(equation.begin(), equation.end(), ' '), equation.end());
    string numbers = "1234567890.";
    string operators = "!()+-*/^";
    string var_symb = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    stringstream num;
    bool var = false;
    vector<string> tokens;
    for (int i = 0; i < equation.length(); i++) {
        if (numbers.find(equation[i]) != string::npos) {
            if (equation[i] == '.') {
                if (num.str().length() != 0) num << equation[i];
                else {
                    cout << "Error in the input at symbol " << i << "!";
                    exit(0);
                }
            }
            else num << equation[i];
        }
        else if (operators.find(equation[i]) != string::npos) {
            if ((i == 0 && equation[i]!='(') || (i>0 && string("!+-*/^").find(equation[i - 1]) != string::npos && string("!+-*/^").find(equation[i]) != string::npos)) {
                cout << "Error in the input at symbol " << i << "!";
                exit(0);
            }
            if (num.str().length() != 0) {
                if (var) {
                    if (variables.find(num.str()) != variables.end()) {
                        //tokens.push_back(num.str());
                        tokens.push_back(variables[num.str()]);
                        num.str("");
                        var = false;
                    }
                    else {
                        cout << "Error unknown variable";
                        exit(0);
                    }
                }
                else {
                    tokens.push_back(num.str());
                    num.str("");
                }
            }
            tokens.push_back(string{ equation[i] });
        }
        else if (equation[i] == 'm') {
            if (!((equation[i + 1] == 'a' && equation[i + 2] == 'x') || (equation[i + 1] == 'i' && equation[i + 2] == 'n'))) {
                cout << "Input error, index: " << i;
                exit(0);
            }
            string s = equation.substr(i);
            int last_par_pos = 0;
            vector<string> parameters = get_parameters(s.substr(s.find('(')), last_par_pos);
            if (parameters.size() != 2) {
                cout << "Error in func min/max parameters";
                exit(0);
            }
            double num1 = solve(parameters[0]);
            double num2 = solve(parameters[1]);
            if (equation[i + 1] == 'a' && equation[i + 2] == 'x') tokens.push_back(to_string(max(num1, num2)));
            else if (equation[i + 1] == 'i' && equation[i + 2] == 'n') tokens.push_back(to_string(min(num1, num2)));
            i = i + s.find('(') + last_par_pos;
        }
        else if (var_symb.find(equation[i]) != string::npos) {
            var = true;
            num << equation[i];
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
vector<string> get_parameters(string s, int& last_par_pos) {
    vector<string> parameters;
    stringstream ss;
    int level = 1;
    bool all_closed = false;
    bool cur_closed = true;
    int i = 1;
    while (!all_closed) {
        if (i >= s.length()) {
            cout << "closed parenthesis was not found.";
            exit(0);
        }
        char c = s[i];
        if (c == '(') {
            cur_closed = false;
            level++;
        }
        else if (c == ')') {
            cur_closed = true;
            level--;
        }
        else if (c == ',' && cur_closed) {
            parameters.push_back(ss.str());
            ss.str("");
            i++;
            continue;
        }
        all_closed = (level == 0);
        if (all_closed) {
            parameters.push_back(ss.str());
            last_par_pos = i;
        }
        ss << c;
        i++;
    }
    return parameters;
}