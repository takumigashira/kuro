#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

enum {
	TK_NUM = 256,
	TK_EOF,
};

//token type
typedef struct {
	int ty;
	int val;
	char *input;
} Token;

Token tokens[100];

void tokenize(char *p){
	int i = 0;
	while(*p){
		//skip whitespace
		if(isspace(*p)){
			p++;
			continue;
		}

		// + or -
		if(*p == '+' || *p == '-'){
			tokens[i].ty = *p;
			tokens[i].input = p;
			i++;
			p++;
			continue;
		}
		//number
		if(isdigit(*p)){
			tokens[i].ty = TK_NUM;
			tokens[i].input = p;
			tokens[i].val = strtol(p,&p,10);
			i++;
			continue;
		}
		fprintf(stderr, "can not tokenize %s", p);
		exit(1);
	}
	tokens[i].ty = TK_EOF;
}

void fail(int i){
	fprintf(stderr, "unexpected token: %s\n", tokens[i].input);
	exit(1);
}

int main(int argc,char **argv){
   if(argc != 2){
      fprintf(stderr,"Usage:kuro<code>\n");
      return 1;
   }

   char *p = argv[1];
   tokenize(argv[1]);

   printf(".intel_syntax noprefix\n");
   printf(".global main\n");
   printf("main:\n");
   
   if(tokens[0].ty != TK_NUM)
	   fail(0);
   printf("   mov rax, %d\n", tokens[0].val);
   
   int i = 1;
   while(tokens[i].ty != TK_EOF){
	   if(tokens[i].ty == '+'){
		   i++;
		   if(tokens[i].ty != TK_NUM)
			   fail(i);
		   printf("   add rax, %d\n", tokens[i].val);
		   i++;
		   continue;
	   }
	   if(tokens[i].ty == '-'){
		   i++;
		   if(tokens[i].ty != TK_NUM)
			   fail(1);
		   printf("   sub rax, %d\n", tokens[i].val);
		   i++;
		   continue;
	   }
	   fail(1);
   }

   printf("   ret\n");
   return 0;
}

