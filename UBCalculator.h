// ============================================================================ 
// Calculator.h
// ~~~~~~~
// author    : Tamal T. Biswas
// - a simple calculator class
// - The class can evaluate any infix expression. 
// ============================================================================ 

#ifndef _UBCALCULATOR_H_
#define _UBCALCULATOR_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <cstdlib>
#include <cstdarg>
#include "Lexer.h"
#include "error_handling.h"

using std::map;
using std::string;
using std::vector;
using std::stack;


enum operation {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    NEGATE
};

/**
 * -----------------------------------------------------------------------------
 * the Calculator class:
 * - take a string to be scanned
 * - scan for tokens and return one at a time
 * -----------------------------------------------------------------------------
 */
class UBCalculator {
public:
    UBCalculator(Lexer lex, Token tok) : _lex(lex), _tok(tok), _lineReady(false){};

    void setLine(string); // Get input from user
    void isWellFormed(); // check line is well-formed
    double setRpn();
    double rpnEval();
    void setVar();

    double sum(int n, ...); // add and subtract
    double mult(int n, ...); // multiply
    double div(int n, ...); // divide (first argument is dividend, n > 0)

    double neg(double operand); // negate
    double inv(double operand); // invert



private:
    static map<char, char> createDelimMatch() {
        map<char, char> m;
        m['('] = ')';
        m['{'] = '}';
        m['['] = ']';
        return m;
    }

    static map<char, int> createPrecedenceMap() {
        map<char, int> m;
        m['+'] = 0;
        m['-'] = 0;
        m['*'] = 1;
        m['/'] = 1;
        m['@'] = 2; // unary minus
        m['('] = -1;
        m['{'] = -1;
        m['['] = -1;

        return m;
    }

    static map<char, int> _precedenceMap;
    const static map<char, char> _delimMatch;
    const static string _openDelims;
    const static string _closeDelims;

    map<string,Token> _varMap;
    string _currentLine;
    Lexer _lex;
    Token _tok;
    vector<Token> _tokens;
    bool _lineReady;
    vector<Token> _postfixExp;
    vector<string> _variables;

};

#endif
