#include "nemu.h"
#include <stdio.h>
#include <stdlib.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ=1,TK_NUM=10,
  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},         // equal
  {"-",'-'},
  {"\\*",'*'},
  {"/",'/'},
  {"\\(",'('},
  {"\\)",')'},
  {"[0-9]+",TK_NUM}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}
typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[64] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        if (rules[i].token_type==TK_NOTYPE){continue;}
        switch (rules[i].token_type) {
	  case '+':{tokens[nr_token].type=rules[i].token_type;
	            strncpy(tokens[nr_token].str,substr_start,substr_len);break;}
	  case TK_EQ:{tokens[nr_token].type=rules[i].token_type;
		     strncpy(tokens[nr_token].str,substr_start,substr_len);break;}
	  case '-':{tokens[nr_token].type=rules[i].token_type;
		   strncpy(tokens[nr_token].str,substr_start,substr_len);break;}
	  case '*':{tokens[nr_token].type=rules[i].token_type;
		   strncpy(tokens[nr_token].str,substr_start,substr_len);break;}
          case '/':{tokens[nr_token].type=rules[i].token_type;
		    strncpy(tokens[nr_token].str,substr_start,substr_len);break;}
	  case '(':{tokens[nr_token].type=rules[i].token_type;
		    strncpy(tokens[nr_token].str,substr_start,substr_len);break;}
	  case ')':{tokens[nr_token].type=rules[i].token_type;
		    strncpy(tokens[nr_token].str,substr_start,substr_len);break;}
	  case TK_NUM:{tokens[nr_token].type=rules[i].token_type;
		       strncpy(tokens[nr_token].str,substr_start,substr_len);break;}
	  default: assert(0);
        }
        nr_token++;
	break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  return true;
}
// use stack to check parentheses
typedef struct stack{
  int top;
  char ch[32];
}Stack;

bool is_empty(Stack st){
  if (st.top==0){return true;}
  return false;
}
void set_empty(Stack st){
  st.top=0;
}
void push(Stack st,char e){
  if (st.top<=31){
    st.ch[st.top]=e;
    st.top++;
  }
  else {printf("stackoverflow");}
}
void pop(Stack st){
  if(!is_empty(st)){
    st.top--;
  }
  else {printf("statck is empty");}
}

static Stack stack1;
bool check_parentheses(int p,int q){
  if (tokens[p].type!='(' || tokens[q].type!=')'){return false;}
  for (int i=p;i<=q;i++){
    if (tokens[i].type=='('){push(stack1,'(');}
    if (tokens[i].type==')'){
      if (is_empty(stack1)){return false;}
      else {pop(stack1);}
    }
  }
  return is_empty(stack1);
}

static bool priority(int op1,int op2){printf("%d,%d\n",op1,'+');
  switch(op1){
    case '+':{if (op2=='*'||op2=='/'){ return true;}
             else {return false;}
             }
    case '-':{if (op2=='*'||op2=='/'){
	       return true;
	     }
	     else {return false;}
	     }
    case '*':{if (op2=='*'||op2=='/'){
               return false;
             }
             else {return true;}
             }
    case '/':{if (op2=='*'||op2=='/'){
               return false;
             }
             else {return true;}
   	     }
    default:assert(0);
  }
}

int main_operator(int p,int q){
  static Stack stack2;
  int op;
  int op_position=p;
  bool init=false;
  for (int i=p;i<=q;i++){
    if (tokens[i].type=='('){
      push(stack2,'(');
      i++;
      while(true){
        if (tokens[i].type=='('){push(stack2,'(');}
	else if(tokens[i].type==')'){pop(stack2);}
	i++;
        if (is_empty(stack2)){break;}
	if(i>q){break;}
      }
    }
    else if (tokens[i].type!=TK_NUM){
      if (init==false){op=tokens[i].type;op_position=i;init=true;}	    
      else if(priority(op,tokens[i].type)==false) {op=tokens[i].type;op_position=i;}
    }
  }
  printf("%s",tokens[op_position].str);
  return op_position;
}

uint32_t eval(int p,int q){
  if (p>q){return -1;}
  else if (p==q) {uint32_t num=0;sscanf(tokens[p].str,"%d",&num);return num;}
  else if (check_parentheses(p,q)) {return eval(p+1,q-1);}
  else {
    int op=main_operator(p,q);printf("%s",tokens[op].str);
    uint32_t val1=eval(p,op-1);
    uint32_t val2=eval(op+1,q);
    switch(tokens[op].type){
      case '+':return val1+val2;
      case '-':return val1-val2;
      case '*':return val1*val2;
      case '/':return val1/val2;
      default:assert(0);	       
    }
  }
}


uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    printf("not make_token");
    return 0;
  }
  /* TODO: Insert codes to evaluate the expression. */
  return eval(0,nr_token);
}
