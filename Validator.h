#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>
#include <string>
#include "Colors.h"

using namespace std;

class Validation {
private:
    static bool isInteger(string value, int& number, int length);
    static bool isStringExactLength(string value,string& re,int length);
    static bool isString(string value,string& re);
    static bool isFloat(const string& value, float& number, int maxDigits);
    static bool isPassword(string value,string& re,int length);
public:

    static int inputInteger(int length, string prompt);
    static string inputString(int maxLength,string prompt);
   static string inputStringExactLength(int length,string prompt);
    static float inputFloat(int maxDigits, const string& prompt);
    static void checkPriority(int& priority);
    static string checkPassword(int length,string prompt);
    static int inputIntegerUpto(int limit, string prompt);
    static bool isIntegerUpto(string value, int& number, int limit);
    static string toLowerCase(string str);
};

#endif