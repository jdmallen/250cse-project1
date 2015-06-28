// UBCalcDriver.cpp : a simple driver for the the Lexer class
#include <cstdlib>
#include "UBCalculator.h"
#include "Lexer.h"

using std::cout;
using std::endl;

int main() {
	UBCalculator calculator;
	Lexer lexer;
	Token tok;
	string line;

    vector<long double> input;
    input.push_back(42);
    input.push_back(7);
    input.push_back(3);

    cout << calculator.sum(input) << endl;
    cout << calculator.mult(input) << endl;
    cout << calculator.div(input) << endl;

//    while (cin) {
//        cout << ">>> ";
//		getline(cin, line);
//		if ((line == "quit") || (line == "exit"))
//		{
//			exit(0);
//		}
//		if (line == "")
//		{
//			continue;
//		}
//		try{
//			calculator.setLine(line);
//		}
//		catch (runtime_error &e) {
//			error_return(e.what());
//		}
//
//    }
    return 0;
}
