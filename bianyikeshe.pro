QT += core
QT -= gui

CONFIG += c++11

TARGET = bianyikeshe
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    lexical_analyzer.cpp \
    arithmetic_expression.cpp \
    grammatical_analyze_innner.cpp \
    grammatical_analyze.cpp \
    grammatical_analyze_fun.cpp \
    semantic_analyze.cpp \
    quaternary_tabel_output.cpp \
    code_optimization.cpp \
    object_code_generate.cpp \
    symbol_tabel.cpp

HEADERS += \
    compile.h \
    prefix_tree.h \
    object_code_generate.h
