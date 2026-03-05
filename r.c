#include <stdio.h>
#include <regex.h>
#include <string.h>

typedef enum {
    TK_WS, TK_SL_CMT,TK_BL_CMT,TK_PREPROC,TK_STRING,TK_CHAR,TK_NUMBER,
    TK_IDENT,TK_OP,TK_SPECIAL,TK_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    const char* name;
    const char* pattern;
    regex_t re;
} Rule;

int main(){
    Rule rules[] = {
        {TK_WS, "Whitespace", "^[[:space:]]+", {0}},
        {TK_SL_CMT, "Single Comment", "^//[^\n]*", {0}},
        {TK_BL_CMT, "Block comment","^/\\*([^*]|\\*[^/])*\\*/", {0}},
        {TK_PREPROC, "Preprocessor", "^#[^\n]*", {0}},
        {TK_STRING, "String", "^\"([^\"\\\\]|\\\\.)*\"", {0}},
        {TK_CHAR, "Char literal", "^'([^'\\\\]|\\\\.)*'", {0}},
        {TK_NUMBER, "Number", "^0[xX][0-9a-fA-F]+", {0}},
        { TK_NUMBER,  "NUMBER",          "^[0-9]+\\.[0-9]+([eE][+-]?[0-9]+)?",   {0} },
        { TK_NUMBER,  "NUMBER",          "^[0-9]+[eE][+-]?[0-9]+",               {0} },
        { TK_NUMBER,  "NUMBER",          "^[0-9]+",                               {0} },
        { TK_IDENT,   "IDENTIFIER",      "^[A-Za-z_][A-Za-z0-9_]*",              {0} },
         { TK_OP,      "OPERATOR",
            "^(==|!=|<=|>=|\\+\\+|--|\\+=|-=|\\*=|/=|%=|&&|\\|\\||<<|>>|->|[+*/%=&|^~<>!-])",
            {0} },
        { TK_SPECIAL, "SPECIAL",         "^[][;:,(){}.]",                         {0} },
        { TK_UNKNOWN, "UNKNOWN",         "^.",                                    {0}}
    };

    int num_rules = sizeof(rules) / sizeof(rules[0]);

    for(int i=0;i<num_rules;i++){
        if(regcomp(&rules[i].re,rules[i].pattern,REG_EXTENDED) != 0){
            printf("Error compiling regex for rule: %s\n", rules[i].name);
            return 1;
        }
    }

    const char *cursor = "";
    
    while (*cursor){
        regmatch_t match;
        int hit = 0;
        for(int i=0;i < num_rules;i++){
            if(regexec(&rules[i].re,cursor,1,&match,0) == 0 && match.rm_so == 0){
                int len = match.rm_eo;
                if(rules[i].type != TK_WS){
                    printf("%-20s : %.*s\n", rules[i].name, len, cursor);
                }
                cursor += len;
                hit = 1;
                break;
            }
        }
        if(!hit) cursor++;
    }

}