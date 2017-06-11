#include"compile.h"
void Grammatical_Analyze::G()
{
    if(now_token.code==3)
    {
        //******************bug************************
        QString tmp_val=tmp_symbol_tabel.s3.at(now_token.num);
        quaternary_stack.push(tmp_val);
        now_token=get_next_token();
    }
    else if(now_token.code==0)
    {
        QString tmp_name=tmp_symbol_tabel.s0.at(now_token.num).name;
        quaternary_stack.push(tmp_name);
        now_token=get_next_token();
        if(now_token.code==27)
        {
            now_token=get_next_token();
            if(now_token.code==3)
            {
                QString tmp_val=tmp_symbol_tabel.s3.at(now_token.num);
                quaternary_stack.push(tmp_val);
                now_token=get_next_token();
                if(now_token.code==28)
                {
                    now_token=get_next_token();
                    QT_Arithmetic_output('a');
                }
            }
        }
    }
    else if(now_token.code==24)
    {
        now_token=get_next_token();
        E();
        if(now_token.code==25)
        {
            now_token=get_next_token();
        }
        else printf("bracket wrong!\n");
    }
    else printf("illegality character!\n");
}
void Grammatical_Analyze::F()
{
    G();
    do
    {
        if(now_token.code==22)
        {
            now_token=get_next_token();
            G();
            QT_Arithmetic_output('*');
        }
        else if(now_token.code==23)
        {
            now_token=get_next_token();
            G();
            QT_Arithmetic_output('/');
        }
        else return ;
    }while(now_token.code==22||now_token.code==23);
}
void Grammatical_Analyze::T()
{
    F();
    do
    {
        if(now_token.code==19)
        {
            now_token=get_next_token();
            F();
            QT_Arithmetic_output('+');
        }
        else if(now_token.code==20)
        {
            now_token=get_next_token();
            F();
            QT_Arithmetic_output('-');
        }
        else return ;
    }while(now_token.code==19||now_token.code==20);
}
void Grammatical_Analyze::E()
{
    T();
    do
    {
        if(now_token.code==15)
        {
            now_token=get_next_token();
            T();
            QT_Arithmetic_output('l');//<=
        }
        else if(now_token.code==16)
        {
            now_token=get_next_token();
            T();
            QT_Arithmetic_output('h');//>=
        }
        else if(now_token.code==17)
        {
            now_token=get_next_token();
            T();
            QT_Arithmetic_output('e');//==
        }
        else if(now_token.code==36)
        {
            now_token=get_next_token();
            T();
            QT_Arithmetic_output('<');//<
        }
        else if(now_token.code==37)
        {
            now_token=get_next_token();
            T();
            QT_Arithmetic_output('>');//>=
        }
        else return;
    }while(now_token.code==15||now_token.code==16||now_token.code==17||now_token.code==36||now_token.code==37);
}
