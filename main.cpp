#include <QCoreApplication>
#include<compile.h>
#include"object_code_generate.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Grammatical_Analyze grammatical_analyze;
    grammatical_analyze.lexical_start();
    grammatical_analyze.grammatical_start();
    /*Object_Code_Generate object_code_generate;
    object_code_generate.generate_QT_active_message();
    object_code_generate.output_object_code();*/
    return a.exec();
}
