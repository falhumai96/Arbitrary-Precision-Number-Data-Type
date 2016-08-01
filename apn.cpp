#include <iostream>
#include <string>
#include <stdexcept>
#include "apn.h"

using namespace std;

bool APN::operator==(APN& num) {
    return equalTo(num);
}

bool APN::operator>=(APN& num) {
    return greaterThanOrEqualTo(num);
}

bool APN::operator<=(APN& num) {
    return lessThanOrEqualTo(num);
}

bool APN::operator>(APN& num) {
    return greaterThan(num);
}

bool APN::operator<(APN& num) {
    return lessThan(num);
}

void APN::shrink() {
    bool endFlag = false;
    while (firstInsertAfterFloat == false && endFlag == false) {
        if (head->data != 0) {
            endFlag = true;
        }

        if (endFlag == false) {
            deleteFront();
        }

        if (head->data == 0 && head->next->isFloatPoint == true) {
            firstInsertAfterFloat = true;
        }

        if (tail->data == 0 && tail->prev->isFloatPoint == true) {
            firstInsertBeforeFloat = true;
        }
    }

    endFlag = false;
    while (firstInsertBeforeFloat == false && endFlag == false) {
        if (tail->data != 0) {
            endFlag = true;
        }

        if (endFlag == false) {
            deleteBack();
        }

        if (head->data == 0 && head->next->isFloatPoint == true) {
            firstInsertAfterFloat = true;
        }

        if (tail->data == 0 && tail->prev->isFloatPoint == true) {
            firstInsertBeforeFloat = true;
        }
    }
}

void APN::fixDigitsTo(APN& num) {
    DigitNode* afterThis = floatPointNd->prev;
    DigitNode* afterNum = num.getFloatPointPtr()->prev;
    DigitNode* beforeThis = floatPointNd->next;
    DigitNode* beforeNum = num.getFloatPointPtr()->next;

    bool endFlag = false;
    while (endFlag == false) {
        if (afterThis == NULL && afterNum != NULL) {
            appendFront(0);
            afterThis = head;
        } else if (afterThis != NULL && afterNum == NULL) {
            num.appendFront(0);
            afterNum = num.getHeadPtr();
        } else if (afterThis == NULL && afterNum == NULL) {
            endFlag = true;
        }

        if (endFlag == false) {
            afterThis = afterThis->prev;
            afterNum = afterNum->prev;
        }
    }



    endFlag = false;
    while (endFlag == false) {
        if (beforeThis == NULL && beforeNum != NULL) {
            appendBack(0);
            beforeThis = tail;
        } else if (beforeThis != NULL && beforeNum == NULL) {
            num.appendBack(0);
            beforeNum = num.getTailPtr();
        } else if (beforeThis == NULL && beforeNum == NULL) {
            endFlag = true;
        }

        if (endFlag == false) {
            beforeThis = beforeThis->next;
            beforeNum = beforeNum->next;
        }
    }
}

APN& APN::operator=(APN& num) {
    return assign(num);
}

APN& APN::operator=(string strNum) {
    APN temp;
    int start = -1;
    int floatPointPos = -1;
    bool isPassedInNegative = false;
    bool continueSearching = true;

    for (int i = 0; i < strNum.size(); ++i) {
        if (strNum[i] != ' ') {
            start = i;
            i = strNum.size() + 1;
        }
    }

    if (start != -1) {
        if (start != strNum.size() - 1) {
            if (strNum[start] == '-') {
                isPassedInNegative = true;
                start = start + 1;
            }

            for (int i = start; i < strNum.size(); ++i) {
                if (strNum[i] == '.') {
                    floatPointPos = i;
                    i = strNum.size() + 1;
                }
            }

            if (floatPointPos != -1) {
                for (int i = floatPointPos - 1; i >= start; --i) {
                    if (strNum[i] >= '0' && strNum[i] <= '9') {
                        int num = strNum[i] - 48;
                        temp.appendFront(num);
                    } else if (strNum[i] != ' ') {
                        temp = *this;
                        continueSearching = false;
                        i = -1;
                    }
                }

                if (continueSearching == true) {
                    for (int i = floatPointPos + 1; i < strNum.size(); ++i) {
                        if (strNum[i] >= '0' && strNum[i] <= '9') {
                            int num = strNum[i] - 48;
                            temp.appendBack(num);
                        } else if (strNum[i] != ' ') {
                            temp = *this;
                            continueSearching = false;
                            i = -1;
                        }
                    }
                }

                if (continueSearching == true) {
                    temp.setToNegative(isPassedInNegative);
                }
            } else {
                for (int i = strNum.size() - 1; i >= start; --i) {
                    if (strNum[i] >= '0' && strNum[i] <= '9') {
                        int num = strNum[i] - 48;
                        temp.appendFront(num);
                    } else if (strNum[i] != ' ') {
                        temp = *this;
                        continueSearching = false;
                        i = -1;
                    }
                }

                if (continueSearching == true) {
                    temp.setToNegative(isPassedInNegative);
                }
            }
        } else if (strNum[start] >= '0' && strNum[start] <= '9') {
            int num = strNum[start] - 48;
            temp.appendFront(num);
        } else {
            temp = *this;
        }
    } else {
        temp = *this;
    }

    *this = temp;
    return *this;
}

APN& APN::assign(APN& num) {
    if (this != &num) {
        removeAll();
        DigitNode* startingPointAfter = num.getFloatPointPtr()->prev;
        DigitNode* startingPointBefore = num.getFloatPointPtr()->next;

        while (startingPointAfter != NULL) {
            appendFront(startingPointAfter->data);
            startingPointAfter = startingPointAfter->prev;
        }

        while (startingPointBefore != NULL) {
            appendBack(startingPointBefore->data);
            startingPointBefore = startingPointBefore->next;
        }

        isNegative = num.isNegativeNum();
    }
    return *this;
}

void APN::setToNegative(bool isNegativeChange) {
    if (firstInsertAfterFloat == false || firstInsertBeforeFloat == false) {
        isNegative = isNegativeChange;
    }
}

void APN::removeAll() {
    while (firstInsertAfterFloat == false) {
        deleteFront();
    }

    while (firstInsertBeforeFloat == false) {
        deleteBack();
    }
}

bool APN::isNegativeNum() {
    return isNegative;
}

APN::DigitNode* APN::getFloatPointPtr() {
    return floatPointNd;
}

APN::DigitNode* APN::getTailPtr() {
    return tail;
}

APN::DigitNode* APN::getHeadPtr() {
    return head;
}

void APN::printNum() {
    if (isNegativeNum() == true) {
        cout << "-";
    }
    DigitNode* temp = head;
    while (temp != NULL) {
        if (temp->isFloatPoint == false) {
            cout << temp->data;
        } else {
            cout << ".";
        }

        temp = temp->next;
    }
}

void APN::subtract(APN& num) {
    shrink();
    num.shrink();
    fixDigitsTo(num);
    if (isNegativeNum() == true && num.isNegativeNum() == false) {
        APN temp1(*this);
        APN temp2(num);
        temp1.setToNegative(false);
        temp2.setToNegative(false);

        if (temp1.greaterThan(temp2)) {
            temp1.add(temp2);
            temp1.setToNegative(true);
            *this = temp1;
        } else if (temp1.lessThan(temp2)) {
            temp1.add(temp2);
            temp1.setToNegative(true);
            *this = temp1;
        } else {
            temp1.add(temp2);
            temp1.setToNegative(true);
            *this = temp1;
        }
    } else if (isNegativeNum() == false && num.isNegativeNum() == true) {
        APN temp1(*this);
        APN temp2(num);
        temp1.setToNegative(false);
        temp2.setToNegative(false);

        if (temp1.greaterThan(temp2)) {
            temp1.add(temp2);
            temp1.setToNegative(false);
            *this = temp1;
        } else if (temp1.lessThan(temp2)) {
            temp1.add(temp2);
            temp1.setToNegative(false);
            *this = temp1;
        } else {
            temp1.add(temp2);
            temp1.setToNegative(false);
            *this = temp1;
        }
    } else if (isNegativeNum() == true && num.isNegativeNum() == true) {
        APN temp1(*this);
        APN temp2(num);
        temp1.setToNegative(false);
        temp2.setToNegative(false);

        if (temp1.greaterThan(temp2)) {
            temp1.subtract(temp2);
            temp1.setToNegative(true);
            *this = temp1;
        } else if (temp1.lessThan(temp2)) {
            temp1.subtract(temp2);
            temp1.setToNegative(false);
            *this = temp1;
        } else {
            removeAll();
        }
    } else {
        DigitNode* newHeadSearch = floatPointNd;
        DigitNode* newTailSearch = floatPointNd;

        while (newHeadSearch->prev != NULL) {
            newHeadSearch = newHeadSearch->prev;
        }
        head = newHeadSearch;

        while (newTailSearch->next != NULL) {
            newTailSearch = newTailSearch->next;
        }
        tail = newTailSearch;

        APN temp1(*this);
        APN temp2(num);
        temp1.setToNegative(false);
        temp2.setToNegative(false);

        if (temp1.greaterThan(temp2)) {
            // The general case.
            temp1.fixDigitsTo(temp2);
            DigitNode* startThis = temp1.getTailPtr();
            DigitNode* startNum = temp2.getTailPtr();
            //bool endFlag = false;
            while (startThis != NULL) {
                if (startThis->data < startNum->data && startThis->isFloatPoint == false) {
                    DigitNode* currentNode = startThis;
                    DigitNode* tempFirstStop = startThis;

                    bool endFlag2 = false;
                    while (endFlag2 == false && currentNode != NULL) {
                        if (currentNode == startThis) {
                            currentNode = currentNode->prev;
                        }

                        if (currentNode != NULL && currentNode->isFloatPoint == true) {
                            currentNode = currentNode->prev;
                        }

                        if (currentNode != NULL && currentNode->data > 0) {
                            endFlag2 = true;
                        }

                        if (currentNode != NULL && endFlag2 == false) {
                            currentNode = currentNode->prev;
                        }
                    }

                    while (currentNode != NULL && currentNode != tempFirstStop) {
                        --currentNode->data;
                        currentNode->next->data += 10;
                        currentNode = currentNode->next;
                    }

                    startThis->data -= startNum->data;
                } else if (startThis->isFloatPoint == false) {
                    startThis->data = startThis->data - startNum->data;
                }

                startThis = startThis->prev;
                startNum = startNum->prev;
            }
            temp1.setToNegative(false);
            *this = temp1;
        } else if (temp1.lessThan(temp2)) {
            temp2.subtract(temp1);
            temp2.setToNegative(true);
            *this = temp2;
        } else {
            removeAll();
        }
    }

    shrink();
    DigitNode* isFirstInsertStart = floatPointNd;
    if (floatPointNd->prev == 0) {
        if (floatPointNd->prev->prev == NULL) {
            firstInsertAfterFloat = true;
        }
    }

    if (floatPointNd->next == 0) {
        if (floatPointNd->next->next == NULL) {
            firstInsertBeforeFloat = true;
        }
    }
}

void APN::add(APN& num) {
    shrink();
    num.shrink();
    fixDigitsTo(num);
    if (num.isNegativeNum() == true && isNegativeNum() == false || num.isNegativeNum() == false && isNegativeNum() == true) {
        APN temp1(*this);
        APN temp2(num);
        temp1.setToNegative(false);
        temp2.setToNegative(false);
        if (temp1.greaterThan(temp2) == true) {
            temp1.subtract(temp2);
            temp1.setToNegative(isNegativeNum());
            *this = temp1;
        } else if (temp1.lessThan(temp2) == true) {
            temp2.subtract(temp1);
            temp2.setToNegative(num.isNegativeNum());
            *this = temp2;
        } else {
            removeAll();
        }
    } else if (isNegativeNum() == true && num.isNegativeNum() == true) {
        APN temp1(*this);
        APN temp2(num);
        temp1.setToNegative(false);
        temp2.setToNegative(false);
        temp1.add(temp2);
        temp1.setToNegative(true);
        *this = temp1;
    } else {
        APN temp(num);
        fixDigitsTo(num);
        num.appendFront(0);
        num.appendFront(0);
        num.appendFront(0);
        num.appendFront(0);
        appendFront(0);
        appendFront(0);
        appendFront(0);
        appendFront(0);
        DigitNode* startThis = tail;
        DigitNode* startNum = num.getTailPtr();

        int carry = 0;
        bool endFlag = false;
        while (endFlag == false && startThis != NULL && startNum != NULL) {
            int result = carry + startThis->data + startNum->data;
            if (result > 9) {
                result = result - 10;
                startThis->data = result;
                carry = 1;
            } else {
                startThis->data = result;
                carry = 0;
            }

            startThis = startThis->prev;
            startNum = startNum->prev;

            if (startThis != NULL && startThis->isFloatPoint == true) {
                startThis = startThis->prev;
                startNum = startNum->prev;
            } else if (startThis == NULL) {
                endFlag = true;
            }
        }
        num = temp;
        shrink();
    }
    shrink();
    DigitNode* isFirstInsertStart = floatPointNd;
    if (floatPointNd->prev == 0) {
        if (floatPointNd->prev->prev == NULL) {
            firstInsertAfterFloat = true;
        }
    }

    if (floatPointNd->next == 0) {
        if (floatPointNd->next->next == NULL) {
            firstInsertBeforeFloat = true;
        }
    }
}

bool APN::greaterThanOrEqualTo(APN& num) {
    if (greaterThan(num) == true || equalTo(num) == true) {
        return true;
    } else {
        return false;
    }
}

bool APN::lessThanOrEqualTo(APN& num) {
    if (lessThan(num) == true || equalTo(num) == true) {
        return true;
    } else {
        return false;
    }
}

bool APN::greaterThan(APN& num) {
    if (equalTo(num) == false && lessThan(num) == false) {
        return true;
    } else {
        return false;
    }
}

bool APN::lessThan(APN& num) {
    if (equalTo(num) == true) {
        return false;
    } else if (num.isNegativeNum() == true && isNegativeNum() == false) {
        return false;
    } else if (num.isNegativeNum() == false && isNegativeNum() == true) {
        return true;
    } else {

        if (isAllZeros(*this)) {
            setToNegative(false);
        }

        if (isAllZeros(num)) {
            num.setToNegative(false);
        }

        shrink();
        num.shrink();
        fixDigitsTo(num);
        DigitNode* afterWithoutZerosThis = head;
        DigitNode* afterWithoutZerosNum = num.getHeadPtr();
        DigitNode* beforeWithoutZerosThis = floatPointNd->next;
        DigitNode* beforeWithoutZerosNum = num.getFloatPointPtr()->next;
        DigitNode* endPtrBeforeThis = tail->next;
        DigitNode* endPtrBeforeNum = num.getTailPtr()->next;

        DigitNode* temp1 = afterWithoutZerosThis;
        DigitNode* temp2 = afterWithoutZerosNum;

        bool endFlag = false;
        while (endFlag == false) {
            if (temp1->next->isFloatPoint == false && temp2->next->isFloatPoint == true) {
                if (isNegativeNum() == false) {
                    return false;
                } else {
                    return true;
                }
            } else if (temp2->next->isFloatPoint == false && temp1->next->isFloatPoint == true) {
                if (isNegativeNum() == false) {
                    return true;
                } else {
                    return false;
                }
            } else if (temp1->data < temp2->data) {
                if (isNegativeNum() == false) {
                    return true;
                } else {
                    return false;
                }
            } else if (temp1->data > temp2->data) {
                if (isNegativeNum() == false) {
                    return false;
                } else {
                    return true;
                }
            } else if (temp1->data == temp2->data && temp1->next->isFloatPoint == true) {
                endFlag = true;
            }

            if (endFlag == false) {
                temp1 = temp1->next;
                temp2 = temp2->next;
            }
        }


        temp1 = beforeWithoutZerosThis;
        temp2 = beforeWithoutZerosNum;
        while (temp1 != endPtrBeforeThis || temp2 != endPtrBeforeNum) {
            if (temp1->data < temp2->data) {
                if (isNegativeNum() == false) {
                    return true;
                } else {
                    return false;
                }
            } else if (temp1->data > temp2->data) {
                if (isNegativeNum() == false) {
                    return false;
                } else {
                    return true;
                }
            } else {
                temp1 = temp1->next;
                temp2 = temp2->next;

                if (temp1 == endPtrBeforeThis && temp2 != endPtrBeforeNum) {
                    if (isNegativeNum() == false) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    if (isNegativeNum() == false) {
                        return false;
                    } else {
                        return true;
                    }
                }
            }
        }




        if (temp1 == endPtrBeforeThis && temp2 != endPtrBeforeNum) {
            if (isNegativeNum() == false) {
                return true;
            } else {
                return false;
            }
        } else if (temp1 != endPtrBeforeThis && temp2 == endPtrBeforeNum) {
            if (isNegativeNum() == false) {
                return false;
            } else {
                return true;
            }
        }
    }

    if (isAllZeros(*this)) {
        setToNegative(false);
    }

    if (isAllZeros(num)) {
        num.setToNegative(false);
    }
}

bool APN::equalTo(APN& num) {
    fixDigitsTo(num);
    if (isAllZeros(num) == true && isAllZeros(*this) == true) {
        return true;
    } else if (isAllZeros(num) == true && isAllZeros(*this) == false || isAllZeros(num) == false && isAllZeros(*this) == true) {
        return false;
    } else if (num.isNegativeNum() != isNegativeNum()) {
        return false;
    } else {

        if (isAllZeros(*this)) {
            setToNegative(false);
        }

        if (isAllZeros(num)) {
            num.setToNegative(false);
        }

        shrink();
        num.shrink();
        fixDigitsTo(num);

        DigitNode* afterWithoutZerosThis = head;
        DigitNode* afterWithoutZerosNum = num.getHeadPtr();
        DigitNode* beforeWithoutZerosThis = tail;
        DigitNode* beforeWithoutZerosNum = num.getTailPtr();

        bool endFlag = false;

        DigitNode* temp1 = afterWithoutZerosThis;
        DigitNode* temp2 = afterWithoutZerosNum;
        endFlag = false;
        while (endFlag == false) {
            if (temp1->isFloatPoint == true && temp2->isFloatPoint == false || temp1->isFloatPoint == false && temp2->isFloatPoint == true) {
                return false;
            } else if (temp1->isFloatPoint == true) {
                endFlag = true;
            }

            if (endFlag == false) {
                temp1 = temp1->next;
                temp2 = temp2->next;
            }
        }

        temp1 = beforeWithoutZerosThis;
        temp2 = beforeWithoutZerosNum;
        endFlag = false;
        while (endFlag == false) {
            if (temp1->isFloatPoint == true && temp2->isFloatPoint == false || temp1->isFloatPoint == false && temp2->isFloatPoint == true) {
                return false;
            } else if (temp1->isFloatPoint == true) {
                endFlag = true;
            }

            if (endFlag == false) {
                temp1 = temp1->prev;
                temp2 = temp2->prev;
            }
        }

        temp1 = afterWithoutZerosThis;
        temp2 = afterWithoutZerosNum;
        while (temp1->isFloatPoint == false) {
            if (temp1->data != temp2->data) {
                return false;
            }

            temp1 = temp1->next;
            temp2 = temp2->next;
        }

        temp1 = beforeWithoutZerosThis;
        temp2 = beforeWithoutZerosNum;
        while (temp1->isFloatPoint == false) {
            if (temp1->data != temp2->data) {
                return false;
            }

            temp1 = temp1->prev;
            temp2 = temp2->prev;
        }
        return true;
    }

    if (isAllZeros(*this)) {
        setToNegative(false);
    }

    if (isAllZeros(num)) {
        num.setToNegative(false);
    }
}

void APN::deleteBack() {
    if (firstInsertBeforeFloat == false) {
        if (tail->prev->isFloatPoint == true) {
            tail->data = 0;
            firstInsertBeforeFloat = true;

            if (firstInsertAfterFloat == true && firstInsertBeforeFloat == true) {
                isNegative = false;
            }

            if (head->data == 0 && head->next->isFloatPoint == true) {
                firstInsertAfterFloat = true;
            }

            if (tail->data == 0 && tail->prev->isFloatPoint == true) {
                firstInsertBeforeFloat = true;
            }
        } else {
            deleteBackHelper(false);

            if (head->data == 0 && head->next->isFloatPoint == true) {
                firstInsertAfterFloat = true;
            }

            if (tail->data == 0 && tail->prev->isFloatPoint == true) {
                firstInsertBeforeFloat = true;
            }
        }
    }

    if (isAllZeros(*this)) {
        isNegative = false;
    }
}

void APN::deleteFront() {
    if (firstInsertAfterFloat == false) {
        if (head->next->isFloatPoint == true) {
            head->data = 0;
            firstInsertAfterFloat = true;

            if (firstInsertAfterFloat == true && firstInsertBeforeFloat == true) {
                isNegative = false;
            }

            if (head->data == 0 && head->next->isFloatPoint == true) {
                firstInsertAfterFloat = true;
            }

            if (tail->data == 0 && tail->prev->isFloatPoint == true) {
                firstInsertBeforeFloat = true;
            }
        } else {
            deleteFrontHelper(false);

            if (head->data == 0 && head->next->isFloatPoint == true) {
                firstInsertAfterFloat = true;
            }

            if (tail->data == 0 && tail->prev->isFloatPoint == true) {
                firstInsertBeforeFloat = true;
            }
        }
    }

    if (isAllZeros(*this)) {
        isNegative = false;
    }
}

bool APN::appendBack(int digit) {
    if (digit >= 0 && digit <= 9) {
        if (firstInsertBeforeFloat == true) {
            tail->data = digit;
            firstInsertBeforeFloat = false;
        } else {
            appendBackHelper(digit, false);
        }

        DigitNode* newHeadSearch = floatPointNd;
        DigitNode* newTailSearch = floatPointNd;

        while (newHeadSearch->prev != NULL) {
            newHeadSearch = newHeadSearch->prev;
        }
        head = newHeadSearch;

        while (newTailSearch->next != NULL) {
            newTailSearch = newTailSearch->next;
        }
        tail = newTailSearch;

        if (isAllZeros(*this)) {
            isNegative = false;
        }

        return true;
    }

    if (isAllZeros(*this)) {
        isNegative = false;
    }

    return false;
}

bool APN::appendFront(int digit) {
    if (digit >= 0 && digit <= 9) {
        if (firstInsertAfterFloat == true) {
            head->data = digit;
            firstInsertAfterFloat = false;
        } else {
            appendFrontHelper(digit, false);
        }

        DigitNode* newHeadSearch = floatPointNd;
        DigitNode* newTailSearch = floatPointNd;

        while (newHeadSearch->prev != NULL) {
            newHeadSearch = newHeadSearch->prev;
        }
        head = newHeadSearch;

        while (newTailSearch->next != NULL) {
            newTailSearch = newTailSearch->next;
        }
        tail = newTailSearch;

        if (isAllZeros(*this)) {
            isNegative = false;
        }

        return true;
    }

    if (isAllZeros(*this)) {
        isNegative = false;
    }

    return false;
}

APN::~APN() {
    removeAll();
    delete head;
    delete tail;
    delete floatPointNd;
    firstInsertAfterFloat = true;
    firstInsertBeforeFloat = true;
    isNegative = false;
}

APN::APN(APN& num) {
    head = NULL;
    tail = NULL;
    floatPointNd = NULL;
    appendBackHelper(0, false);
    appendBackHelper(0, true);
    appendBackHelper(0, false);
    firstInsertAfterFloat = true;
    firstInsertBeforeFloat = true;

    DigitNode* startingPointAfter = num.getFloatPointPtr()->prev;
    DigitNode* startingPointBefore = num.getFloatPointPtr()->next;

    while (startingPointAfter != NULL) {
        appendFront(startingPointAfter->data);
        startingPointAfter = startingPointAfter->prev;
    }

    while (startingPointBefore != NULL) {
        appendBack(startingPointBefore->data);
        startingPointBefore = startingPointBefore->next;
    }

    isNegative = num.isNegativeNum();

    DigitNode* isFirstInsertStart = floatPointNd;
    if (floatPointNd->next == 0) {
        if (floatPointNd->next->next == NULL) {
            firstInsertAfterFloat = true;
        }
    }

    if (floatPointNd->prev == 0) {
        if (floatPointNd->prev->prev == NULL) {
            firstInsertBeforeFloat = true;
        }
    }
}

APN::APN() {
    head = NULL;
    tail = NULL;
    floatPointNd = NULL;
    appendBackHelper(0, false);
    appendBackHelper(0, true);
    appendBackHelper(0, false);
    firstInsertAfterFloat = true;
    firstInsertBeforeFloat = true;
    isNegative = false;
}

APN::APN(string strNum) {
    head = NULL;
    tail = NULL;
    floatPointNd = NULL;
    appendBackHelper(0, false);
    appendBackHelper(0, true);
    appendBackHelper(0, false);
    firstInsertAfterFloat = true;
    firstInsertBeforeFloat = true;
    isNegative = false;

    APN temp;
    int start = -1;
    int floatPointPos = -1;
    bool isPassedInNegative = false;
    bool continueSearching = true;

    for (int i = 0; i < strNum.size(); ++i) {
        if (strNum[i] != ' ') {
            start = i;
            i = strNum.size() + 1;
        }
    }

    if (start != -1) {
        if (start != strNum.size() - 1) {
            if (strNum[start] == '-') {
                isPassedInNegative = true;
                start = start + 1;
            }

            for (int i = start; i < strNum.size(); ++i) {
                if (strNum[i] == '.') {
                    floatPointPos = i;
                    i = strNum.size() + 1;
                }
            }

            if (floatPointPos != -1) {
                for (int i = floatPointPos - 1; i >= start; --i) {
                    if (strNum[i] >= '0' && strNum[i] <= '9') {
                        int num = strNum[i] - 48;
                        temp.appendFront(num);
                    } else {
                        temp = *this;
                        continueSearching = false;
                        i = -1;
                    }
                }

                if (continueSearching == true) {
                    for (int i = floatPointPos + 1; i < strNum.size() && strNum[i] != ' '; ++i) {
                        if (strNum[i] >= '0' && strNum[i] <= '9') {
                            int num = strNum[i] - 48;
                            temp.appendBack(num);
                        } else {
                            temp = *this;
                            continueSearching = false;
                            i = -1;
                        }
                    }
                }

                if (continueSearching == true) {
                    temp.setToNegative(isPassedInNegative);
                }
            } else {
                for (int i = strNum.size() - 1; i >= start && strNum[i] != ' '; --i) {
                    if (strNum[i] >= '0' && strNum[i] <= '9') {
                        int num = strNum[i] - 48;
                        temp.appendFront(num);
                    } else {
                        temp = *this;
                        continueSearching = false;
                        i = -1;
                    }
                }

                if (continueSearching == true) {
                    temp.setToNegative(isPassedInNegative);
                }
            }
        } else if (strNum[start] >= '0' && strNum[start] <= '9') {
            int num = strNum[start] - 48;
            temp.appendFront(num);
        } else {
            temp = *this;
        }
    } else {
        temp = *this;
    }

    *this = temp;
}

bool APN::isAllZeros(APN& num) {
    DigitNode* temp = num.getHeadPtr();
    while (temp->isFloatPoint == false) {
        if (temp->data != 0) {
            return false;
        }
        temp = temp->next;
    }

    temp = num.getTailPtr();
    while (temp->isFloatPoint == false) {
        if (temp->data != 0) {
            return false;
        }
        temp = temp->prev;
    }
    return true;
}

APN::DigitNode* APN::deleteBackHelper(bool getNodeAfter) {
    return deleteDigitHelper(tail, getNodeAfter);
}

APN::DigitNode* APN::deleteFrontHelper(bool getNodeAfter) {
    return deleteDigitHelper(head, getNodeAfter);
}

APN::DigitNode* APN::appendBackHelper(int digit, bool isFloatPoint) {
    return insertDigitHelper(tail, digit, true, isFloatPoint);
}

APN::DigitNode* APN::appendFrontHelper(int digit, bool isFloatPoint) {
    return insertDigitHelper(head, digit, false, isFloatPoint);
}

APN::DigitNode* APN::deleteDigitHelper(DigitNode* nd, bool getAfter) {
    if (getAfter == true) {
        if (nd == head && nd->next == NULL && nd->prev == NULL) {
            delete head;
            head = NULL;
            tail = NULL;
            return NULL;
        } else if (nd == head) {
            DigitNode* temp = head;
            head = head->next;
            head->prev = NULL;
            delete temp;
            return head;
        } else if (nd == tail) {
            DigitNode* temp = tail;
            tail = tail->prev;
            tail->next = NULL;
            delete temp;
            return NULL;
        } else {
            DigitNode* before = nd->prev;
            DigitNode* after = nd->next;
            before->next = after;
            after->prev = before;
            delete nd;
            return after;
        }
    } else {
        if (nd == head && nd->next == NULL && nd->prev == NULL) {
            delete head;
            head = NULL;
            tail = NULL;
            return NULL;
        } else if (nd == head) {
            DigitNode* temp = head;
            head = head->next;
            head->prev = NULL;
            delete temp;
            return NULL;
        } else if (nd == tail) {
            DigitNode* temp = tail;
            tail = tail->prev;
            tail->next = NULL;
            delete temp;
            return tail;
        } else {
            DigitNode* before = nd->prev;
            DigitNode* after = nd->next;
            before->next = after;
            after->prev = before;
            delete nd;
            return before;
        }
    }
}

APN::DigitNode* APN::insertDigitHelper(DigitNode* nd, int digit, bool insertAfter, bool isFloatPoint) {
    if (insertAfter == true) {
        if (head == NULL) {
            head = new DigitNode;
            head->data = digit;
            head->next = NULL;
            head->prev = NULL;

            if (isFloatPoint == true) {
                head->isFloatPoint = true;
                floatPointNd = head;
            } else {
                head->isFloatPoint = false;
            }

            tail = head;
            return head;
        } else if (nd == head) {
            DigitNode* newNode = new DigitNode;
            newNode->data = digit;
            if (isFloatPoint == true) {
                newNode->isFloatPoint = true;
                floatPointNd = newNode;
            } else {
                newNode->isFloatPoint = false;
            }


            newNode->next = NULL;
            newNode->prev = NULL;
            newNode->next = head->next;
            newNode->prev = head;
            head->next = newNode;
            if (newNode->next != NULL) {
                newNode->next->prev = newNode;
            }

            if (newNode->next == NULL) {
                tail = newNode;
            }
            return newNode;
        } else if (nd == tail) {
            DigitNode* newNode = new DigitNode;
            newNode->data = digit;
            if (isFloatPoint == true) {
                newNode->isFloatPoint = true;
                floatPointNd = newNode;
            } else {
                newNode->isFloatPoint = false;
            }

            newNode->next = NULL;
            newNode->prev = NULL;
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
            return newNode;
        } else {
            DigitNode* newNode = new DigitNode;
            newNode->data = digit;
            if (isFloatPoint == true) {
                newNode->isFloatPoint = true;
                floatPointNd = newNode;
            } else {
                newNode->isFloatPoint = false;
            }

            newNode->next = NULL;
            newNode->prev = NULL;
            DigitNode* afterNd = nd->next;
            nd->next = newNode;
            newNode->prev = nd;
            newNode->next = afterNd;
            afterNd->prev = newNode;
            return newNode;
        }
    } else {
        if (head == NULL) {
            head = new DigitNode;
            head->data = digit;
            head->next = NULL;
            head->prev = NULL;
            if (isFloatPoint == true) {
                head->isFloatPoint = true;
                floatPointNd = head;
            } else {
                head->isFloatPoint = false;
            }

            tail = head;
            return head;
        } else if (nd == head) {
            DigitNode* newNode = new DigitNode;
            newNode->data = digit;
            if (isFloatPoint == true) {
                newNode->isFloatPoint = true;
                floatPointNd = newNode;
            } else {
                newNode->isFloatPoint = false;
            }

            newNode->next = NULL;
            newNode->prev = NULL;
            head->prev = newNode;
            newNode->next = head;
            if (head->next == NULL) {
                tail = head;
            }
            head = newNode;
            return newNode;
        } else if (nd == tail) {
            DigitNode* newNode = new DigitNode;
            newNode->data = digit;
            if (isFloatPoint == true) {
                newNode->isFloatPoint = true;
                floatPointNd = newNode;
            } else {
                newNode->isFloatPoint = false;
            }

            newNode->next = NULL;
            newNode->prev = NULL;
            newNode->prev = tail->prev;
            newNode->next = tail;
            tail->prev = newNode;
            if (newNode->prev != NULL) {
                newNode->prev->next = newNode;
            }

            return newNode;
        } else {
            DigitNode* newNode = new DigitNode;
            newNode->data = digit;

            if (isFloatPoint == true) {
                newNode->isFloatPoint = true;
                floatPointNd = newNode;
            } else {
                newNode->isFloatPoint = false;
            }

            newNode->next = NULL;
            newNode->prev = NULL;
            DigitNode* before = nd->prev;
            before->next = newNode;
            newNode->prev = before;
            newNode->next = nd;
            nd->prev = newNode;
            return newNode;
        }
    }
}

APN APN::frontSize() {
    APN result;
    APN one;
    one.appendFront(1);
    if (firstInsertAfterFloat == false) {
        DigitNode* nd = head;
        while (nd->isFloatPoint == false) {
            result.add(one);
            nd = nd->next;
        }
    }
    return result;
}

APN APN::backSize() {
    APN result;
    APN one;
    one.appendFront(1);
    if (firstInsertBeforeFloat == false) {
        DigitNode* nd = tail;
        while (nd->isFloatPoint == false) {
            result.add(one);
            nd = nd->prev;
        }
    }
    return result;
}

APN APN::numSize() {
    APN result(frontSize());
    result.add(backSize());
    return result;
}

APN::DigitNode* APN::getPrev(DigitNode* nd) {
    if (nd != NULL) {
        if (nd->prev->isFloatPoint == false) {
            return nd->prev;
        } else {
            return nd->prev->prev;
        }
    }
    return nd;
}

APN::DigitNode* APN::getNext(DigitNode* nd) {
    if (nd != NULL) {
        if (nd->next->isFloatPoint == false) {
            return nd->next;
        } else {
            return nd->next->next;
        }
    }
    return nd;
}

void APN::simpleMultiplication(APN& num) {
    APN i;
    APN one;
    APN temp(*this);
    removeAll();
    one.appendFront(1);
    for (; i < num; i.add(one)) {
        add(temp);
    }

    if (isAllZeros(*this)) {
        setToNegative(false);
    }

    if (isAllZeros(num)) {
        num.setToNegative(false);
    }
}

void APN::multiply(APN& num) {
    if (isNegativeNum() == true && num.isNegativeNum() == false || isNegativeNum() == false && num.isNegativeNum() == true) {
        APN temp1(*this);
        APN temp2(num);
        temp1.setToNegative(false);
        temp2.setToNegative(false);
        temp1.multiply(temp2);
        temp1.setToNegative(true);
        *this = temp1;

        if (isAllZeros(*this)) {
            setToNegative(false);
        }

        if (isAllZeros(num)) {
            num.setToNegative(false);
        }
    } else {
        if (isAllZeros(*this)) {
            setToNegative(false);
        }

        if (isAllZeros(num)) {
            num.setToNegative(false);
        }

        APN temp1(*this);
        APN temp2(num);
        fixDigitsTo(num);
        appendFront(0);
        appendFront(0);
        appendFront(0);
        appendFront(0);
        num.appendFront(0);
        num.appendFront(0);
        num.appendFront(0);
        num.appendFront(0);
        APN one;
        one.appendFront(1);
        APN zero;
        APN res;
        APN j;
        DigitNode* currentNodeThis = tail;
        while (currentNodeThis != NULL) {
            DigitNode* currentNodeNum = num.getTailPtr();
            APN tempRes;
            int currentDigitRes = 0;
            int carry = 0;
            while (currentNodeNum != NULL) {
                currentDigitRes = (currentNodeThis->data * currentNodeNum->data) + carry;
                if (currentDigitRes > 9) {
                    int temp = currentDigitRes;
                    for (int h = 0; h < 9; ++h) {
                        if ((temp - h) % 10 == 0) {
                            currentDigitRes = h;
                            carry = (temp - h) / 10;
                            h = 10;
                        }
                    }
                } else {
                    carry = 0;
                }
                tempRes.appendFront(currentDigitRes);

                currentNodeNum = currentNodeNum->prev;
                if (currentNodeNum != NULL && currentNodeNum->isFloatPoint == true) {
                    currentNodeNum = currentNodeNum->prev;
                }
            }

            for (APN l; l < j; l.add(one)) {
                tempRes.moveFloatingPointDown();
            }
            res.add(tempRes);
            currentNodeThis = currentNodeThis->prev;
            if (currentNodeThis != NULL && currentNodeThis->isFloatPoint == true) {
                currentNodeThis = currentNodeThis->prev;
            }
            j.add(one);
        }
        temp1.shrink();
        temp2.shrink();
        APN timesToMoveFloat;
        DigitNode* searching = temp1.getFloatPointPtr()->next;
        while (searching != NULL) {
            timesToMoveFloat.add(one);
            searching = searching->next;
        }

        searching = temp2.getFloatPointPtr()->next;
        while (searching != NULL) {
            timesToMoveFloat.add(one);
            searching = searching->next;
        }

        *this = res;
        for (APN u; u < timesToMoveFloat; u.add(one)) {
            moveFloatingPointUp();
        }
        shrink();

        DigitNode* isFirstInsertStart = floatPointNd;
        if (floatPointNd->prev == 0) {
            if (floatPointNd->prev->prev == NULL) {
                firstInsertAfterFloat = true;
            }
        }

        if (floatPointNd->next == 0) {
            if (floatPointNd->next->next == NULL) {
                firstInsertBeforeFloat = true;
            }
        }
    }

    if (isAllZeros(*this)) {
        setToNegative(false);
    }

    if (isAllZeros(num)) {
        num.setToNegative(false);
    }
}

bool APN::divide(APN& num, APN& percision) {
    if (isAllZeros(*this)) {
        setToNegative(false);
    }

    if (isAllZeros(num)) {
        num.setToNegative(false);
    }

    if (isAllZeros(num) == true) {
        throw std::runtime_error("Division by 0!!");
    } else if (isAllZeros(*this) == true) {
        APN temp;
        *this = temp;
        return true;
    } else {
        if (num.isNegativeNum() == true && isNegativeNum() == false) {
            APN temp1(*this);
            APN temp2(num);
            temp1.setToNegative(false);
            temp2.setToNegative(false);
            temp1.divide(temp2, percision);
            temp1.setToNegative(true);
            *this = temp1;
        } else if (isNegativeNum() == true && num.isNegativeNum() == false) {
            APN temp1(*this);
            APN temp2(num);
            temp1.setToNegative(false);
            temp2.setToNegative(false);
            temp1.divide(temp2, percision);
            temp1.setToNegative(true);
            *this = temp1;
        } else if (isNegativeNum() == true && num.isNegativeNum() == true) {
            APN temp1(*this);
            APN temp2(num);
            temp1.setToNegative(false);
            temp2.setToNegative(false);
            temp1.divide(temp2, percision);
            temp1.setToNegative(false);
            *this = temp1;
        } else {
            APN Q;
            APN R(*this);
            APN one;
            one.appendFront(1);
            while (R >= num) {
                Q.add(one);
                R.subtract(num);
            }
            *this = Q;
            shrink();
            APN tempRes(percision);
            tempRes.add(one);
            bool allZero = false;
            while (isAllZeros(tempRes) == false && isAllZeros(R) == false) {
                Q.removeAll();
                APN hun;
                hun.appendFront(0);
                hun.appendFront(1);
                R.moveFloatingPointDown();
                R.shrink();
                while (R >= num) {
                    Q.add(one);
                    R.subtract(num);
                }
                Q.shrink();
                R.shrink();
                appendBack(Q.getHeadPtr()->data);
                tempRes.subtract(one);
                if (isAllZeros(tempRes) == true && isAllZeros(R) == true) {
                    allZero = true;
                }
            }

            if (isAllZeros(R) == false || allZero == true) {
                round(*this);
            }

            shrink();
        }

        return true;
    }
}

void APN::moveFloatingPointUp() {
    appendFront(0);
    appendFront(0);
    appendFront(0);
    appendFront(0);
    appendFront(0);
    appendFront(0);
    appendBack(0);
    appendBack(0);
    appendBack(0);
    appendBack(0);
    appendBack(0);
    appendBack(0);

    DigitNode* beforeFloat = floatPointNd->prev;
    int temp = floatPointNd->data;
    floatPointNd->data = beforeFloat->data;
    beforeFloat->data = temp;
    floatPointNd->isFloatPoint = false;
    beforeFloat->isFloatPoint = true;
    floatPointNd = beforeFloat;

    DigitNode* newHeadSearch = floatPointNd;
    DigitNode* newTailSearch = floatPointNd;

    while (newHeadSearch->prev != NULL) {
        newHeadSearch = newHeadSearch->prev;
    }
    head = newHeadSearch;

    while (newTailSearch->next != NULL) {
        newTailSearch = newTailSearch->next;
    }
    tail = newTailSearch;
    shrink();

    if (isAllZeros(*this)) {
        setToNegative(false);
    }
}

void APN::moveFloatingPointDown() {
    appendFront(0);
    appendFront(0);
    appendFront(0);
    appendFront(0);
    appendFront(0);
    appendFront(0);
    appendBack(0);
    appendBack(0);
    appendBack(0);
    appendBack(0);
    appendBack(0);
    appendBack(0);

    DigitNode* afterFloat = floatPointNd->next;
    int temp = floatPointNd->data;
    floatPointNd->data = afterFloat->data;
    afterFloat->data = temp;
    floatPointNd->isFloatPoint = false;
    afterFloat->isFloatPoint = true;
    floatPointNd = afterFloat;

    DigitNode* newHeadSearch = floatPointNd;
    DigitNode* newTailSearch = floatPointNd;

    while (newHeadSearch->prev != NULL) {
        newHeadSearch = newHeadSearch->prev;
    }
    head = newHeadSearch;

    while (newTailSearch->next != NULL) {
        newTailSearch = newTailSearch->next;
    }
    tail = newTailSearch;
    shrink();

    if (isAllZeros(*this)) {
        setToNegative(false);
    }
}

bool APN::divideRemainder(APN& num, APN& percision) {
    if (isAllZeros(*this)) {
        setToNegative(false);
    }

    if (isAllZeros(num)) {
        num.setToNegative(false);
    }


    if (isAllZeros(num) == true) {
        throw std::runtime_error("Division by 0!!");
    } else if (isAllZeros(*this) == true) {
        APN temp;
        *this = temp;
        return true;
    } else {
        if (num.isNegativeNum() == true && isNegativeNum() == false) {
            APN temp1(*this);
            APN temp2(num);
            temp1.setToNegative(false);
            temp2.setToNegative(false);
            temp1.divide(temp2, percision);
            temp1.setToNegative(true);
            *this = temp1;
        } else if (isNegativeNum() == true && num.isNegativeNum() == false) {
            APN temp1(*this);
            APN temp2(num);
            temp1.setToNegative(false);
            temp2.setToNegative(false);
            temp1.divide(temp2, percision);
            temp1.setToNegative(true);
            *this = temp1;
        } else if (isNegativeNum() == true && num.isNegativeNum() == true) {
            APN temp1(*this);
            APN temp2(num);
            temp1.setToNegative(false);
            temp2.setToNegative(false);
            temp1.divide(temp2, percision);
            temp1.setToNegative(false);
            *this = temp1;
        } else {
            bool allZero = false;
            APN Q;
            APN R(*this);
            APN one;
            one.appendFront(1);
            while (R >= num) {
                Q.add(one);
                R.subtract(num);
            }
            *this = Q;
            shrink();
            APN tempRes(percision);
            tempRes.add(one);
            while (isAllZeros(tempRes) == false && isAllZeros(R) == false) {
                Q.removeAll();
                APN hun;
                hun.appendFront(0);
                hun.appendFront(1);
                R.moveFloatingPointDown();
                R.shrink();
                while (R >= num) {
                    Q.add(one);
                    R.subtract(num);
                }
                Q.shrink();
                R.shrink();
                appendBack(Q.getHeadPtr()->data);
                tempRes.subtract(one);

                if (isAllZeros(tempRes) == true && isAllZeros(R) == true) {
                    allZero = true;
                }
            }

            if (isAllZeros(R) == false || allZero == true) {
                round(*this);
            }

            *this = R;
            shrink();
        }

        return true;
    }
}

void APN::round(APN& num) {
    appendFront(0);
    appendFront(0);
    DigitNode* search = num.getTailPtr();
    if (search->data < 5) {
        search->data = 0;
    } else {
        search->data = 0;
        DigitNode* convert = search->prev;
        if (convert->isFloatPoint == true) {
            convert = convert->prev;
        }

        bool endFlag = false;
        while (convert != NULL && endFlag == false) {
            if (convert->data < 9) {
                ++convert->data;
                endFlag = true;
            } else {
                convert->data = 0;
            }

            if (endFlag == false) {
                convert = convert->prev;
                if (convert->isFloatPoint == true) {
                    convert = convert->prev;
                }
            }
        }
    }
    num.shrink();
}

APN& APN::operator*(APN& num) {
    multiply(num);
    return *this;
}

APN& APN::operator/(APN& num) {
    APN percision;
    percision = "50";
    if (isAllZeros(num) == true) {
        throw std::runtime_error("Division by 0!!");
    } else {
        return *this;
    }
}

APN& APN::operator+(APN& num) {
    add(num);
    return *this;
}

APN& APN::operator-(APN& num) {
    subtract(num);
    return *this;
}

APN& APN::operator++() {
    APN one;
    one.appendFront(1);
    add(one);
    return *this;
}

APN& APN::operator--() {
    APN one;
    one.appendFront(1);
    subtract(one);
    return *this;
}
