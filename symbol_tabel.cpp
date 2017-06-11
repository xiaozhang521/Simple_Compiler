#include"compile.h"
int Symbol_Tabel::find_identifiter(QString string)
{
    for(int i=0;i<s0.size();++i)
    {
        if(s0.at(i).name==string)
        {
            return i;
        }
    }
    if(last_tabel!=NULL)
    {
        int tmp=last_tabel->find_identifiter(string);
        if(tmp==-1) return -1;
        else return tmp;
    }
    return -1;
}
int Symbol_Tabel::find_this_identifiter(QString string)
{
    for(int i=0;i<s0.size();++i)
    {
        if(s0.at(i).name==string)
        {
            return i;
        }
    }
    return -1;
}
int Symbol_Tabel::get_address(QString query_string)
{
    int offset=start_offset;
    for(int i=0;i<s0.size();++i)
    {
        if(s0.at(i).name==query_string)
            return offset;
       else  offset+=s0.at(i).addr;
    }
    if(last_tabel!=NULL)
    offset=last_tabel->get_address(query_string);
    else offset=-1;
    return offset;
}
int Symbol_Tabel::get_array_address(QString query_string,int number)
{
    int offset=start_offset;
    for(int i=0;i<s0.size();++i)
    {
        if(s0.at(i).name==query_string)
        {
            int type=s0.at(i).TYPE;
            offset+=number*arrry_tabel.at(type_table.at(type).second).ctp;//得出每个偏移量
            return offset;
        }
       else  offset+=s0.at(i).addr;
    }
    if(last_tabel!=NULL)
    offset=last_tabel->get_address(query_string);
    else offset=-1;
    return offset;
}
void Symbol_Tabel::fill_start_offset()
{
    if(last_tabel!=NULL)
    {
        int tmp_tot=last_tabel->start_offset;
        for(int i=0;i<last_tabel->s0.size();++i)
        {
            tmp_tot+=last_tabel->s0.at(i).addr;
        }
        start_offset=tmp_tot;
    }
    else start_offset=0;
}
Symbol_Tabel::Symbol_Tabel()
{
    last_tabel=NULL;
    type_pointer=0;
    array_pointer=0;
    start_offset=0;
}
