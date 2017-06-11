#include"compile.h"
#include<QDebug>
void Grammatical_Analyze::fill_table(Symbol_Tabel &symbol_tabel)
{
    int tmp=now_token.num;
    SYNB now=tmp_symbol_tabel.s0.at(tmp);
    fill_type_table(symbol_tabel);
    now.TYPE=symbol_tabel.type_pointer++;
    now.cat=cat_front;
    if(front==1&&now.cat==5)
    {
        now.addr=2;
    }
    else if(front==2&&now.cat==5)
    {
        now.addr=1;
    }
    now.active_message=1;
    symbol_tabel.s0.push_back(now);
}
void Grammatical_Analyze::fill_type_table(Symbol_Tabel& symbol_tabel)
{
    if(front==1||front==2)
    {
        symbol_tabel.type_table.push_back(qMakePair(front,-1));
    }
}
void Grammatical_Analyze::fill_tmp_variable_tabel(Symbol_Tabel &symbol_tabel)
{
    for(int i=0;i<output_quaternary_tabel.size();++i)
    {
        QString test_identifiter=output_quaternary_tabel.at(i).tmp_val;
        if(test_identifiter=="_") continue;
        if('0'<=test_identifiter.at(0).unicode()&&test_identifiter.at(0).unicode()<='9') continue;
        if(symbol_tabel.find_identifiter(test_identifiter)==-1)
        {
            SYNB new_val;
            new_val.addr=2;
            front=1;
            fill_type_table(symbol_tabel);
            new_val.TYPE=symbol_tabel.type_pointer++;
            new_val.name=test_identifiter;
            new_val.cat=5;
            new_val.active_message=0;
            symbol_tabel.s0.push_back(new_val);
        }
    }
}
void Grammatical_Analyze::fill_array_tabel(Symbol_Tabel &symbol_tabel)
{
    int nowpoint=symbol_tabel.s0.size()-1;
    int tmp_type=symbol_tabel.s0[nowpoint].TYPE;
    symbol_tabel.type_table[tmp_type].second=symbol_tabel.array_pointer++;
    AINFL new_array;
    new_array.low=0;
    bool ok;
    new_array.up=tmp_symbol_tabel.s3.at(now_token.num).toInt(&ok,10)-1;
    if(front==1)
    new_array.clen=2;
    else if(front==2)
    new_array.clen=1;
    fill_type_table(symbol_tabel);
    new_array.ctp=symbol_tabel.type_pointer++;
    symbol_tabel.arrry_tabel.push_back(new_array);
    symbol_tabel.s0[nowpoint].addr=(new_array.up-new_array.low+1)*new_array.clen;
}
