#include "UBCalculator.h"
#include <sstream>

using std::stack;
using std::cout;
using std::endl;
using std::runtime_error;

const std::map<char, char> UBCalculator::_delimMatch = UBCalculator::createDelimMatch();
std::map<char, int> UBCalculator::_precedenceMap = UBCalculator::createPrecedenceMap();
const std::string UBCalculator::_openDelims = "([{";
const std::string UBCalculator::_closeDelims = ")]}";

// Get input from user 
void UBCalculator::setLine(std::string str) {
    _currentLine = str;
    _lex.set_input(_currentLine);
    _tokens = _lex.tokenize();
    if (_tokens.empty()) {
        throw Syntax_Error_Exception("No tokens found.");
    }

//    for (int i = 0; i < _tokens.size(); i++) {
//        _tok = _tokens[i];
//        switch (_tok.type) {
//            case IDENT:
//                cout << "IDENT:   " << _tok.value << endl;
//                break;
//            case DELIM:
//                cout << "DELIM:   " << _tok.value << endl;
//                break;
//            case OPERATOR:
//                cout << "OPERATOR:" << _tok.value << endl;
//                break;
//            case COMMENT:
//                cout << "COMMENT: " << _tok.value << endl;
//                break;
//            case NUMBER:
//                cout << "NUMBER:  " << _tok.value << endl;
//                break;
//            case STRING:
//                cout << "STRING:  " << _tok.value << endl;
//                break;
//            case ENDTOK:
//                cout << "EOF:     " << endl;
//                break;
//            case ERRTOK:
//                cout << "[ ERROR ]" << endl;
//                break;
//        }
//    }
    _lex.restart();
    _lineReady = true;
}


bool UBCalculator::isWellFormed() {
    if (_lineReady) {
        stack<string> operators;
        return true;


    } else throw Syntax_Error_Exception("Line not ready.");
}

void UBCalculator::setRpn() {
    stack<Token> operatorStack;
    stack<char> delimStack;
    string lastOperator("");
    int lastOperatorPrecedence(-1);

    if (!_postfixExp.empty())
        _postfixExp.clear();

    while (_lex.has_more_token()) {
        _tok = _lex.next_token();

        if (_tok.type == NUMBER) {
            _postfixExp.push_back(_tok);
            continue;
        }

        if (_tok.type == OPERATOR) {
            int currentOperatorPrecedence = _precedenceMap[_tok.value[0]];
            while (currentOperatorPrecedence <= lastOperatorPrecedence && lastOperatorPrecedence > -1) {
                _postfixExp.push_back(operatorStack.top());
                operatorStack.pop();
                if (!operatorStack.empty()) {
                    lastOperatorPrecedence = _precedenceMap[operatorStack.top().value[0]];
                } else {
                    lastOperatorPrecedence = -1;
                }
            }
            operatorStack.push(_tok);
            lastOperatorPrecedence = currentOperatorPrecedence;
        }

        if (_tok.type == DELIM) {
            if (_openDelims.find(_tok.value) != string::npos) {
                // value is open delim
                operatorStack.push(_tok);
                lastOperatorPrecedence = -1;
            } else if (_closeDelims.find(_tok.value) != string::npos) {
                // value is close delim
                while (_openDelims.find(operatorStack.top().value) == string::npos) {
                    _postfixExp.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                operatorStack.pop(); // pop the closing delim
                if (!operatorStack.empty()) lastOperatorPrecedence = _precedenceMap[operatorStack.top().value[0]];
                else lastOperatorPrecedence = -1;
            } else {
                // value is other
                throw Undefined_Operator_Exception("Not a valid delimiter");
            }
        }
    }

    while (!operatorStack.empty()) {
        _postfixExp.push_back(operatorStack.top());
        operatorStack.pop();
    }

//    for (int i = 0; i < _postfixExp.size(); i++) {
//        cout << _postfixExp[i].value << " ";
//    }
//    cout << endl;

}

double UBCalculator::rpnEval() {
    double result(0);
    size_t postfixLength(_postfixExp.size());

    stack<double> values;
    stack<char> opers;

    for (int i = 0; i < postfixLength; i++) {
        _tok = _postfixExp[i];
        if (_tok.type == NUMBER) {
            double realVal = std::strtod(_tok.value.c_str(), NULL);
            values.push(realVal);
        } else if (_tok.type == OPERATOR) {
            if (values.size() < 2)
                throw runtime_error("Invalid number of arguments for operator:" +
                                    _tok.value);
            double arg1, arg2;
            arg1 = values.top();
            values.pop();
            arg2 = values.top();
            values.pop();

            if (_tok.value.compare("+") == 0){
                values.push(sum(2, arg1, arg2));
            }

            if (_tok.value.compare("-") == 0){
                values.push(sum(2, neg(arg1), arg2));
            }

            if (_tok.value.compare("*") == 0){
                values.push(mult(2, arg1, arg2));
            }

            if (_tok.value.compare("/") == 0){
                values.push(div(2, arg2, arg1));
            }

        }

    }

    if (values.size() != 1)
        throw Syntax_Error_Exception("Malformed expression.");

    return values.top();
}


double UBCalculator::sum(int n, ...) {
    if (n < 1) return 0;

    double ret = 0.0, val;

    va_list vl;
    va_start(vl, n);
    for (int i = 0; i < n; i++) {
        val = va_arg(vl, double);
        ret += val;
    }
    va_end(vl);

    return ret;
}


double UBCalculator::mult(int n, ...) {
    if (n < 1) return 0;

    double ret = 1.0, val;

    va_list vl;
    va_start(vl, n);
    for (int i = 0; i < n; i++) {
        val = va_arg(vl, double);
        ret *= val;
    }
    va_end(vl);

    return ret;
}

double UBCalculator::div(int n, ...) {
    if (n < 1) return 0;

    double ret, val;

    va_list vl;
    va_start(vl, n);

    ret = va_arg(vl, double);
    for (int i = 1; i < n; i++) {
        val = va_arg(vl, double);
        ret /= val;
    }
    va_end(vl);

    return ret;
}

long double UBCalculator::neg(long double operand) {
    return -operand;
}

long double UBCalculator::inv(long double operand) {
    return 1 / operand;
}

