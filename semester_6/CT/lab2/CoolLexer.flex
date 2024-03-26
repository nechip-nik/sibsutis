%{
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

#include "CoolParser.h"
#include "CoolLexer.h"

#undef YY_DECL
#define YY_DECL int CoolLexer::yylex()
#define ERROR -1;
int comment_level = 0;
%}
digit             [0-9]
alpha             [A-Za-z_]
alpha_num         ({alpha}|{digit})
white_space       [ \t]*


STR [A-Za-z_]

%option batch noyywrap c++
%option yylineno
%option yyclass="CoolLexer"

%start COMMENT_INLINE
%start COMMENT
%start STR


%%
<INITIAL>--.*\n     { ++lineno; }
<INITIAL>"*)"       { Error("Wrong comment close-bracket"); BEGIN(INITIAL); return ERROR; }
<INITIAL>"(*"       { BEGIN(COMMENT); comment_level = 0; }
<COMMENT>"(*"       { ++comment_level; }
<COMMENT><<EOF>>    { Error("Unterminated comment"); BEGIN(INITIAL); return ERROR; }
<COMMENT>\n         { ++lineno; }
<COMMENT>[^\n*(]*   { }
<COMMENT>[*()]      { }
<COMMENT>"*)"       {
                      if (comment_level == 0) BEGIN(INITIAL);
                      --comment_level;
                    }





<INITIAL>(\")   {yymore();
                BEGIN(STR);
                }
<STR>\n         { Error("one_line_string");
                  BEGIN(INITIAL);
                  ++lineno;
                  return 0;
                }
<STR><<EOF>>    {return 0;}
<STR>\0         {return 0;}
<STR>[^\\\"\n]* {yymore();}
<STR>\\[^\n]    {yymore();}
<STR>\\\n       {++lineno;
                  yymore();
                }
<STR>\"         {
                Escape();
                BEGIN(INITIAL);
                return TOKEN_STR;
                }



(?i:class)           return TOKEN_CLASS;
t(?i:rue)            return TOKEN_TRUE;
f(?i:alse)           return TOKEN_FALSE;
(?i:else)            return TOKEN_ELSE;
(?i:fi)              return TOKEN_FI;
(?i:if)              return TOKEN_IF;
(?i:in)              return TOKEN_IN;
(?i:inherits)        return TOKEN_INHERITS;
(?i:isvoid)          return TOKEN_ISVOID;
(?i:let)             return TOKEN_LET;
(?i:loop)            return TOKEN_LOOP;
(?i:pool)            return TOKEN_POOL;
(?i:then)            return TOKEN_THEN;
(?i:while)           return TOKEN_WHILE;
(?i:case)            return TOKEN_CASE;
(?i:esac)            return TOKEN_ESAC;
(?i:new)             return TOKEN_NEW;
(?i:of)              return TOKEN_OF;
(?i:not)             return TOKEN_NOT;

","                  return TOKEN_COMMA;
"."                  return TOKEN_DOT;
"@"                  return TOKEN_AT;
"~"                  return TOKEN_TILDA;
"*"                  return TOKEN_MUL;
"/"                  return TOKEN_DIV;
"+"                  return TOKEN_PLUS;
"-"                  return TOKEN_MINUS;
"<="                 return TOKEN_LE;
"=>"                 return TOKEN_ME;
">"                  return TOKEN_MORE;
"<"                  return TOKEN_LESS;
"="                  return TOKEN_EQ;
"<-"                 return TOKEN_ARROW;

\(             return TOKEN_OPEN_CURLY_BRACE;
\)             return TOKEN_CLOSE_CURLY_BRACE;

\{             return TOKEN_OPEN_PARENTHESIS;
\}             return TOKEN_CLOSE_PARENTHESIS;

\[             return TOKEN_OPEN_SQUARE_BRACKET;
\]             return TOKEN_CLOSE_SQUARE_BRACKET;

\;                  return TOKEN_SEMICOLON;
\:                  return TOKEN_COLON;

[0-9]+                return TOKEN_DIGITAL;

[a-z]{alpha_num}*    return TOKEN_IDENTIFIER_OBJECT;
[A-Z]{alpha_num}*    return TOKEN_IDENTIFIER_TYPE;
_{alpha_num}*        return TOKEN_IDENTIFIER_OTHER;



\n                   ++lineno;
{white_space}   { /* skip spaces */}
.                   Error("unrecognized character");

%%

void CoolLexer::Error(const char* msg) const
{
    std::cerr << "Lexer error (line " << lineno << "): " << msg << ": lexeme '" << YYText() << "'\n";
    std::exit(YY_EXIT_FAILURE);
}

void CoolLexer::Escape(){
    const char *input = yytext;
    char *output = yytext;
    input++; // Skip opening '\"'
    while (*(input + 1) /* Skip closing '\"' */ ) {
        if (*input == '\\') {
            input++; // Skip '\\'
            switch (*input) {
                case 'n': *output++ = '\n'; break;
                case 't': *output++ = '\t'; break;
                case 'f': *output++ = '\f'; break;
                case 'b': *output++ = '\b'; break;
                default: *output++ = *input;
            }
        } else {
            *output++ = *input;
        }
        input++;
    }
    *output = '\0';
    std::string str = yytext;
    std::cout <<"CODE_SYMBOL:  ";
    for(char ch : str){
        std::cout << static_cast<int>(ch) << " ";
    }
    std::cout << "\n";
}
