#include"compile.h"
int Grammatical_Analyze::varible_declar(Symbol_Tabel &symbol_tabel)//变量声明
{
    int tmp=3;
    tmp=type();
    if(!tmp)
    {
        if(now_token.code==0)
        {
            cat_front=5;
            fill_table(symbol_tabel);
            now_token=get_next_token();
            if(now_token.code==27)
            {
                now_token=get_next_token();
                int tmp=array_definition(symbol_tabel);
                if(!tmp)
                {
                    tmp=varible_declar(symbol_tabel);
                    if(!tmp)
                    {
                        return 0;
                    }else return tmp;
                }else return tmp;
            }
            else
            {
                tmp=same_definition_table(symbol_tabel);
                if(!tmp)
                {
                    tmp=varible_declar(symbol_tabel);
                    if(!tmp)
                    {
                        return 0;
                    }else return tmp;
                } else return tmp;
            }
        }else return 4;
    }else return 0;
}
int Grammatical_Analyze::identifiter()//标识符
{
    if(now_token.code==0)
    {
        now_token=get_next_token();
        return 0;
    }
    else return 4;
}
int Grammatical_Analyze::compound_statement(Symbol_Tabel &symbol_tabel)//复合语句
{
    int tmp;
    if(now_token.code==9)
    {
        now_token=get_next_token();
        tmp=if_statement(symbol_tabel);
        if(!tmp)
        {
            tmp=compound_statement(symbol_tabel);
            if(!tmp)
            return 0;
            else return tmp;
        }else return tmp;
    }
    else if(now_token.code==12)
    {
        now_token=get_next_token();
        tmp=while_statement(symbol_tabel);
        if(!tmp)
        {
            tmp=compound_statement(symbol_tabel);
            if(!tmp)
            return 0;
            else return tmp;
        }else return tmp;
    }
    else if(now_token.code==13)
    {
        now_token=get_next_token();
        tmp=print_statement();
        if(!tmp)
        {
            tmp=compound_statement(symbol_tabel);
            if(!tmp)
            return 0;
            else return tmp;
        }else return tmp;
    }
    else if(!assign_state(symbol_tabel))
    {
        tmp=compound_statement(symbol_tabel);
        if(!tmp)
        return 0;
        else return tmp;
    }
    else
    {
        return 0;
    }
}
int Grammatical_Analyze::type()//类型
{
    if(now_token.code==4)
    {
        front=1;
        now_token=get_next_token();
        return 0;
    }
    else if(now_token.code==5)
    {
        front=2;
        now_token=get_next_token();
        return 0;
    }
    else
    {
        return 5;
    }
}
int Grammatical_Analyze::same_definition_table(Symbol_Tabel &symbol_tabel)//同类定义表
{
    int tmp=3;
    if(now_token.code==33)
    {
        now_token=get_next_token();
        if(now_token.code==0)
        {
            cat_front=5;
            fill_table(symbol_tabel);
            now_token=get_next_token();
            tmp=same_definition_table(symbol_tabel);
            if(!tmp)
            {
                return 0;
            }else return tmp;
        }else return 4;
    }
    else if(now_token.code==32)
    {
        now_token=get_next_token();
        return 0;
    }
    else return tmp;
}
/*int Grammatical_Analyze::definition_table()//定义表
{
    int tmp=3;
    int same_type;
    tmp=type(same_type);
    if(!tmp)
    {
        tmp=identifiter();
        if(!tmp)
        {
            tmp=same_definition_table(same_type);
            if(!tmp)
            {
                tmp=definition_table();
                if(!tmp)
                {
                    return 0;
                }else return tmp;
            }else return tmp;
        }else return tmp;
    }else return 0;
}*/
/*int Grammatical_Analyze::assign_state_table()//赋值语句表
{
    int tmp=6;
    tmp=assign_state();
    if(!tmp)
    {
        tmp=assign_state_table();
        if(!tmp)
        {
           return 0;
        }return tmp;
    }else return 0;
}*/
int Grammatical_Analyze::assign_state(Symbol_Tabel &symbol_tabel)//赋值语句
{
    int tmp=6;
    if(now_token.code==0)
    {
        QString tmp_name=tmp_symbol_tabel.s0.at(now_token.num).name;
        quaternary_stack.push_back(tmp_name);//压入标识符
        now_token=get_next_token();
        if(now_token.code==18)
        {
            now_token=get_next_token();
            E();
            if(now_token.code==32)
            {
                QT_equal_output();//输出赋值四元式
                now_token=get_next_token();
                return 0;
            }else {erro_message.push_back(6); return tmp;}
        }
        else if(now_token.code==27)//数组[
        {
            now_token=get_next_token();
            if(now_token.code==3)
            {
                quaternary_stack.push_back(tmp_symbol_tabel.s3.at(now_token.num));//压入偏移量
                now_token=get_next_token();
                if(now_token.code==28)//]
                {
                    now_token=get_next_token();
                    if(now_token.code==18)
                    {
                        now_token=get_next_token();
                        E();
                        if(now_token.code==32)
                        {
                            QT_array_equal_output();
                            now_token=get_next_token();
                            return 0;
                        }else {erro_message.push_back(6); return tmp;}
                    }else {erro_message.push_back(6); return tmp;}
                }else {erro_message.push_back(6); return tmp;}
            }else {erro_message.push_back(6); return tmp;}
        }
        else {erro_message.push_back(6); return tmp;}
    }
    else return 4;
}
int Grammatical_Analyze::if_statement(Symbol_Tabel &symbol_tabel)
{
    int tmp=7;
    if(now_token.code==24)//(
    {
        now_token=get_next_token();
        E();//表达式
        QT_if_output();
        //块优化生成
        code_optimization_output(symbol_tabel,1);
        /*change_output_tabel(1);
        insert();//优化
        recombine_QT();
        fill_tmp_variable_tabel(symbol_tabel);
        test(symbol_tabel);//测试*/
        if(now_token.code==25)//)
        {
            now_token=get_next_token();
            if(now_token.code==29)//{
            {
                Symbol_Tabel new_tabel;
                new_tabel.last_tabel=&symbol_tabel;
                new_tabel.fill_start_offset();
                now_token=get_next_token();
                tmp=varible_declar(new_tabel);
                if(!tmp)
                {
                    tmp=compound_statement(new_tabel);
                    if(!tmp)
                    {
                        tmp=7;
                        if(now_token.code==30)//}
                        {
                            now_token=get_next_token();
                            tmp=then_statement(new_tabel);
                            if(!tmp)
                            {
                                return 0;
                            }else return tmp;
                        }else return tmp;
                    }else return tmp;
                }else return tmp;
            } else return tmp;
        }else return tmp;
    }else return tmp;
}
int Grammatical_Analyze::then_statement(Symbol_Tabel &symbol_tabel)
{
    int tmp=7;
    if(now_token.code==10)//else
    {
        QT_else_output();
        code_optimization_output(symbol_tabel,2);
        /*change_output_tabel(2);
        insert();//优化
        recombine_QT();
        fill_tmp_variable_tabel(symbol_tabel);
        test(symbol_tabel);//测试*/

        now_token=get_next_token();
        if(now_token.code==29)//{
        {
            Symbol_Tabel new_tabel;
            new_tabel.last_tabel=symbol_tabel.last_tabel;
            new_tabel.fill_start_offset();
            now_token=get_next_token();
            tmp=varible_declar(new_tabel);
            if(!tmp)
            {
                tmp=compound_statement(new_tabel);
                if(!tmp)
                {
                    tmp=7;
                    if(now_token.code==30)//}
                    {
                        code_optimization_output(new_tabel,3);
                        /*change_output_tabel(3);
                        insert();
                        recombine_QT();
                        fill_tmp_variable_tabel(new_tabel);
                        test(new_tabel);//测试*/
                        QT_end_if_output(true);
                        now_token=get_next_token();
                        return 0;
                    }else return tmp;
                }else return tmp;
            }else return tmp;
        }else return tmp;
    }else
    {
        code_optimization_output(symbol_tabel,2);
        /*change_output_tabel(2);
        insert();//优化
        recombine_QT();
        fill_tmp_variable_tabel(symbol_tabel);
        test(symbol_tabel);//测试*/
        QT_end_if_output(false);
        return 0;
    }
}
int Grammatical_Analyze::while_statement(Symbol_Tabel &symbol_tabel)
{
    int tmp=8;
    if(now_token.code==24)//(
    {
        now_token=get_next_token();
        E();//表达式
        if(now_token.code==25)//)
        {
            now_token=get_next_token();
            if(now_token.code==29)//{
            {
                Symbol_Tabel new_tabel;
                new_tabel.last_tabel=&symbol_tabel;
                symbol_tabel.fill_start_offset();
                now_token=get_next_token();
                tmp=varible_declar(new_tabel);
                if(!tmp)
                {
                    tmp=compound_statement(new_tabel);
                    if(!tmp)
                    {
                        tmp=8;
                        if(now_token.code==30)//}
                        {
                            now_token=get_next_token();
                            return 0;
                        }else return tmp;
                    }else return tmp;
                }else return tmp;
            } else return tmp;
        }else return tmp;
    }else return tmp;
}
int Grammatical_Analyze::print_statement()
{
    int tmp=9;
    if(now_token.code==24)//(
    {
        now_token=get_next_token();
        E();
        if(now_token.code==25)//)
        {
            QT_print_output();
            now_token=get_next_token();
            if(now_token.code==32)
            {
                now_token=get_next_token();
                return 0;
            }else return tmp;
        }else return tmp;
    }else return tmp;
}
int Grammatical_Analyze::array_definition(Symbol_Tabel &symbol_tabel)
{
    int tmp=10;
    if(now_token.code==3)
    {
        fill_array_tabel(symbol_tabel);
        now_token=get_next_token();
        if(now_token.code==28)
        {
            now_token=get_next_token();
            if(now_token.code==32)
            {
                now_token=get_next_token();
                return 0;
            }else return tmp;
        }else return tmp;
    }else return tmp;
}
