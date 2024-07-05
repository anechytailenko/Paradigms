#include <iostream>
#include <dlfcn.h>

using namespace std;


extern "C" {
    char* encrypt(char*,int);
    char* decrypt(char*,int);

}


void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char* getTextFromUser() {
        const int InitialNumOfLetters = 10;
        const int increaseNumOfLetters = 5;
        int counter;
        char* ptrText;
        char userLetter ;
        int currentNumOfLetters;

        ptrText = new char[InitialNumOfLetters]();
        counter = 0;
        currentNumOfLetters = InitialNumOfLetters;
        while((userLetter = (char)getchar()) != '\n') {
            if (currentNumOfLetters <= counter+1) {
                currentNumOfLetters += increaseNumOfLetters;
                char* newPtrText = new char[currentNumOfLetters];
                memcpy(newPtrText, ptrText, currentNumOfLetters * sizeof(char));
                delete[] ptrText;
                ptrText = newPtrText;

            }
            ptrText[counter] = userLetter;
            counter ++;
        }

        ptrText[counter] = '\0';

        return ptrText;
    }





int main() {

    while(true) {

        int numberOfCommand;
        int shiftNum;
        char* ptrEncryptedMessage = nullptr;
        cout << "Enter the number of command:\n1.Encrypt message;\n2.Decrypt message;\n3.Terminate program;\n";
        cin >> numberOfCommand;
        if (numberOfCommand==3) {
            break;
        }
        cout << "Enter a key shift: ";
        cin >> shiftNum;
        clearInputBuffer();
        if (numberOfCommand==1) {
            cout << "Write the message to encrypt:\n";
            char* userMessage = getTextFromUser();
            try {
            char* ptrEncryptedMessage = encrypt(userMessage,shiftNum);
                if (!ptrEncryptedMessage) {
                    throw "Error: encrypt function returned nullptr."; // Throw an error if function returns nullptr
                } else {
                    cout << "Your encrypted message:\n" << ptrEncryptedMessage << endl;
                }
            }
            catch (const char* errorMesssage){
                cout << errorMesssage;
                return 1;
            }
        } else if (numberOfCommand==2) {
            cout << "Write the message to decrypt:\n";
            char* userMessage = getTextFromUser();
            try {
                char* ptrEncryptedMessage = decrypt(userMessage,shiftNum);
                if (!ptrEncryptedMessage) {
                    throw "Error: encrypt function returned nullptr."; // Throw an error if function returns nullptr
                } else {
                    cout << "Your encrypted message:\n" << ptrEncryptedMessage << endl;
                }
            }
            catch (const char* errorMesssage){
                cout << errorMesssage;
                return 1;
            }

        }
    }
        return 0;


}

