/**
 * -----------------------------------------------------------
 * Projekt: Implementace prekladace imperativniho jazyka IFJ21
 * Varianta: I (Binarni vyhledavaci strom)
 * Autori:		Gáfrik Patrik	xgafri00@stud.fit.vutbr.cz
	            Horváth Adrián	xhorva14@stud.fit.vutbr.cz
	            Kipila Maxim	xkipil00@stud.fit.vutbr.cz
	            Dohnal Ondřej	xdohna45@stud.fit.vutbr.cz

 * Datum: 20.11.2021
 *
 *
 * -----------------------------------------------------------
 */
#include "scanner.h"
#include <ctype.h>

TTokenType id_or_kw(string *str){
    const char* s = strToConst(str);
    if(!strcmp(s, "do")) return kwDO;
    else if(!strcmp(s,"else"))return kwELSE;
    else if(!strcmp(s, "end"))return kwEND;
    else if(!strcmp(s, "function"))return kwFUNCTION;
    else if(!strcmp(s,"global"))return kwGLOBAL;
    else if(!strcmp(s,"if"))return kwIF;
    else if(!strcmp(s,"integer"))return kwINTEGER;
    else if(!strcmp(s,"local"))return kwLOCAL;
    else if(!strcmp(s,"nil"))return kwNIL;
    else if(!strcmp(s,"number"))return kwNUMBER;
    else if(!strcmp(s,"require"))return kwREQUIRE;
    else if(!strcmp(s,"return"))return kwRETURN;
    else if(!strcmp(s,"string"))return kwSTRING;
    else if(!strcmp(s,"then"))return kwTHEN;
    else if(!strcmp(s,"while"))return kwWHILE;
    else if(!strcmp(s,"write"))return kwWRITE;
    else if(!strcmp(s,"reads"))return kwREADS;
    else if(!strcmp(s,"readi"))return kwREADI;
    else if(!strcmp(s,"readn"))return kwREADN;
    else if(!strcmp(s,"tointeger"))return kwTOINTEGER;
    else if(!strcmp(s,"substr"))return kwSUBSTR;
    else if(!strcmp(s,"ord"))return kwORD;
    else if(!strcmp(s,"chr"))return kwCHR;
    else return ID;
}

Token* getNextToken(){
    Token *token = token_new(EMPTY, "EMPTY", false);

    char symbol;
    short escape;
    string str;


    if(!strInit(&str)){
        ERROR("Internal error occured.");
        exit(ERR_INTERNAL);
    };    

    ScannerState state = STATE_START;

    while(true){
        symbol = getc(stdin);
        if(symbol == EOF){
            token_set_type(token, EOFI);
            token_set_value(token, "EOFI");
            token_set_nonterm(token, false);
            return token;
        }
        switch (state){
            case STATE_START:
                if(isspace(symbol)) state = STATE_START;
                else if(symbol == '/') state = STATE_DIV_NUM;
                else if(symbol == '=') state = STATE_ASSIGN;
                else if(symbol == '(') state = STATE_LBRACKET;
                else if(symbol == ')') state = STATE_RBRACKET;
                else if(symbol == '-') state = STATE_MINUS;
                else if(symbol == '*') state = STATE_MUL;
                else if(symbol == '+') state = STATE_PLUS;
                else if(symbol == '.') state = STATE_DOT;
                else if(symbol == '~') state = STATE_TILDE;
                else if(symbol == '#') state = STATE_HASH;
                else if(symbol == '<') state = STATE_LT;
                else if(symbol == '>') state = STATE_GT;
                else if(symbol == '[') state = STATE_BLOCK_COMMENT_START;
                else if(symbol == ':') state = STATE_COLON;
                else if(symbol == ',') state = STATE_COMMA;
                else if(symbol == '"'){
                    state = STATE_STRING_START;
                    strClear(&str);
                    escape = 0;
                }
                else if(isalpha(symbol) || symbol == '_'){
                     state = STATE_KW_OR_ID;
                     strClear(&str);
                     strAddChar(&str, symbol);
                }
                else if(isdigit(symbol)){
                    state = STATE_NUMERIC;
                    strClear(&str);
                    strAddChar(&str, symbol);
                }
                else{
                    ERROR("Chyba v ramci lexikalni analyzy - neocekavany symbol");
                    exit(ERR_LEXICAL);
                }
                break;
            case STATE_DIV_NUM:
                if(symbol == '/') state = STATE_DIV_INT;
                else {
                    ungetc(symbol, stdin);
                    token_set_type(token, DIV_NUM);
                    token_set_value(token, "/");
                    token_set_nonterm(token, false);
                    return token;
                }
                break;
            case STATE_DIV_INT:
                ungetc(symbol, stdin);
                token_set_type(token, DIV_INT);
                token_set_value(token, "//");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_ASSIGN:
                if(symbol == '=') state = STATE_EQ;
                else{
                    ungetc(symbol, stdin);
                    token_set_type(token, ASSIGN);
                    token_set_value(token, "=");
                    token_set_nonterm(token, false);
                    return token;
                }
                break;
            case STATE_EQ:
                if(symbol == '='){
                    exit(ERR_LEXICAL);
                }
                ungetc(symbol, stdin);
                token_set_type(token, EQ);
                token_set_value(token, "==");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_LBRACKET:
                ungetc(symbol, stdin);
                token_set_type(token, LBRACKET);
                token_set_value(token, "(");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_RBRACKET:
                ungetc(symbol, stdin);
                token_set_type(token, RBRACKET);
                token_set_value(token, ")");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_MINUS:
                if(symbol =='-') state = STATE_COMMENT;
                else{
                    ungetc(symbol, stdin);
                    token_set_type(token, MINUS);
                    token_set_value(token, "-");
                    token_set_nonterm(token, false);
                    return token;
                }
                break;
            case STATE_COMMENT:
                if(symbol == '\n') state = STATE_START;
                break;
            case STATE_COMMA:
                ungetc(symbol, stdin);
                token_set_type(token, COMMA);
                token_set_value(token, ",");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_MUL:
                ungetc(symbol, stdin);
                token_set_type(token, MUL);
                token_set_value(token, "*");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_PLUS:
                ungetc(symbol, stdin);
                token_set_type(token, PLUS);
                token_set_value(token, "+");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_DOT:
                if(symbol == '.') state = STATE_DOUBLEDOT;
                else{
                    ERROR("Chyba v ramci lexikalni analyzy. Neocekavana .");
                    exit(ERR_LEXICAL);
                }
                break;
            case STATE_DOUBLEDOT:
                ungetc(symbol, stdin);
                token_set_type(token, DOUBLEDOT);
                token_set_value(token, "..");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_TILDE:
                if(symbol == '=') state = STATE_NEQ;
                else{
                    ERROR("Chyba v ramci lexikalni analyzy. Neocekavana ~");
                    exit(ERR_LEXICAL);
                }
            case STATE_NEQ:
                if(symbol == '='){
                    exit(ERR_LEXICAL);
                }
                ungetc(symbol, stdin);
                token_set_type(token, NEQ);
                token_set_value(token, "~=");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_HASH:
                ungetc(symbol, stdin);
                token_set_type(token, HASH);
                token_set_value(token, "#");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_LT:
                if(symbol == '=') state = STATE_LTEQ;
                else{
                    ungetc(symbol, stdin);
                    token_set_type(token, LT);
                    token_set_value(token, "<");
                    token_set_nonterm(token, false);
                    return token;
                }
                break;
            case STATE_LTEQ:
                if(symbol == '='){
                    exit(ERR_LEXICAL);
                }
                ungetc(symbol, stdin);
                token_set_type(token, LTEQ);
                token_set_value(token, "<=");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_GT:
                if(symbol == '=') state = STATE_GTEQ;
                else{
                    ungetc(symbol, stdin);
                    token_set_type(token, GT);
                    token_set_value(token, ">");
                    token_set_nonterm(token, false);
                    return token;
                }
                break;
            case STATE_GTEQ:
                if(symbol == '='){
                    exit(ERR_LEXICAL);
                }
                ungetc(symbol, stdin);
                token_set_type(token, GTEQ);
                token_set_value(token, ">=");
                token_set_nonterm(token, false);
                return token;
                break;
            case STATE_KW_OR_ID:
                if(isalnum(symbol) || symbol == '_'){
                    strAddChar(&str, symbol);
                }
                else{
                    ungetc(symbol, stdin);
                    token_set_type(token, id_or_kw(&str));
                    token_set_value(token, strToConst(&str));
                    token_set_nonterm(token, false);
                    return token;
                }
                break;
            case STATE_BLOCK_COMMENT_START:
                if(symbol == '[') state = STATE_BLOCK_COMMENT;
                else{
                    ERROR("Chyba v ramci lexikalni analyzy. Neocekavana [");
                    exit(ERR_LEXICAL);
                }
                break;
            case STATE_BLOCK_COMMENT:
                if(symbol == ']') state = STATE_BLOCK_COMMENT_END;
                break;
            case STATE_BLOCK_COMMENT_END:
                if(symbol == ']') state = STATE_START;
                else state = STATE_BLOCK_COMMENT;
                break;
            case STATE_STRING_START:
                if(symbol == '"') state = STATE_STRING_END;
                else if(symbol == '\\') state = STATE_STRING_ESCAPE;
                else{
                    strAddChar(&str, symbol);
                    state = STATE_STRING_START;
                }
                break;
            case STATE_STRING_ESCAPE:
                if(symbol == 'n'){
                    state = STATE_STRING_START;
                    strAddChar(&str, '\n');
                }
                else if(symbol == 't'){
                    state = STATE_STRING_START;
                    strAddChar(&str, '\t');
                }
                else if(symbol == '"'){
                    state = STATE_STRING_START;
                    strAddChar(&str, '"');
                }
                else if(symbol == '\\'){
                    state = STATE_STRING_START;
                    strAddChar(&str, '\\');
                }
                else if(BETWEEN(symbol, '0', '2')){
                    state = STATE_STRING_ESCAPE_1;
                    escape += 100*(symbol - '0');
                }
                else{
                    ERROR("Chyba v ramci lexikalni analyzy. Escape sekvence je chybna");
                    exit(ERR_LEXICAL);
                }
                break;
            case STATE_STRING_ESCAPE_1:
                if(BETWEEN(symbol, '0', '9')){
                    state = STATE_STRING_ESCAPE_2;
                    escape += 10*(symbol - '0');
                }
                else{
                    ERROR("Chyba v ramci lexikalni analyzy. Escape sekvence je chybna");
                    exit(ERR_LEXICAL);
                }
                break;
            case STATE_STRING_ESCAPE_2:
                if(BETWEEN(symbol, '0', '9')){
                    state = STATE_STRING_START;
                    escape += (symbol - '0');
                    if(escape > 255){
                        ERROR("Chyba v ramci lexikalni analyzy. Escape sekvence musi byt v rozsahu 000-255");
                        exit(ERR_LEXICAL);    
                    }
                    strAddChar(&str, escape);
                }
                else{
                    ERROR("Chyba v ramci lexikalni analyzy. Escape sekvence je chybna");
                    exit(ERR_LEXICAL);
                }
                break;
            case STATE_STRING_END:
                ungetc(symbol, stdin);
                token_set_type(token, STRING);
                token_set_value(token, strToConst(&str));
                token_set_nonterm(token, false);
                return token;
            case STATE_COLON:
                ungetc(symbol, stdin);
                token_set_type(token, COLON);
                token_set_value(token, ":");
                token_set_nonterm(token, false);
                return token;
            case STATE_NUMERIC:
                if(isdigit(symbol)) strAddChar(&str, symbol);
                else if(symbol == '.'){
                    strAddChar(&str, symbol);
                    state = STATE_DECIMAL;
                }
                else if(symbol == 'e' || symbol == 'E'){
                    strAddChar(&str, symbol);
                    state = STATE_EXP;
                }
                else {
                    ungetc(symbol, stdin);
                    token_set_type(token, INTEGER);
                    token_set_value(token, strToConst(&str));
                    token_set_nonterm(token, false);
                    return token;
                }
                break;
            case STATE_DECIMAL:
                if(isdigit(symbol)){
                    strAddChar(&str, symbol);
                    state = STATE_DECIMAL_VALUE;
                }
                else{
                    ERROR("Chyba v ramci lexikalni analyzy. Neocekavany symbol v desetinnem cisle");
                    exit(ERR_LEXICAL);
                }
                break;
            case STATE_DECIMAL_VALUE:
                if(isdigit(symbol)) strAddChar(&str, symbol);
                else if(symbol == 'e' || symbol == 'E'){
                    strAddChar(&str, symbol);
                    state = STATE_EXP;
                }
                else{
                    ungetc(symbol, stdin);
                    token_set_type(token, NUMBER);
                    token_set_value(token, strToConst(&str));
                    token_set_nonterm(token, false);
                    return token;
                }
                break;
            case STATE_EXP:
                if(isdigit(symbol)){
                    strAddChar(&str, symbol);
                    state = STATE_EXP_VALUE;
                }
                else if(symbol == '+' || symbol == '-'){
                    strAddChar(&str, symbol);
                    state = STATE_EXP_SIGN;
                }
                else{
                    ERROR("Chyba v ramci lexikalni analyzy. Neocekavany znak v exponentu.");
                    exit(ERR_LEXICAL);
                }
                break;
            case STATE_EXP_VALUE:
                if(isdigit(symbol)) strAddChar(&str, symbol);
                else{
                    ungetc(symbol, stdin);
                    token_set_type(token, NUMBER);
                    token_set_value(token, strToConst(&str));
                    token_set_nonterm(token, false);
                    return token;
                }
                break;
            case STATE_EXP_SIGN:
                if(isdigit(symbol)){
                    state = STATE_EXP_VALUE;
                    strAddChar(&str, symbol);
                }
                else{
                    ERROR("Chyba v ramci lexikalni analyzy. Neocekavany znak v exponentu.");
                    exit(ERR_LEXICAL);
                }
                break;
        }
    }
}

void freeTokenData(Token* token) {
    if (token->type == ID || token->type == STRING) {
        free(token->value);
        token->value = NULL;
    }
}
