#include <iostream>

using namespace std;

int main()
{   
    cout << "Calculator" << endl;

    double num1, num2;

    char op; 
    cin >> num1 >> op >> num2;

    if(op == '+') cout << num1 + num2 << endl; 
    else if (op == '-') cout << num1 - num2 << endl; 
    else if (op == '*') cout << num1 * num2 << endl; 
    else if (op == '/') cout << num1 / num2 << endl; 

    system("pause"); // stop the Console from Closings 
    return 0;
} 
 
