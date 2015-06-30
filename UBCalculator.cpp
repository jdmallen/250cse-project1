#include "UBCalculator.h"
#include <sstream>

using std::stack;
using std::cout;
using std::endl;
using std::runtime_error;
using std::ostringstream;

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

    isWellFormed();

    double result = setRpn();

    cout << ">>> " << result << endl;
}


void UBCalculator::isWellFormed() {
    if (_lineReady) {
        stack<string> operators;
        vector<Token> tokens = _lex.tokenize();
        Token lastTok;
        _lex.restart();

        if (tokens.size() == 0)
            throw runtime_error("Not well-formed: nothing to check!");

        if (_closeDelims.find(tokens[0].value) != string::npos){
            throw Syntax_Error_Exception("Cannot start with closing parenthesis.");
        }

        for (int i = 0; i < tokens.size(); i++){
            if (tokens[i].type == DELIM  && _openDelims.find(tokens[i].value) != string::npos){
                operators.push(tokens[i].value);
                continue;
            }
            if (tokens[i].type == DELIM  && _closeDelims.find(tokens[i].value) != string::npos){
                char topOfStack = (operators.top())[0];
                char closingParen = _delimMatch.find(topOfStack)->second;
                if (closingParen != tokens[i].value[0])
                    throw Syntax_Error_Exception("Malformed expression: mismatched parentheses.");
                else operators.pop();
            }
        }

        if (!operators.empty())
            throw Syntax_Error_Exception("Malformed expression: not all parentheses closed.");

    } else throw Syntax_Error_Exception("Line not ready.");
}

double UBCalculator::setRpn() {
    stack<Token> operatorStack;
    Token lastTok;
    bool clearNextIdent(false), negateNext(false);
    int lastOperatorPrecedence(-1);
    int negCount(0), posCount(0);

    if (_currentLine.find("=") != string::npos)
        setVar();

    if (!_postfixExp.empty())
        _postfixExp.clear();

    while (_lex.has_more_token()) {
        _tok = _lex.next_token();

        if (_tok.type == COMMENT) {
            throw Syntax_Error_Exception("Comments not allowed.");
        }

        if (_tok.type == IDENT) {
            if (_tok.value.size() > 1 && _tok.value != "clear" && _tok.value != "all")
                throw Syntax_Error_Exception("No idea what you're trying to say. Maybe clear?");

            if (_tok.value == "clear" && _lex.has_more_token()) {
                clearNextIdent = true;
                lastTok = _tok;
                continue;
            } else if (_tok.value == "clear" && !_lex.has_more_token())
                throw Syntax_Error_Exception("Usage: \"clear all\" or \"clear <var>\".");

            if (_tok.value == "all" && clearNextIdent && !_lex.has_more_token()) {
                _varMap.clear();
                cout << ">>> All variables cleared." << endl;
                lastTok = _tok;
                break;
            }

            if (_varMap.find(_tok.value) == _varMap.end()) {
                throw Undefined_Variable_Exception("Variable not defined: " + _tok.value);
            } else {
                if (clearNextIdent){
                    _varMap.erase(_tok.value);
                    cout << ">>> Variable " + _tok.value + " cleared." << endl;
                    lastTok = _tok;
                    break;
                }
                Token varValue = _varMap[_tok.value];
                if (varValue.type != ENDTOK) { // if no value for var yet-- ignore
                    _postfixExp.push_back(varValue);
                }
                lastTok = _tok;
                continue;
            }

        }

        if (_tok.type == NUMBER) {
            if (negCount % 2 == 1 /*&& negateNext*/){
                _tok.value.insert(0,"-");
                negCount = 0;
                posCount = 0;
//                negateNext = false;
            }
            if (lastTok.type == OPERATOR && lastTok.value == "/" && _tok.value == "0")
                throw DBZ_Exception();
            _postfixExp.push_back(_tok);
            lastTok = _tok;
            continue;
        }

        if (_tok.type == OPERATOR && _tok.value != "=" && _tok.value != "+" && _tok.value != "-") {
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
            lastTok = _tok;
            continue;
        }

        if (_tok.type == OPERATOR && (_tok.value == "+" || _tok.value == "-")) {
            int currentOperatorPrecedence = _precedenceMap[_tok.value[0]];
            while (currentOperatorPrecedence <= lastOperatorPrecedence && lastOperatorPrecedence > -1 && lastTok.value != _tok.value) {
                _postfixExp.push_back(operatorStack.top());
                operatorStack.pop();
                if (!operatorStack.empty()) {
                    lastOperatorPrecedence = _precedenceMap[operatorStack.top().value[0]];
                } else {
                    lastOperatorPrecedence = -1;
                }
            }
            if (lastTok.type == NUMBER || lastTok.type == IDENT){
                operatorStack.push(_tok);
                lastOperatorPrecedence = currentOperatorPrecedence;
            } else if (_tok.value == "-") {
//                negateNext = true;
                negCount++;
            }
            if (_tok.value == "+") posCount++;

            lastTok = _tok;
            continue;
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
            lastTok = _tok;
            continue;
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
    return (!_postfixExp.empty()) ? rpnEval() : 0.0;
}

void UBCalculator::setVar() {
    _lex.restart();
    vector<Token> lineTokens = _lex.tokenize();
    _lex.restart();
    Token valueTok;
    bool valueReached = false;
    if (lineTokens.empty())
        throw Syntax_Error_Exception("No valid tokens!");

    if (lineTokens[0].type != IDENT)
        throw Syntax_Error_Exception("Cannot assign to non-ident entries.");

    for (int i = 0; i < lineTokens.size(); i++) {
        if (lineTokens[i].type == IDENT && lineTokens[i].value.size() > 1)
            throw Syntax_Error_Exception("Single character variables only, please.");
        if (i == lineTokens.size() - 1 && lineTokens[i].value == "=")
            throw Syntax_Error_Exception("Cannot end with '='.");
//        if (lineTokens[i].type == OPERATOR && lineTokens[i].value != "=")
//            throw Syntax_Error_Exception("Expecting '=' for assignment operator.");
        if (lineTokens[i].type == IDENT)
            _variables.push_back(lineTokens[i].value);
        if (lineTokens[i].type == NUMBER || (lineTokens[i].type == OPERATOR && lineTokens[i].value != "=")) {
            break;
        }
    }

    for (int i = 0; i < _variables.size(); i++) {
        _varMap[_variables[i]] = valueTok;
    }
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

            if (_tok.value.compare("+") == 0) {
                values.push(sum(2, arg1, arg2));
            }

            if (_tok.value.compare("-") == 0) {
                values.push(sum(2, neg(arg1), arg2));
            }

            if (_tok.value.compare("*") == 0) {
                values.push(mult(2, arg1, arg2));
            }

            if (_tok.value.compare("/") == 0) {
                values.push(div(2, arg2, arg1));
            }

        }

    }

    if (values.size() != 1)
        throw Syntax_Error_Exception("Malformed expression.");

    ostringstream strResult;
    strResult << values.top();
    Token resultTok(NUMBER, strResult.str());
    for (int i = 0; i < _variables.size(); i++) {
        _varMap[_variables[i]] = resultTok;
    }

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

double UBCalculator::neg(double operand) {
    return -operand;
}

double UBCalculator::inv(double operand) {
    return 1 / operand;
}

