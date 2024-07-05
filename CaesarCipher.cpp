#include <stdio.h>
#include <stdlib.h>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

char* encrypt(char* userText, int shift) {
    const int InitialNumOfLetters = 10;
    const int increaseNumOfLetters = 5;
    int currentNumOfLetters = InitialNumOfLetters;
    char* ptrResultedText = new char[InitialNumOfLetters]();
    size_t i ;
    for (i = 0; userText[i]!= '\0'; i++) {

        if (currentNumOfLetters <= i+1) {
            currentNumOfLetters += increaseNumOfLetters;
            ptrResultedText = (char*) realloc(ptrResultedText,currentNumOfLetters);
        }

        if (userText[i] >= 'A' && userText[i] <= 'Z') {
            ptrResultedText[i] = ((userText[i] - 'A' + shift) % 26) + 'A';
        } else if (userText[i] >= 'a' && userText[i] <= 'z') {
            ptrResultedText[i] = ((userText[i] - 'a' + shift) % 26) + 'a';
        } else {
            ptrResultedText[i] = userText[i];
        }
    }
    ptrResultedText[i] = '\0';
    return ptrResultedText;
    }



char* decrypt(char* userText, int shift) {
    return encrypt(userText, 26 - shift);
}


#ifdef __cplusplus
}
#endif

