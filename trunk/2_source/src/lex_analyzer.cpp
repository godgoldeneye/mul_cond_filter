
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
            if(!(isalpha(current_char) || isdigit(current_char) || '_'==current_char))
            {
                if(NULL!=errCode) 
            }
        }
    }
}



