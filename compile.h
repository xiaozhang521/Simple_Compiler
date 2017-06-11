#ifndef COMPILE_H
#define COMPILE_H
#include<QString>
#include<QVector>
#include<QStack>
struct tokenType{
    int code;
    int num;
};
struct QT//四元式
{
    QString oprate_val;
    QString first_val,second_val;
    QString tmp_val;
};
struct SYNB
{
    QString name;
    int TYPE;
    int cat;
    int addr;
    int active_message;
};
struct optimize_node
{
    QString opreate;
    QVector<QString> node_name;
    QVector<int>father;
    int child1,child2;
};
struct AINFL
{
    int low;
    int up;
    int ctp;
    int clen;
};
class Symbol_Tabel
{
public:
    QVector<SYNB> s0;//标识符
    QVector<char>s1;//字符
    QVector<QString>s2;//字符串
    QVector<QString>s3;//常数
    QString s4[11]={"int","char","void","main","include","if","else","for","while","print","return"};//14
    QString s5[24]={"<=",">=","==","=","+","-","%","*","/","(",")","&","[","]","{","}","\"",";",",","'","|","<",">","#"};
    QVector<QPair<int,int> >type_table;//第一个数表示类型第二个数为数组表指针
    QVector<AINFL>arrry_tabel;
    int type_pointer;//type指针
    int array_pointer;//数组指针
    int start_offset;
    int get_address(QString query_string);
    int get_array_address(QString query_string,int number);
    int find_identifiter(QString string);
    int find_this_identifiter(QString string);
    void fill_start_offset();
    Symbol_Tabel();
    Symbol_Tabel* last_tabel;
};
class Lexical_Analyze
{
private:
    QString FileString;
    QString tmp;
    char ch;
    int state;
    int state_before;
    Symbol_Tabel symbol_tabel;
public:
    Lexical_Analyze();
    ~Lexical_Analyze();
    QVector<tokenType> token;//token序列表
    int change_state();
    void token_generate();
    void addtoken(int co);
    void addtoken(int co,int number);
    Symbol_Tabel return_tabel();
    void start();
    QVector<tokenType> return_token();
};
class Grammatical_Analyze
{
private:
    Symbol_Tabel tmp_symbol_tabel;
    QVector<tokenType> token;
    tokenType now_token;
    int token_tot;//记录TOKEN号
    tokenType get_next_token();
public:
    //Lexical_Analyze lexical;
    Grammatical_Analyze();//初始化
    QVector<int>erro_message;
    void lexical_start();
    bool grammatical_start();
private:
    int program();//程序
    int function();//函数
    int main_function();//主函数
    int return_type();//返回值类型
    int identifiter();//标识符
    int para_passing();//参数传递
    int varible_declar(Symbol_Tabel &symbol_tabel);//变量说明
    int compound_statement(Symbol_Tabel &symbol_tabel);//复合语句
    int constant();//常数
    int type();//类型
    int para_passing_table();//参数传递
    int same_definition_table(Symbol_Tabel &symbol_tabel);//同类定义
    int array_definition(Symbol_Tabel &symbol_tabel);//数组定义
    //int definition_table();//定义表
    //int assign_state_table();//赋值语句表
    int assign_state(Symbol_Tabel &symbol_tabel);//赋值语句
    int if_statement(Symbol_Tabel &symbol_tabel);//if语句
    int then_statement(Symbol_Tabel &symbol_tabel);//else语句
    int while_statement(Symbol_Tabel &symbol_tabel);//while语句
    int print_statement();//prrint语句
    //*********************算术部分*******************
    void output();
    void E();
    void F();
    void T();
    void G();
    //*********************END**********************
//*****************************语义分析**************************
private:
    void code_optimization_output(Symbol_Tabel &symbol_tabel, int option);
    void fill_table(Symbol_Tabel &symbol_tabel);
    void fill_type_table(Symbol_Tabel &symbol_tabel);//填写type表
    void fill_tmp_variable_tabel(Symbol_Tabel &symbol_tabel);
    void fill_array_tabel(Symbol_Tabel &symbol_tabel);
    int front;//1代表int,2代表char
    int cat_front;//记录CAT
    int addr_pionter;//记录地址指针
    //************************四元式*****************************
    QVector<QT>quaternary_tabel;
    QVector<QT>output_quaternary_tabel;
    QStack<QString>quaternary_stack;
    void QT_Arithmetic_output(char tmpc);//输出算术四元式
    void QT_equal_output();//输出=四元式
    void QT_if_output();//输出if四元式
    void QT_end_if_output(bool flag);//跳转距离填写
    void QT_program_output();//函数头标记
    void QT_else_output();//输出else四元式
    void QT_print_output();//输出print四元式
    void QT_array_equal_output();//输出数组四元式
    void QT_program_end_output();
    void change_output_tabel(int change_state);//生成输出四元式
    int middle_num;//标记中间变量
public:
    QStack<QString>jump_label;//目标代码暂存
    int label_val;
    QVector<QT> get_QT_tabel();
    //*********************************************************
//*************************************************************
//*****************************四元式优化***************************
private:
        QVector<optimize_node>DAG;
        bool node_vis[1000];//访问数组
        void insert();
        int node_number;
        void move_tabel_head(int tabel_number);
        void delete_node_name(QString delete_name,int delete_number);
        void recombine_QT();
        void dfs_DAG(int now_node);//深搜遍历图
//****************************************************************
public:
        void test(Symbol_Tabel symbol_tabel);
};
#endif // COMPILE_H
