// UBCalcDriver.cpp : a simple driver for the the Lexer class
#include <cstdlib>
#include "UBCalculator.h"
#include "Lexer.h"

using std::cout;
using std::cin;
using std::endl;
using std::runtime_error;

int main() {
    Lexer lexer;
    Token tok;
    UBCalculator calculator(lexer,tok);
    string line;

    while (cin) {
        cout << ">>> ";
        getline(cin, line);
        if ((line == "quit") || (line == "exit")) {
            exit(0);
        }
        if (line == "") {
            continue;
        }
        try {

//            calculator.setLine("a=b=c=3");
////            calculator.setLine("clear b");
//            calculator.setLine("a+b+c");
//            calculator.setLine("b");
//            calculator.setLine("c");
//            calculator.setLine("d");

            calculator.setLine(line);
        } catch (runtime_error &e) {
            error_return(e.what());
        }
    }

    return 0;
}
