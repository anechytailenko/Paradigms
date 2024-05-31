
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// clear buffer - essential action for properly using function getchar()
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void validationOfRealocation(void* ptrRow) {
    if (ptrRow == NULL) {
        printf("Error occur during reallocation");
        exit(0);
    }
}



// user comand
int userCommand () {
    int userMode ;
    printf("\nChoose the command:\n1.Append text symbols to the end:\n2.Start the new line:\n3.Insert the text by line and symbol index\n4.Print the current text to console\n5.Use files to saving the information\n6.Search (please note that text can be found more than once)\n7.Use files to loading the information\n");
    scanf("%d",&userMode);
    clearInputBuffer();
    return userMode;
}


// user input string that is stoed in dynamic array
char* accessPtrString() {
    const int InitialNumOfBytePerString = 10;
    const int numByIncreseBytes = 5;
    int counter;
    char* ptrRow;
    char userLetter ;
    int currentSizeArrayInBytes;

    ptrRow = (char*) malloc(InitialNumOfBytePerString);
    counter = 0;
    currentSizeArrayInBytes = InitialNumOfBytePerString;
    while((userLetter = (char)getchar()) != '\n') {
        if (currentSizeArrayInBytes <= counter+1) {
            currentSizeArrayInBytes += numByIncreseBytes;
            ptrRow = (char*) realloc(ptrRow,currentSizeArrayInBytes);
            validationOfRealocation(ptrRow);
        }
        ptrRow[counter] = userLetter;
        counter ++;
    }

    ptrRow[counter] = '\0';

    return ptrRow;
    }



// structure of nodes
typedef struct nodeOfLinkedList {
    char* ptrOnRow;
    struct nodeOfLinkedList* nextNodeAdress;
} node;



// function create node
node* createNodeWithText() {
    node* Node = (node*) malloc (sizeof(node));
    Node->ptrOnRow = accessPtrString();
    Node->nextNodeAdress = NULL;
    return Node;
}


node* createNodeWithoutText() {
    node* Node = (node*) malloc (sizeof(node));
    char* ptrRow = (char*) malloc (3);
    ptrRow[0] = '\0';
    Node->ptrOnRow = ptrRow;
    Node->nextNodeAdress = NULL;
    return Node;
}

// function of add node
void addNode(node* headOfLinkedList) {
    if ((headOfLinkedList->ptrOnRow) == 0) {
        headOfLinkedList->ptrOnRow= accessPtrString();
        headOfLinkedList->nextNodeAdress = NULL;
    } else {
        node* currentNode = headOfLinkedList;
        while(currentNode->nextNodeAdress != NULL) {
            currentNode = currentNode->nextNodeAdress;
        }
        currentNode->nextNodeAdress = createNodeWithText();

    }
}

// access last string and append text to it
void appendTextToEnd(node* headOfLinkedList) {
    if ((headOfLinkedList->ptrOnRow) == 0) {
        addNode(headOfLinkedList);
    }else {
        node* currentNode = headOfLinkedList;
        while(currentNode->nextNodeAdress != NULL) {
            currentNode = currentNode->nextNodeAdress;
        }
        char* ptrOldRow = currentNode->ptrOnRow ;
        char* ptrNewRow = accessPtrString();
        int amountOfLettersInOldRow = strlen(ptrOldRow);
        int amountOfLettersInNewRow = strlen(ptrNewRow);
        ptrOldRow = (char*) realloc(ptrOldRow,amountOfLettersInOldRow+amountOfLettersInNewRow+3);
        validationOfRealocation(ptrOldRow);
        for (int i = 0; i <= amountOfLettersInNewRow; i ++) {
            ptrOldRow[amountOfLettersInOldRow] = ptrNewRow[i];
            amountOfLettersInOldRow ++;
        }
        currentNode->ptrOnRow = ptrOldRow;
    }
}


//add new string
void newLine(node* headOfLinkedList) {
    if ((headOfLinkedList->ptrOnRow) == 0) {
        char* ptrRow =(char*) malloc(3);
        ptrRow[0] = '\n';
        ptrRow[1] = '\0';
        headOfLinkedList->ptrOnRow = ptrRow;
        headOfLinkedList->nextNodeAdress = createNodeWithoutText();
    } else {
        node* currentNode = headOfLinkedList;
        while(currentNode->nextNodeAdress != NULL) {
            currentNode = currentNode->nextNodeAdress;
        }
        char* ptrRow = currentNode->ptrOnRow;
        int amountOfLetters = strlen(ptrRow);
        ptrRow =  (char*) realloc(ptrRow,amountOfLetters+ 3);
        validationOfRealocation(ptrRow);
        ptrRow[amountOfLetters] = '\n';
        amountOfLetters++;
        ptrRow[amountOfLetters] = '\0';
        currentNode->ptrOnRow = ptrRow;
        currentNode->nextNodeAdress = createNodeWithoutText();

    }

}




// read text from memory
int readTextFromMemory(node* headOfLinkedList) {
    int numberOfString = 0;
    node* currentNode = headOfLinkedList;
    while(currentNode != NULL) {
        printf("%d",numberOfString);
        printf("%s",currentNode->ptrOnRow);
        // printf("NODE WAS READED");
        currentNode = currentNode->nextNodeAdress;
        numberOfString ++;
    }
    return numberOfString;
}

// function for working with file
char* getName() {
    printf("Enter the name of file with .txt extention where u wanna upload current text: ");
    char* fileName = accessPtrString();
    return fileName;
}


void saveToFile(char* fileName,node* headOfLinkedList) {
    FILE* fptr = fopen(fileName,"w");
    node* currentNode = headOfLinkedList;
    while(currentNode != NULL) {
        fprintf(fptr,"%s",currentNode->ptrOnRow);
        currentNode = currentNode->nextNodeAdress;
    }
    fclose(fptr);
}

// algorith for search output the array of int where each represents the start point of string

int* indexSearchAlgorithm(char* fullText, int lenFullText, char* substring, int lenSubstring) {
    int i, j;
    int counter = 0;
    int initialSizeDynamicArray = 10;
    int currentSizeDynamicArray = initialSizeDynamicArray;
    int* dynamicArray = (int*) calloc(initialSizeDynamicArray, sizeof(int));


    for (i = 0; i <= lenFullText - lenSubstring; i++) {
        for (j = 0; j < lenSubstring; j++) {
            if (fullText[i + j] != substring[j]) {
                break;
            }
        }

        if (j == lenSubstring) {  // If the substring matches
            if (counter >= currentSizeDynamicArray) {
                currentSizeDynamicArray += 5;
                dynamicArray = (int*) realloc(dynamicArray, currentSizeDynamicArray * sizeof(int));
                validationOfRealocation(dynamicArray);
            }
            dynamicArray[counter] = i;
            counter++;
            // Move the index past the current match
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

// function for loading file means read the text with fptr and write it in node by \n splitting, so it make sense to implement first of all the finding substing in list
int* findIndexSubstring(char* fullText, char* substring) {
    int lenFullText = strlen(fullText);
    int lenSubstring = strlen(substring);
    int* indexPosition = indexSearchAlgorithm(fullText,lenFullText,substring,lenSubstring);
    return indexPosition;
}



// get substring to search
char * getSubstring(){
    printf("Enter the substring which u wanna find: ");
    char* userSubstring = accessPtrString();
    return userSubstring;
}



void showIndexesSubstring(node* headOfLinkedList,char* userSubstring) {

    printf("\nHere is the coordinates of the searched substring:");
    node* currentNode = headOfLinkedList;
    char* fullText;
    int* indexes;
    int numOfLine = 0;
    int* ptrArray = (int*) calloc(2,sizeof(char)); // if one value will be not 0 -> there is a match

    while(currentNode != NULL) {
        fullText = currentNode->ptrOnRow;
        indexes = findIndexSubstring(fullText, userSubstring);

        if (indexes != NULL) {
            for( int i = 0; indexes[i] != -1; i++) {
                printf("(%d, %d);",numOfLine,indexes[i]);
                ptrArray[0] = 1;
            }
            free(indexes);
        }
        numOfLine ++;
        currentNode = currentNode->nextNodeAdress;
    }
    if (ptrArray[0] != 1) {
        printf("Your substring does not located in text ");
    }
}


char* getLoadName() {
    printf("Enter the name of file with .txt extention which should be uploaded: ");
    char* fileName = accessPtrString();
    return fileName;
}
// load file
char* getLoadedText(char* fileLoad) {
    FILE *fptr= fopen(fileLoad, "r");
    int counter = 0;
    int initialAmountCharacters= 500;
    int currentAmountOfCharacters = initialAmountCharacters;
    char* fullText = (char*)calloc(currentAmountOfCharacters + 1,sizeof(char));
    char myString[100];
    if (fptr == NULL) {
        printf("Error opening file");
    }else {
        while(fgets(myString, 100, fptr)) {
            counter +=100;
            if (counter+50>= currentAmountOfCharacters) {
                currentAmountOfCharacters += initialAmountCharacters;
                fullText = (char*) realloc(fullText,currentAmountOfCharacters*sizeof(char));
                validationOfRealocation(fullText);
            }
            strcat(fullText,myString);
        }
    }
    fclose(fptr);
    return fullText;
}

int* findTabSubstring(char* fullText) {
    int lenFullText = strlen(fullText);
    int lenSubstring = 1; // means '\n'
    int* indexPosition = indexSearchAlgorithm(fullText,lenFullText,"\n",1);
    return indexPosition;
}

// creation of node from load file. Rule: each node is a separate string
void loadedFileToNode( char* fullText,node* headOfLinkedList) {
    int indexPlaceOfTab;
    int startIndexPosition = 0;
    int lengthOfFullText = strlen(fullText);
    int* indexArray ;
    int counterInsertFirstRow = 0; // ralated to the fact that the first row can be added to the current row (that was passed to console) or been inserted in empty row -> first row always inserted in existed row (exception: only for first string of the compiler we create a node for the first loaded string)

    // fullText[lengthOfFullText] = '\n';

    while ((indexArray = findTabSubstring(fullText)) != NULL) {
        indexPlaceOfTab = indexArray[0];
        char* textToNode = (char*) calloc(indexPlaceOfTab+2,sizeof(char));
        strncpy(textToNode,fullText,indexPlaceOfTab+1);
        fullText = fullText +(indexPlaceOfTab+1);

        if (((headOfLinkedList->nextNodeAdress) == NULL) && (headOfLinkedList->ptrOnRow == 0) ) {
            headOfLinkedList->ptrOnRow= textToNode;
            headOfLinkedList->nextNodeAdress = NULL;
            counterInsertFirstRow ++;
        }
        else {
            node* currentNode = headOfLinkedList;
            while(currentNode->nextNodeAdress != NULL) {
                currentNode = currentNode->nextNodeAdress;
            }
            if (counterInsertFirstRow == 0) {
                char* currentRow = currentNode->ptrOnRow;
                int lengthOfCurrentRow = strlen(currentRow);
                int lengthFirstRowOfFile = strlen(textToNode);
                currentRow = (char*) realloc ( currentRow, lengthOfCurrentRow+ lengthFirstRowOfFile+2 );
                validationOfRealocation(currentRow);
                strcat(currentRow,textToNode);
                currentNode->ptrOnRow = currentRow;
                counterInsertFirstRow ++;
            } else {
                node* Node = (node*) malloc (sizeof(node));
                Node->ptrOnRow = textToNode;
                Node->nextNodeAdress = NULL;
                currentNode->nextNodeAdress = Node;
            }
        }

    }
    // for last row of loaded file; if full text zero means that the last scanned row was with \n-> new row has to be created; otherwise if we have more text, that means that it will be started from the new row but will be current in our program
    if (*fullText != '\0') {
        int lengthOfLastRow = strlen(fullText);
        char* ptrLastRow = (char*) calloc(lengthOfLastRow+2, sizeof(char));
        strncpy(ptrLastRow,fullText,lengthOfFullText);
        if ((headOfLinkedList->ptrOnRow) == 0) {
            headOfLinkedList->ptrOnRow= ptrLastRow;
            headOfLinkedList->nextNodeAdress = NULL;
        } else {
            node* currentNode = headOfLinkedList;
            while(currentNode->nextNodeAdress != NULL) {
                currentNode = currentNode->nextNodeAdress;
            }
            node* Node = (node*) malloc (sizeof(node));
            Node->ptrOnRow = ptrLastRow;
            Node->nextNodeAdress = NULL;
            currentNode->nextNodeAdress = Node;
        }
    }else {
        char* ptrEmptyRow = (char*) malloc (3);
        ptrEmptyRow[0] = '\0';
        if ((headOfLinkedList->ptrOnRow) == 0) {
            headOfLinkedList->ptrOnRow= ptrEmptyRow;
            headOfLinkedList->nextNodeAdress = NULL;
        } else {
            node* currentNode = headOfLinkedList;
            while(currentNode->nextNodeAdress != NULL) {
                currentNode = currentNode->nextNodeAdress;
            }
            node* Node = (node*) malloc (sizeof(node));
            Node->ptrOnRow = ptrEmptyRow;
            Node->nextNodeAdress = NULL;
            currentNode->nextNodeAdress = Node;
        }

        }

    }

// insert into a line if it exists exception only the first line
void insertionIntoLine(node* headOfLinkedList){
    printf("%s","Enter two coordinates(line and row beginning from 0) for insertion by whitespace: ");
    int coordLine, coordRow;
    int* indexTabArray;
    char* ptrRow;
    int indexOfTab;

    int counterWhiteSpace ;
    int i ;
    int j ;
    scanf("%d %d", &coordLine,&coordRow);
    clearInputBuffer();

    if ((headOfLinkedList->ptrOnRow) == 0) {
        addNode(headOfLinkedList);
    }

    else {
        node* currentNode = headOfLinkedList;
        for( i = 0; i < coordLine; i++) {
            currentNode = currentNode->nextNodeAdress;
        }

        char* ptrOldRow = currentNode->ptrOnRow ;
        printf("Enter a string to be inserted");
        char* ptrAddedRow = accessPtrString(); //with \0 character

        int amountOfLettersInOldRow = strlen(ptrOldRow);
        int amountOfLettersInAddedRow = strlen(ptrAddedRow);

        indexTabArray = findTabSubstring(ptrOldRow);
        if (*ptrOldRow == '\0'){
            ptrOldRow = (char*) realloc(ptrOldRow, (amountOfLettersInAddedRow+coordRow+1)*sizeof(char));
            char* spacesArray = (char*) calloc(coordRow+1, sizeof(char));
            for (  counterWhiteSpace = 0; counterWhiteSpace < coordRow; counterWhiteSpace++) {
                spacesArray[counterWhiteSpace]= ' ';
            }
            strcat(spacesArray,ptrAddedRow);
            currentNode->ptrOnRow = spacesArray;

        }else if(indexTabArray==NULL) {// maybe not esential condition due to the fact that is actually appending text at the eand
            ptrOldRow = (char*) realloc(ptrOldRow, (coordRow+ amountOfLettersInAddedRow+1)*sizeof(char));
            validationOfRealocation(ptrOldRow);
            int amountOfSpaces = coordRow-amountOfLettersInOldRow;
            char* arrayWhiteSpace = (char*) calloc(amountOfSpaces +1,sizeof(char));
            for (  counterWhiteSpace = 0; counterWhiteSpace < amountOfSpaces; counterWhiteSpace++) {
                arrayWhiteSpace[counterWhiteSpace]= ' ';
            }
            if (amountOfSpaces>=0) {
                strcat(ptrOldRow,arrayWhiteSpace);
                strcat(ptrOldRow,ptrAddedRow);
                currentNode->ptrOnRow = ptrOldRow;
            } else {
                ptrAddedRow = (char*) realloc(ptrAddedRow,amountOfLettersInAddedRow+amountOfLettersInOldRow);
                validationOfRealocation(ptrAddedRow);
                strcat(ptrAddedRow,ptrOldRow);
                currentNode->ptrOnRow = ptrAddedRow;
            }
        }else if (coordRow+1 < amountOfLettersInOldRow) {
            ptrRow = (char*) malloc( amountOfLettersInOldRow+amountOfLettersInAddedRow);
            strncpy(ptrRow, ptrOldRow, coordRow);
            strcat(ptrRow, ptrAddedRow);
            strcat(ptrRow, ptrOldRow + coordRow);
            currentNode->ptrOnRow = ptrRow;
        } else if (coordRow == amountOfLettersInOldRow)  {    // actually case when the string is just a considts of \n and \0;
            ptrOldRow = (char*) realloc(ptrOldRow, amountOfLettersInOldRow+ amountOfLettersInAddedRow+2);
            validationOfRealocation(ptrOldRow);
            indexTabArray = findTabSubstring(ptrOldRow);
            indexOfTab = indexTabArray[0];
            ptrOldRow[indexOfTab] = '\0';
            strcat(ptrOldRow, ptrAddedRow);
            ptrOldRow[amountOfLettersInAddedRow+amountOfLettersInAddedRow]  = '\n';
            ptrOldRow[amountOfLettersInOldRow+amountOfLettersInOldRow+1] = '\0';
            currentNode->ptrOnRow = ptrOldRow;
        } else {
            indexTabArray = findTabSubstring(ptrOldRow); // it only will retutn one value cause one node - one string
            indexOfTab = indexTabArray[0];
            int amountOfWhiteSpaces = coordRow - indexOfTab;
            ptrRow = (char*) calloc( amountOfLettersInOldRow+amountOfWhiteSpaces+amountOfLettersInAddedRow + 2, sizeof(char));
            char* ptrArrayOfWhiteSpaces = (char*) calloc(amountOfWhiteSpaces+1,sizeof(char));

            ptrAddedRow[amountOfLettersInAddedRow] = '\n';
            ptrAddedRow[amountOfLettersInAddedRow+1] = '\0';
            j = 0;
            while (j < amountOfWhiteSpaces) {
                ptrArrayOfWhiteSpaces[j] = ' ';
                j++;
            }
            ptrArrayOfWhiteSpaces[amountOfWhiteSpaces] = '\0';


            strncpy(ptrRow,ptrOldRow,indexOfTab);
            strcat(ptrRow,ptrArrayOfWhiteSpaces);
            strcat(ptrRow,ptrAddedRow);
            currentNode->ptrOnRow = ptrRow;
        }

    }


}







int main(){
    int mode;
    int n = 1;
    node* headOfLinkedList = (node*) calloc ( n, sizeof(node));
    while(true) {
        mode = userCommand();
        switch(mode) {
            case 1:
                printf("Enter text to append: ");
                appendTextToEnd(headOfLinkedList);
                break;
            case 2 :
                newLine(headOfLinkedList);
                break;
            case 3 :
                insertionIntoLine(headOfLinkedList);
                break;
            case 4:
                readTextFromMemory(headOfLinkedList);
                break;
            case 5:
                char* nameOfSaveFile;
                nameOfSaveFile= getName();
                saveToFile(nameOfSaveFile,headOfLinkedList);
                break;
            case 6:
                char* userSubstring;
                userSubstring= getSubstring();
                showIndexesSubstring(headOfLinkedList,userSubstring);
                break;
            case 7:
                char* nameOfLoadFile;
                int* indexArray;
                char* fullText;
                nameOfLoadFile = getLoadName();
                fullText = getLoadedText(nameOfLoadFile);
                loadedFileToNode(fullText,headOfLinkedList);
                break;
            case 8:
                // code for clean console
                break;
            default:
                printf("Given command does not exists");

        }
    }
    return 0;
}













