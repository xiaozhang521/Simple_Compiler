#include"compile.h"
#include"object_code_generate.h"
#include<QDebug>
Grammatical_Analyze::Grammatical_Analyze()
{
    token_tot=0;
    front=0;
    cat_front=0;
    middle_num=0;
    node_number=0;
    label_val=0;
}
void init_output_file()
{
    //QFile file("output.txt");
    QFile file("./output.asm");
    if (file.open(QFile::WriteOnly | QIODevice::Truncate))
    {
        QTextStream cout(&file);
        cout<<"SSEG SEGMENT STACK"<<"\r\n";
        cout<<"STK DB 100 DUP(0)"<<"\r\n";
        cout<<"SSEG ENDS"<<"\r\n";
        cout<<"MAIN SEGMENT\r\n";
        cout<<"ASSUME CS:MAIN,DS:MAIN\r\n";
        cout<<"ASSUME SS:SSEG\r\n";
    }
}
tokenType Grammatical_Analyze::get_next_token()
{
    return token.at(token_tot++);
}
QVector<QT> Grammatical_Analyze::get_QT_tabel()
{
    return output_quaternary_tabel;
}
bool Grammatical_Analyze::grammatical_start()
{
    init_output_file();
    int tmp=program();
    if(tmp==0&&erro_message.size()==0)
    {
        printf("correct!\n");
    }
    else
    {
        if(tmp!=0)
        erro_message.push_back(tmp);
        for(int i=0;i<erro_message.size();++i)
        {
        int tmp=erro_message.at(i);
        switch(tmp)
        {
        case 0:
            printf("yes");
        break;
        case 1:
            printf("main_function wrong!");
        break;
        case 2:
            printf("return type wrong!");
        break;
        case 3:
            printf("varible_declar wrong!");
        break;
        case 4:
            printf("not a identifier!");
        break;
        case 5:
            printf("no this data type!");
        break;
        case 6:
            printf("arithmetic_expression wrong!");
        break;
        default:
            printf("no");
        }
        }
    }
    return true;
}
void Grammatical_Analyze::lexical_start()
{
    Lexical_Analyze lexical;
    lexical.start();
    tmp_symbol_tabel=lexical.return_tabel();//可替代
    token=lexical.return_token();
    now_token=get_next_token();
}
void Grammatical_Analyze::test(Symbol_Tabel symbol_tabel)
{
    printf("*************test***************\n");
    for(int i=0;i<symbol_tabel.s0.size();++i)//符号表测试
    {
        SYNB now=symbol_tabel.s0.at(i);
        qDebug()<<now.name<<now.TYPE<<now.cat<<now.addr<<now.active_message;
        if(symbol_tabel.type_table.at(now.TYPE).second!=-1)
        {
            int tmp=symbol_tabel.type_table.at(now.TYPE).second;
            qDebug()<<"array tabel"<<symbol_tabel.arrry_tabel.at(tmp).low<<symbol_tabel.arrry_tabel.at(tmp).up<<symbol_tabel.arrry_tabel.at(tmp).ctp<<symbol_tabel.arrry_tabel.at(tmp).clen;
        }
    }
    /*for(int i=0;i<output_quaternary_tabel.size();++i)//四元式测试
    {
        QT tmp=output_quaternary_tabel.at(i);
        qDebug()<<tmp.oprate_val<<tmp.first_val<<tmp.second_val<<tmp.tmp_val;
    }*/
    printf("********************************\n");
}
void Grammatical_Analyze::code_optimization_output(Symbol_Tabel &symbol_tabel,int option)
{
    change_output_tabel(option);
    insert();//优化
    recombine_QT();
    fill_tmp_variable_tabel(symbol_tabel);
    test(symbol_tabel);//测试
    Object_Code_Generate object_code_generate(symbol_tabel);
    object_code_generate.generate_QT_active_message(*this);
    jump_label=object_code_generate.output_object_code();
    label_val=object_code_generate.return_label_val();
}
