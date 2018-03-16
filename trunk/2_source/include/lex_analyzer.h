#ifndef __LEX_ANALYZER_H_
#define __LEX_ANALYZER_H_

#include "token_define.h"
#include "token_detail.hpp"

#define SUCC_CODE 0
#define ERR_CODE -1
//定义词法分析器状态
typedef enum {
    BAT_TOKEN,
    INIT_STATUS,
    IN_VARIABLE, //读取变量
    IN_CONST_STR, //读取常量
    IN_STR_SINGLE_QUOTE,//单引号字符串
    IN_STR_DOUBLE_QUOTE,//双引号字符串
    IN_LOGIC,     //逻辑运算符
    IN_COND       //条件运算符
}LexerStatus;

class LexAnalyzer
{
  public:
    LexAnalyzer(char *express_st);
    
    //设置表达式
    int set_expression(char *express_str);
    //获取下一个token
    int get_next_token(ST_TOKEN *token, int *errCode, char *szMsg);
  public:
  	//另一种扫描获取token方式
  	int scan_token();
	int scan_operator(ST_TOKEN *token, int *errCode, char *szMsg);
	int scan_symbol(ST_TOKEN *token, int *errCode, char *szMsg);
	int scan_string(ST_TOKEN *token, int *errCode, char *szMsg);
  private:
  	bool is_end(const char *iter)
  	{
  	    return (m_end_iter==iter);
  	}
	bool is_comment_start(const char *iter)
  	{
        return false;
	}

	void skip_comments() 
	{
	    //注释格式
	    //  1. //...\n
	    //  2. # ...\n
	    //  3. /*...*/\n
	}
	void skip_whitespace()
	{
	    if(!is_end(m_iter) &&token_detail::is_whitespace(*m_iter))
	    {
	        ++m_iter;
	    }
	}
	
  private:
    char m_expression_str[1024];//表达式字符串
    int  m_cur_pos;//当前词法分析位置

	const char *m_iter;
	const char *m_head_iter;
	const char *m_end_iter;
};

#endif
