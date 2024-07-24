#include <iostream>
#include <string>
#include <map>
#include <typeinfo>
#include <cctype>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

// dictionarly for storing variables
map<string, int> varMap;
class  Lexer;
class Parser;
class Interpreter;


// creation of class token
enum tokenType {
    START,
    INTEGER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    VAR,
    FUNC,
    ASSIGN,
    END,
    COMMA,
    LPAREN,
    RPAREN
};
// to do is read the input of characters and convert it into a stream of tokens. The part of the interpreter that does it is called a lexical analyzer, or lexer for short

class Token{
public:
    tokenType type;
    // int valueInt;
    string valueString;
    // Token(tokenType type, int valueInt): type(type), valueInt(valueInt){}
    Token(tokenType type, const string valueStr): type(type), valueString(valueStr){}
};


// split text by token
class Lexer {
private:
    friend class Parser;
    const string textOFUserCommand;
    int indexPos = 0;
    char currentChar = textOFUserCommand[indexPos];
public:
    Lexer(string userCommand): textOFUserCommand(userCommand){};
    void goToNextChar() {
        ++indexPos;
        if (indexPos > textOFUserCommand.length()) {
            currentChar = EOF;
        } else{
            currentChar = textOFUserCommand[indexPos];
        }
    }
    void skipWhiteSpace() {
        while ( (currentChar != EOF) && (currentChar == ' ') ) {
            goToNextChar();
        }
    }
    string extractMultiDigit() {
        string digit = "";
        while ((currentChar != EOF) && (isdigit(currentChar)) ) {
            digit += currentChar;
            goToNextChar();
        }
        return digit;

    }


    Token getTokenVarOrFunc() {
        string name = "";
        while((currentChar != EOF) && (isalpha(currentChar)) ) {
            name += currentChar;
            goToNextChar();
            if ( (name == "var") || (name == "def")) {
                name = "";
                goToNextChar();
            }
        }
        if ( currentChar == '(') {
            Token tokenFunc = Token(FUNC,name);
            indexPos--;
            return tokenFunc;
        } else {
            indexPos--;
            Token tokenVar = Token(VAR,name);
            if (varMap[name] != NULL){}
            else{varMap[name] = NULL;}

            return tokenVar;
        }

    }



    Token getNextToken() {
        while (currentChar != EOF) {
            if ( currentChar == ' ') {
                skipWhiteSpace();
                continue; // maybe not make sence
            }
            if( isalpha(currentChar) ) {
                return getTokenVarOrFunc();
            }
            if ( isdigit(currentChar) ) {
                return Token(INTEGER,extractMultiDigit());
            }
            if( currentChar == '=') {
                goToNextChar();
                return Token(ASSIGN,"=");
            }
            if( currentChar == '+') {
                goToNextChar();
                return Token(PLUS,"+");
            }
            if( currentChar == '-') {
                goToNextChar();
                return Token(MINUS,"-");
            }
            if( currentChar == '*') {
                goToNextChar();
                return Token(MUL,"*");
            }
            if( currentChar == '/') {
                goToNextChar();
                return Token(DIV,"/");
            }

            if( (currentChar == '(')) {
                goToNextChar();
                return Token(LPAREN,"(");
            }
            if( (currentChar == ')')) {
                goToNextChar();
                return Token(RPAREN,")");
            }
            if ( currentChar == ',') {
                return Token(COMMA,",");
            }

            return Token(END,"END");
        }

    }
};
// creating nodes
class AST {
public:
    virtual ~AST() = 0;
};
AST::~AST() {}

class BinOpNode : public AST{
public:
    AST*  leftNode ;
    Token token ;
    AST* rightNode ;
    BinOpNode(AST* left, Token selfToken, AST* right): leftNode(left), token(selfToken),rightNode(right){}
};

class IntegerNode: public AST{
public:
    Token token ;
    string value = token.valueString ;
    IntegerNode(Token selfToken): token(selfToken){}
};

class AssignNode: public AST {
public:
    AST*  leftNode ;
    Token token ;
    AST* rightNode ;
    AssignNode(AST* left, Token selfToken, AST* right): leftNode(left),token(selfToken),rightNode(right){}
};

class VarNode:public AST {
public:
    Token token ;
    string nameOfVar = token.valueString;
    VarNode(Token selfToken): token(selfToken){}
};

class UnaryOpNode: public AST {
public:
    Token token;
    AST* lowerNode;
    UnaryOpNode(Token selfToken, AST* nextNode): token(selfToken),lowerNode(nextNode){}
};

class FuncNode: public AST {
public:
    vector<AST*> param;
    Token token ;
    FuncNode(vector<AST*> paramVec, Token selfToken): param(paramVec),token(selfToken){}

};

