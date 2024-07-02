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
    char* getTextFromUser(char* announceTextForUser = "\nEnter the text to append") {
        const int InitialNumOfBytePerString = 10;
        const int numByIncreseBytes = 5;
        int counter = 0;
        char userLetter;
        int currentSizeArrayInBytes;
        char* ptrRow;

        clearInputBuffer();
        ptrRow = new char[InitialNumOfBytePerString + 1]();
        currentSizeArrayInBytes = InitialNumOfBytePerString;
        cout << announceTextForUser;

        while ((userLetter = (char)getchar()) != '\n') {
            if (currentSizeArrayInBytes <= counter + 1) {
                currentSizeArrayInBytes += numByIncreseBytes;
                char* newPtrRow = new char[currentSizeArrayInBytes + 1]();
                memcpy(newPtrRow, ptrRow, counter);
                delete[] ptrRow;
                ptrRow = newPtrRow;
            }
            ptrRow[counter] = userLetter;
            counter++;
        }

        ptrRow[counter] = '\0';

        return ptrRow;
    }



    // print all strings
    void outputText() {
        lineClass* currentLine = this;
        int numberOfLine = 0;
        int cursorLine = this->getCursorLine();
        int cursorIndex = this->getCursorIndex();
        while (currentLine != NULL) {
            cout << numberOfLine;
            if (numberOfLine == cursorLine) {
                const char* MAGENTA = "\033[95m";
                const char* ENDOFCOLOR = "\033[0m";
                int indexPositon = cursorIndex;
                char* textOfIndexedLine = currentLine->ptrRow;
                char* firstPartOfLine = new char[indexPositon+2]();
                char* secondPartOfLine = new char[strlen(textOfIndexedLine) - indexPositon + 2];

                strncpy(firstPartOfLine,textOfIndexedLine,indexPositon);
                strcpy(secondPartOfLine,textOfIndexedLine+ indexPositon);
                cout << firstPartOfLine;
                cout << MAGENTA<<"|"<<ENDOFCOLOR;
                cout << secondPartOfLine;
                currentLine = currentLine->ptrNextLine;
                numberOfLine++;
                continue;
            }
            cout << (currentLine->ptrRow);
            currentLine = currentLine->ptrNextLine;
            numberOfLine++;
        }

    }


    // acess node by given number of line
    lineClass& iterationToGivenLine(int numOfString ) {
        lineClass* currentObj;
        currentObj = this;
        for (int i = 0; i < numOfString; i++) {
            currentObj = currentObj->ptrNextLine;
        }
        return *currentObj;

    }

    // acces the last node
    lineClass& iterationToLastLine() {
        int numberOfLine = 0;
        lineClass* currentObj;
        currentObj = this;
        while( (currentObj->ptrNextLine) != NULL) {
            currentObj = currentObj->ptrNextLine;
            numberOfLine++;
        }
        this->changeCursorLine(numberOfLine);

        return *currentObj;
    }
