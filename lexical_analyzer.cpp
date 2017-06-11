#include"compile.h"
#include<QFile>
#include<QDebug>
#include<iostream>
using namespace std;

Lexical_Analyze::Lexical_Analyze()
{
    FileString="./input.txt";
}
Lexical_Analyze::~Lexical_Analyze()
{
    printf("\nlexical finish\n");
}
void Lexical_Analyze::addtoken(int co)
{
    tokenType tmp;
    tmp.code=co;
    token.push_back(tmp);
}
void Lexical_Analyze::addtoken(int co, int number)
{
    tokenType tmp;
    tmp.code=co;
    tmp.num=number;
    //printf("||%d||",number);
    token.push_back(tmp);
}
Symbol_Tabel Lexical_Analyze::return_tabel()
{
    return symbol_tabel;
}
int Lexical_Analyze::change_state()
{
    if(state==1)
    {
        if(ch==' '||ch=='\n')
            return 1;
        else if(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z'))
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 2;
        }
        else if('0'<=ch&&ch<='9')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 3;
        }
        else if(ch=='\'')
        {
            return 9;
        }
        else if(ch=='"')
        {
            return 11;
        }
        else if(ch=='>')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 4;
        }
        else if(ch=='<')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 5;
        }
        else if(ch=='=')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 6;
        }
        else {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 18;
        }
    }
    else if(state==2)
    {
        if(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z')||('0'<=ch&&ch<='9'))
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 2;
        }
        else
        {
            int flag=0;
            for(int i=0;i<24;++i)
            {
                if(symbol_tabel.s5[i][0]==ch)  flag=1;
            }
            if(flag||ch==' '||ch=='\n')
                return 0;
            else return -1;//´íÎó
        }
    }
    else if(state==3)
    {
        if('0'<=ch&&ch<='9')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 3;
        }
        else if(ch=='.')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 8;
        }
        else
        {
            int flag=0;
            for(int i=0;i<24;++i)
            {
                if(symbol_tabel.s5[i][0]==ch)  flag=1;
            }
            if(flag||ch==' '||ch=='\n')
                return 0;
            else return -1;//´íÎó
        }
    }
    else if(state==4)
    {
        if(ch=='=')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 13;
        }
        else
        {
            int flag=0;
            for(int i=0;i<24;++i)
            {
                if(symbol_tabel.s5[i][0]==ch)  flag=1;
            }
            if(flag||ch==' '||ch=='\n'||(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z'))||('0'<=ch&&ch<='9'))
                return 0;
            else return -1;//´íÎó
        }
    }
    else if(state==5)
    {
        if(ch=='=')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 14;
        }
        else
        {
            int flag=0;
            for(int i=0;i<24;++i)
            {
                if(symbol_tabel.s5[i][0]==ch)  flag=1;
            }
            if(flag||ch==' '||ch=='\n'||(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z'))||('0'<=ch&&ch<='9'))
                return 0;
            else return -1;//´íÎó
        }
    }
    else if(state==6)
    {
        if(ch=='=')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 15;
        }
        else
        {
            int flag=0;
            for(int i=0;i<24;++i)
            {
                if(symbol_tabel.s5[i][0]==ch)  flag=1;
            }
            if(flag||ch==' '||ch=='\n'||(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z'))||('0'<=ch&&ch<='9'))
                return 0;
            else return -1;//´íÎó
        }
    }
    else if(state==7)
    {
        if('0'<=ch&&ch<='9')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 7;
        }
        else
        {
            int flag=0;
            for(int i=0;i<24;++i)
            {
                if(symbol_tabel.s5[i][0]==ch)  flag=1;
            }
            if(flag||ch==' '||ch=='\n')
                return 0;
            else return -1;//异常
        }
    }
    else if(state==8)
    {
        if('0'<=ch&&ch<='9')
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 7;
        }
        else return -1;//异常
    }
    else if(state==9)
    {
        if(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z')||('0'<=ch&&ch<='9'))
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 10;
        }
        else return -1;//异常
    }
    else if(state==10)
    {
        if(ch=='\'') return 16;
        else return -1;
    }
    else if(state==11)
    {
        if(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z')||('0'<=ch&&ch<='9'))
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 12;
        }
        else return -1;
    }
    else if(state==12)
    {
        if(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z')||('0'<=ch&&ch<='9'))
        {
            char X[2]={ch,0};
            QString S(X);
            tmp=tmp+S;
            return 12;
        }
        else if(ch=='"')
        {
            return 17;
        }
        else return -1;
    }
    else if(state==13)
    {
        int flag=0;
        for(int i=0;i<24;++i)
        {
            if(symbol_tabel.s5[i][0]==ch)  flag=1;
        }
        if(flag||ch==' '||ch=='\n'||(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z'))||('0'<=ch&&ch<='9'))
            return 0;
        else return -1;//´íÎó
    }
    else if(state==14)
    {
        int flag=0;
        for(int i=0;i<24;++i)
        {
            if(symbol_tabel.s5[i][0]==ch)  flag=1;
        }
        if(flag||ch==' '||ch=='\n'||(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z'))||('0'<=ch&&ch<='9'))
            return 0;
        else return -1;//´íÎó
    }
    else if(state==15)
    {
        int flag=0;
        for(int i=0;i<24;++i)
        {
            if(symbol_tabel.s5[i][0]==ch)  flag=1;
        }
        if(flag||ch==' '||ch=='\n'||(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z'))||('0'<=ch&&ch<='9'))
            return 0;
        else return -1;//´íÎó
    }
    else if(state==16)
    {
        int flag=0;
        for(int i=0;i<24;++i)
        {
            if(symbol_tabel.s5[i][0]==ch)  flag=1;
        }
        if(flag||ch==' '||ch=='\n')
            return 0;
        else return -1;//异常
    }
    else if(state==17)
    {
        int flag=0;
        for(int i=0;i<24;++i)
        {
            if(symbol_tabel.s5[i][0]==ch)  flag=1;
        }
        if(flag||ch==' '||ch=='\n')
            return 0;
        else return -1;//异常
    }
    else if(state==18)
    {
        int flag=0;
        for(int i=0;i<24;++i)
        {
            if(symbol_tabel.s5[i][0]==ch)  flag=1;
        }
        if(flag||ch==' '||ch=='\n'||(('a'<=ch&&ch<='z')||('A'<=ch&&ch<='Z'))||('0'<=ch&&ch<='9'))
            return 0;
        else return -1;//´íÎó
    }
    return -1;
}
void Lexical_Analyze::token_generate()
{
    if(state==0)
    {
        if(state_before==2)
        {
            //qDebug()<<tmp;
            int flag=1;
            for(int i=0;i<11;++i)
            {
                if(symbol_tabel.s4[i]==tmp) {printf("<%d> ",i+4);addtoken(i+4);
                    flag=0;break;}//关键字查找
            }
            if(flag) {
                int flag2=1;
                int number=symbol_tabel.s0.size();
                for(int j=0;j<symbol_tabel.s0.size();++j)//自定义表
                {
                    if(symbol_tabel.s0[j].name==tmp)
                    {
                        flag2=0;
                        number=j;
                        break;
                    }
                }
                if(flag2)
                {
                    SYNB new_partner;
                    new_partner.name=tmp;
                    new_partner.addr=-1;
                    new_partner.cat=-1;
                    new_partner.TYPE=-1;
                    symbol_tabel.s0.push_back(new_partner);
                }
                printf("<00> ");
                addtoken(0,number);
            }
        }
        else if(state_before==3)
        {
            //cout<<tmp;
            int flag=1;
            int number=symbol_tabel.s3.size();
            for(int j=0;j<symbol_tabel.s3.size();++j)
            {
                if(symbol_tabel.s3[j]==tmp)
                {
                    flag=0;
                    number=j;
                    break;
                }
            }
            if(flag) symbol_tabel.s3.push_back(tmp);
            printf("<03> ");
            addtoken(3,number);
         }
         else if(state_before==4)
         {
             //cout<<tmp;
             for(unsigned int j=0;j<24;++j)
                if(tmp==symbol_tabel.s5[j])
                {
                    printf("<%d>",15+j);addtoken(15+j);
                }
         }
         else if(state_before==5)
         {
             //cout<<tmp;
             for(unsigned int j=0;j<24;++j)
                if(tmp==symbol_tabel.s5[j])
                {
                    printf("<%d>",15+j);addtoken(15+j);
                }
         }
         else if(state_before==6)
         {
             //cout<<tmp;
             for(unsigned int j=0;j<24;++j)
                if(tmp==symbol_tabel.s5[j])
                {
                    printf("<%d>",15+j);addtoken(15+j);
                }
         }
         else if(state_before==7)
         {
             //cout<<tmp;
             int flag=1;
             for(int j=0;j<symbol_tabel.s3.size();++j)
             {
                if(symbol_tabel.s3[j]==tmp)
                {
                    flag=0;
                    break;
                }
             }
             if(flag) symbol_tabel.s3.push_back(tmp);
                printf("<03> ");
             addtoken(3);
         }
         else if(state_before==8)
         {

         }
         else if(state_before==9)
         {

         }
         else if(state_before==10)
         {

         }
         else if(state_before==11)
         {

         }
         else if(state_before==12)
         {

         }
         else if(state_before==13)
         {
            //cout<<tmp;
            for(unsigned int j=0;j<24;++j)
                if(tmp==symbol_tabel.s5[j])
                {
                    printf("<%d>",15+j);addtoken(15+j);
                }
         }
         else if(state_before==14)
         {
            //cout<<tmp;
            for(unsigned int j=0;j<24;++j)
                if(tmp==symbol_tabel.s5[j])
                {
                    printf("<%d>",15+j);addtoken(15+j);
                }
         }
         else if(state_before==15)
         {
            //cout<<tmp;
            for(unsigned int j=0;j<24;++j)
                if(tmp==symbol_tabel.s5[j])
                {
                    printf("<%d>",15+j);addtoken(15+j);
                }
         }
         else if(state_before==16)
         {
            //cout<<tmp;
            int flag=1;
            int number=symbol_tabel.s1.size();
            for(int j=0;j<symbol_tabel.s1.size();++j)
            {
                if(symbol_tabel.s1[j]==tmp[0])
                {
                    flag=0;
                    number=j;
                    break;
                }
            }
            if(flag) symbol_tabel.s1.push_back(tmp[0].unicode());
                printf("<01> ");
            addtoken(1,number);
         }
         else if(state_before==17)
         {
            //cout<<tmp;
            int flag=1;
            int number=symbol_tabel.s1.size();
            for(int j=0;j<symbol_tabel.s2.size();++j)
            {
                if(symbol_tabel.s2[j]==tmp)
                {
                    flag=0;
                    number=j;
                    break;
                }
            }
            if(flag) symbol_tabel.s2.push_back(tmp);
                printf("<02> ");
            addtoken(2,number);
         }
         else if(state_before==18)
         {
            //cout<<tmp;
            for(unsigned int j=0;j<24;++j)
            {
                if(symbol_tabel.s5[j]==tmp)
                {
                    printf("<%d> ",15+j);addtoken(15+j);
                }
            }
         }
         tmp.clear();
         state=1;
         state_before=state;
         state=change_state();
    }
    else if(state<0)
    {
        printf("erro\n");
        tmp.clear();
        state=1;
        state_before=state;
        state=change_state();
    }
}
void Lexical_Analyze::start()
{
    QFile file(FileString);
    if( ! file.open( QIODevice::ReadOnly))
    {
        printf("open file erro!");
        return;
    }
    else
    {
        state=1;//初始态
        while(!file.atEnd())
        {
            file.getChar(&ch);
            if(ch=='\r')
            continue;
            state_before=state;
            state=change_state();
            if(state<=0)
            token_generate();
        }
    }
    /*for(int i=0;i<token.size();++i)
    {
        printf("%d ",token.at(i).code);
    }*/
}
QVector<tokenType> Lexical_Analyze::return_token()
{
    return token;
}
