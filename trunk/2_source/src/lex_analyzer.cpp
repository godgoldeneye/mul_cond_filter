
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
        if(IN_VARIABLE==lex_status)
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
			    return SUCC_CODE;
			}
			else
            {
                if(NULL!=errCode) *errrCode=-1001;
				if(NULL!=szMsg)    sprintf(szMsg, "位置[%d]处不符合变量命名规则");
            }
        }
		else if(IN_CONST_STR==lex_status)
		{
		    if(NULL!=strchr(" \t()=><!~[]\"\r\n")) //碰到这几个字符,意味字符串定义结束
		    {
		        //++m_cur_pos; //位置不能递增
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
		else if('@'==current_char)
		{
		    if(INITIAL_STATUS==lex_status)
		    {
		        token->token_type = IN_VARIABLE;
				++m_cur_pos;
				continue;
		    }
		}
    }
}



