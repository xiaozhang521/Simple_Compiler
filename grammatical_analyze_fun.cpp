#include"compile.h"
int Grammatical_Analyze::program()//程序
{
    int tmp=return_type();
    if(!tmp)
    {
        tmp=function();
        if(!tmp)
        {
            return program();
        }
        else
        {
            tmp=main_function();
            if(!tmp) return 0;
            else return 1;
        }
    }
    else
    {
        return tmp;
    }
}
int Grammatical_Analyze::function()//函数
{
    int tmp=11;
    Symbol_Tabel function_tabel;//产生函数符号表
    function_tabel.fill_start_offset();
    if(now_token.code==0)
    {
        now_token=get_next_token();
        if(now_token.code==24)
        {
            now_token=get_next_token();
            if(now_token.code==25)
            {
                now_token=get_next_token();
                if(now_token.code==29)
                {
                    now_token=get_next_token();
                    tmp=varible_declar(function_tabel);
                    if(!tmp)
                    {
                        tmp=compound_statement(function_tabel);
                        if(!tmp)
                        {
                            if(now_token.code==30)
                            {
                                now_token=get_next_token();
                                return 0;
                            }
                        }else return tmp;
                    }else return tmp;
                }else return tmp;
            }else return tmp;
        }else return tmp;
    }else return tmp;
}
int Grammatical_Analyze::main_function()//主函数
{
    int tmp=1;
    if(now_token.code==7)//是main
    {
        now_token=get_next_token();
        if(now_token.code==24)//是(
        {
            now_token=get_next_token();
            if(now_token.code==25)//是)
            {
                now_token=get_next_token();
                    if(now_token.code==29)//是{
                    {
                        Symbol_Tabel main_function_tabel;//产生符号表
                        main_function_tabel.fill_start_offset();
                        QT_program_output();//程序开始四元式
                        now_token=get_next_token();
                        tmp=varible_declar(main_function_tabel);
                        if(!tmp)
                        {
                            tmp=1;
                            tmp=compound_statement(main_function_tabel);
                            if(!tmp)
                            {
                                tmp=1;
                                if(now_token.code==14)//是return
                                {
                                    now_token=get_next_token();
                                    if(now_token.code==3)//是常数
                                    {
                                        now_token=get_next_token();
                                        if(now_token.code==32)//是;
                                        {
                                            now_token=get_next_token();
                                            if(now_token.code==30)
                                            {
                                                QT_program_end_output();//程序结束四元式
                                                code_optimization_output(main_function_tabel,1);
                                                /*change_output_tabel(1);
                                                insert();//优化
                                                recombine_QT();
                                                fill_tmp_variable_tabel(main_function_tabel);
                                                test(main_function_tabel);//测试*/
                                                return 0;
                                            }else return 1;
                                        }else return 1;
                                    }else return 1;
                                }else return 1;
                            }else return tmp;
                        } else return tmp;
                    }else return tmp;
            }else return tmp;
        }else return tmp;
    }
    else return tmp;
}
int Grammatical_Analyze::return_type()//返回类型
{
    if(now_token.code==4||now_token.code==5||now_token.code==6)
    {
        now_token=get_next_token();
        return 0;
    }
    else return 2;
}
