//
// Created by Anna Nechytailenko on 25.05.2024.
//
//
// Created by Anna Nechytailenko on 24.05.2024.
//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>




int userCommand () {
   int userMode ;
   printf("Choose the command:\n1.Append text symbols to the end:\n2.Start the new line:\n3.Use files to loading the information\n4.Use files to saving the information\n5.Print the current text to console\n6.Insert the text by line and symbol index\n7.Search (please note that text can be found more than once)\n8.Clear the console\n");
   scanf("%d",&userMode);
   return userMode;
}
int main() {
    int mode;
    const int InitialNumOfBytePerString = 5;
    const int numByIncreseBytes = 2;
    // mode = userCommand();
    // switch(mode) {
    //     case 1:
    //         printf("Enter text to append:");
    //     // code for input text
    //     break;
    //     case 2:
    //         printf("New line is started");
    //     /// code for adding line
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
    // }



// implementation of data structure
    int counter;
    char* ptrRow;
    char userLetter ;
    int currentSizeArrayInBytes;

    ptrRow = (char*) malloc(InitialNumOfBytePerString);
    counter = 0;
    currentSizeArrayInBytes = InitialNumOfBytePerString;


    while((userLetter = (char)getchar()) != '\n') {
        if (currentSizeArrayInBytes <= counter) {
            currentSizeArrayInBytes = counter+numByIncreseBytes+2;
            ptrRow = (char*) realloc(ptrRow,currentSizeArrayInBytes);
       }
        ptrRow[counter] = userLetter;

        counter ++;
       }
    ptrRow[counter]= '\0';
    for(int i = 0; i < counter ; i++) {
        printf("%c",ptrRow[i]);
   };
    size_t length = strlen(ptrRow);
    printf("The length of the input string is: %zu\n", length);
    printf("%s",ptrRow);

    return 0;
}













