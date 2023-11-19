#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

// Function to calculate the result based on the operation
double calculate(double result, double number, char operation) {
    switch (operation) {
        case '+':
            return result + number;
        case '-':
            return result - number;
        case 'x':
            return result * number;
        case ':':
            return result / number;
        default:
            return result;
    }
}

// Function to print the result
void printResult(double res) {
    if (isnan(res) || isinf(res)) {
        cout << "Error." << endl;
    } else {
        if (res != 0 && (fabs(res) < 0.000001 || fabs(res) >= 100000000)) {
            cout << scientific << setprecision(4) << uppercase << left << res << endl;
        } else {
            stringstream stream;
            stream << fixed << setprecision(7) << res;
            string s = stream.str();
            s.erase(s.find_last_not_of('0') + 1, string::npos);
            if (s.back() == '.') {
                s.pop_back();
            }
            if (floor(res) == res) {
                s += '.';
            }
            if (s.length() > 10) {
                s = s.substr(0, 10);
            }
            cout << left << s << endl;
        }
    }
}



// Function to process the input and calculate the result.
void processTestcase(const vector<string>& tokens) {
    double number = 0;
    bool hasDecimal = false;
    char operation = '+';
    double result = 0;
    double decimalFactor = 1;
    int digitCount = 0;
    bool foundEqual = false;

    for (const string& token : tokens) {
        if (token == "[C]") {
            result = 0;
            number = 0;
            operation = '+';
            hasDecimal = false;
            decimalFactor = 1;
            digitCount = 0;
            foundEqual = false;
            continue;
        }

        if (isdigit(token[1])) {
            if (digitCount < 8) {
                double digit = token[1] - '0';
                if (hasDecimal) {
                    decimalFactor *= 0.1;
                    number += digit * decimalFactor;
                } else {
                    number = number * 10 + digit;
                }
                if (!hasDecimal) {
                    digitCount++;
                }
            }
        } else if (token[1] == '.') {
            if (!hasDecimal) {
                hasDecimal = true;
                decimalFactor = 1;
                digitCount = 0;
            }
        } else if (token[1] == '+' || token[1] == '-' || token[1] == 'x' || token[1] == ':') {
            if (number != 0 || hasDecimal) {
                result = calculate(result, number, operation);
                number = 0;
                hasDecimal = false;
                decimalFactor = 1;
                digitCount = 0;
                foundEqual = false;
            }
            operation = token[1];
        } else if (token[1] == '=') {
            if (!foundEqual) {
                result = calculate(result, number, operation);
                foundEqual = true;
            }
        }
    }

    if (!foundEqual) {
        result = calculate(result, number, operation);
    }
    printResult(result);
}


int main() {
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line after the integer input.

    for (int i = 0; i < n; ++i) {
        string line, token;
        getline(cin, line);
        stringstream ss(line);

        vector<string> tokens;
        while (ss >> token) {
            tokens.push_back(token);
        }

        processTestcase(tokens);
    }

    return 0;
}
