#ifndef PREFIX_TREE_H
#define PREFIX_TREE_H
#include<QString>
class Prefix_Tree
{
private:
    int ch[1000][80];//字典树
    int val[1000];
    int sz;
    int idx(char c);
public:
    int insert_tree(QString s, int v);
    void update_tree(QString s,int v);
    Prefix_Tree();
    ~Prefix_Tree();
};
#endif // PREFIX_TREE_H
