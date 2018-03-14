/**
 * 文件名:
 *   定义多条件过滤(mul_cond_filter)规则用到的token,包括运算符等;
 **/

#ifndef __TOKEN_DEFINE_H_

#define __TOKEN_DEFINE_H_

//定义token类型
typedef enum {
    TKN_NONE,//不是任何token
    TKN_BAD,
    TKN_AND,
    TKN_OR,
    TKN_NOT,
    
    TKN_GT,
    TKN_GE,
    TKN_LT,
    TKN_LE,
    TKN_NE,
    TKN_EQ,
    TKN_FUZZY,
    TKN_SYMBOL, //变量,以$开头
    TKN_CONST_STR,  //常量,无类型,即词法解析器无法确定是symbol还是字符串
    TKN_STRING,//字符串,使用单引号或双引号
    TKN_PARENTHESE_LEFT, //'('
    TKN_PARENTHESE__RIGHT, //'('

	TKN_ERR_SYMBOL,//错误符号
}TokenKind;

#define MAX_TOKEN_SIZE (256)

//定义token,token包括种类、原始字符、值
typedef struct _st_token_
{
   TokenKind  token_type;//记号种类
   char token_orig_str[MAX_TOKEN_SIZE];
   char token_value[256];
   int  pos;
}ST_TOKEN;


//逻辑运算符
#define OP_LOGIC_AND  "&&"
#define OP_LOGIC_OR   "||"
#define OP_LOGIC_NOT  "!"
//条件运算符
#define OP_COND_GT    ">"
#define OP_COND_GE    ">="
#define OP_COND_LT    "<"
#define OP_COND_LE    "<="
#define OP_COND_NE    "!="
#define OP_COND_EQ    "=="
#define OP_COND_FUZZY "~=" //fuzzy match

//关键字
#define TK_KW_TIME  "time" //时间
#define TK_KW_TID   "tid"  //tid
#define TK_KW_FID   "fid"  //field_id
#define TK_KW_FNAME "fld_name" //数据包名

//特殊字符
#define TK_CHAR_ESC                  '\\' //escape character 转义符
#define TK_CHAR_WILDCARD             '*' //wildcard character 通配符
#define TK_CHAR_SPACE                ' ' //space 空格
#define TK_CHAR_TAB                  '\t' //tab character
#define TK_CHAR_PARENTHESE_LEFT      '(' //left parenthese 左小括号
#define TK_CHAR_PARENTHESE__RIGHT    ')' //right parenthese 右小括号
#define TK_CHAR_SQUARE_BRACKET_LEFT  ']' //中括号
#define TK_CHAR_SQUARE_BRACKET_RIGHT ']' //中括号
//#define TK_CHAR_ANGLE_BRACKET "<>" //尖括号
//#define TK_CHAR_BRACES  "{}" //大括号

#define TK_CHAR_COLON                ':' //冒号
//#define TK_CHAR_AT     '@'

#endif

