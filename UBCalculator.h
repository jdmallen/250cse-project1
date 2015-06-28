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
#include <cstdarg>
#include "Lexer.h"
#include "error_handling.h"

using std::map;
using std::string;
using std::vector;


enum operation {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE
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
    // constructor
    UBCalculator() { };

    // a couple of modifiers
    void setLine(string); // Get input from user
    long double sum(long double oper1, long double oper2);
    long double sum(vector<long double> operands);
    long double mult(long double oper1, long double oper2);
    long double mult(vector<long double> operands);
    long double div(long double dividend, long double divisor);
    long double div(vector<long double> operands);

    long double neg(long double operand);
    long double inv(long double operand);
private:
    static map<char, char> createDelimMatch() {
        map<char, char> m;
        m[')'] = '(';
        m['}'] = '{';
        m[']'] = '[';
        return m;
    }

    static map<char, int> createPrecedenceMap() {
        map<char, int> m;
        m['+'] = 0;
        m['-'] = 0;
        m['*'] = 1;
        m['/'] = 1;
        m['('] = -1;
        m['{'] = -1;
        m['['] = -1;

        return m;
    }

    static map<char, int> precedenceMap;
    const static map<char, char> delimMatch;
    const static string openDelims;
    const static string closeDelims;

//    map vars;
    string currentLine;

};

#endif
