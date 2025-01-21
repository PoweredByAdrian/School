#include "parser.h"
#include "scanner.h"
#include "symtable.h"
#include "type.h"
#include "expression.h"


static char tokenTypeToString[][64] = {
    // Terminaly
    "EMPTY",
    "kwDO",
    "kwELSE",
    "kwEND",
    "kwFUNCTION",
    "kwGLOBAL",
    "kwIF",
    "kwINTEGER",
    "kwLOCAL",
    "kwNIL",
    "kwNUMBER",
    "kwREQUIRE",
    "kwRETURN",
    "kwSTRING",
    "kwTHEN",
    "kwWHILE",
    "kwWRITE",
    "kwREADS",
    "kwREADI",
    "kwREADN",
    "kwTOINTEGER",
    "kwSUBSTR",
    "kwORD",
    "kwCHR",
    "COLON",
    "LT",
    "GT",
    "LTEQ",
    "GTEQ",
    "EQ",
    "NEQ",
    "DOUBLEDOT",
    "ASSIGN",
    "PLUS",
    "MINUS",
    "MUL",
    "DIV_NUM",
    "DIV_INT",
    "COMMENT",
    "HASH",
    "LBRACKET",
    "RBRACKET",
    "COMMA",
    "EOFI",
    "DOLLAR",
    "EXPRESSION",
    "HANDLE",
    "tTerm",
    "tExpr",
    "tExpr2",
    "IFJ21",
    "NIL",
    "INTEGER",
    "NUMBER",
    "STRING",
    "ID",
    "PROG",
    "PROLOG",
    "RETURNTYPE",
    "PARAMSTYPE",
    "PARAMSTYPEN",
    "STATEMENT",
    "PARAMS",
    "PARAMSN",
    "DEFVALUE",
    "FUNC",
    "IDLIST",
    "RETURNEXP",
    "EXPLIST",
    "BUILTIN",
    "ARG",
    "VALUE",
    "ARGN",
    "TYPE",
    ""
};



Stack stack;
List list;
SymStack symStack;

void symStInit(int size) { //inicializace zásobníku
    symStack.top = -1;
    symStack.size = size;
    symStack.symTable = malloc(size * sizeof(symtable_bst_node));
    fprintf(stderr,"%s", tokenTypeToString[74]);
}

void symStPush(symtable_bst_node table) {  //vložení na vrchol zásobníku
    symStack.top++;
    if (symStack.top >= symStack.size) {
        symStack.size *= 2;
        symStack.symTable = realloc(symStack.symTable, symStack.size * sizeof(symtable_bst_node));
    }
    symStack.symTable[symStack.top] = table;
}

symtable_bst_node symStPop() { //odstranění z vrcholu zásobníku
    if (symStack.top >= 0) {
        return symStack.symTable[symStack.top--];
    }
    else {
        return NULL;
    }
}

symtable_data symTableSearchAll(symtable_bst_node* tree, char* key) {
    // prohledá aktuální tabulku a všechny tabulky na zásobníku od konce k začátku
    symtable_data result = symtable_search(tree, key);
    if (result != NULL) {
        return result;
    }
    for (int i = symStack.top; i >= 0; i--) {
        result = symtable_search(&symStack.symTable[i], key);
        if (result != NULL) {
            return result;
        }
    }
    return NULL;
}


void stInit(int size) { //inicializace zásobníku
    stack.top = -1;
    stack.size = size;
    stack.data = malloc(size * sizeof(TTokenType));
}

void stPush(TTokenType T) {  //vložení na vrchol zásobníku
    stack.top++;
    if (stack.top >= stack.size) {
        stack.size *= 2;
        stack.data = realloc(stack.data, stack.size * sizeof(TTokenType));
    }
    stack.data[stack.top] = T;
}

TTokenType stPop() { //odstranění z vrcholu zásobníku
    if (stack.top < 0) {
        return EOFI;
    }
    else {
        return stack.data[stack.top--];
    }
    
}

TTokenType stTop() { //vrací vrchol zásobníku
    if (stack.top < 0) {
        return EOFI;
    }
    else {
        return stack.data[stack.top];
    }
}

void listInit(int size) { //inicializace seznamu
    list.size = size;
    list.count = 0;
    list.data = malloc(size * sizeof(Token*));
}

void listAdd(Token* T) {  //přidání do seznamu
    list.count++;
    if (list.count > list.size) {
        list.size *= 2;
        list.data = realloc(list.data, list.size * sizeof(Token*));
    }
    list.data[list.count - 1] = T;
}

void listClear() {
    for (int i = 0; i < list.count; i++) {
        freeTokenData(list.data[i]);
        free(list.data[i]);
    }
    list.count = 0;
}

bool isTerm(TTokenType token) {
    return (token == INTEGER || token == NUMBER || token == STRING || token == ID);
}

bool isExpr(TTokenType token) {
    return (token == INTEGER || token == NUMBER || token == STRING  || token == kwNIL ||
        token == DIV_NUM || token == MUL || token == MINUS || token == PLUS ||
        token == EQ || token == NEQ || token == LT || token == GT ||
        token == LTEQ || token == GTEQ || token == LBRACKET || token == RBRACKET);
}

//vložení funkce do tabulky symbolů
int addFn(symtable_bst_node *symTable, char* name) {
    if (symtable_search(symTable, name) != NULL) {
        return ERR_SEM_DEFINE;
    }

    symtable_data sTData = malloc(sizeof(struct SymtableData));
    sTData->type = TYPE_FUNCTION;
    sTData->localFrame = NULL;
    sTData->globalFrame = NULL;

    char* tmpName = malloc(strlen(name) + 1);
    strcpy(tmpName, name);

    symtable_insert(symTable, tmpName, sTData);
    return 0;
}

//vložení proměnné do tabulky symbolů
int addVar(symtable_bst_node* symTable, char* name) {
    if (symtable_search(symTable, name) != NULL) {
        return ERR_SEM_DEFINE;
    }
    symtable_data sTData = malloc(sizeof(struct SymtableData));
    sTData->type = TYPE_VARIABLE;
    sTData->localFrame = NULL;
    sTData->globalFrame = NULL;

    char* tmpName = malloc(strlen(name) + 1);
    strcpy(tmpName, name);

    symtable_insert(symTable, tmpName, sTData);
    return 0;
}
bool defVar = false;
int parse() {
    char fncID[64] = "";
    bool fncDef = false;
    Token* token;
    Token* lastToken = NULL;
    token = malloc(sizeof(Token));
    symtable_bst_node symTable;

    stInit(100);
    stPush(PROLOG);
    listInit(100);
    symStInit(100);

    symtable_init(&symTable);

    //vestavene funkce a promenne
    addFn(&symTable, "reads");
    addFn(&symTable, "readi");
    addFn(&symTable, "readn");
    addFn(&symTable, "write");
    addFn(&symTable, "tointeger");
    addFn(&symTable, "substr");
    addFn(&symTable, "ord");
    addFn(&symTable, "chr");
    addVar(&symTable, "_");
    
    //volání lexikální analýzy
    token = getNextToken();
    

    while (token != NULL) {

        //ERROR("STACK: %s\n", tokenTypeToString[stTop()]);
        ERROR("TOKEN: %s, %d, %s\n", token->value, token->nonterm, tokenTypeToString[token->type]);
        if(token->type == ID){
            memset(fncID, 0, 64);
            strcpy(fncID,token->value);
        }
        if (stTop() >= PROG) {
            int expResult = 0;
            //neterminál

            if ( token != NULL && token->type == ID){
                symtable_data dat = symTableSearchAll(&symTable, token->value);
                    Token * next = getNextToken();
                    if(isExpr(next->type) && (dat != NULL && dat->type != TYPE_FUNCTION)){  
                        ERROR("asdfgreeee %s \n", token->value);                    
                        expResult |= expand(EXPRESSION);
                    }
                    else{
                        ERROR("reee%d:%s - %s : %s\n", __LINE__,tokenTypeToString[token->type], token->value, tokenTypeToString[stTop()]);
                        ERROR("reee%d:%s - %s\n", __LINE__,tokenTypeToString[next->type], next->value);
                        expResult |= expand(token->type);
                        token = next;
                        stPop();
                    }
            }
            else{
                    expResult |= expand(token->type);
            }
            if (expResult != 0) {
                ERROR("%d - %s expected: %s\n", __LINE__, token->value, tokenTypeToString[stTop()]);
                ERROR("%d:%s - %s\n", __LINE__,tokenTypeToString[stTop()], token->value); return expResult;
            }
        }
        else {
            // terminál
            
            if (token->type == stTop()) {
                // ok
                if(token->type == ID){
                memset(fncID, 0, 64);
                strcpy(fncID, token->value);
                }
                if (token->type == EOFI) {
                    // parsovani skoncilo uspesne
                    // semanticka analyza
                    symtable_data sTData = symtable_search(&symTable, MAIN);
                    if (sTData == NULL || sTData->type != TYPE_FUNCTION) {
                        ERROR("%d - %s\n", __LINE__, token->value); return ERR_SEM_DEFINE;  // chybi funkce main
                    }
                    symtable_dispose(&symTable);
                    ERROR("%d - %s\n", __LINE__, token->value); return 0;
                }
                if (lastToken != NULL && token->type == ID && lastToken->type == kwFUNCTION) {
                    // definice funkce
                    ERROR("Defining func %s\n", token->value);
                    int addFnRet;
                    addFnRet = addFn(&symTable, token->value);
                    if (addFnRet != 0) {
                        ERROR("%d - %s\n", __LINE__, token->value); return addFnRet;
                    }
                    fncDef = true;
                }
                else if ( lastToken != NULL && (token->type == PLUS || token->type == MINUS || token->type == MUL || token->type == DIV_NUM || 
                token->type == DIV_INT || token->type == EQ || token->type == LTEQ || token->type == GT || token->type == GTEQ || token->type == NEQ) ){
                    //ERROR("EXPReeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
                }
                else if (lastToken != NULL && token->type == ID && lastToken->type == kwLOCAL) {
                    // definice proměnné
                    
                    int addVarRet;
                    addVarRet = addVar(&symTable, token->value);
                    if (addVarRet != 0) {
                        ERROR("%d - %s\n", __LINE__, token->value); return addVarRet;
                    }
                }
                else if (lastToken != NULL && (token->type == kwINTEGER || token->type == kwSTRING || token->type == kwNUMBER || token->type == kwNIL) && lastToken->type == COLON) {
                    // definice parametru
                    ERROR("I am defining %s\n", fncID);
                    if(fncDef){
                    int addVarRet;
                    addVarRet = addVar(&symTable, fncID);
                    if (addVarRet != 0) {
                        ERROR("%d - %s\n", __LINE__, fncID); return addVarRet;
                    }
                     memset(fncID, 0, 64);
                    fncDef = false;
                    }

                    // if(defVar){
                    //     int addVarRet;
                    // addVarRet = addVar(&symTable, fncID);

                    // if (addVarRet != 0) {
                    //     ERROR("%d - %s     %d\n", __LINE__, fncID, addVarRet); return addVarRet;
                    // }
                    //                     memset(fncID, 0, 64);
                    // defVar = false;
                    // }

                    
                }
                else if (lastToken != NULL && (token->type == LBRACKET) && lastToken->type == ID) {
                    // volání funkce
                    if (symTableSearchAll(&symTable, lastToken->value) == NULL) {
                        ERROR("%d - %s\n", __LINE__, token->value); return ERR_SEM_DEFINE;
                    }
                }
                else if (lastToken != NULL && (token->type == ASSIGN || token->type == COMMA) && lastToken->type == ID) {
                    // prirazeni do promenne
                    if (symTableSearchAll(&symTable, lastToken->value) == NULL) {
                        ERROR("%d - %s\n", __LINE__, token->value); return ERR_SEM_DEFINE;
                    }
                }
                else if(lastToken != NULL && (token->type == INTEGER || token->type == NUMBER || token->type == STRING || token->type == NIL) && lastToken->type == kwRETURN){
                    ERROR("REJSZD\n");
                    stPop();
                }

                stPop();
                if (lastToken != NULL) {
                    freeTokenData(lastToken);
                    free(lastToken);
                }
                lastToken = token;
                token = malloc(sizeof(Token));
                token = getNextToken();
                // codeFromToken(token, lastToken);
            }
            else if (stTop() == tTerm && !token->nonterm) {
                // nalezen term - ok
                if (token->type == ID && symTableSearchAll(&symTable, token->value) == NULL) {
                    // kontrola jestli je promenna definovana
                    ERROR("%d - %s\n", __LINE__, token->value); return ERR_SEM_DEFINE;
                }
                stPop();
                lastToken = token;
                token = malloc(sizeof(Token));
                token = getNextToken();
                // codeFromToken(token, lastToken);
            }
            else if (stTop() == EXPRESSION || stTop() == tExpr2) {
                // dočtu celý výraz
                ERROR("Greee%d - %s\n", __LINE__, token->value);
                if (stTop() == tExpr2 && lastToken != NULL) { //vložím id z předchozího tokenu
                ERROR("reee%d - %s\n", __LINE__, token->value);
                    if (symTableSearchAll(&symTable, lastToken->value) == NULL) {
                        // kontrola jestli je promenna definovana
                        ERROR("reee%d - %s\n", __LINE__, token->value); return ERR_SEM_DEFINE;
                    }
                    listAdd(lastToken);
                    lastToken = NULL;
                }
                while (token != NULL) {
                    symtable_data dat = symTableSearchAll(&symTable, token->value);
                    if (isExpr(token->type) || (dat != NULL && dat->type != TYPE_FUNCTION)) {
                        ERROR("Greee%d - %s\n", __LINE__, token->value);
                        if (token->type == ID && symTableSearchAll(&symTable, token->value) == NULL) {
                            // kontrola jestli je promenna definovana
                            ERROR("%d - %s\n", __LINE__, token->value); return ERR_SEM_DEFINE;
                        }
                        listAdd(token);
                        token = malloc(sizeof(Token));
                        token = getNextToken();
                    }
                    else {
                        stPop();
                        stPush(tExpr2);
                        break;
                    }
                }
                if (list.count == 0) {
                    ERROR("%d - %s\n", __LINE__, token->value); return ERR_SYNTAX;
                }
                else if (list.count == 1) {
                    if (!isTerm(list.data[0]->type)) {
                        ERROR("%d - %s\n", __LINE__, token->value); return ERR_SYNTAX;
                    }
                }
                else {
                    // int exprRes = expressionsParse(&list, &symTable);
                    // if (exprRes != 0) {
                    //     ERROR("%d - %s\n", __LINE__, token->value); return exprRes;
                    // }
                }
                listClear();
                stPop();
            }
            else {
                ERROR("%d - %s expected: %s\n", __LINE__, token->value, tokenTypeToString[stTop()]); return ERR_SYNTAX;
            }

        }

    } 
    return 0;
}

int expand(TTokenType nextToken) {
    if (stTop() == PROG) {
        ERROR("eeeeeeeee%d - %s expected: %s\n", __LINE__, tokenTypeToString[nextToken], tokenTypeToString[stTop()]);
        switch (nextToken) {
        case kwGLOBAL: // <prog> → global ID : function ( <params_type> ) <ret_type> <prog>
            stPop();
            stPush(PROG);
            stPush(RETURNTYPE);
            stPush(RBRACKET);
            stPush(PARAMSTYPE);
            stPush(LBRACKET);
            stPush(kwFUNCTION);
            stPush(COLON);
            stPush(ID);
            stPush(kwGLOBAL);
            break;
        case kwFUNCTION: // <prog> → function ID ( <params> ) <ret_type> <statement> end <prog>
            stPop();
            stPush(PROG);
            stPush(kwEND);
            stPush(STATEMENT);
            stPush(RETURNTYPE);
            stPush(RBRACKET);
            stPush(PARAMS);
            stPush(LBRACKET);
            stPush(ID);
            stPush(kwFUNCTION);
            break;
        case ID: // <prog> → ID ( <arg> )
            ERROR("PROG POG REEEEEE\n");
            stPop();
            stPush(RBRACKET);
            stPush(ARG);
            stPush(LBRACKET);
            stPush(ID);
            break;
        default:
            return ERR_SYNTAX;
        }
    }
    else if (stTop() == PROLOG) {
        switch (nextToken) {
        case kwREQUIRE: // <prolog> → require “ifj21” <prog> EOF
            stPop();
            stPush(EOFI);
            stPush(PROG);
            stPush(STRING); // IFJ21
            stPush(kwREQUIRE);
            break;
        default:
            return ERR_SYNTAX;
        }
    }
    else if (stTop() == PARAMSTYPE) {
        switch (nextToken) {
        case TYPE: // <params_type> → <type> <params_type_n>
            stPop();
            stPush(PARAMSTYPEN);
            stPush(TYPE);
            break;
        default:
            stPop();
            return 0;
        // case LBRACKET: // <params_type> → ε
        //     stPop();
        //     break;
        // default:
        //     return ERR_SYNTAX;
        }
    }
    else if (stTop() == PARAMSTYPEN) {
        switch (nextToken) {
        case COMMA: // <params_type_n> → , <type> <params_type_n>
            stPop();
            stPush(PARAMSTYPEN);
            stPush(TYPE);
            stPush(COMMA);
            break;
        default:
            stPop();
            return 0;
        // case LBRACKET: // <params_type_n> → ε
        //     stPop();
        //     break;
        // default:
        //     return ERR_SYNTAX;
        }
    }
    else if (stTop() == RETURNTYPE) {
        switch (nextToken) {
        case COLON: // <ret_type> → : <type>
            stPop();
            stPush(TYPE);
            stPush(COLON);
            break;
        //case EOFI:  <ret_type> → ε
          //  stPop();
            //break;
        default:
            stPop();
            return 0;
        }
    }
    else if (stTop() == PARAMS) {
        switch (nextToken) {
        case ID: // <params> → ID : <type> <params_n>
            stPop();
            stPush(PARAMSN);
            stPush(TYPE);
            stPush(COLON);
            stPush(ID);
            break;
        default:
            stPop();
            return 0;
        // case LBRACKET: // <params> → ε
        //     stPop();
        //     break;
        // default:
        //     return ERR_SYNTAX;
        }
    }
    else if (stTop() == PARAMSN) {
        switch (nextToken) {
        case COMMA: // <params_n> → , ID : <type> <params_n>
            stPop();
            stPush(PARAMSN);
            stPush(TYPE);
            stPush(COLON);
            stPush(ID);
            stPush(COMMA);
            break;
        //case : // <params_n> → ε
          //  stPop();
           // break;
        default:
            stPop();
            return 0;
        }
    }
    else if (stTop() == STATEMENT) {
        switch (nextToken) {
        case kwLOCAL: // <statement> → local ID : <type> <eq> <statement>
            defVar = true;
            stPop();
            stPush(STATEMENT);
            stPush(EQL);
            stPush(TYPE);
            stPush(COLON);
            stPush(ID);
            stPush(kwLOCAL);
            break;
        case ID: // <statement> → ID <id_list> = <def_value> <statement>
            stPop();
            stPush(STATEMENT);
            stPush(DEFVALUE);
            stPush(ASSIGN);
            stPush(IDLIST);
            stPush(ID);
            break;
        case kwIF: // <statement> → if <expression> then <statement> else <statement> end <statement>
            stPop();
            stPush(STATEMENT);
            stPush(kwEND);
            stPush(STATEMENT);
            stPush(kwELSE);
            stPush(STATEMENT);
            stPush(kwTHEN);
            stPush(EXPRESSION);
            stPush(kwIF);
            break;
        case kwWHILE: // <statement> → while <expression> do <statement> end <statement>
            stPop();
            stPush(STATEMENT);
            stPush(kwEND);
            stPush(STATEMENT);
            stPush(kwDO);
            stPush(EXPRESSION);
            stPush(kwWHILE);
            break;
        case kwRETURN: // <statement> → return <ret_exp> <statement>
            stPop();
            stPush(STATEMENT);
            stPush(RETURNEXP);
            stPush(kwRETURN);
            break;
        case kwWRITE: // <statement> → write ( <arg> ) <statement>
            stPop();
            stPush(STATEMENT);
            stPush(RBRACKET);
            stPush(ARG);
            stPush(LBRACKET);
            stPush(kwWRITE);
            break;
        //case EPS: // <statement> → ε
          //  stPop();
          //  break;
        default:
            stPop();
            return 0;
        }
    }
    else if (stTop() == RETURNEXP) {
        switch (nextToken) {
        case RETURNEXP: // <ret_exp> → <expression> <expression_list>
        case INTEGER:
        case NUMBER:
        case STRING:
        case NIL:
        case ID:
            stPop();
            stPush(EXPLIST);
            stPush(EXPRESSION);
            break;
        //case EPS: // <ret_exp> → ε
          //  stPop();
            //break;
        default:
            stPop();
            return 0;
        }
    }
    else if (stTop() == DEFVALUE) {
        ERROR("adasda   %d - %s\n", __LINE__, tokenTypeToString[nextToken]);
        switch (nextToken) {
        case kwREADI: // <def_value> → <builtin>
        case kwREADN:
        case kwREADS:
        case kwTOINTEGER:
        case kwORD:
        case kwCHR:
        case kwSUBSTR:
            stPop();
            stPush(BUILTIN);
            break;
        case ID: // <def_value> → ID <func>
        ERROR("adasda   %d - %s\n", __LINE__, tokenTypeToString[nextToken]);
            stPop();
            stPush(FUNC);
            stPush(ID);
            break;
        case EXPRESSION: // <def_value> → <expression> <expression_list>
            stPop();
            stPush(EXPLIST);
            stPush(EXPRESSION);
            break;
        case VALUE: // <def_value> -> <value> 
        case INTEGER:
        case NUMBER:
        case STRING:
        case NIL:
            stPop();
            stPush(VALUE);
            break;
        default:
            return ERR_SYNTAX;
        }
    }
    else if (stTop() == FUNC){
        switch(nextToken){
            case LBRACKET:
                stPop();
                stPush(RBRACKET);
                stPush(ARG);
                stPush(LBRACKET);
                break;
            default:
                stPop();
                break;
        }
    }
    else if (stTop() == IDLIST) {
        switch (nextToken) {
        case COMMA: // <id_list> → , ID <id_list>
            stPop();
            stPush(IDLIST);
            stPush(ID);
            stPush(COMMA);
            break;
        //case EPS: // <id_list> → ε
          //  stPop();
           // break;
        default:
            stPop();
            return 0;
        }
    }
    else if (stTop() == EXPLIST) {
        switch (nextToken) {
        case COMMA: // <expression_list> → , <expression> <expression_list>
            stPop();
            stPush(EXPLIST);
            stPush(EXPRESSION);
            stPush(COMMA);
            break;
        // case EPS: // <expression_list> → ε
        //     stPop();
        //     break;
        default:
            stPop();
            return 0;
        }
    }
    else if (stTop() == EQL) {
        switch (nextToken) {
        case ASSIGN: // <eq> → = <def_value>
            ERROR("aaaaa %s\n", tokenTypeToString[nextToken]);
            stPop();
            stPush(DEFVALUE);
            stPush(ASSIGN);
            break;
        // case EPS: // <eq> → ε
        //     stPop();
        //     break;
        default:
            ERROR("bbbbb %s\n", tokenTypeToString[nextToken]);
            stPop();
            return 0;
        }
    }
    else if (stTop() == BUILTIN) {
        switch (nextToken) {
        case kwREADS : // <builtin> → reads ( <arg> )
            stPop();
            stPush(RBRACKET);
            stPush(ARG);
            stPush(LBRACKET);
            stPush(kwREADS);
            break;
        case kwREADI: // <builtin> → readi ( <arg> )
            stPop();
            stPush(RBRACKET);
            stPush(ARG);
            stPush(LBRACKET);
            stPush(kwREADI);
            break;
        case kwREADN: // <builtin> → readn ( <arg> )
            stPop();
            stPush(RBRACKET);
            stPush(ARG);
            stPush(LBRACKET);
            stPush(kwREADN);
            break;
        case kwTOINTEGER: // <builtin> → tointeger ( <arg> )
            stPop();
            stPush(RBRACKET);
            stPush(ARG);
            stPush(LBRACKET);
            stPush(kwTOINTEGER);
            break;
        case kwSUBSTR: // <builtin> → substr ( <arg> )
            stPop();
            stPush(RBRACKET);
            stPush(ARG);
            stPush(LBRACKET);
            stPush(kwSUBSTR);
            break;
        case kwORD: // <builtin> → ord ( <arg> )
            stPop();
            stPush(RBRACKET);
            stPush(ARG);
            stPush(LBRACKET);
            stPush(kwORD);
            break;
        case kwCHR: // <builtin> → chr ( <arg> )
            stPop();
            stPush(RBRACKET);
            stPush(ARG);
            stPush(LBRACKET);
            stPush(kwCHR);
            break;
        default:
            return ERR_SYNTAX;
        }
    }
    else if (stTop() == ARG) {
        
        switch (nextToken) {
        case STRING: // <arg> → <value> <arg_n>
        case INTEGER:
        case NUMBER:
        case NIL:
        case VALUE:
        ERROR("cccc %s\n", tokenTypeToString[nextToken]);
            stPop();
            stPush(ARGN);
            stPush(VALUE);
            break;
        // case LBRACKET: // <arg> → ε
        //     stPop();
        //     break;
        default:
            stPop();
            return 0;
        }
    }
    else if (stTop() == ARGN) {
        switch (nextToken) {
        case COMMA: // <arg_n> → , <value> <arg_n>
            stPop();
            stPush(ARGN);
            stPush(VALUE);
            stPush(COMMA);
            break;
        // case EPS: // <arg_n> → ε
        //     stPop();
        //     break;
        default:
            stPop();
            return 0;
        }
    }
    // else if (stTop() == DEFVALUE){
    //     switch(nextToken){
    //         case  ID: // <def_val> -> ID ( <arg> )
    //             stPop();
    //             stPush(ID);
    //             stPush(LBRACKET);
    //             stPush(ARG);
    //             stPush(RBRACKET);
    //             break;
    //         case EXPRESSION:
    //             stPop(); // <def_value> → <expression> <expression_list>
    //             stPush(EXPRESSION);
    //             stPush(EXPLIST);
    //             break;
    //         case BUILTIN: // <def_value> → <builtin>
    //             stPop();
    //             stPush(BUILTIN);
    //             break;
    //         case VALUE: // <def_value> -> <value>
    //             stPop();
    //             stPush(VALUE);
    //             break;
    //         default:
    //             return ERR_SYNTAX;
            
    //     }
    // }
    else if (stTop() == VALUE) {
        switch (nextToken) {
        case INTEGER: // <value> → INTEGER
            stPop();
            stPush(INTEGER);
            break;
        case NUMBER: // <value> → NUMBER
            stPop();
            stPush(NUMBER);
            break;
        case STRING: // <value> → STRING
            stPop();
            stPush(STRING);
            break;
        case ID: // <value> → ID
            stPop();
            stPush(ID);
            break;
        case NIL: // <value> → NIL
            stPop();
            stPush(NIL);
            break;
        default:
            return ERR_SYNTAX;
        }
    }
    else if (stTop() == TYPE) {
        switch (nextToken) {
        case kwINTEGER: // <type> → integer
            stPop();
            stPush(kwINTEGER);
            break;
        case kwNUMBER: // <type> → number
            stPop();
            stPush(kwNUMBER);
            break;
        case kwSTRING: // <type> → string
            stPop();
            stPush(kwSTRING);
            break;
        case kwNIL: // <type> → nil
            stPop();
            stPush(kwNIL);
            break;
        default:
            return ERR_SYNTAX;
        }
    }
    
    return 0;
}