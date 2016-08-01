#include <iostream>

using namespace std;

class APN {
private:
    // digit or floating point node

    class DigitNode {
    public:
        int data;
        bool isFloatPoint;
        DigitNode* next;
        DigitNode* prev;
    };

    // positions for the most important positions in the number
    DigitNode* head;
    DigitNode* tail;
    DigitNode* floatPointNd;

    // whether the number is negative or not
    bool isNegative;

    // to make sure that the last digit, in either before or after the float point, is rather changed to 0 than delete it
    //   i.e. the minimum number of nodes are the nodes representing the number 0.0 (three nodes)
    bool firstInsertAfterFloat;
    bool firstInsertBeforeFloat;

    // general insertion function
    DigitNode* insertDigitHelper(DigitNode* nd, int digit, bool insertAfter, bool isFloatPoint);

    // general deletion function
    DigitNode* deleteDigitHelper(DigitNode* nd, bool getAfter);

    // insert at front (head) helper
    DigitNode* appendFrontHelper(int digit, bool isFloatPoint);

    // insert at back (tail) helper
    DigitNode* appendBackHelper(int digit, bool isFloatPoint);

    // delete at front (head) helper
    DigitNode* deleteFrontHelper(bool getNodeAfter);

    // delete at back (tail) helper
    DigitNode* deleteBackHelper(bool getNodeAfter);

    // return all digits == 0
    bool isAllZeros(APN& num);

    // the most simple multiplication ever, which is adding this's initial number "num" times
    void simpleMultiplication(APN& num);

    // remove the first digit and round
    void round(APN& num);

public:
    // just like dividing by 10
    void moveFloatingPointUp();

    // just like multiplying by 10
    void moveFloatingPointDown();

    // default constructor - set to 0.0
    APN();

    // deep copy constructor
    APN(APN& num);

    // parametrized constructor - string to APN converter
    APN(string strNum);

    // destructor - deletes every node
    ~APN();

    // number of nodes before the floating point (integer part)
    APN frontSize();

    // number of nodes after the floating point (decimal part)
    APN backSize();

    // frontSize + backSize
    APN numSize();

    // get nd->next
    DigitNode* getNext(DigitNode* nd);

    // get nd->prev
    DigitNode* getPrev(DigitNode* nd);

    // insert at front (head)
    bool appendFront(int digit);

    // insert at back (tail)
    bool appendBack(int digit);

    // delete at front (head)
    void deleteFront();

    // delete at back (tail)
    void deleteBack();

    // return this == num
    bool equalTo(APN& num);

    // return this < num
    bool lessThan(APN& num);

    // return this > num
    bool greaterThan(APN& num);

    // return this <= num
    bool lessThanOrEqualTo(APN& num);

    // return this >= num
    bool greaterThanOrEqualTo(APN& num);

    // this = this + num
    void add(APN& num);

    // this = this - num
    void subtract(APN& num);

    // this = this * num
    void multiply(APN& num);

    // this = this / num with some percision passed in as argument
    bool divide(APN& num, APN& percision);

    // similar to divide, but it will return the reminder rather than the quotient
    bool divideRemainder(APN& num, APN& percision);

    // cout the current number without '\n' at the end
    void printNum();

    // get front position
    DigitNode* getHeadPtr();

    // get back position
    DigitNode* getTailPtr();

    // get float point position
    DigitNode* getFloatPointPtr();

    // return num < 0
    bool isNegativeNum();

    // set to 0.0 with positive sign
    void removeAll();

    // change sign
    void setToNegative(bool isNegativeChange);

    // assignment function for deep copy
    APN& assign(APN& num);

    // making two digits have the same number of nodes by adding 0's after (for front insertion) or before (for back insertion)
    //   for the number, either this or num, that require them
    //   i.e. this = 90.687, num = 8989.3 will be this = 0090.687, num = 8989.300
    //   needed for functions such as subtract
    void fixDigitsTo(APN& num);

    // remove unnecessary 0's, at front or back, that will not change the meaning of the number
    void shrink();

    // assignment operator for deep copy
    APN& operator=(APN& num);

    // assignment operator for string to APN conversion
    APN& operator=(string strNum);

    // less than operator
    bool operator<(APN& num);

    // greater than operator
    bool operator>(APN& num);

    // less than or equal to operator
    bool operator<=(APN& num);

    // greater than or equal to operator
    bool operator>=(APN& num);

    // equal to operator 
    bool operator==(APN& num);

    // increment operator
    APN& operator++();

    // decrement operator
    APN& operator--();

    // multiplication operator
    APN& operator*(APN& num);

    // division operator with percision of 50
    APN& operator/(APN& num);

    // addition operator
    APN& operator+(APN& num);

    // subtraction operator
    APN& operator-(APN& num);
};