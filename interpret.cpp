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
