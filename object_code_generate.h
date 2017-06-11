#ifndef OBJECT_CODE_GENERATE_H
#define OBJECT_CODE_GENERATE_H
#include<QVector>
#include<QTextStream>
#include<QFile>
#include"compile.h"
struct new_QT
{
    QString oprate_val;
    bool fisrt_active_message;
    bool second_active_message;
    bool tmp_active_message;
    QString first_val,second_val;
    QString tmp_val;
};
class Object_Code_Generate
{
private:
    QVector<new_QT>finnal_QT_tabel;
    Symbol_Tabel out_symbol_tabel;
    QString register_val[4];//AX,BX,CX,DX
    QStack<QString>jump_label;
    int label_val;
public:
    Object_Code_Generate(Symbol_Tabel &tmp_table);
    void generate_QT_active_message(Grammatical_Analyze grammatical_analyze);
    int test_register_val(QString find_stirng);
    QString get_register_name(int number);
    int register_allocation(QString new_val);
    QStack< QString> output_object_code();
    void release_register(new_QT test);
    int return_label_val();
};
#endif // OBJECT_CODE_GENERATE_H
