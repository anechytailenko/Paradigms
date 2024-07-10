#include <iostream>
#include <cstring>
#include <fstream>
#include <dlfcn.h>
using namespace std;

class textEditorClass;
class lineClass;
class cursorClassl;
class CaesarCipher;



class cursorClass {
protected:
    int numOfCursorIndex ;
    int numOfCursorLine ;
public:
    cursorClass(): numOfCursorIndex(0), numOfCursorLine(0) {};

    void changeCursorCoord(int linePosition,int indexPosition) {
        this->numOfCursorLine = linePosition;
        this->numOfCursorIndex = indexPosition;
    }
    int getCursorLine() {
        return numOfCursorLine;
    }

    int getCursorIndex() {
        return numOfCursorIndex;
    }
    void changeCursorIndex(int numOfIndex) {
        this->numOfCursorIndex = numOfIndex;
    }

    friend class textEditorClass;
    friend class lineClass;
};



class lineClass {
protected:
    char* ptrRow;
    lineClass* ptrNextLine;
    char* splitInsertBeginPartText;
    char* splitInsertMiddlePartText;
    char* splitInsertLastPartText;
public:
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
    lineClass(char* splitInsertFirstPartText,char* splitInsertSecondPartText,char* splitInsertThirdPartText):
    splitInsertBeginPartText(splitInsertFirstPartText),
    splitInsertMiddlePartText(splitInsertSecondPartText),
    splitInsertLastPartText(splitInsertThirdPartText){}

    char* getLineText() {
        return ptrRow;
    }

    lineClass& iterationToGivenLine(int numOfString ) {
        lineClass* currentObj;
        currentObj = this;
        for (int i = 0; i < numOfString; i++) {
            currentObj = currentObj->ptrNextLine;
        }
        return *currentObj;

    }

    // acces the last node
    lineClass& iterationToLastLine(cursorClass& cursor) {
        int numberOfLine = 0;
        lineClass* currentObj;
        currentObj = this;
        while( (currentObj->ptrNextLine) != NULL) {
            currentObj = currentObj->ptrNextLine;
            numberOfLine++;
        }
        cursor.numOfCursorLine =numberOfLine;

        return *currentObj;
    }



    friend class textEditorClass;
};

class undoRedo {
    private:
        int amountOfConsecutiveUndo ;
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


class textEditorClass: public lineClass, public cursorClass,public CaesarCipher {
public:
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

    void outputText(lineClass& headnode, cursorClass& cursor) {
        lineClass* ptrCurrentLine = &headnode;
        int numberOfLine = 0;
        int cursorLine = cursor.numOfCursorLine;
        int cursorIndex = cursor.numOfCursorIndex;
        while (ptrCurrentLine != NULL) {
            cout << numberOfLine;
            if (numberOfLine == cursorLine) {
                const char* MAGENTA = "\033[95m";
                const char* ENDOFCOLOR = "\033[0m";
                int indexPositon = cursorIndex;
                char* textOfIndexedLine = ptrCurrentLine->ptrRow;
                char* firstPartOfLine = new char[indexPositon+2]();
                char* secondPartOfLine = new char[strlen(textOfIndexedLine) - indexPositon + 2];

                strncpy(firstPartOfLine,textOfIndexedLine,indexPositon);
                strcpy(secondPartOfLine,textOfIndexedLine+ indexPositon);
                cout << firstPartOfLine;
                cout << MAGENTA<<"|"<<ENDOFCOLOR;
                cout << secondPartOfLine;
                ptrCurrentLine = ptrCurrentLine->ptrNextLine;
                numberOfLine++;
                continue;
            }
            cout << (ptrCurrentLine->ptrRow);
            ptrCurrentLine = ptrCurrentLine->ptrNextLine;
            numberOfLine++;
        }

    }

    // usually append getTextFromUser
    char* concatenate(lineClass& currentLine, char* textToAppend = "\n\0") {
        // Calculate the length of both strings
        lineClass* ptrCurrentLine = &currentLine;
        size_t sizeFormerText = strlen(ptrCurrentLine->ptrRow);
        const char* userText = textToAppend;
        size_t sizeAppendText = strlen(userText);

        char* resultText = new char[sizeFormerText + sizeAppendText + 1];
        strcpy(resultText, ptrCurrentLine->ptrRow);
        strcpy(resultText + sizeFormerText,userText) ;

        return resultText;
    }

    // append text to the end / usually input concatenate + change the position of cursor
    void appendTextToTheEnd(lineClass& currentLine,cursorClass& cursor,char* appendText) {
        lineClass* ptrCurrentLine = &currentLine;
        ptrCurrentLine->ptrRow = appendText;
        int sizeOfAltetredText = strlen(appendText);
        cursor.numOfCursorIndex = sizeOfAltetredText;
    }


    char* copiedText(lineClass& currentLine, cursorClass& cursor, int userAmountCharToCopy) {
        int startIndex = cursor.numOfCursorIndex;
        lineClass* ptrCurrentLine = &currentLine;
        char* textFromLine = ptrCurrentLine->ptrRow;
        char* copiedTextFromLine = new char[userAmountCharToCopy+ 1];
        int indexCopyResultText = 0;
        for(int i = startIndex; i < (startIndex+userAmountCharToCopy) ; i++) {
            copiedTextFromLine[indexCopyResultText] = textFromLine[i];
            indexCopyResultText++;
        }
        copiedTextFromLine[indexCopyResultText] = '\0';
        return copiedTextFromLine;
    }


    void deletedText(lineClass& currentLine,cursorClass& cursor,int userAmountLetterToCopy) {
        lineClass* ptrCurrentLine = &currentLine;

        int indexStartTextDelete = cursor.numOfCursorIndex;
        char* textOfLine = ptrCurrentLine->ptrRow;
        char* firstPartOfText = new char[indexStartTextDelete+1];

        // kinda validation
        if (textOfLine[cursor.numOfCursorIndex] == '\0' || textOfLine[cursor.numOfCursorIndex]== '\n') {
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

        ptrCurrentLine->ptrRow = textAferDelete;
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


    void showIndexesSubstring(lineClass& headLine,char* userSubstring) {

        cout<<"\nHere is the coordinates of the searched substring:"<< endl;
        lineClass* currentLine = &headLine;
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

    lineClass partsForInsert(lineClass& currentLine,cursorClass& cursor, char* modeInsert = "Non-replacement", char* modeTextFromUser = "Insert" ) {
        char* formerText = currentLine.ptrRow;
        char* userText;
        if (modeTextFromUser == "Insert") {
            userText = getTextFromUser();
        } else {
            userText = modeTextFromUser;
        }

        int cursorIndex = cursor.numOfCursorIndex;
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
        if (modeInsert != "Non-replacement") {
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

        lineClass objectForInsert(firstPartOfSentence,userText,secondPartOfSentence);
        return objectForInsert;
    }


    void startNewLine(lineClass& currentLine,cursorClass& cursor) {
        int indexOfCursor = cursor.numOfCursorIndex;
        lineClass* ptrNewLine = new lineClass();
        lineClass* ptrCurrentLine = &currentLine;
        char* textOfPreviousLine = ptrCurrentLine->ptrRow;
        char* newTextOfPreviousLine = new char[strlen(textOfPreviousLine)+3]();
        char* textForNewLine = new char[strlen(textOfPreviousLine)+1]();
        int indexOfPreviosLine = cursor.numOfCursorLine;
        if (ptrCurrentLine->ptrNextLine == nullptr){
            if(textOfPreviousLine[indexOfCursor]=='\0') {
                strcpy(newTextOfPreviousLine,textOfPreviousLine);
                strcat(newTextOfPreviousLine,"\n\0");
                ptrNewLine->ptrRow = "\0";
                cursor.numOfCursorLine = indexOfPreviosLine+1;
                cursor.numOfCursorIndex = 0;
            }else {
                strncpy(newTextOfPreviousLine,textOfPreviousLine,indexOfCursor);
                strcat(newTextOfPreviousLine,"\n\0");
                strcpy(textForNewLine,textOfPreviousLine+indexOfCursor);
                ptrNewLine->ptrRow = textForNewLine;
                int sizeOfText = strlen(textForNewLine);
                cursor.numOfCursorLine = indexOfPreviosLine+1;
                cursor.numOfCursorIndex = sizeOfText;
            }
            ptrNewLine->ptrNextLine = nullptr;
            ptrCurrentLine->ptrNextLine = ptrNewLine;
            ptrCurrentLine->ptrRow = newTextOfPreviousLine;
        } else {// locate in middle
            if((textOfPreviousLine[indexOfCursor]== '\n') || (textOfPreviousLine[indexOfCursor]== '\0') ) {
                strcpy(newTextOfPreviousLine,textOfPreviousLine);
                ptrNewLine->ptrRow = "\n\0";
                cursor.numOfCursorLine = indexOfPreviosLine+1;
                cursor.numOfCursorIndex = 0;
            }else {
                strncpy(newTextOfPreviousLine,textOfPreviousLine,indexOfCursor);
                strcat(newTextOfPreviousLine,"\n\0");
                strcpy(textForNewLine,textOfPreviousLine+indexOfCursor);
                ptrNewLine->ptrRow = textForNewLine;
                int sizeOfText = strlen(textForNewLine);
                cursor.numOfCursorLine = indexOfPreviosLine+1;
                cursor.numOfCursorIndex = sizeOfText-1;
            }
            ptrCurrentLine->ptrRow = newTextOfPreviousLine;
            lineClass* formerNextAdressLine = ptrCurrentLine->ptrNextLine;
            ptrCurrentLine->ptrNextLine = ptrNewLine;
            ptrNewLine->ptrNextLine = formerNextAdressLine;
        }
    }

    void insert(lineClass& currentLine,cursorClass& cursor, lineClass& ObjectForInsert){
        char* firstPartOfFormerText = ObjectForInsert.splitInsertBeginPartText;
        char* userText = ObjectForInsert.splitInsertMiddlePartText;
        char* secondPartOfFormerText = ObjectForInsert.splitInsertLastPartText;
        char* resultedText = new char[strlen(firstPartOfFormerText)+ strlen(userText)+strlen(secondPartOfFormerText)+1];
        strcpy(resultedText,firstPartOfFormerText);
        strcat(resultedText,userText);
        int sizeOfResultedText = strlen(resultedText);
        cursor.numOfCursorIndex = sizeOfResultedText;
        strcat(resultedText,secondPartOfFormerText);
        currentLine.ptrRow = resultedText;
    }


    void saveToFile(lineClass& currentLine,char* fileName) {
        ofstream saveFile(fileName);
        lineClass* currentObj;
        currentObj = &currentLine;
        while( currentObj != NULL) {
            saveFile<<currentObj->ptrRow;
            currentObj = currentObj->ptrNextLine;
        }
        saveFile.close();
    }

    void uploadFromFile(lineClass& headNode, cursorClass& cursor,char* fileName) {
        ifstream uploadedFile(fileName);




        if (!uploadedFile.is_open()) {
            cout << "Failed to open the file" << fileName << endl;
        } else {


            int indexOfCurrentCharInLine = 0;
            char currentChar;
            lineClass* ptrCurrentObjLine =  &(headNode.iterationToGivenLine(cursor.numOfCursorLine));
            int indexPositionPreviousCursor = cursor.numOfCursorIndex;
            indexOfCurrentCharInLine = indexPositionPreviousCursor;
            char* previousTextOfStartLine = ptrCurrentObjLine->ptrRow;
            const int initialSizeOfPtr  = strlen(previousTextOfStartLine) +3;
            char* ptrOnTextOfLine = new char[initialSizeOfPtr];
            char* endedPartPreviousText = new char[initialSizeOfPtr];
            strncpy(ptrOnTextOfLine,previousTextOfStartLine,indexPositionPreviousCursor);
            strcpy(endedPartPreviousText,previousTextOfStartLine+indexPositionPreviousCursor);

            int indexOfStartLine =  cursor.numOfCursorLine;
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



                    ptrOnTextOfLine = new char[initialSizeOfPtr];
                    indexOfCurrentCharInLine = 0;
                    numberOfUplodedFile ++;
                    continue;
                }




                ptrOnTextOfLine[indexOfCurrentCharInLine++] = currentChar;
            }

            int indexOfCursorLine = numberOfUplodedFile + indexOfStartLine;
            lineClass* ptrCurrentObj = &(headNode.iterationToGivenLine(indexOfCursorLine));
            char* textToLastUploadedLine = new char[strlen(endedPartPreviousText)+strlen(ptrOnTextOfLine)+3];
            strcat(textToLastUploadedLine,ptrOnTextOfLine);
            strcat(textToLastUploadedLine,endedPartPreviousText);
            ptrCurrentObj->ptrRow = textToLastUploadedLine;
            cursor.numOfCursorIndex = strlen(ptrOnTextOfLine);
            cursor.numOfCursorLine = indexOfCursorLine;
            uploadedFile.close();

        }

    }
};
int main(){
    int mode ;
    lineClass* headNodeLine = new lineClass();
    lineClass* headNodeLineCopy  = new lineClass(*headNodeLine);
    textEditorClass alteration;
    cursorClass cursor;
    undoRedo state(headNodeLineCopy);

    int amountCharToSave;
    char* cutCopiedText = nullptr;
    char* nameOfSaveFile;
    char* nameOfUploadFile;
    lineClass* lastNode;
    char* textOfLastNode;

    while(true) {
        cout <<"\nEnter the number of the command:\n1.Change the position of a cursor (line,index)\n2.Append text to the end;\n3.Start new line;\n4.Delete substring;\n5.Insertion without replacement;\n6.Insertion with replacement;\n7.Print current text;\n8.Cut command;\n9.Copy command;\n10.Paste command;\n11.Search by substring;\n12.Command save to file;\n13.Upload from file;\n14.Undo;\n15.Redo;\n16.File encryption/decryption;\n";
        cin >> mode;


        switch (mode) {
            case 1: //checked
                cout << "Enter a coordinate of cursor by whitespace like \"Line Index\" " << endl;
            int linePosition, indexPosition;
            cin >> linePosition >> indexPosition;
            cursor.changeCursorCoord(linePosition,indexPosition);
            state.setAmountOfConsecutiveUndo();
            headNodeLineCopy = new lineClass(*headNodeLine);
            state.funcToFillArray(headNodeLineCopy,"Undo");
            break;
            case 2: // checked
            {
                cout << "Append text to the end"<< endl;
                lineClass& currentObj = headNodeLine->iterationToLastLine(cursor);
                char* appendText = alteration.getTextFromUser();
                alteration.appendTextToTheEnd(currentObj,cursor,appendText);
                state.setAmountOfConsecutiveUndo();
                headNodeLineCopy = new lineClass(*headNodeLine);
                state.funcToFillArray(headNodeLineCopy,"Undo");
            }
            break;
            case 3:// checked
            {
                cout << "Start new line"<<endl;
                lineClass& currentObj = headNodeLine->iterationToGivenLine(cursor.getCursorLine());
                alteration.startNewLine(currentObj,cursor);
                state.setAmountOfConsecutiveUndo();
                headNodeLineCopy = new lineClass(*headNodeLine);
                state.funcToFillArray(headNodeLineCopy,"Undo");
            }
            break;
            case 4: //checked
            {
                cout << "Delete substring"<< endl;
                cout << "Enter the amount of letter that you wanna delete"<< endl;
                int amountCharToDelete;
                cin >> amountCharToDelete;
                lineClass& currentObj = headNodeLine->iterationToGivenLine(cursor.getCursorLine());
                alteration.deletedText(currentObj,cursor,amountCharToDelete);
                state.setAmountOfConsecutiveUndo();
                headNodeLineCopy = new lineClass(*headNodeLine);
                state.funcToFillArray(headNodeLineCopy,"Undo");
            }
            break;
            case 5: // checked
            {
                cout << "Insertion without replacement"<< endl;
                lineClass& currentObj = headNodeLine->iterationToGivenLine(cursor.getCursorLine());
                lineClass partsForInsertion = alteration.partsForInsert(currentObj,cursor);
                alteration.insert(currentObj,cursor,partsForInsertion);
                state.setAmountOfConsecutiveUndo();
                headNodeLineCopy = new lineClass(*headNodeLine);
                state.funcToFillArray(headNodeLineCopy,"Undo");
            }
            break;
            case 6: // checked
            {
                cout << "Insertion with replacement"<< endl;
                lineClass& currentObj = headNodeLine->iterationToGivenLine(cursor.getCursorLine());
                lineClass partsForInsertion = alteration.partsForInsert(currentObj,cursor,"Replacement");
                state.setAmountOfConsecutiveUndo();
                headNodeLineCopy = new lineClass(*headNodeLine);
                state.funcToFillArray(headNodeLineCopy,"Undo");
            }
            break;
            case 7: // checked
                cout<< "Print current text"<< endl;
            alteration.outputText(*headNodeLine,cursor);
            break;
            case 8: {
                cout << "Cut command"<< endl;
                cout << "Enter the amount of letter that you wanna cut"<< endl;
                int amountCharToCut;
                cin >> amountCharToCut;
                lineClass& currentObj = headNodeLine->iterationToGivenLine(cursor.getCursorLine());
                cutCopiedText = alteration.copiedText(currentObj,cursor,amountCharToCut);
                alteration.deletedText(currentObj,cursor,amountCharToCut);
                state.setAmountOfConsecutiveUndo();
                headNodeLineCopy = new lineClass(*headNodeLine);
                state.funcToFillArray(headNodeLineCopy,"Undo");
            }
            break;
            case 9: {
                cout << "Copy command";
                lineClass& currentObj = headNodeLine->iterationToGivenLine(cursor.getCursorLine());
                cout << "Enter the amount of letter that you wanna copy"<< endl;
                int amountCharToCopy;
                cin >> amountCharToCopy;
                cutCopiedText = alteration.copiedText(currentObj,cursor,amountCharToCopy);
            }
            break;
            case 10:
                cout<< "Paste command";
            if (cutCopiedText == nullptr) {
                cout<< "\nNothing to paste";
                break;
            }else{
                lineClass& currentObj = headNodeLine->iterationToGivenLine(cursor.getCursorLine());
                lineClass partsForInsertion = alteration.partsForInsert(currentObj,cursor, "Non-replacement", cutCopiedText);
                alteration.insert(currentObj,cursor,partsForInsertion);
                state.setAmountOfConsecutiveUndo();
                headNodeLineCopy = new lineClass(*headNodeLine);
                state.funcToFillArray(headNodeLineCopy,"Undo");
            }
            break;
            case 11: {
                //checked
                cout<< "Search by substring";
                char* searchedSubstring = alteration.getTextFromUser();
                alteration.showIndexesSubstring(*headNodeLine,searchedSubstring);
                state.funcToFillArray(headNodeLineCopy,"Undo");
            }
            break;
            case 12:
                cout<< "Command save to file";
            nameOfSaveFile = alteration.getTextFromUser("\nEnter the name of file with .txt extention");
            alteration.saveToFile(*headNodeLine,nameOfSaveFile);
            break;
            case 13:// checked
                cout << "Upload from text";
            nameOfUploadFile = alteration.getTextFromUser("\nEnter the name of file with .txt extention");
            alteration.uploadFromFile(*headNodeLine,cursor,nameOfUploadFile);
            state.setAmountOfConsecutiveUndo();
            headNodeLineCopy = new lineClass(*headNodeLine);
            state.funcToFillArray(headNodeLineCopy,"Undo");
            break;
            case 14:
                cout << "Undo";
            delete[] headNodeLine;
            headNodeLine = new lineClass(*(state.undoOperation()));
            lastNode = &(headNodeLine->iterationToLastLine(cursor));
            textOfLastNode = lastNode->getLineText();
            cursor.changeCursorIndex(strlen(textOfLastNode));
            state.setAmountOfConsecutiveUndo(1);
            break;
            case 15:
                cout << "Redo";
            state.funcToFillArray(headNodeLineCopy,"Undo");
            headNodeLine = state.redoOperation();
            break;