
//定义词法分析器状态
typedef enum {
    INIT_STATUS,
    IN_VARIABLE,
    IN_CONST_STR,
    IN_LOGIC,
    IN_COND
}LexerStatus;

class LexAnalyzer
{
  public:
    LexAnalyzer(char *express_str=NULL);
    
    //获取下一个token
    int get_next_token(ST_TOKEN *token);
  private:
    char m_expression_str[1024];//表达式字符串
    int m_cur_pos;//当前词法分析位置
    
};