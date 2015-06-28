#include "UBCalculator.h"
#include <sstream>

const std::map<char, char> UBCalculator::delimMatch = UBCalculator::createDelimMatch();
std::map<char, int> UBCalculator::precedenceMap = UBCalculator::createPrecedenceMap();
const std::string UBCalculator::openDelims = "([{";
const std::string UBCalculator::closeDelims = ")]}";

// Get input from user 
void UBCalculator::setLine(std::string str) {
    currentLine = str;
}

long double UBCalculator::sum(long double oper1, long double oper2) {
    vector<long double> operands;
    operands.push_back(oper1);
    operands.push_back(oper2);
    return sum(operands);
}

//long double UBCalculator::sum(vector<long double> operands) {
//    long double ret;
//    ret = 0.0;
//    for (int i = 0; i < operands.size(); i++){
//        ret += operands[i];
//    }
//    return ret;
//}

long double UBCalculator::mult(long double oper1, long double oper2) {
    vector<long double> operands;
    operands.push_back(oper1);
    operands.push_back(oper2);
    return mult(operands);
}

//long double UBCalculator::mult(vector<long double> operands) {
//    long double ret;
//    ret = 1.0;
//    for (int i = 0; i < operands.size(); i++){
//        ret *= operands[i];
//    }
//    return ret;
//}

long double UBCalculator::div(long double dividend, long double divisor) {
    vector<long double> operands;
    operands.push_back(dividend);
    operands.push_back(divisor);
    return div(operands);
}

//long double UBCalculator::div(vector<long double> operands) {
//    long double ret;
//    ret = operands[0];
//    for (int i = 1; i < operands.size(); i++){
//        ret /= operands[i];
//    }
//    return ret;
//}

long double UBCalculator::neg(long double operand) {
    return -operand;
}

long double UBCalculator::inv(long double operand) {
    return 1/operand;
}
