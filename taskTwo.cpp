#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;



class lineClass {
private:
    char* ptrRow ;
    lineClass* ptrNextLine ;
    static int numOfCursorIndex ;
    static int numOfCursorLine ;
    int numOfLine;



public:
    char* getLineText() {
        return ptrRow;
    }
    int getCursorLine() {
        return numOfCursorLine;
    }

    int getCursorIndex() {
        return numOfCursorIndex;
    }


    void changeCursorCoord(int numOfLine, int numOfIndex) {
        this->numOfCursorLine = numOfLine;
        this->numOfCursorIndex = numOfIndex;
    }

    void changeCursorIndex(int numOfIndex) {
        this->numOfCursorIndex = numOfIndex;
    }
    void changeCursorLine(int numOfLine) {
        this->numOfCursorLine = numOfLine;
    }
