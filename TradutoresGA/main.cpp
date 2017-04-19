#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <strings.h>
#include <cstring>
#include <map>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define MAX 9 // max length of each word string, not including '\0'

typedef struct {
    char str[MAX];
    char* tokenType;
    int lineNum;
} Token;

char delimiters[9] = {'.', '(', ')', ',', '{', '}', ';', '[', ']'};
char arithOperators[5] = { '+', '-', '*', '/', '%' };

const int TOTAL_RESERVED_WORDS = 13;

char *keywords[TOTAL_RESERVED_WORDS] = {
    (char*)"do",
    (char*)"while",
    (char*)"if",
    (char*)"else",
    (char*)"for",
    (char*)"printf",
    (char*)"return",
    (char*)"null",
    (char*)"int",
    (char*)"float",
    (char*)"double",
    (char*)"string",
    (char*)"bool"
};    

int nextWordIdx = 1;
std::map<std::string, int> mapOfWords;



char word[MAX];
int wi = 0; // index of word string

char numStr[MAX];
int ni = 0;

int isKeyword(char *str);
int isDelimiter(char c);
int isArithOperators(char c);

int main() {
    FILE *filePtr = stdin;
      //if ((filePtr = fopen("input.txt", "r")) == NULL) {
    //    printf("Cannot open input file.\n");
    //    exit(1);
    //}
    
    // check if input file is empty:
    fseek(filePtr, 0, SEEK_END);
    if (ftell(filePtr) == 0) {
        printf("File is empty.\n");
        exit(1);
    } else {
        rewind(filePtr);
    }
    
    int lineNum = 1;
    char ch;
    
    while ((ch = fgetc(filePtr)) != EOF) {
        if (ch == '\n') 
            lineNum++;
        
        // Ignore comment starting with // to the end of line
        if (ch == '/') {
            // comment //
            if (fgetc(filePtr) == '/') {
                while ((ch = fgetc(filePtr)) != '\n') {}
                lineNum++;
            } else {
                fseek(filePtr, -1, SEEK_CUR);
            }
            // comment /* */
            if (fgetc(filePtr) == '*') {
                bool close = false;
                while (close == false) {
                    while ((ch = fgetc(filePtr)) != '*' && ch != EOF) {
                        if (ch == '\n')
                            lineNum++;
                    }
                    if (ch == EOF)
                        break;
                    if ((ch = fgetc(filePtr)) == '/')
                        close = true;
                }
                ch = fgetc(filePtr);
                if (ch == '\n')
                    lineNum++;
            } else {
                fseek(filePtr, -1, SEEK_CUR);
            }
        }
        
        Token token;
        // reconhece variaveis ou identificadores
        if (isalpha(ch)) {
            word[wi++] = ch;
            while (isalpha(ch = fgetc(filePtr))) {
                word[wi++] = ch;
            }
            word[wi] = '\0';
            wi = 0;
            strcpy(token.str, word);
            
            if (isKeyword(word)) {
                token.tokenType = (char*)"reserved_word";    
            } else {
               if (mapOfWords.find(token.str) == mapOfWords.end()){
                   mapOfWords[token.str] = nextWordIdx++;   
               }

                strcpy(token.str, std::to_string(mapOfWords[token.str]).c_str());
               token.tokenType = (char*)"id";    
            }
            
            
            token.lineNum = lineNum;

            std::cout <<"["<<token.tokenType<<", "<< token.str <<"]";
            fseek(filePtr, -1, SEEK_CUR);
            continue;
        }    
        
        // Constantes numéricas (números inteiros):
        else if (isdigit(ch)) {
            numStr[ni++] = ch;
            ch = fgetc(filePtr);
            while (isdigit(ch) || ch == '.') {
                numStr[ni++] = ch;
                ch = fgetc(filePtr);
            }
            numStr[ni] = '\0';
            ni = 0;
            strcpy(token.str, numStr);
            token.tokenType = (char*)"num";
            token.lineNum = lineNum;

            fseek(filePtr, -1, SEEK_CUR);
            std::cout <<"["<<token.tokenType<<", "<< token.str <<"]";
            continue;
        }
        
        else if (ispunct(ch)) {
            
            if (isDelimiter(ch)) {
                // ignora
                continue;
            }else if (isArithOperators(ch)) {
                token.str[0] = ch;
                token.str[1] = '\0';
                token.tokenType = (char*)"Arith_Op";
                token.lineNum = lineNum;
                std::cout <<"["<<token.tokenType<<", "<< token.str <<"]" ;
                continue;
            }else if (ch == '=' || ch == '<' || ch == '>' || ch == '!') { //  <, <=, ==, !=, >=, >
                token.str[0] = ch;
                token.str[1] = '\0';
                token.tokenType = (char*)"Relational_Op";
                char ch2 = fgetc(filePtr);
                if (ch2 == '='){
                    token.str[1] = ch2;
                    token.str[2] = '\0';
                }else if (ch == '='){
                    token.tokenType = (char*)"Equal_Op";
                }
                token.lineNum = lineNum;
                std::cout <<"["<<token.tokenType<<", "<< token.str <<"]" ;
                continue;
            }
            continue;
        }
        
    }
    
    return 0;
}



int isKeyword(char *str) {
    int i;
    int result = 0; // false
    for (i = 0; i < TOTAL_RESERVED_WORDS; i++) {
        if (!strcasecmp(keywords[i], str))
            result = 1;
    }
    return result;
}

int isDelimiter(char c) {
     int i;
     int result = 0; // false
     for (i = 0; i < MAX; i++) {
        if (delimiters[i] == c) 
        result = 1;
     }
     return result;
}

int isArithOperators(char c) {
    int i;
    int result = 0;
    for (i = 0; i < 5; i++) {
        if (arithOperators[i] == c)
            result = 1;
    }
    return result;
}