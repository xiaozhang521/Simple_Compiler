#include"compile.h"
#include<QDebug>
void Grammatical_Analyze::QT_Arithmetic_output(char tmpc)
{
    //printf("tmpc=%c\n",tmpc);
    QString tmp;
    if(tmpc=='+')
        tmp="+";
    else if(tmpc=='-')
        tmp="-";
    else if(tmpc=='*')
        tmp="*";
    else if(tmpc=='/')
        tmp="/";
    else if(tmpc=='=')
        tmp="=";
    else if(tmpc=='<')
        tmp="<";
    else if(tmpc=='>')
        tmp=">";
    else if(tmpc=='h')//大于等于
        tmp=">=";
    else if(tmpc=='l')//小于等于
        tmp="<=";
    else if(tmpc=='e')//等等于
        tmp="==";
    else if(tmpc=='a')//数组取值
        tmp="=[]";
    QT n;
    n.oprate_val=tmp;
    if(!quaternary_stack.empty())
    {
        tmp=quaternary_stack.top();
        quaternary_stack.pop();
        n.second_val=tmp;
    }
    if(!quaternary_stack.empty())
    {
        tmp=quaternary_stack.top();
        quaternary_stack.pop();
        n.first_val=tmp;
    }
    char buf[10];
    sprintf(buf,"%d",middle_num++);
    QString en=buf;
    en="t"+en;
    n.tmp_val=en;
    quaternary_stack.push(en);
    quaternary_tabel.push_back(n);
}
void Grammatical_Analyze::QT_equal_output()
{
    QString tmp;
    QT n;
    n.oprate_val="=";
    if(!quaternary_stack.empty())
    {
        tmp=quaternary_stack.top();
        quaternary_stack.pop();
        n.first_val=tmp;
    }
    n.second_val="_";
    if(!quaternary_stack.empty())
    {
        tmp=quaternary_stack.top();
        quaternary_stack.pop();
        n.tmp_val=tmp;
    }
    quaternary_tabel.push_back(n);
}
void Grammatical_Analyze::QT_program_output()
{
    QT new_tabel;
    new_tabel.oprate_val="start";
    new_tabel.first_val="_";
    new_tabel.second_val="_";
    new_tabel.tmp_val="_";
    quaternary_tabel.push_back(new_tabel);
}
void Grammatical_Analyze::QT_program_end_output()
{
    QT new_tabel;
    new_tabel.oprate_val="end";
    new_tabel.first_val="_";
    new_tabel.second_val="_";
    new_tabel.tmp_val="_";
    quaternary_tabel.push_back(new_tabel);
}
void Grammatical_Analyze::QT_if_output()
{
    QString tmp;
    QT new_tabel;
    new_tabel.oprate_val="if";
    if(!quaternary_stack.empty())
    {
        tmp=quaternary_stack.top();
        quaternary_stack.pop();
        new_tabel.first_val=tmp;
    }
    new_tabel.second_val="_";
    new_tabel.tmp_val="_";
    quaternary_tabel.push_back(new_tabel);
}
void Grammatical_Analyze::QT_end_if_output(bool flag)
{
    QT new_tabel;
    new_tabel.oprate_val="ie";
    new_tabel.first_val="_";
    new_tabel.second_val="_";
    new_tabel.tmp_val="_";
    QString tmp;
    if(flag)
    {
        tmp="el";
    }
    else tmp="if";
    int offset=0;
    for(int i=quaternary_tabel.size()-1;i>=0;--i)
    {
        offset++;
        if(quaternary_tabel.at(i).oprate_val==tmp&&quaternary_tabel.at(i).tmp_val=="_")
        {
            quaternary_tabel[i].tmp_val=QString::number(offset,10);
            break;
        }
    }
    quaternary_tabel.push_back(new_tabel);
}
void Grammatical_Analyze::QT_else_output()
{
    QT new_tabel;
    new_tabel.oprate_val="el";
    new_tabel.first_val="_";
    new_tabel.second_val="_";
    new_tabel.tmp_val="_";
    int offset=1;
    for(int i=quaternary_tabel.size()-1;i>=0;--i)
    {
        offset++;
        if(quaternary_tabel.at(i).oprate_val=="if"&&quaternary_tabel.at(i).tmp_val=="_")
        {
            quaternary_tabel[i].tmp_val=QString::number(offset,10);
            break;
        }
    }
    quaternary_tabel.push_back(new_tabel);
}
void Grammatical_Analyze::QT_print_output()
{
    QString tmp;
    QT new_tabel;
    new_tabel.oprate_val="print";
    if(!quaternary_stack.empty())
    {
        tmp=quaternary_stack.top();
        quaternary_stack.pop();
        new_tabel.first_val=tmp;
    }
    new_tabel.second_val="_";
    new_tabel.tmp_val="_";
    quaternary_tabel.push_back(new_tabel);
}
void Grammatical_Analyze::QT_array_equal_output()
{
    QString tmp;
    QT new_tabel;
    new_tabel.oprate_val="[]=";
    if(!quaternary_stack.empty())
    {
        tmp=quaternary_stack.top();
        quaternary_stack.pop();
        new_tabel.first_val=tmp;
    }
    if(!quaternary_stack.empty())
    {
        tmp=quaternary_stack.top();
        quaternary_stack.pop();
        new_tabel.second_val=tmp;
    }
    if(!quaternary_stack.empty())
    {
        tmp=quaternary_stack.top();
        quaternary_stack.pop();
        new_tabel.tmp_val=tmp;
    }
    quaternary_tabel.push_back(new_tabel);
}
void Grammatical_Analyze::change_output_tabel(int change_state)
{
    output_quaternary_tabel.clear();
    QStack<QT> tmp_qt;
    QString find_string;//寻找块入口
    switch (change_state) {
    case 1:
        find_string="";
        break;
    case 2:
        find_string="if";
        break;
    case 3:
        find_string="if";
        break;
    case 4:
        find_string="while";
        break;
    default:
        find_string="";
        break;
    }
    for(int i=quaternary_tabel.size()-1;i>=0;--i)
    {
        if(quaternary_tabel.at(i).oprate_val==find_string)
        {
              break;
        }
        else
        {
            tmp_qt.push(quaternary_tabel.at(i));
            quaternary_tabel.remove(i);
        }
    }
    while(!tmp_qt.empty())
    {
        output_quaternary_tabel.push_back(tmp_qt.top());
        tmp_qt.pop();
    }
    /*for(int i=0;i<output_quaternary_tabel.size();++i)
    {
        qDebug()<<output_quaternary_tabel.at(i).oprate_val<<output_quaternary_tabel.at(i).first_val<<output_quaternary_tabel.at(i).second_val<<output_quaternary_tabel.at(i).tmp_val;
    }*/
}
