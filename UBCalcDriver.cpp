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
            calculator.setLine(line);
//            calculator.setLine("24+4-15*8/(2+5*2-8)+4");
//            calculator.setLine("24+4-15*8/4");
//            calculator.setLine("((((5*2)/4)*6)*4)/5");
            calculator.setRpn();
            cout << ">>> " << calculator.rpnEval() << endl;
        } catch (runtime_error &e) {
            error_return(e.what());
        }
    }

    return 0;
}
