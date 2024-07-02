#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class sentencePartsInsert {
private:
    char* firstPartOfSentence;
    char* userText;
    char* secondPartOfSentence;
public:
    sentencePartsInsert(char* firstPartOfSentence, char* userText, char* secondPartOfSentence)
    :firstPartOfSentence{firstPartOfSentence},
    userText{userText},
    secondPartOfSentence{secondPartOfSentence}{}

    char* getFirstPartOfSentence() {
        return firstPartOfSentence;
    }
    char* getUserPartOfSentence() {
        return userText;
    }
    char* getSecondPartOfSentence() {
        return secondPartOfSentence;
    }

};

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

    // usually append getTextFromUser
    char* concatenate(char* textToAppend = "\n\0") {
        // Calculate the length of both strings
        size_t sizeFormerText = strlen(this->ptrRow);
        const char* userText = textToAppend;
        size_t sizeAppendText = strlen(userText);

        char* resultText = new char[sizeFormerText + sizeAppendText + 1];
        strcpy(resultText, this->ptrRow);
        strcpy(resultText + sizeFormerText,userText) ;

        return resultText;
    }
    // append text to the end / usually input concatenate + chamge the position of cursor
    void appendTextToTheEnd(char* appendText) {
        this->ptrRow = appendText;
        int sizeOfAltetredText = strlen(appendText);
        this->changeCursorIndex(sizeOfAltetredText);

    }
    char* copiedText(int cursorIndex, int userAmountCharToCopy) {
        int startIndex = cursorIndex;
        char* textFromLine = this->ptrRow;
        char* copiedTextFromLine = new char[userAmountCharToCopy+ 1];
        int indexCopyResultText = 0;
        for(int i = startIndex; i < (startIndex+userAmountCharToCopy) ; i++) {
            copiedTextFromLine[indexCopyResultText] = textFromLine[i];
            indexCopyResultText++;
        }
        copiedTextFromLine[indexCopyResultText] = '\0';
        return copiedTextFromLine;
    }

    void deletedText(int userAmountLetterToCopy) {
        int indexStartTextDelete = numOfCursorIndex;
        char* textOfLine = this->ptrRow;
        char* firstPartOfText = new char[indexStartTextDelete+1];

        // kinda validation
        if (textOfLine[this->getCursorIndex()] == '\0' || textOfLine[this->getCursorIndex()]== '\n') {
            cout << "\nThere is no text remained, so u cant delete"<< endl;
            return;
        }

        for(int j = 0; j < indexStartTextDelete; j++) {
            firstPartOfText[j] = textOfLine[j];
        }
        firstPartOfText[indexStartTextDelete]='\0';

        int indexStartSecondPart = indexStartTextDelete + userAmountLetterToCopy ; // 11
        char* secondPartOfText = new char[strlen(textOfLine)-indexStartSecondPart+1];

        int indexInSecondPart = 0;
        for (int k = indexStartSecondPart; k < (strlen(textOfLine)); k++ ) {
            secondPartOfText[indexInSecondPart] = textOfLine[k];
            indexInSecondPart++;
        }
        secondPartOfText[indexInSecondPart] = '\0';


        size_t sizeOfFirstPart = strlen(firstPartOfText);
        size_t sizeOfSecondPart = strlen(secondPartOfText);
        char* textAferDelete = new char[sizeOfFirstPart + sizeOfSecondPart + 1];
        strcpy(textAferDelete,firstPartOfText);
        strcat(textAferDelete,secondPartOfText);

        this->ptrRow = textAferDelete;
    }

    int* indexSearchAlgorithm( char* currentText, char* substring = "\n") {
        int i, j;
        size_t sizeOfCurrentText = strlen(currentText);
        size_t sizeOfSubstring = strlen(substring);
        int counter = 0;
        int initialSizeDynamicArray = 10;
        int currentSizeDynamicArray = initialSizeDynamicArray;
        int* dynamicArray = new int[initialSizeDynamicArray];


        for ( i = 0; i <= sizeOfCurrentText - sizeOfSubstring; i++) {
            for (  j = 0; j < sizeOfSubstring; j++) {
                if (currentText[i + j] != substring[j]) {
                    break;
                }
            }

            if (j == sizeOfSubstring) {
                if (counter >= currentSizeDynamicArray) {
                    int* new_arr = new int[currentSizeDynamicArray+5];
                    copy(dynamicArray, dynamicArray + currentSizeDynamicArray, new_arr);
                    delete[] dynamicArray;
                    dynamicArray = new_arr;
                    currentSizeDynamicArray += 5;
                }
                dynamicArray[counter] = i;
                counter++;
                i += j - 1;
            }
        }

        if (counter == 0) {
            free(dynamicArray);
            return NULL;
        }else {
            dynamicArray[counter] = -1;
            return dynamicArray;
        }
    }

    void showIndexesSubstring(char* userSubstring) {

        cout<<"\nHere is the coordinates of the searched substring:"<< endl;
        lineClass* currentLine;
        currentLine = this;
        bool existenceOfSubstring = 0;

        char* currentTextOfLine;
        int* indexes;
        int numOfLine = 0;


        while(currentLine != NULL) {
            currentTextOfLine = currentLine->ptrRow;
            indexes = indexSearchAlgorithm(currentTextOfLine,userSubstring);

            if (indexes != NULL) {
                for( int i = 0; indexes[i] != -1; i++) {
                    cout << "("<< numOfLine<<","<<indexes[i]<< ")";
                    existenceOfSubstring = 1;
                }
                delete indexes;
            }
            numOfLine ++;
            currentLine = currentLine->ptrNextLine;
        }
        if (existenceOfSubstring == 0) {
            cout << "Your substring does not located in text ";
        }
    }
    sentencePartsInsert partsForInsert(char* modeInsert = "Non-replacement", char* modeTextFromUser = "Insert" ) {
        char* formerText = this->ptrRow;
        char* userText;
        if (modeTextFromUser == "Insert") {
            userText = getTextFromUser();
        } else {
            userText = modeTextFromUser;
        }

        int cursorIndex = this->getCursorIndex();
        char* firstPartOfSentence = new char[cursorIndex+1];
        size_t sizeOfUserText = strlen(userText);
        size_t sizeOfFirstPart = strlen(firstPartOfSentence);
        size_t sizeOfFormerText = strlen(formerText);
        char* secondPartOfSentence = new char[sizeOfFormerText - sizeOfFirstPart + 2];
        int indexEndOfFirstPart;

        int* arrayOfTab = indexSearchAlgorithm(formerText);

        if (arrayOfTab!= NULL) {
            int indexOfTab = arrayOfTab[0];
            formerText[indexOfTab] = '\0';
        }


        if(formerText[cursorIndex]!='\0' ) {
            indexEndOfFirstPart = cursorIndex - 1;
            for (int i= 0; i <= indexEndOfFirstPart; i ++) {
                firstPartOfSentence[i] = formerText[i];
            }
            firstPartOfSentence[indexEndOfFirstPart+1] = '\0';
        }
        if (cursorIndex == 0) {
            firstPartOfSentence[0] = '\0';
        }
        if(formerText[cursorIndex]=='\0' ) {
            strcpy(firstPartOfSentence,formerText);
        }


        int indexEndOfSecondPart = sizeOfFormerText;
        int j = 0;
        for( int i = cursorIndex; i <= indexEndOfSecondPart; i ++) {
            secondPartOfSentence[j] = formerText[i];
            j++;
        }
        if((arrayOfTab != NULL)) {
            strcat(secondPartOfSentence,"\n\0");
        }
        if (modeInsert == "Replacement") {
            int indexStartAfterReplacement = strlen(userText);
            char* secondPartOfSentenceWithReplacement = new char[strlen(secondPartOfSentence)];
            for(int i = 0; secondPartOfSentence[indexStartAfterReplacement] != '\0'; i ++) {
                secondPartOfSentenceWithReplacement[i] = secondPartOfSentence[indexStartAfterReplacement];
                indexStartAfterReplacement++;
            }
            if(arrayOfTab != NULL && strlen(secondPartOfSentenceWithReplacement) == 0) {
                strcat(secondPartOfSentenceWithReplacement,"\n\0");
            }
            secondPartOfSentence = secondPartOfSentenceWithReplacement;
        }



        sentencePartsInsert objectForInsert = (sentencePartsInsert (firstPartOfSentence,userText,secondPartOfSentence));
        return objectForInsert;
    }
    void startNewLine() {
        int indexOfCursor = this->getCursorIndex();
        lineClass* ptrNewLine = new lineClass();
        char* textOfPreviousLine = this->ptrRow;
        char* newTextOfPreviousLine = new char[strlen(textOfPreviousLine)+3]();
        char* textForNewLine = new char[strlen(textOfPreviousLine)+1]();
        int indexOfPreviosLine = this->getCursorLine();
        if (this->ptrNextLine == nullptr){
            if(textOfPreviousLine[indexOfCursor]=='\0') {
                strcpy(newTextOfPreviousLine,textOfPreviousLine);
                strcat(newTextOfPreviousLine,"\n\0");
                ptrNewLine->ptrRow = "\0";
                this->changeCursorCoord(indexOfPreviosLine+1,0);
            }else {
                strncpy(newTextOfPreviousLine,textOfPreviousLine,indexOfCursor);
                strcat(newTextOfPreviousLine,"\n\0");
                strcpy(textForNewLine,textOfPreviousLine+indexOfCursor);
                ptrNewLine->ptrRow = textForNewLine;
                int sizeOfText = strlen(textForNewLine);
                this->changeCursorCoord(indexOfPreviosLine+1,sizeOfText);
            }
            ptrNewLine->ptrNextLine = nullptr;
            this->ptrNextLine = ptrNewLine;
            this->ptrRow = newTextOfPreviousLine;
        } else {// locate in middle
            if((textOfPreviousLine[indexOfCursor]== '\n') || (textOfPreviousLine[indexOfCursor]== '\0') ) {
                strcpy(newTextOfPreviousLine,textOfPreviousLine);
                ptrNewLine->ptrRow = "\n\0";
                this->changeCursorCoord(indexOfPreviosLine+1,0);

            }else {
                strncpy(newTextOfPreviousLine,textOfPreviousLine,indexOfCursor);
                strcat(newTextOfPreviousLine,"\n\0");
                strcpy(textForNewLine,textOfPreviousLine+indexOfCursor);
                ptrNewLine->ptrRow = textForNewLine;
                int sizeOfText = strlen(textForNewLine);
                this->changeCursorCoord(indexOfPreviosLine+1,sizeOfText-1);
            }
            this->ptrRow = newTextOfPreviousLine;
            lineClass* formerNextAdressLine = this->ptrNextLine;
            this->ptrNextLine = ptrNewLine;
            ptrNewLine->ptrNextLine = formerNextAdressLine;

        }
    }




    void insert(sentencePartsInsert ObjectForInsert) {
        char* firstPartOfFormerText = ObjectForInsert.getFirstPartOfSentence();
        char* userText = ObjectForInsert.getUserPartOfSentence();
        char* secondPartOfFormerText = ObjectForInsert.getSecondPartOfSentence();
        char* resultedText = new char[strlen(firstPartOfFormerText)+ strlen(userText)+strlen(secondPartOfFormerText)+1];
        strcpy(resultedText,firstPartOfFormerText);
        strcat(resultedText,userText);
        int sizeOfResultedText = strlen(resultedText);
        this->changeCursorIndex(sizeOfResultedText);
        strcat(resultedText,secondPartOfFormerText);
        this->ptrRow = resultedText;
    }
    void saveToFile(char* fileName) {
        ofstream saveFile(fileName);
        lineClass* currentObj;
        currentObj = this;
        while( currentObj != NULL) {
            saveFile<<currentObj->ptrRow;
            currentObj = currentObj->ptrNextLine;
        }
        saveFile.close();
    }

    void uploadFromFile(char* fileName, lineClass* headNode) {
        ifstream uploadedFile(fileName); // rewrite to put file name




        if (!uploadedFile.is_open()) {
            cout << "Failed to open the file" << fileName << endl;
        } else {


            int indexOfCurrentCharInLine = 0;

            char currentChar;

            lineClass* ptrCurrentObjLine =  &(headNode->iterationToGivenLine(this->getCursorLine()));
            int indexPositionPreviousCursor = ptrCurrentObjLine->numOfCursorIndex;
            indexOfCurrentCharInLine = indexPositionPreviousCursor;
            char* previousTextOfStartLine = ptrCurrentObjLine->ptrRow;
            const int initialSizeOfPtr  = strlen(previousTextOfStartLine) +3;
            char* ptrOnTextOfLine = new char[initialSizeOfPtr];
            char* endedPartPreviousText = new char[initialSizeOfPtr];
            strncpy(ptrOnTextOfLine,previousTextOfStartLine,indexPositionPreviousCursor);
            strcpy(endedPartPreviousText,previousTextOfStartLine+indexPositionPreviousCursor);

            int indexOfStartLine =  this->getCursorLine();
            int numberOfUplodedFile = 0;


            while (uploadedFile.get(currentChar)) {

                if (indexOfCurrentCharInLine == initialSizeOfPtr - 1) { //
                    char *new_ptrOnTextOfLine = new char[initialSizeOfPtr * 2];
                    strcpy(new_ptrOnTextOfLine, ptrOnTextOfLine);
                    delete[] ptrOnTextOfLine;
                    ptrOnTextOfLine = new_ptrOnTextOfLine;
                }
                if (currentChar == '\n') {
                    ptrOnTextOfLine[indexOfCurrentCharInLine] = '\0';
                    char* textForNewLine = new char[strlen(ptrOnTextOfLine)+3];
                    strcpy(textForNewLine,ptrOnTextOfLine);
                    strcat(textForNewLine,"\n\0");
                    ptrCurrentObjLine->ptrRow = textForNewLine;
                    lineClass* ptrNextObjLine = new lineClass();
                    ptrCurrentObjLine->ptrNextLine = ptrNextObjLine;
                    ptrCurrentObjLine = ptrNextObjLine;


                    delete[] ptrOnTextOfLine;
                    ptrOnTextOfLine = new char[initialSizeOfPtr];
                    indexOfCurrentCharInLine = 0;
                    numberOfUplodedFile ++;
                    continue;
                }




                ptrOnTextOfLine[indexOfCurrentCharInLine++] = currentChar;
            }

            int indexOfCursorLine = numberOfUplodedFile + indexOfStartLine;
            lineClass* ptrCurrentObj = &(this->iterationToGivenLine(indexOfCursorLine));
            char* textToLastUploadedLine = new char[strlen(endedPartPreviousText)+strlen(ptrOnTextOfLine)+3];
            strcat(textToLastUploadedLine,ptrOnTextOfLine);
            strcat(textToLastUploadedLine,endedPartPreviousText);
            ptrCurrentObj->ptrRow = textToLastUploadedLine;
            this->changeCursorCoord(indexOfCursorLine,strlen(ptrOnTextOfLine));

            uploadedFile.close();

        }

    }
};

class undoRedo {
    private:
        static int amountOfConsecutiveUndo ;
        lineClass* undoArray[4] = {nullptr};
        lineClass* redoArray[4] = {nullptr};
    public:
    undoRedo(lineClass* headNode) {
        undoArray[0]= headNode;
    }
        void setAmountOfConsecutiveUndo(int userUndo = 0) {
        this->amountOfConsecutiveUndo = userUndo;
    }

        void funcToFillArray(lineClass* ptrOnHead, char* modeOfReturn) {
            lineClass** givenArray;
            if (modeOfReturn == "Undo") {
                givenArray = undoArray;
            } else {
                givenArray = redoArray;
            }
            for (int i = 0; i <=3; i++) {
                if (i == 3 && givenArray[i] != nullptr) {// change size
                    givenArray[i+1] = ptrOnHead;
                    for ( int j = 0 ; j <=3; j++) {
                        givenArray[j] = givenArray[j+1];
                    }
                }
                if (givenArray[i] != nullptr) {
                    continue;
                }
                if (givenArray[i] == nullptr) {
                    givenArray[i] = ptrOnHead;
                    break;
                }
            }
        }
        lineClass* funcToDeleteLastElementInArray( char* modeOfReturn ) {
            lineClass** givenArray;
            if (modeOfReturn == "Undo") {
                givenArray = this->undoArray;
            } else {
                givenArray = this->redoArray;
            }
            for (int i = 3; i >=0 ; i--) {
                if (i== 0 && givenArray[i]==nullptr) {
                    cout<< "End of undo stack" << endl;
                    return nullptr;
                }
                if (givenArray[i] != nullptr) {
                    lineClass* deletedState = new lineClass(*(givenArray[i]));
                    givenArray[i] = nullptr;
                    return deletedState;
                }
            }
        }
        lineClass* getLastState(char* modeOfReturn) {
            lineClass** givenArray;
            if (modeOfReturn == "Undo") {
                givenArray = this->undoArray;
            } else {
                givenArray = this->redoArray;
            }
            for (int i = 3; i >=0 ; i--) {
                if (givenArray[i] != nullptr) {
                    lineClass* lastState = givenArray[i];
                    return lastState;

                }
            }
        }

        void clearRedoArray() {
            for (int i =0; i <=3; i++) {
                redoArray[i] =nullptr;
            }
    }


        lineClass* undoOperation() {
            lineClass* deletedState = funcToDeleteLastElementInArray("Undo");
            if(amountOfConsecutiveUndo == 0) {
                clearRedoArray();
            }
            funcToFillArray(deletedState,"Redo");
            lineClass* currentState = getLastState("Undo");
            return currentState;
        }

        lineClass* redoOperation() {
            lineClass* deletedState = funcToDeleteLastElementInArray("Redo");
            funcToFillArray(deletedState,"Undo");
            lineClass* currentState = getLastState("Undo");
            return currentState;
        }

    };
