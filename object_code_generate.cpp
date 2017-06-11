#include"object_code_generate.h"
#include<QDebug>
Object_Code_Generate::Object_Code_Generate(Symbol_Tabel &tmp_table)
{
    out_symbol_tabel=tmp_table;
    for(int i=0;i<4;++i)
        register_val[i]="";
    label_val=0;
}
void Object_Code_Generate::generate_QT_active_message(Grammatical_Analyze grammatical_analyze)
{
    QVector<QT>tmp_qt=grammatical_analyze.get_QT_tabel();
    jump_label=grammatical_analyze.jump_label;//跳转栈更新
    label_val=grammatical_analyze.label_val;
    QVector<SYNB>tmp_symbol_tabel=out_symbol_tabel.s0;
    for(int i=tmp_qt.size()-1;i>=0;--i)
    {
        new_QT new_qt;
        new_qt.first_val=tmp_qt.at(i).first_val;
        new_qt.second_val=tmp_qt.at(i).second_val;
        new_qt.oprate_val=tmp_qt.at(i).oprate_val;
        new_qt.tmp_val=tmp_qt.at(i).tmp_val;
        int number=out_symbol_tabel.find_this_identifiter(new_qt.first_val);
        if(number!=-1)
        {
            if(!tmp_symbol_tabel[number].active_message)
            {
                new_qt.fisrt_active_message=0;
                tmp_symbol_tabel[number].active_message=1;
            }
            else new_qt.fisrt_active_message=1;
        }
        else
        {
            number=out_symbol_tabel.find_identifiter(new_qt.first_val);
            if(number!=-1)
            {
                new_qt.fisrt_active_message=1;
            }
            else
            new_qt.fisrt_active_message=0;
        }
        number=out_symbol_tabel.find_this_identifiter(new_qt.second_val);
        if(number!=-1)
        {
            if(!tmp_symbol_tabel[number].active_message)
            {
                new_qt.second_active_message=0;
                tmp_symbol_tabel[number].active_message=1;
            }
            else new_qt.second_active_message=1;
        }
        else
        {
            number=out_symbol_tabel.find_identifiter(new_qt.second_val);
            if(number!=-1)
            {
                new_qt.second_active_message=1;
            }
            else
            new_qt.second_active_message=0;
        }
        number=out_symbol_tabel.find_this_identifiter(new_qt.tmp_val);
        if(number!=-1)
        {
            if(!tmp_symbol_tabel[number].active_message)
            {
                new_qt.tmp_active_message=0;
                tmp_symbol_tabel[number].active_message=1;
            }
            else new_qt.tmp_active_message=1;
        }
        else
        {
            number=out_symbol_tabel.find_identifiter(new_qt.tmp_val);
            if(number!=-1)
            {

                new_qt.tmp_active_message=1;
            }
            else new_qt.tmp_active_message=0;
        }
        finnal_QT_tabel.push_front(new_qt);
    }
}
QStack<QString> Object_Code_Generate::output_object_code()
{
    //QFile file("output.txt");
    QFile file("./output.asm");
    if (file.open(QFile::WriteOnly | QIODevice::Append))
    {
        QTextStream cout(&file);
        //******************初始化****************
        for(int i=0;i<finnal_QT_tabel.size();++i)
        {
            //**************特判区************
            if(finnal_QT_tabel.at(i).oprate_val=="start")
            {
                cout<<"start: MOV AX,MAIN\r\nMOV DS,AX\r\n";
                cout<<"MOV AX,SSEG\r\nMOV SS,AX\r\nMOV SP,SIZE STK\r\n";
                cout<<"MOV BP,0\r\n";
            }
            else if(finnal_QT_tabel.at(i).oprate_val=="end")
            {
                //**************基本块完成存数****************
                for(int i=0;i<4;++i)
                {
                    if(register_val[i]!="")
                    {
                        int tmp_address=out_symbol_tabel.get_address(register_val[i]);
                        if(tmp_address!=-1)
                        cout<<"MOV "<<"[BP+"<<tmp_address<<"],"<<get_register_name(i)<<"\r\n";
                        register_val[i]="";
                    }
                }
                cout<<"MOV AH,4CH"<<"\r\n";
                cout<<"INT 21H"<<"\r\n";
                //*****************************************
                cout<<"MAIN ENDS\r\n";
                cout<<"    END start";
            }
            else if(finnal_QT_tabel.at(i).oprate_val=="if")
            {
                QString register_name;//寄存器名
                int register_number=test_register_val(finnal_QT_tabel.at(i).first_val);//当前值是否在寄存器里
                if(register_number==-1)//不在寄存器
                {
                    int new_register_number=register_allocation(finnal_QT_tabel.at(i).first_val);
                    if(new_register_number==-1)//无空闲寄存器
                    {
                        new_register_number=0;
                        int tmp_address=out_symbol_tabel.get_address(register_val[0]);
                        cout<<"MOV "<<"[BP+"<<tmp_address<<"],"<<"AX"<<"\r\n";
                        register_val[new_register_number]=finnal_QT_tabel.at(i).first_val;
                    }
                    register_name=get_register_name(new_register_number);
                    register_number=new_register_number;
                    int tmp_address=out_symbol_tabel.get_address(finnal_QT_tabel.at(i).first_val);
                    cout<<"MOV "<<register_name+",";
                    if(tmp_address!=-1)
                    cout<<"[BP+"<<tmp_address<<"]\r\n";
                    else cout<<finnal_QT_tabel.at(i).first_val<<"\r\n";
                }
                else//在寄存器
                {
                    register_name=get_register_name(register_number);
                }
                cout<<"TEST "<<register_name+","<<register_name<<"\r\n";
                //**************基本块完成存数****************
                for(int i=0;i<4;++i)
                {
                    if(register_val[i]!="")
                    {
                        int tmp_address=out_symbol_tabel.get_address(register_val[i]);
                        if(tmp_address!=-1)
                        cout<<"MOV "<<"WORD PTR [BP+"<<tmp_address<<"],"<<get_register_name(i)<<"\r\n";
                        register_val[i]="";
                    }
                }
                QString jump_to_label="L"+QString::number(label_val,10);
                QString jump_to_label2="Jump"+QString::number(label_val,10);
                QString jump_to_label3="Notjump"+QString::number(label_val,10);
                label_val++;
                cout<<"JZ "<<jump_to_label2<<"\r\n";
                cout<<"JMP "<<jump_to_label3<<"\r\n";
                cout<<jump_to_label2<<":"<<"JMP "<<jump_to_label<<"\r\n";
                cout<<jump_to_label3<<":\r\n";
                jump_label.push(jump_to_label);
            }
            else if(finnal_QT_tabel.at(i).oprate_val=="el")
            {
                //**************基本块完成存数****************
                for(int i=0;i<4;++i)
                {
                    if(register_val[i]!="")
                    {
                        int tmp_address=out_symbol_tabel.get_address(register_val[i]);
                        if(tmp_address!=-1)
                        cout<<"MOV "<<"WORD PTR [BP+"<<tmp_address<<"],"<<get_register_name(i)<<"\r\n";
                        register_val[i]="";
                    }
                }
                QString jump_to_label="L"+QString::number(label_val,10);
                label_val++;
                cout<<"JMP "<<jump_to_label<<"\r\n";
                if(!jump_label.empty())
                {
                    cout<<jump_label.top()<<":"<<"\r\n";
                    jump_label.pop();
                }
                //else错误
                jump_label.push(jump_to_label);
            }
            else if(finnal_QT_tabel.at(i).oprate_val=="ie")
            {
                if(!jump_label.empty())
                {
                    cout<<jump_label.top()<<":"<<"\r\n";
                    jump_label.pop();
                }
                //else错误
            }
            else if(finnal_QT_tabel.at(i).oprate_val=="=[]")
            {
                QString register_name;//寄存器名
                int register_number=test_register_val(finnal_QT_tabel.at(i).tmp_val);//当前值是否在寄存器里
                if(register_number==-1)//不在寄存器
                {
                    int new_register_number=register_allocation(finnal_QT_tabel.at(i).tmp_val);
                    if(new_register_number==-1)//无空闲寄存器
                    {
                        new_register_number=0;
                        int tmp_address=out_symbol_tabel.get_address(register_val[0]);
                        cout<<"MOV "<<"WORD PTR [BP+"<<tmp_address<<"],"<<"AX"<<"\r\n";
                        register_val[new_register_number]=finnal_QT_tabel.at(i).tmp_val;
                    }
                    register_name=get_register_name(new_register_number);
                    register_number=new_register_number;
                    int tmp_address=out_symbol_tabel.get_address(finnal_QT_tabel.at(i).tmp_val);
                    cout<<"MOV "<<register_name+",";
                    if(tmp_address!=-1)
                    cout<<"[BP+"<<tmp_address<<"]\r\n";
                    else cout<<finnal_QT_tabel.at(i).tmp_val<<"\r\n";
                }
                else//在寄存器
                {
                    register_name=get_register_name(register_number);
                }
                bool ok;
                int array_number=finnal_QT_tabel.at(i).second_val.toInt(&ok,10);
                int tmp_address=out_symbol_tabel.get_array_address(finnal_QT_tabel.at(i).first_val,array_number);
                if(tmp_address!=-1)
                cout<<"MOV "<<register_name<<",[BP+"<<tmp_address<<"]\r\n";
            }
            //**************************************
            else
            {
                QString register_name;//寄存器名
                int register_number=test_register_val(finnal_QT_tabel.at(i).first_val);//当前值是否在寄存器里
                if(register_number==-1)//不在寄存器
                {
                    int new_register_number=register_allocation(finnal_QT_tabel.at(i).first_val);
                    if(new_register_number==-1)//无空闲寄存器
                    {
                        new_register_number=0;
                        int tmp_address=out_symbol_tabel.get_address(register_val[0]);
                        cout<<"MOV "<<"WORD PTR [BP+"<<tmp_address<<"],"<<"AX"<<"\r\n";
                        register_val[new_register_number]=finnal_QT_tabel.at(i).first_val;
                    }
                    register_name=get_register_name(new_register_number);
                    register_number=new_register_number;
                    int tmp_address=out_symbol_tabel.get_address(finnal_QT_tabel.at(i).first_val);
                    cout<<"MOV "<<register_name+",";
                    if(tmp_address!=-1)
                    cout<<"[BP+"<<tmp_address<<"]\r\n";
                    else cout<<finnal_QT_tabel.at(i).first_val<<"\r\n";
                }
                else//在寄存器
                {
                    register_name=get_register_name(register_number);
                }
                if(finnal_QT_tabel.at(i).oprate_val=="+"||finnal_QT_tabel.at(i).oprate_val=="-")
                {

                    if(finnal_QT_tabel.at(i).fisrt_active_message&&finnal_QT_tabel.at(i).first_val!=finnal_QT_tabel.at(i).tmp_val)
                    {
                        cout<<"MOV ";
                        int tmp_address=out_symbol_tabel.get_address(finnal_QT_tabel.at(i).first_val);
                        if(tmp_address!=-1)
                        cout<<"WORD PTR [BP+"<<tmp_address<<"]";//地址
                        else cout<<finnal_QT_tabel.at(i).first_val;//立即数
                        cout<<","+register_name<<"\r\n";
                    }
                    if(finnal_QT_tabel.at(i).oprate_val=="+")
                        cout<<"ADD";
                    else
                        cout<<"SUB";
                    cout<<" "<<register_name<<",";
                    int second_register_number=test_register_val(finnal_QT_tabel.at(i).second_val);
                    if(second_register_number==-1)
                    {
                        int tmp_address=out_symbol_tabel.get_address(finnal_QT_tabel.at(i).second_val);
                        if(tmp_address!=-1)
                        cout<<"[BP+"<<tmp_address<<"]\r\n";
                        else cout<<finnal_QT_tabel.at(i).second_val<<"\r\n";
                    }
                    else
                    {
                        QString second_register_name=get_register_name(second_register_number);
                        cout<<second_register_name<<"\r\n";
                    }
                    register_val[register_number]=finnal_QT_tabel.at(i).tmp_val;
                }
                else if(finnal_QT_tabel.at(i).oprate_val=="*")
                {
                    if(register_name!="AX")//将乘数1放入AX
                    {
                        cout<<"MOV "<<register_val[0]<<","<<"AX"<<"\r\n";
                        cout<<"MOV "<<"AX"<<","<<register_name<<"\r\n";//AX会被更新成最后的数
                    }
                    else cout<<"MOV "<<finnal_QT_tabel.at(i).first_val<<"z"<<register_name<<"\r\n";//存AX寄存器里的值
                    if('0'<=finnal_QT_tabel.at(i).second_val.at(0)&&finnal_QT_tabel.at(i).second_val.at(0)<='9')//立即数
                    {
                        int second_register_number=register_allocation(finnal_QT_tabel.at(i).second_val);
                        if(second_register_number==-1)
                        {
                            cout<<"MOV "<<register_val[1]<<"BX"<<"\r\n";
                            register_val[1]=finnal_QT_tabel.at(i).second_val;
                            second_register_number=1;
                        }
                        QString second_register_name=get_register_name(second_register_number);
                        cout<<"MOV "<<second_register_name<<finnal_QT_tabel.at(i).second_val<<"\r\n";
                        cout<<"MUL "<<second_register_name;
                        continue;
                    }
                    cout<<"MUL "<<finnal_QT_tabel.at(i).second_val<<"\r\n";
                    register_val[0]=finnal_QT_tabel.at(i).tmp_val;//最后一个数
                }
                else if(finnal_QT_tabel.at(i).oprate_val=="/")
                {
                    cout<<"DIV";
                }
                else if(finnal_QT_tabel.at(i).oprate_val=="<"||finnal_QT_tabel.at(i).oprate_val==">"||finnal_QT_tabel.at(i).oprate_val=="<="||finnal_QT_tabel.at(i).oprate_val==">="||finnal_QT_tabel.at(i).oprate_val=="==")
                {
                    cout<<"CMP "<<register_name<<",";
                    int tmp_address=out_symbol_tabel.get_address(finnal_QT_tabel.at(i).second_val);
                    if(tmp_address!=-1)
                    cout<<"[BP+"<<tmp_address<<"]"<<"\r\n";
                    else
                    cout<<finnal_QT_tabel.at(i).second_val<<"\r\n";
                    //*******赋初值1，为真********
                    tmp_address=out_symbol_tabel.get_address(finnal_QT_tabel.at(i).tmp_val);
                    if(tmp_address!=-1)
                    cout<<"MOV "<<"WORD PTR [BP+"<<tmp_address<<"]"<<",1"<<"\r\n";
                    //else错误
                    //*************************
                    //**********真跳转***********
                    QString jump_to_label="L"+QString::number(label_val,10);
                    label_val++;
                    if(finnal_QT_tabel.at(i).oprate_val=="<")//jb<,jbe<=,ja>,jae>=,je==
                    {
                        cout<<"JB "<<jump_to_label<<"\r\n";
                    }
                    else if(finnal_QT_tabel.at(i).oprate_val=="<=")
                    {
                        cout<<"JBE "<<jump_to_label<<"\r\n";
                    }
                    else if(finnal_QT_tabel.at(i).oprate_val==">")
                    {
                        cout<<"JA "<<jump_to_label<<"\r\n";
                    }
                    else if(finnal_QT_tabel.at(i).oprate_val==">=")
                    {
                        cout<<"JAE "<<jump_to_label<<"\r\n";
                    }
                    else if(finnal_QT_tabel.at(i).oprate_val=="==")
                    {
                        cout<<"JE "<<jump_to_label<<"\r\n";
                    }
                    cout<<"MOV "<<"WORD PTR [BP+"<<tmp_address<<"]"<<",0"<<"\r\n";
                    cout<<jump_to_label<<": "<<"\r\n";
                    //
                }
                else if(finnal_QT_tabel.at(i).oprate_val=="[]=")
                {
                    bool ok;
                    int array_number=finnal_QT_tabel.at(i).second_val.toInt(&ok,10);
                    int tmp_address=out_symbol_tabel.get_array_address(finnal_QT_tabel.at(i).tmp_val,array_number);
                    if(tmp_address!=-1)
                    {
                        cout<<"MOV "<<"[BP+"<<tmp_address<<"],"<<register_name<<"\r\n";
                    }
                }
                else if(finnal_QT_tabel.at(i).oprate_val=="=")
                {
                    if(!finnal_QT_tabel.at(i).fisrt_active_message)
                    {
                        register_val[register_number]=finnal_QT_tabel.at(i).tmp_val;//寄存器值不活跃，把当前寄存器给新值
                    }
                    else
                    {
                        int new_register_number=register_allocation(finnal_QT_tabel.at(i).first_val);
                        if(new_register_number==-1)//无空闲寄存器
                        {
                            new_register_number=0;
                            int tmp_address=out_symbol_tabel.get_address(register_val[0]);
                            if(tmp_address!=-1)
                            cout<<"MOV [BP+"<<tmp_address<<"]"<<","<<"AX"<<"\r\n";
                            //立即数省略
                            if(register_name!="AX")
                            cout<<"MOV "<<"AX"<<","<<register_name<<"\r\n";
                        }
                        else
                        {
                            cout<<"MOV "<<get_register_name(new_register_number)<<","<<register_name<<"\r\n";
                        }
                        register_val[new_register_number]=finnal_QT_tabel.at(i).tmp_val;
                    }
                }
                else if(finnal_QT_tabel.at(i).oprate_val=="print")
                {
                    //***********保护寄存器值***********
                    if(register_val[1]!="")
                    {
                        if(register_name!="BX")
                        {
                            int tmp_address=out_symbol_tabel.get_address(register_val[1]);
                            if(tmp_address!=-1)
                            cout<<"MOV [BP+"<<tmp_address<<"],"<<"BX"<<"\r\n";
                            //立即数省略
                        }
                    }
                    if(register_name!="BX")
                    cout<<"MOV "<<"BX,"<<register_name<<"\r\n";//将要输出的值送到BX
                    register_val[1]=finnal_QT_tabel.at(i).first_val;
                    if(register_val[0]!="")
                    {
                        if(register_name!="AX")
                        {
                            int tmp_address=out_symbol_tabel.get_address(register_val[0]);
                            if(tmp_address!=-1)
                            {cout<<"MOV [BP+"<<tmp_address<<"],"<<"AX"<<"\r\n";}
                            //立即数省略
                        }register_val[0]="";
                    }
                    if(register_val[3]!="")
                    {
                        if(register_name!="DX")
                        {
                            int tmp_address=out_symbol_tabel.get_address(register_val[3]);
                            if(tmp_address!=-1)
                            cout<<"MOV [BP+"<<tmp_address<<"],"<<"DX"<<"\r\n";
                            //立即数省略
                        }register_val[3]="";
                    }
                    QString jump_to_label1="ZEROJUMP"+QString::number(label_val,10);
                    label_val++;
                    QString jump_to_label2="ZEROJUMP"+QString::number(label_val,10);
                    label_val++;
                    QString jump_to_label3="ZEROJUMP"+QString::number(label_val,10);
                    label_val++;
                    QString jump_to_label4="CHARJUMP"+QString::number(label_val,10);
                    label_val++;
                    QString jump_to_label5="CHARJUMP"+QString::number(label_val,10);
                    label_val++;
                    QString jump_to_label6="CHARJUMP"+QString::number(label_val,10);
                    label_val++;
                    QString jump_to_label7="CHARJUMP"+QString::number(label_val,10);
                    label_val++;
                    //************16进制输出**************
                    cout<<"MOV DH,BH\r\n";
                    cout<<"MOV DL,DH\r\n";
                    cout<<"PUSH CX\r\n";
                    cout<<"MOV CL,4\r\n";
                    cout<<"SHR DL,CL\r\n";//最高位
                    cout<<"POP CX\r\n";
                    cout<<"TEST DL,DL\r\n";
                    cout<<"JZ "<<jump_to_label1<<"\r\n";
                    cout<<"ADD DL,30H\r\n";
                    cout<<"CMP DL,3AH\r\n";
                    cout<<"JB  "<<jump_to_label4<<"\r\n";
                    cout<<"ADD DL,7\r\n"<<jump_to_label4<<":\r\n";//是否超过9
                    cout<<"MOV AH,02H\r\n";
                    cout<<"INT 21H\r\n";
                    cout<<jump_to_label1<<": \r\nMOV DL,DH\r\n";
                    cout<<"AND DL,0FH\r\n";
                    cout<<"JZ "<<jump_to_label2<<"\r\n";
                    cout<<"ADD DL,30H\r\n";
                    cout<<"CMP DL,3AH\r\n";
                    cout<<"JB  "<<jump_to_label5<<"\r\n";
                    cout<<"ADD DL,7\r\n"<<jump_to_label5<<":\r\n";//是否超过9
                    cout<<"MOV AH,02H\r\n";
                    cout<<"INT 21H\r\n";//次高位
                    cout<<jump_to_label2<<": \r\nMOV DH,BL\r\n";
                    cout<<"MOV DL,DH\r\n";
                    cout<<"PUSH CX\r\n";
                    cout<<"MOV CL,4\r\n";
                    cout<<"SHR DL,CL\r\n";//最高位
                    cout<<"POP CX\r\n";
                    cout<<"TEST DL,DL\r\n";
                    cout<<"JZ "<<jump_to_label3<<"\r\n";
                    cout<<"ADD DL,30H\r\n";
                    cout<<"CMP DL,3AH\r\n";
                    cout<<"JB  "<<jump_to_label6<<"\r\n";
                    cout<<"ADD DL,7\r\n"<<jump_to_label6<<":\r\n";//是否超过9
                    cout<<"MOV AH,02H\r\n";
                    cout<<"INT 21H\r\n";
                    cout<<jump_to_label3<<":\r\n MOV DL,DH\r\n";
                    cout<<"AND DL,0FH\r\n";
                    cout<<"ADD DL,30H\r\n";
                    cout<<"CMP DL,3AH\r\n";
                    cout<<"JB  "<<jump_to_label7<<"\r\n";
                    cout<<"ADD DL,7\r\n"<<jump_to_label7<<":\r\n";//是否超过9
                    cout<<"MOV AH,02H\r\n";
                    cout<<"INT 21H\r\n";//次高位
                    cout<<"MOV DL,0AH\r\n";
                    cout<<"MOV AH,02H\r\n";
                    cout<<"INT 21H\r\n";//次高位
                }
                release_register(finnal_QT_tabel.at(i));
            }
        }
        //**************基本块完成存数****************
        for(int i=0;i<4;++i)
        {
            if(register_val[i]!="")
            {
                int tmp_address=out_symbol_tabel.get_address(register_val[i]);
                if(tmp_address!=-1)
                cout<<"MOV "<<"WORD PTR [BP+"<<tmp_address<<"],"<<get_register_name(i)<<"\r\n";
                register_val[i]="";
            }
        }
    }
    return jump_label;
}
int Object_Code_Generate::register_allocation(QString new_val)//寄存器分配更新
{
    for(int i=0;i<4;++i)
    {
        if(register_val[i]=="")
        {
            register_val[i]=new_val;
            return i;
        }
    }//寄存器都不为空
    return -1;
}
int Object_Code_Generate::test_register_val(QString find_stirng)//测试是否在寄存器
{
    for(int i=0;i<4;++i)
    {
        if(register_val[i]==find_stirng)
        {
            return i;
        }
    }
    return -1;
}
QString Object_Code_Generate::get_register_name(int number)
{
    switch (number) {
    case 0:
        return "AX";
        break;
    case 1:
        return "BX";
        break;
    case 2:
        return "CX";
        break;
    case 3:
        return "DX";
        break;
    default:
        break;
    }
    return "";
}
void Object_Code_Generate::release_register(new_QT test)
{
    if(test.first_val!="_"&&!test.fisrt_active_message)
    {
        int tmp_number=test_register_val(test.first_val);
        if(tmp_number!=-1)
        {
            register_val[tmp_number]="";
        }
    }
    else if(test.second_val!="_"&&!test.second_active_message)
    {
        int tmp_number=test_register_val(test.second_val);
        if(tmp_number!=-1)
        {
            register_val[tmp_number]="";
        }
    }
    else if(test.tmp_val!="_"&&!test.tmp_active_message)
    {
        int tmp_number=test_register_val(test.tmp_val);
        if(tmp_number!=-1)
        {
            register_val[tmp_number]="";
        }
    }
}
int Object_Code_Generate::return_label_val()
{
    return label_val;
}
