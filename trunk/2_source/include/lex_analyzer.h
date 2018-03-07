#ifndef __LEX_ANALYZER_H_
#define __LEX_ANALYZER_H_

#define SUCC_CODE 0
//定义词法分析器状态
typedef enum {
    BAT_TOKEN,
    INIT_STATUS,
    IN_VARIABLE, //读取变量
    IN_CONST_STR, //读取常量
    IN_LOGIC,     //逻辑运算符
    IN_COND       //条件运算符
}LexerStatus;

class LexAnalyzer
{
  public:
    LexAnalyzer(char *express_str=NULL);
    
    //设置表达式
    int set_expression(char *express_str);
    //获取下一个token
    int get_next_token(ST_TOKEN *token);
  private:
    char m_expression_str[1024];//表达式字符串
    int  m_cur_pos;//当前词法分析位置
};

#endif
