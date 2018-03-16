
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include <iterator>

#include "lex_analyzer.h"

LexAnalyzer::LexAnalyzer(char *express_str)
{
    m_cur_pos = 0;
    memset(m_expression_str, 0, sizeof(m_expression_str));
    if(NULL!=express_str)
        strncpy(m_expression_str, express_str, sizeof(m_expression_str)-1);
}

int LexAnalyzer::set_expression(char *express_str/*=NULL*/)
{
    m_cur_pos = 0;
    memset(m_expression_str, 0, sizeof(m_expression_str));
    if(NULL!=express_str)
        strncpy(m_expression_str, express_str, sizeof(m_expression_str)-1);
    return 0;
}

int LexAnalyzer::get_next_token(ST_TOKEN *token, int *errCode, char *szMsg)
{
    if(NULL==token)
        return -1;
    
    int out_pos=0;
    LexerStatus lex_status=INIT_STATUS;
    char current_char;
    
    ST_TOKEN *token_ptr=token;
    token_ptr->token_type = TKN_NONE;//初始状态没有token;
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
			else if(NULL!=strchr(" \t()=><!~[]\"\r\n", current_char)) //碰到这几个字符,意味变量定义结束
			{
			    //++m_cur_pos; //位置不能递增
			    token->token_type = TKN_SYMBOL;
			    return SUCC_CODE;
			}
			else
            {
                token_ptr->token_type = TKN_BAD;
                if(NULL!=errCode) *errCode=-1001;
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
		    if(INIT_STATUS==lex_status)
		    {
		        lex_status = IN_STR_SINGLE_QUOTE;
		        //token->token_type = TKN_VARIABLE;
				++m_cur_pos;
				continue;
		    }
		}
		else if('"'==current_char)
		{
		    if(INIT_STATUS==lex_status)
		    {
		        lex_status = IN_STR_DOUBLE_QUOTE;
		        //token->token_type = TKN_VARIABLE;
				++m_cur_pos;
				continue;
		    }
		}
		else if('@'==current_char)
		{
		    if(INIT_STATUS==lex_status)
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
		else if(strchr("<>=!~|&", current_char)!=NULL)
		{
		    if(0==strncmp(m_expression_str+m_cur_pos, ">=", 2))
		    {
		        token->token_type = TKN_GE;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;				
		    }
			else if(0==strncmp(m_expression_str+m_cur_pos, "==", 2))
			{
			    token->token_type = TKN_EQ;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if(0==strncmp(m_expression_str+m_cur_pos, "!=", 2))
			{
			    token->token_type = TKN_NE;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if(0==strncmp(m_expression_str+m_cur_pos, "<=", 2))
			{
			    token->token_type = TKN_LE;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if(0==strncmp(m_expression_str+m_cur_pos, "~=", 2))//模糊匹配
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
			else if(0==strncmp(m_expression_str+m_cur_pos, "||", 2))
			{
			    token->token_type = TKN_OR;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else if(0==strncmp(m_expression_str+m_cur_pos, "&&", 2))
			{
			    token->token_type = TKN_OR;
				++m_cur_pos;
				++m_cur_pos;
				return SUCC_CODE;	
			}
			else
			{
			    token_ptr->token_type = TKN_BAD;
                if(NULL!=errCode) *errCode=-1002;
				if(NULL!=szMsg)    sprintf(szMsg, "位置[%d]处不符合词法规则", m_cur_pos);
				return ERR_CODE;
			}
		}
		else if (strchr(" \t\r\n", current_char)!=NULL)
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
		    token_ptr->token_type = TKN_BAD;
            if(NULL!=errCode) *errCode=-1003;
			if(NULL!=szMsg)    sprintf(szMsg, "位置[%d]处不符合词法规则", m_cur_pos);
			return ERR_CODE;
		}
    }

	return SUCC_CODE;
}

int LexAnalyzer::scan_operator(ST_TOKEN *token, int *errCode, char *szMsg)
{
    memset(token, 0, sizeof(ST_TOKEN));
	
    const char c0=m_iter[0];
	if(!is_end(m_iter+1))//还有后续符号
	{
	    const char c1=m_iter[1];
		//不支持交换符号"<=>",故c2暂时不做判断
		token->token_type = TKN_NONE;
		if((c0 == '<') && (c1 == '='))      token->token_type = TKN_LE;
		else if((c0 == '>') && (c1 == '=')) token->token_type = TKN_GE;
		else if((c0 == '<') && (c1 == '>')) token->token_type = TKN_NE;
		else if((c0 == '!') && (c1 == '=')) token->token_type = TKN_NE;
		else if((c0 == '=') && (c1 == '=')) token->token_type = TKN_EQ;
		else if((c0 == '&') && (c1 == '&')) token->token_type = TKN_AND;
		else if((c0 == '|') && (c1 == '|')) token->token_type = TKN_OR;
		if(TKN_NONE != token->token_type)
		{
	        strncpy(token->token_value, m_iter, 2);
	        token->pos = std::distance(m_iter, (const char *)m_expression_str);
			m_iter += 2;
	       return SUCC_CODE;
		}
	}
	if('<'==c0)      token->token_type = TKN_LT;
	else if('>'==c0) token->token_type = TKN_GT;
	else if('!'==c0) token->token_type = TKN_NOT;
	
	token->pos = std::distance(m_iter, (const char *)m_expression_str);
	++m_iter;
	return SUCC_CODE;
}

int LexAnalyzer::scan_symbol(ST_TOKEN *token, int *errCode, char *szMsg)
{
    /*
     * 符号只能由数字、字母、下划线组成
     */
    memset(token, 0, sizeof(ST_TOKEN));
	if(is_end(m_iter))
		return -1;
	if(*m_iter!='@')
		return -2;
	++m_iter;
	
	const char *init_iter = m_iter;
	while(!is_end(m_iter))
	{
	    if((!token_detail::is_letter_or_digit(*m_iter)) && ('_'!=*m_iter))
			break;

		++m_iter;
	}


	token->token_type = TKN_SYMBOL;
	strncpy(token->token_value, init_iter, std::distance(m_iter, init_iter));
	token->pos = std::distance(init_iter, (const char *)m_expression_str);
	return SUCC_CODE;
}

int LexAnalyzer::scan_string(ST_TOKEN *token, int *errCode, char *szMsg)
{
    /*
     * 格式:
     *    1. "string value" //使用双引号包括
     *    2. 'string value' //使用单引号包括
     *    3.  stringvalue    //不使用单引号和双引号,使用空白字符、操作符等特殊符号分割
     */
    memset(token, 0, sizeof(ST_TOKEN));
	if(is_end(m_iter))
		return -1;
	LexerStatus lex_status = INIT_STATUS;
	if('\''==*m_iter)
	{
		lex_status = IN_STR_SINGLE_QUOTE;
		++m_iter;
	}
	else if('"'==*m_iter)
	{
	    lex_status=IN_STR_DOUBLE_QUOTE;
		++m_iter;
	}
	
	
	const char *init_iter = m_iter;
	if(IN_STR_SINGLE_QUOTE==lex_status)//单引号包括字符串
	{
	    while(!is_end(m_iter))
	    {
	        if('\''==*m_iter)//另一个单引号
				break;
			if(('\r' == *m_iter) || ('\n'==*m_iter))
				break;
			
	        ++m_iter;
	    }
	}
	else if(IN_STR_DOUBLE_QUOTE==lex_status)//双括号包括字符串
	{
	    while(!is_end(m_iter))
	    {
	        if('"'==*m_iter)//另一个单引号
				break;
			if(('\r' == *m_iter) || ('\n'==*m_iter))
				break;
			
	        ++m_iter;
	    }
	}
	else
	{
	    while(!is_end(m_iter))
	    {
	        if(token_detail::is_whitespace(*m_iter))
				break;
			if(token_detail::is_operator_char(*m_iter))
				break;
			
	        ++m_iter;
	    }
	}

    if((IN_STR_SINGLE_QUOTE==lex_status)||(IN_STR_DOUBLE_QUOTE==lex_status))
    {
	    token->token_type = TKN_STRING;
	    strncpy(token->token_value, init_iter, std::distance(m_iter, init_iter));
	    token->pos = std::distance(init_iter, (const char *)m_expression_str);
    }
	else
	{
	    token->token_type = TKN_CONST_STR;
	    strncpy(token->token_value, init_iter, std::distance(m_iter, init_iter));
	    token->pos = std::distance(init_iter, (const char *)m_expression_str);
    }
	return SUCC_CODE;
}




