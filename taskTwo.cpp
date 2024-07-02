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





    lineClass(){
        ptrRow = "\0";
        ptrNextLine = nullptr;

    }

    lineClass(const lineClass& other) {
        if (other.ptrRow != nullptr) {
            ptrRow = new char[strlen(other.ptrRow) + 1];
            strcpy(ptrRow, other.ptrRow);
        } else {
            ptrRow = nullptr;
        }
        if (other.ptrNextLine != nullptr) {
            ptrNextLine = new lineClass(*other.ptrNextLine);
        } else {
            ptrNextLine = nullptr;
        }
    }

    void clearInputBuffer() {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
