
#include <string.h>
include "lex_analyzer.h"

LexAnalyzer::LexAnalyzer(char *express_str/*=NULL*/)
{
    m_cur_pos = 0;
    memset(m_expression_str, 0, sizeof(m_expression_str));
    if(NULL!=express_str)
        strncpy(m_expression_str, express_str);
}

int LexAnalyzer::set_expression(char *express_str/*=NULL*/)
{
    m_cur_pos = 0;
    memset(m_expression_str, 0, sizeof(m_expression_str));
    if(NULL!=express_str)
        strncpy(m_expression_str, express_str);
    return 0;
}

int LexAnalyzer::get_next_token(ST_TOKEN *token, int *errCode, char *szMsg)
{
    if(NULL==token)
        return -1;
    
    int out_pos=0;
    LexerStatus lex_status=INITIAL_STATUS;
    char current_char;
    
    ST_TOKEN *token_ptr=token;
    token_ptr->token_type = BAT_TOKEN;//初始状态没有token;
    while(m_expression_str[m_cur_pos]!='\0')
    {
        current_char=m_expression_str[m_cur_pos];
        // if (BAT_TOKEN==token_ptr->token_type)
        // {
            // switch(current_char)
            // {
                // case '@':
                    // token_ptr->token_type = IN_VARIABLE;
                    // break;
                // case '(':
                    // token_ptr->token_type = TK_CHAR_PARENTHESE_LEFT;
                    // break;
                // case ')':
                    // token_ptr->token_type = TK_CHAR_PARENTHESE_RIGHT;
                    // break;
            // }
        // }
        if(IN_VARIABLE==lex_status) //位于变量中
        {
            //变量命名规则:数字、字母、下划线
            if(isalpha(current_char) || isdigit(current_char) || '_'==current_char)
            {
                token_ptr->token_orig_str[out_pos]=current_char;
				++out_pos;
				++m_cur_pos;
				continue;
            }
			else if(NULL!=strchr(" \t()=><!~[]\"\r\n")) //碰到这几个字符,意味变量定义结束
			{
			    //++m_cur_pos; //位置不能递增
			    token->token_type = TKN_VARIABLE;
			    return SUCC_CODE;
			}
			else
            {
                token_ptr->token_type = BAT_TOKEN;
                if(NULL!=errCode) *errrCode=-1001;
				if(NULL!=szMsg)    sprintf(szMsg, "位置[%d]处不符合变量命名规则",m_cur_pos);
				return ERR_CODE;
            }
        }
		else if(IN_CONST_STR==lex_status) //常量字符串中
		{
		    if(NULL!=strchr(" \t()=><!~[]'\"\r\n", current_char)) //碰到这几个字符,意味字符串定义结束
		    {
		        //++m_cur_pos; //位置不能递增
		        token->token_type = TKN_CONST_STR;
		        return SUCC_CODE;
		    }
			else
		    {
		        token_ptr->token_orig_str[out_pos]=current_char;
			    ++out_pos;
			    ++m_cur_pos;
				continue;
		    }
		}
		else if(IN_STR_DOUBLE_QUOTE==lex_status) //双引号字符串
		{
		    if(NULL!=strchr("\"\r\n", current_char))
		    {
		        ++m_cur_pos; //位置必须递增
			    token->token_type = TKN_STRING;
			    return SUCC_CODE;
		    }
			else
		    {
		        token_ptr->token_orig_str[out_pos]=current_char;
			    ++out_pos;
			    ++m_cur_pos;
				continue;
		    }
		}
		else if(IN_STR_SINGLE_QUOTE==lex_status) //单引号字符串
		{
		    if(NULL!=strchr("'\r\n", current_char))
		    {
		        ++m_cur_pos; //位置必须递增
			    token->token_type = TKN_STRING;
			    return SUCC_CODE;
		    }
			else
		    {
		        token_ptr->token_orig_str[out_pos]=current_char;
			    ++out_pos;
			    ++m_cur_pos;
				continue;
		    }
		}
		else if('\''==current_char)//单引号
		{
		    if(INITIAL_STATUS==lex_status)
		    {
		        lex_status = IN_STR_SINGLE_QUOTE;
		        //token->token_type = TKN_VARIABLE;
				++m_cur_pos;
				continue;
		    }
		}
		else if('"'==current_char)
		{
		    if(INITIAL_STATUS==lex_status)
		    {
		        lex_status = IN_STR_DOUBLE_QUOTE;
		        //token->token_type = TKN_VARIABLE;
				++m_cur_pos;
				continue;
		    }
		}
		else if('@'==current_char)
		{
		    if(INITIAL_STATUS==lex_status)
		    {
		        lex_status = IN_VARIABLE;
		        //token->token_type = TKN_VARIABLE;
				++m_cur_pos;
				continue;
		    }
		} 
		else if('('==current_char)
		{
		    token->token_type = TKN_PARENTHESE_LEFT;
			++m_cur_pos;
		    return SUCC_CODE;
		}
		else if(')'==current_char)
		{
		    token->token_type = TKN_PARENTHESE_RIGHT;
			++m_cur_pos;
		    return SUCC_CODE;
		}
		else if(strchar("<>=!~|&", current_char)!=NULL)
		{
		    if(0==strncmp(m_expression_str[m_cur_pos], ">=", 2))
		    {
		        token->token_type = TKN_GE;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;				
		    }
			else if(0==strncmp(m_expression_str[m_cur_pos], "==", 2))
			{
			    token->token_type = TKN_EQ;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if(0==strncmp(m_expression_str[m_cur_pos], "!=", 2))
			{
			    token->token_type = TKN_NE;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if(0==strncmp(m_expression_str[m_cur_pos], "<=", 2))
			{
			    token->token_type = TKN_LE;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if(0==strncmp(m_expression_str[m_cur_pos], "~=", 2))//模糊匹配
			{
			    token->token_type = TKN_FUZZY;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if('>'==current_char)
			{
			    token->token_type = TKN_GT;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if('<'==current_char)
			{
			    token->token_type = TKN_LT;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if('!'==current_char)
			{
			    token->token_type = TKN_NOT;
				++m_cur_pos;
				return SUCC_CODE;
			}
			else if(0==strncmp(m_expression_str[m_cur_pos], "||", 2))
			{
			    token->token_type = TKN_OR;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if(0==strncmp(m_expression_str[m_cur_pos], "&&", 2))
			{
			    token->token_type = TKN_OR;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else
			{
			    token_ptr->token_type = BAT_TOKEN;
                if(NULL!=errCode) *errrCode=-1002;
				if(NULL!=szMsg)    sprintf(szMsg, "位置[%d]处不符合词法规则", m_cur_pos);
				return ERR_CODE;
			}
		}
		else if (strchr(' \t\r\n', current_char)!=NULL)
		{
		    if(INIT_STATUS==lex_status)
		    {
		        ++m_cur_pos;
				continue;
		    }
		}
		else if(isalpha(current_char) || isdigit(current_char) || '_'==current_char)
        {
            if(INIT_STATUS==lex_status)
		    {
		        token_ptr->token_orig_str[out_pos]=current_char;
				++out_pos;
		        ++m_cur_pos;
				lex_status = IN_CONST_STR;
				continue;
		    }
        }
		else
		{
		    token_ptr->token_type = BAT_TOKEN;
            if(NULL!=errCode) *errrCode=-1003;
			if(NULL!=szMsg)    sprintf(szMsg, "位置[%d]处不符合词法规则", m_cur_pos);
			return ERR_CODE;
		}
    }

	return SUCC_CODE;
}



