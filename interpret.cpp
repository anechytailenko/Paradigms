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
    // build AST( bond the nodes into tree)
    class Parser {
    private:
        Lexer lexerObj;
        string startStr = "Start";
        Token currentToken = lexerObj.getNextToken();
    public:
        friend class Interpreter;
        Parser(Lexer instanceLexer): lexerObj(instanceLexer){};

        void goToNextToken() {
            currentToken = lexerObj.getNextToken();
        }

        AST* variableToTree() {
            AST* Node =  new VarNode(currentToken);
            goToNextToken();
            return Node;
        }

        AST* primaryUnit() {
            Token selfToken = currentToken;

            if (selfToken.type == PLUS) {
                goToNextToken();
                AST* Node = new UnaryOpNode(selfToken,primaryUnit());
                return Node;
            }

            else if (selfToken.type == MINUS) {
                goToNextToken();
                AST* Node = new UnaryOpNode(selfToken,primaryUnit());
                return Node;
            }
            else if (selfToken.type == INTEGER) {
                goToNextToken();
                AST* Node = new IntegerNode(selfToken);
                return Node;
            }
            else if (selfToken.type == FUNC) {
                Token funcToken = currentToken;
                goToNextToken();
                lexerObj.goToNextChar();
                goToNextToken();
                vector<AST*> paramVector = getParam();
                AST* Node = new FuncNode(paramVector,funcToken);
                // AST* Node = funcExpr(selfToken);
                return Node;

            }
            else if (selfToken.type == VAR) {
                string nameOfVar =  selfToken.valueString;
                int valueOfVar = varMap[nameOfVar];;
                if (valueOfVar == NULL) {
                    AST* Node = assignmentExpr() ;
                    return Node;
                }
                else {
                    AST* Node = variableToTree();
                    return Node;
                }

            }
        }


        AST* productionExpr() {
            AST* Node = primaryUnit();
            while ( (currentToken.type == MUL) || (currentToken.type == DIV)) {
                Token selftoken = currentToken;
                goToNextToken();
                Node = new BinOpNode(Node,selftoken,primaryUnit());
            }
            return Node;
        }

        AST* sumExpr() {
            AST* Node = productionExpr();
            while ( (currentToken.type == PLUS) || (currentToken.type == MINUS)) {
                Token selftoken = currentToken;
                goToNextToken();
                Node = new BinOpNode(Node,selftoken,productionExpr());
            }
            return Node;
        }

         vector<AST*> getParam() {
            vector<AST*> paramVec;
            AST* Node = sumExpr();
            paramVec.push_back(Node);

            if(currentToken.type == COMMA) {
                lexerObj.goToNextChar();
                goToNextToken();
                Node = sumExpr();
                paramVec.push_back(Node);
            }
                goToNextToken();
                if(currentToken.type != COMMA){}

            return paramVec;
        }

        AST* assignmentExpr() {
            AST* leftNode = variableToTree();
            goToNextToken();
            Token selfToken = currentToken;
            goToNextToken();
            AST* rightNode = sumExpr();
            AST* Node = new AssignNode(leftNode,selfToken,rightNode);
            return Node;
        }

        AST* getRootNode() {
            AST* Node = sumExpr();
            return Node;
        }
    };
// interpreter visit each node
class Interpreter{
private:
    Parser parserObj;
public:
    Interpreter(Parser parserInstance): parserObj(parserInstance){};

    int readTree(AST* Node) {
        string typeName = typeid(*Node).name();
        if(typeName.size() > 10) {
            typeName = typeName.substr(2);
        } else {
            typeName = typeName.substr(1);
        }
        if ( typeName == "BinOpNode") {
            BinOpNode* node = dynamic_cast<BinOpNode*>(Node);
            return visitBinOpNode(node);
        }
        if (typeName == "IntegerNode") {
            IntegerNode* node = dynamic_cast<IntegerNode*>(Node);
            return visitIntegerNode(node);
        }
        if (typeName == "AssignNode") {
            AssignNode* node = dynamic_cast<AssignNode*>(Node);
            return visitAssignNode(node);
        }
        if (typeName == "VarNode") {
            VarNode* node = dynamic_cast<VarNode *>(Node);
            return visitVarNode(node);
        }
        if (typeName == "UnaryOpNode") {
            UnaryOpNode* node = dynamic_cast<UnaryOpNode *>(Node);
            return visitUnaryOpNode(node);
        }
        if (typeName == "FuncNode") {
            FuncNode* node = dynamic_cast<FuncNode *>(Node);
            return visitFuncNode(node);
        }
    }

    int visitBinOpNode(BinOpNode* node) {
        if ((node->token.type) == PLUS) {
            int valueOfLeftNode = readTree(node->leftNode);
            int valueOfRightNode = readTree(node->rightNode);
            int result =valueOfLeftNode + valueOfRightNode;
            return result;
        } if ((node->token.type) == MINUS) {
            return readTree(node->leftNode) - readTree(node->rightNode);
        } if ((node->token.type) == MUL){
            return readTree(node->leftNode) * readTree(node->rightNode);
        } if ((node->token.type) == DIV){
            return readTree(node->leftNode) / readTree(node->rightNode);
        }
    };

    int visitIntegerNode(IntegerNode* node) {
        int givenInteger = stoi(node->value);
        return givenInteger;
    };

    int visitAssignNode(AssignNode* node) {
        int valueForVar = readTree(node->rightNode);
        AST* nodeOfVar = node->leftNode;
        VarNode* nodeVar = dynamic_cast<VarNode *>(nodeOfVar);
        string nameOfVar = nodeVar->nameOfVar;
        varMap[nameOfVar] = valueForVar;
    };


    int visitVarNode(VarNode* node) {
        string varName = node->nameOfVar;
        auto it = varMap.find(varName);
        int valueOfVar = it->second;
        return valueOfVar;


    };
    int visitUnaryOpNode(UnaryOpNode* node) {
        tokenType sign = node->token.type;
        if (sign== PLUS) {
            return readTree(node->lowerNode);
        }
        if (sign == MINUS) {
            return -1*readTree(node->lowerNode);
        }
    };
    int visitFuncNode(FuncNode* node) {
        string funcName = node->token.valueString;
        if (funcName == "min") {
            int valueOfLeftNode = readTree(node->param[0]);
            int valueOfRightNode = readTree(node->param[1]);
            int theSmallestResult = min(valueOfLeftNode,valueOfRightNode);
            return theSmallestResult;
        }
        if (funcName == "max") {
            int valueOfLeftNode = readTree(node->param[0]);
            int valueOfRightNode = readTree(node->param[1]);
            int theGreaterResult = max(valueOfLeftNode,valueOfRightNode);
            return theGreaterResult;
        }
        if (funcName == "pow") {
            int valueOfLeftNode = readTree(node->param[0]);
            int valueOfRightNode = readTree(node->param[1]);
            int thePowerResult = pow(valueOfLeftNode,valueOfRightNode);
            return thePowerResult;
        }
        if (funcName == "abs") {
            int valueOfNode = readTree(node->param[0]);
            int theAbsoluteResult = fabs(valueOfNode);
            return theAbsoluteResult;
        }
    }

    AST* getRootNode() {
        AST* root = parserObj.getRootNode();
        return root;
    }
    int interpreter() {
        AST*root = getRootNode();
        int result =  readTree(root);
        return result;
    }

};


int main() {
    while (true) {
        string input;
        getline(cin, input);

        if (input == "exit") {
            break;
        }

        Lexer lexer = Lexer(input);
        Parser parser = Parser(lexer);
        Interpreter interpreter = Interpreter(parser);
        int result = interpreter.interpreter();
        string typeName = typeid(*(parser.getRootNode())).name();
        if(typeName.size() > 10) {
            typeName = typeName.substr(2);
        } else {
            typeName = typeName.substr(1);
        }
        if (typeName == "VarNode") {
            continue;
        }
        cout << result << endl;

    }
    return 0;
};
