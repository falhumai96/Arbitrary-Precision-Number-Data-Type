#include <iostream>
#include <string>
#include "apn.h"

using namespace std;

int main() {
    APN a;
    APN b;
    APN percision;
    a = "634729123456789934434334";
    b = "63472912345678993443.0";
    a.printNum();
    cout << endl;
    b.printNum();
    cout << endl;
    percision = "40";
    a.divide(b, percision);
    a.printNum();
    cout << endl;
    cout << "Press [Enter] to continue...";
    cin.get();
}