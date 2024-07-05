#include <iostream>
#include <dlfcn.h>

using namespace std;

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
    void* handle = dlopen("./implementationOfDynamicLibrary/libCaesarCipher.dylib", RTLD_LAZY);
    if (!handle) {
        cerr << "Lib not found: " << dlerror() <<endl;
        return 1;
    }
    typedef char* (*funcPtr)(char*, int);

    funcPtr encryptFunc = (funcPtr) dlsym(handle,"encrypt");
    if (encryptFunc == nullptr) {
        cout << "Function of encryption was not found: " << dlerror()<< endl;
        dlclose(handle);
        return 1;
    }

    funcPtr decryptFunc = (funcPtr)dlsym(handle,"decrypt");
    if (encryptFunc == nullptr) {
        cout << "Function of decryption was not found:: " << dlerror()<< endl;
        dlclose(handle);
        return 1;
    }
    while(true) {

        int numberOfCommand;
        int shiftNum;
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
            char* ptrEncryptedMessage = encryptFunc(userMessage,shiftNum);
            cout << "Your encrypted message:\n";
            cout<<ptrEncryptedMessage<< endl;
        } else if (numberOfCommand==2) {
            cout << "Write the message to decrypt:\n";
            char* userMessage = getTextFromUser();
            char* ptrDecryptedMessage = decryptFunc(userMessage,shiftNum);
            cout << "Your decrypted message:\n";
            cout<<ptrDecryptedMessage<< endl;
        }
    }
        return 0;


}
