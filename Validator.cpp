#include "Validator.h"
#include<iostream>
#include"Colors.h"

using namespace std;

bool Validation::isInteger(string value, int& number, int length)
{
    bool isNumber = true;
    int counter = 0;

    while (counter<value.length())
    {
        if (value[counter] >= '0' && value[counter] <= '9')
        {
            counter++;
        }
        else
        {
            isNumber = false;
            break;
        }
    }


    if (counter == length && isNumber)
    {
        number = stoi(value); 
        return true;
    }
    else {
        cout << RED << " Invalid input! Please enter a " << length << "-digit numeric value." << RESET << endl;
        return false;
    }
}



bool Validation::isStringExactLength(string value,string& re,int length){
    int counter=0;
    bool isString=true;
    while(counter<value.length()){
        if((value[counter]>='a' && value[counter]<='z') || (value[counter]>='A' && value[counter]<='Z')){
            counter++;
        }
        else{
            isString=false;
            break;
        }

    }
    if(counter==length && isString){
        re=value;
       return true;
    }
    else {
        cout << RED << " Invalid input! Please enter a " << length << "-string  value." << RESET << endl;
        return false;
    }
    
}


bool Validation::isString(string value,string& re){
      int counter=0;
      bool isStr=true;
      while(counter<value.length()){
         if((value[counter]>='a' && value[counter]<='z') || (value[counter]>='A' && value[counter]<='Z')){
            counter++;
        }
        else{
            isStr=false;
            break;
        }
      }

      if(counter==value.length()&& isStr){
        re=value;
        return true;
      }
       else {
        cout << RED << " Invalid input! Please enter a string " << RESET << endl;
        return false;
    }
}

int Validation::inputInteger(int length, string prompt) {
    int n;
    string input;
    do {
        cout << prompt;
        getline(cin, input);
    } while (!Validation::isInteger(input, n, length));
    
    return n;
}


string Validation::inputString(int maxLength,string prompt){
     string n;
    string input;
    do
    {
      cout<<prompt<<endl;
      getline(cin,input);
      if (input == "0") return "CANCEL_INPUT";
      while(input.length()<=0 || input.length()>maxLength){
        cout << YELLOW << "  » Size Constraint: " << WHITE << "[" << RESET 
     << GREEN << "1 to " << maxLength << RESET 
     << WHITE << "]" << YELLOW << " characters required." << RESET << endl;
        getline(cin,input);
      }

    } while (!Validation::isString(input,n));
    return n;
}


string Validation::inputStringExactLength(int length,string prompt ){
  
     string n;
    string input;
    do{
        cout<<prompt<<endl;
        getline(cin,input);
    }while(!Validation::isStringExactLength(input,n,length));
    return n;
}

bool Validation::isFloat(const std::string& value, float& number, int maxDigits) {
    bool isValid = true;
    int dotCount = 0;
    int digitCount = 0;

    for (char ch : value) {
        if (ch >= '0' && ch <= '9') {
            digitCount++;
        } 
        else if (ch == '.') {
            dotCount++;
            if (dotCount > 1) {  
                isValid = false;
                break;
            }
        } 
        else {
            isValid = false;  
            break;
        }
    }

    if (digitCount > maxDigits) {
        isValid = false;
    }

    if (isValid) {
        number = std::stof(value);
        return true;
    } else {
        cout << "\033[31m Invalid input! Enter a valid float with up to " 
                  << maxDigits << " digits.\033[0m" << std::endl;
        return false;
    }
}

float Validation::inputFloat(int maxDigits, const std::string& prompt) {
    float n;
    std::string input;

    do {
        cout << prompt;
        if (!(cin >> std::ws)) return 0.0f; 
        
        std::getline(cin, input);

        if (input.empty()) {
            std::cout << RED << "[!] Input cannot be empty." << RESET << std::endl;
            continue;
        }

    } while (!Validation::isFloat(input, n, maxDigits));

    return n;
}

  void Validation::checkPriority(int& priority){
    while(priority<0 || priority>3){
        cout<<"Enter priority between 1 and 3  1-OverNight 2- 2days 3-Normal  "<<endl;
          priority=inputInteger(1," ");
    }
    return ;
  }


  bool Validation::isPassword(string value,string& re,int length){
    bool containNumber=false;
    bool containSymbol=false;
    bool containeUpperCase=false;
    bool containLowerCase=false;
    int counter=0;
    while(counter<value.length()){
        if(value[counter]=='@' || value[counter]=='&' || value[counter]=='#' || value[counter]=='!'){
            containSymbol=true;
        }
        if(value[counter]>='0' && value[counter]<='9' ){
            containNumber=true;
        }
        if(value[counter]>='a'  && value[counter]<='z'){
            containLowerCase=true;
        }

        if(value[counter]>='A' && value[counter]<='Z'){
            containeUpperCase=true;
        }
   counter++;
        
    }
 
    if(containNumber && containLowerCase && containeUpperCase && containSymbol){
        if(value.length() == length){
            re = value;
            return true;
        } else {
            cout << "Enter the password of exactly " << length << " characters\n";
            return false;
        }
    } else {
        cout << "Password must contain at least:\n"
             << "1 Uppercase, 1 Lowercase, 1 Number, 1 Special (@ & # !)\n";
        return false;
    }
  }


  string Validation:: checkPassword(int length,string prompt){
    string n;
    string input;
    do
    {
       cout<<prompt<<endl;
       getline(cin,input);
    } while (!Validation::isPassword(input,n,length));
    return n;
  }





 bool Validation::isIntegerUpto(string value, int& number, int maxLength) {
    bool isNumber = true;
    int counter = 0;

    // Guard: Prevent empty input
    if (value.length() == 0) return false;

    // Check if every character is a digit
    while (counter < value.length()) {
        if (value[counter] >= '0' && value[counter] <= '9') {
            counter++;
        } else {
            isNumber = false;
            break;
        }
    }

    // Logic: Length must be > 0 and <= maxLength
    if (isNumber && value.length() <= maxLength) {
        number = stoi(value); 
        return true;
    } 
    else if (!isNumber) {
        cout << RED << BOLD << "[!] FORMAT ERROR: " << RESET 
             << YELLOW << "Numeric digits only, please." << RESET << endl;
        return false;
    }
    else {
        cout << RED << BOLD << "[!] LENGTH ERROR: " << RESET 
             << YELLOW << "Input must be between 1 and " << maxLength << " digits long." << RESET << endl;
        return false;
    }
}




int Validation::inputIntegerUpto(int maxLength, string prompt) {
    int n;
    string input;
    
    do {
        cout << WHITE << prompt << RESET << GRAY << " (Max " << maxLength << " digits, or 0 to go back): " << RESET;
        getline(cin, input);

        // Escape Hatch: Returns -1 to return to the menu
        if (input == "0") {
            return -1; 
        }

    } while (!Validation::isIntegerUpto(input, n, maxLength));
    
    return n;
}


string Validation::toLowerCase(string str) {
    string result = str;
    for (int i = 0; i < result.length(); i++) {
        if (result[i] >= 'A' && result[i] <= 'Z') {
            result[i] = result[i] + 32; // Convert to lowercase using ASCII
        }
    }
    return result;
}