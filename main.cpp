#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <limits>
#include <algorithm>

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
void printResult(double res, bool isoperation) {

    if (isnan(res) || isinf(res)) {
        cout << setw(10) << "Error." << endl;
    } else {
        if (res != 0 && (fabs(res) < 0.0000001 || fabs(res) >= 100000000)) {
            stringstream stream;
            stream << scientific << setprecision(4) << res;
            string s = stream.str();
            size_t found = s.find('e');
            if (found != string::npos) {
                s.erase(found, 1); // remove 'e'
            }
            found = s.find('+');
            if (found != string::npos) {
                s.replace(found, 1, " "); // remove '+'
            }
            // Remove trailing zeros after the decimal point but leave non-zero digits
            size_t decimalPos = s.find('.');
            size_t exponentPos = s.find_first_of("- ", decimalPos);
            string decimalPart = s.substr(decimalPos + 1, exponentPos - decimalPos - 1);
            decimalPart.erase(decimalPart.find_last_not_of('0') + 1, string::npos);
            if (!decimalPart.empty()) {
                s = s.substr(0, decimalPos + 1) + decimalPart + s.substr(exponentPos);
            } else {
                s = s.substr(0, decimalPos + 1) + s.substr(exponentPos);
            }
            cout << setw(10) << s << endl;
        } else {

            stringstream stream;
            stream << fixed << setprecision(8) << res;
            string s = stream.str();

            if(isoperation == 1){
                size_t decimalPos = s.find('.');
                if (decimalPos != std::string::npos) {
                    size_t countDigits = 0;
                    size_t startPos = (s[0] == '-') ? 1 : 0; // Menentukan posisi awal untuk menghitung digit bilangan

                    for (size_t i = startPos; i < s.length(); ++i) {
                        if (s[i] != '.' && s[i] != '-') {
                            countDigits++;
                        }
                        if (countDigits == 8 && s[0] != '-') {
                            if (s[i + 1] >= '5') {
                                if(s[i] == '9') {
                                    s[i-1]++;
                                    s[i] = '0';
                                }
                                else s[i]++;
                            }
                            /*if (s[i + 1] >= '5') {
                                s[i]++;
                            }*/
                            s = s.substr(0, i + 1);
                            break;
                        }
                        if (countDigits == 9 && s[0] == '-') {
                            if (s[i + 1] >= '5') {
                                if(s[i] == 9) s[i-1]++;
                                else s[i]++;
                            }
                            s = s.substr(0, i + 1);
                            break;
                        }
                        }
                        s.erase(s.find_last_not_of('0') + 1, std::string::npos);
                        if (s.back() == '.') {
                            s.pop_back();
                        }
                        if (std::floor(res) == res) {
                            s += '.';
                        }
                        if (s.length() > 10) {
                            s = s.substr(0, 10);
                        }
                        cout << setw(10) << s << endl;
                    }
            }
            if(isoperation != 1){
                    size_t countDigits = 0;
                    size_t startPos = (s[0] == '-') ? 1 : 0;
                    for (size_t i = startPos; i < s.length(); ++i){
                        if (s[i] != '.' || s[i] != '-') {
                            countDigits++;
                        }
                        if (countDigits == 9) {
                            s = s.substr(0,  i + 1);
                            break;
                        }
                    }

                    s.erase(s.find_last_not_of('0') + 1, std::string::npos);
                    if (s.back() == '.') {
                        s.pop_back();
                    }
                    if (std::floor(res) == res) {
                        s += '.';
                    }

                    cout << setw(10) << s << endl;
            }
        }
    }
}




// Function to process the input and calculate the result.
void processTestcase(const vector<string>& tokens) {
    double number = 0;
    char operation = '+';  // Default operation
    double result = 0;
    bool hasDecimal = false;
    bool isoperation = false;
    double decimalFactor = 1;
    int digitCount = 0;
    bool isFinalEqual = false; // Flag to check if '=' is the final token

    for (size_t i = 0; i < tokens.size(); ++i) {
        const string& token = tokens[i];

        if (isdigit(token[1])) {
            if (digitCount < 8) {
                double digit = token[1] - '0';
                /*if (!hasDecimal && ) {
                    ;
                }*/
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
                digitCount++;
            }
        } else if (token[1] == '+' || token[1] == '-' || token[1] == 'x' || token[1] == ':') {
            if (number != 0 || hasDecimal) {
                if (operation == '=') {
                    result = number;
                } else {
                    result = calculate(result, number, operation);
                }
                number = 0;
                hasDecimal = false;
                isoperation = true;
                decimalFactor = 1;
                digitCount = 0;
            }
            operation = token[1];
        } else if (token[1] == '=') {
            if (operation == '=') {
                // Update result with the current number
                result = number;
            } else {
                // Perform calculation considering the previous number and operation
                result = calculate(result, number, operation);
            }
            number = 0;
            hasDecimal = false;
            decimalFactor = 1;
            digitCount = 0;

            // Check if this '=' is the final token
            isFinalEqual = (i == tokens.size() - 1);
            if (!isFinalEqual) {
                // Reset for next calculation if it's not the final token
                operation = '+';
            }
        }

        else {
            // Reset everything
            result = 0;
            number = 0;
            operation = '+';
            hasDecimal = false;
            decimalFactor = 1;
            digitCount = 0;
            isoperation = false;
            continue;
        }
    }

    if (isFinalEqual) {
        // Print the final result only if '=' is the final token
        printResult(result, isoperation);
    }
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
