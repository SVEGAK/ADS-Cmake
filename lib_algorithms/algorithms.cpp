// Copyright 2024 Marina Usova

#include <stdexcept>
#include "../lib_algorithms/algorithms.h"

float division(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Input Error: can't divide by zero!");
    }
    return static_cast<float>(a) / b;
}

bool check_breckets(const std::string& expression)
{
    if (expression.empty()) {
        throw std::invalid_argument("Input Error: empty string");
    }
    TStackOnMass<char> Stack(expression.length());

    for (size_t i = 0; i < expression.length(); i++) {
        if ((expression[i] == '(') || (expression[i] == '[') || (expression[i] == '{')) {
            Stack.push(expression[i]);
        }
        else if ((expression[i] == ')') || (expression[i] == ']') || (expression[i] == '}')) {
            if (Stack.is_empty()) {
                return false;
            }
            if ((Stack.top() == '(')&&(expression[i] == ')')|| 
                (Stack.top() == '[') && (expression[i] == ']')|| 
                (Stack.top() == '{') && (expression[i] == '}')) {
                Stack.pop();
                continue;
            }
            else {
                return false;
            }

        }
    }
    if (Stack.is_empty()) {
        return true;
    }
    return false;
}
