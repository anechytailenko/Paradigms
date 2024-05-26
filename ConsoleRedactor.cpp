//
// Created by Anna Nechytailenko on 25.05.2024.
//
//
// Created by Anna Nechytailenko on 24.05.2024.
//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>



void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int userCommand () {
    int userMode ;
    printf("Choose the command:\n1.Append text symbols to the end:\n2.Start the new line:\n3.Use files to loading the information\n4.Use files to saving the information\n5.Print the current text to console\n6.Insert the text by line and symbol index\n7.Search (please note that text can be found more than once)\n8.Clear the console\n");
    scanf("%d",&userMode);
    clearInputBuffer();
    return userMode;
}


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
            if (ptrRow == NULL) {
                printf("Error occur during reallocation");
                exit(0);
            }
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
node* createNode() {
    node* Node = (node*) malloc (sizeof(node));
    Node->ptrOnRow = accessPtrString();
    Node->nextNodeAdress = NULL;
    return Node;
}

// function of add node
void* addNode(node* headOfLinkedList) {
    if ((headOfLinkedList->ptrOnRow) == 0) {
        headOfLinkedList->ptrOnRow= accessPtrString();
        headOfLinkedList->nextNodeAdress = NULL;
    } else {
        node* currentNode = headOfLinkedList;
        while(currentNode->nextNodeAdress != NULL) {
            currentNode = currentNode->nextNodeAdress;
        }
        currentNode->nextNodeAdress = createNode();

    }
}

// access last string and get the pointer of string
void* appendTextToEnd(node* headOfLinkedList) {
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
        for (int i = 0; i <= amountOfLettersInNewRow; i ++) {
            ptrOldRow[amountOfLettersInOldRow] = ptrNewRow[i];
            amountOfLettersInOldRow ++;
        }
        currentNode->ptrOnRow = ptrOldRow;
    }




    node* currentNode = headOfLinkedList;
    while(currentNode->nextNodeAdress != NULL) {
        currentNode = currentNode->nextNodeAdress;
    }
    char* ptrRow = currentNode->ptrOnRow ;
    return ptrRow;

}

// read text from memory
void readTextFromMemory(node* headOfLinkedList) {
    node* currentNode = headOfLinkedList;
    while(currentNode != NULL) {
        printf("%s",currentNode->ptrOnRow);
        currentNode = currentNode->nextNodeAdress;
    }
}





int main(){
    int mode;
    int n = 1;
    node* headOfLinkedList = (node*) calloc ( n, sizeof(node));
    mode = userCommand();
    switch(mode) {
        case 1:

            printf("Enter text to append: ");
            appendTextToEnd(headOfLinkedList);
            printf("Enter text to append: ");
            appendTextToEnd(headOfLinkedList);
            printf("Enter text to append: ");
            appendTextToEnd(headOfLinkedList);

            readTextFromMemory(headOfLinkedList);
            break;
        // case 2:


        //     printf("New line is started");
        //     addNode(headOfLinkedList);


        //     break;
        //     case 3:
        //         printf("Enter the file name for saving:");
        //     break;
        //     // user input file name
        //     // print the succes of the operation otherwise ask to create file
        //     case 4 :
        //         printf("Enter the file name for loading:");
        //     break;
        //     // user input file name
        //     // print the succes of the operation otherwise ask to create file
        //     case 5:
        //         printf("Print the current text to console:");
        //     break;
        //     // code for iteration through structure
        //     case 6:
        //         printf("Choose line and index:");
        //     break;
        //     // code for input parameters
        //     printf("Enter text to insert:");
        //     case 7:
        //         printf("Enter text to search:");
        //     // user word input
        //     printf("Text is present in this position:");
        //     break;
        //     // code for algorithm search
        //     case 8:
        //         // code for clearing console
        //             break;
        //     default:
        //         printf("There is no such command");
    }


    return 0;
}













