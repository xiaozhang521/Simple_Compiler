#include"compile.h"
#include"prefix_tree.h"
#include<QDebug>
Prefix_Tree::Prefix_Tree()
{
    sz=1;
    memset(ch[0],0,sizeof(ch[0]));
    memset(val,-1,sizeof(val));
}
int Prefix_Tree::insert_tree(QString s,int v)
{
    int u=0;
    int n=s.size();
    for(int i=0;i<n;++i)
    {
        int c=idx(s[i].unicode());
        if(!ch[u][c])
        {
            memset(ch[sz],0,sizeof(ch[sz]));
            val[sz]=-1;
            ch[u][c]=sz++;
        }
        u=ch[u][c];
    }
    if(val[u]>=0) return val[u];
    else
    {val[u]=v; return v;}
}
void Prefix_Tree::update_tree(QString s, int v)
{
    int u=0;
    int n=s.size();
    for(int i=0;i<n;++i)
    {
        int c=idx(s[i].unicode());
        u=ch[u][c];
    }
    val[u]=v;
}
Prefix_Tree::~Prefix_Tree()
{
    printf("optimization finish\n");
}
int Prefix_Tree::idx(char a)
{
    if('0'<=a&&a<='9')
    {
        return a-'0';
    }
    else if('a'<=a&&a<='z')
    {
        return a-'a'+10;
    }
    else if('A'<=a&&a<='Z')
    {
        return a-'A'+36;
    }
    else if(a=='_')
    {
        return 62;
    }
    else
    {
        return 70;
    }
}

void Grammatical_Analyze::insert()
{
    DAG.clear();//初始化
    node_number=0;
    Prefix_Tree Trie;
    for(int i=0;i<output_quaternary_tabel.size();++i)
    {
        QT tmp_now=output_quaternary_tabel.at(i);
        //*********特别操作**************
        if(tmp_now.oprate_val=="start"||tmp_now.oprate_val=="end"||tmp_now.oprate_val=="ie"||tmp_now.oprate_val=="el")
        {
            optimize_node new_node;
            node_number++;
            new_node.father.clear();
            new_node.child1=-1;
            new_node.child2=-1;//初始化
            new_node.opreate=tmp_now.oprate_val;
            DAG.push_back(new_node);
            continue;
        }
        //******************************
        //**************数字运算优化************
        if(tmp_now.oprate_val!="[]="&&'0'<=tmp_now.first_val.at(0).unicode()&&tmp_now.first_val.at(0).unicode()<='9'&&'0'<=tmp_now.second_val.at(0).unicode()&&tmp_now.second_val.at(0).unicode()<='9')
        {
            bool ok,ok2;
            int op_number1=tmp_now.first_val.toInt(&ok,10);
            int op_number2=tmp_now.second_val.toInt(&ok2,10);
            if(ok2&&ok)
            {
                if(tmp_now.oprate_val=="+")
                {
                    op_number1=op_number1+op_number2;
                }
                else if(tmp_now.oprate_val=="-")
                {
                    op_number1=op_number1-op_number2;
                }
                else if(tmp_now.oprate_val=="*")
                {
                    op_number1=op_number1*op_number2;
                }
                else if(tmp_now.oprate_val=="/")
                {
                    if(op_number2!=0)
                    op_number1=op_number1/op_number2;
                }
                else if(tmp_now.oprate_val=="<")
                {
                    op_number1=(op_number1<op_number2);
                }
                else if(tmp_now.oprate_val==">")
                {
                    op_number1=(op_number1>op_number2);
                }
                else if(tmp_now.oprate_val=="==")
                {
                    op_number1=(op_number1==op_number2);
                }
                else if(tmp_now.oprate_val==">=")
                {
                    op_number1=(op_number1>=op_number2);
                }else if(tmp_now.oprate_val=="<=")
                {
                    op_number1=(op_number1<=op_number2);
                }
                tmp_now.first_val=QString::number(op_number1,10);
                tmp_now.second_val="_";
                tmp_now.oprate_val="=";
            }
        }
        //***********************************
        int tmp_val1=Trie.insert_tree(tmp_now.first_val,node_number);//测试第一个值是否存在
        if(tmp_val1==node_number)//新建节点1
        {
            optimize_node new_node;
            node_number++;
            new_node.father.clear();
            new_node.child1=-1;
            new_node.child2=-1;//初始化
            new_node.node_name.push_back(tmp_now.first_val);
            new_node.opreate="";
            DAG.push_back(new_node);
        }
        int tmp_val2=-1;
        if(tmp_now.second_val!="_")//第二个不是_
        {
            tmp_val2=Trie.insert_tree(tmp_now.second_val,node_number);//测试第二个值是否存在
            if(tmp_val2==node_number)//新建节点2
            {
                optimize_node new_node;
                node_number++;
                new_node.father.clear();
                new_node.child1=-1;
                new_node.child2=-1;//初始化
                new_node.node_name.push_back(tmp_now.second_val);
                new_node.opreate="";
                DAG.push_back(new_node);
            }
        }
        //******************test tmp_val is exist*******************
        if(tmp_now.oprate_val=="=")
        {
            DAG[tmp_val1].node_name.push_back(tmp_now.tmp_val);
            move_tabel_head(tmp_val1);//更新顺序
            int delete_node=Trie.insert_tree(tmp_now.tmp_val,tmp_val1);
            //******************测试是否出现********************
            if(delete_node!=tmp_val1)
            {
                delete_node_name(tmp_now.tmp_val,delete_node);
                Trie.update_tree(tmp_now.tmp_val,tmp_val1);
            }
            //***********************************************
            continue;
        }
        if(tmp_now.oprate_val=="if"||tmp_now.oprate_val=="print")
        {
            optimize_node new_node;
            node_number++;
            new_node.father.empty();
            new_node.child1=tmp_val1;
            new_node.child2=tmp_val2;//初始化
            new_node.node_name.push_back(tmp_now.tmp_val);
            new_node.opreate=tmp_now.oprate_val;
            DAG.push_back(new_node);
            continue;
        }
        if(DAG[tmp_val1].father.empty()||(tmp_val2!=-1&&DAG[tmp_val2].father.empty()))
        {
            int  delete_node=Trie.insert_tree(tmp_now.tmp_val,node_number);
            //******************测试是否出现过赋值********************
            if(delete_node!=node_number)
            {
                delete_node_name(tmp_now.tmp_val,delete_node);
                Trie.update_tree(tmp_now.tmp_val,node_number);
            }
            //***********************************************
            DAG[tmp_val1].father.push_back(node_number);
            DAG[tmp_val2].father.push_back(node_number);
            optimize_node new_node;
            node_number++;
            new_node.father.empty();
            new_node.child1=tmp_val1;
            new_node.child2=tmp_val2;//初始化
            new_node.node_name.push_back(tmp_now.tmp_val);
            new_node.opreate=tmp_now.oprate_val;
            DAG.push_back(new_node);
            continue;
        }
        if(tmp_val2==-1)
        {
            int flag=1;
            for(int i=0;i<DAG[tmp_val1].father.size();++i)
            {
                int first_father=DAG[tmp_val1].father.at(i);
                if(DAG[first_father].opreate==tmp_now.oprate_val)
                {
                    flag=0;
                    DAG[first_father].node_name.push_back(tmp_now.tmp_val);
                    move_tabel_head(first_father);//更新顺序
                    int delete_node=Trie.insert_tree(tmp_now.tmp_val,first_father);
                    //******************测试是否出现过赋值********************
                    if(delete_node!=first_father)
                    {
                        delete_node_name(tmp_now.tmp_val,delete_node);
                        Trie.update_tree(tmp_now.tmp_val,first_father);
                    }
                    //***********************************************
                    break;
                }
            }
            if(flag)
            {
                int delete_node=Trie.insert_tree(tmp_now.tmp_val,node_number);
                //******************测试是否出现过赋值********************
                if(delete_node!=node_number)
                {
                    delete_node_name(tmp_now.tmp_val,delete_node);
                    Trie.update_tree(tmp_now.tmp_val,node_number);
                }
                //***********************************************
                DAG[tmp_val1].father.push_back(node_number);
                DAG[tmp_val2].father.push_back(node_number);
                optimize_node new_node;
                node_number++;
                new_node.father.empty();
                new_node.child1=tmp_val1;
                new_node.child2=tmp_val2;//初始化
                new_node.node_name.push_back(tmp_now.tmp_val);
                new_node.opreate=tmp_now.oprate_val;
                DAG.push_back(new_node);
            }
        }
        else
        {
            int flag=1;
            for(int i=0;i<DAG[tmp_val1].father.size();++i)
            {
                int first_father=DAG[tmp_val1].father.at(i);
                int tmp_child=DAG[first_father].child1;
                if(tmp_child==tmp_val1)
                {
                    tmp_child=DAG[first_father].child2;
                }
                if(tmp_child!=-1&&tmp_child==tmp_val2&&DAG[first_father].opreate==tmp_now.oprate_val)
                {
                    DAG[first_father].node_name.push_back(tmp_now.tmp_val);
                    move_tabel_head(first_father);//更新顺序
                    int delete_node=Trie.insert_tree(tmp_now.tmp_val,first_father);
                    //******************测试是否出现过赋值********************
                    if(delete_node!=first_father)
                    {
                        delete_node_name(tmp_now.tmp_val,delete_node);
                        Trie.update_tree(tmp_now.tmp_val,first_father);
                    }
                    //***********************************************
                    flag=0;
                    break;
                }
            }
            if(flag)
            {
                int delete_node=Trie.insert_tree(tmp_now.tmp_val,node_number);
                //******************测试是否出现过赋值********************
                if(delete_node!=node_number)
                {
                    delete_node_name(tmp_now.tmp_val,delete_node);
                    Trie.update_tree(tmp_now.tmp_val,node_number);
                }
                //***********************************************
                DAG[tmp_val1].father.push_back(node_number);
                DAG[tmp_val2].father.push_back(node_number);
                optimize_node new_node;
                node_number++;
                new_node.father.empty();
                new_node.child1=tmp_val1;
                new_node.child2=tmp_val2;//初始化
                new_node.node_name.push_back(tmp_now.tmp_val);
                new_node.opreate=tmp_now.oprate_val;
                DAG.push_back(new_node);
            }
        }
    }
    /*for(int i=0;i<DAG.size();++i)
    {
        qDebug()<<DAG.at(i).opreate;
        for(int j=0;j<DAG.at(i).node_name.size();++j)
            qDebug()<<i<<DAG.at(i).node_name.at(j)<<DAG.at(i).child1<<DAG.at(i).child2;
    }*/
}
void Grammatical_Analyze::move_tabel_head(int tabel_number)
{
    int tmp_name_length=DAG[tabel_number].node_name.size();
    if(tmp_symbol_tabel.find_identifiter(DAG[tabel_number].node_name[tmp_name_length-1])!=-1)
    {
        int val=0;
        if('0'<=DAG[tabel_number].node_name[0][0]&&DAG[tabel_number].node_name[0][0]<='9')
        {
            val=1;
        }
        while(val<tmp_name_length-1)
        {
            if(tmp_symbol_tabel.find_identifiter(DAG[tabel_number].node_name[val])!=-1)
                val++;
            else break;
        }
        QString exchange_string=DAG[tabel_number].node_name[val];
        DAG[tabel_number].node_name[val]=DAG[tabel_number].node_name[tmp_name_length-1];
        DAG[tabel_number].node_name[tmp_name_length-1]=exchange_string;
    }
}
void Grammatical_Analyze::delete_node_name(QString delete_name,int delete_number)
{
    for(int i=1;i<DAG[delete_number].node_name.size();++i)
    {
        if(DAG[delete_number].node_name.at(i)==delete_name)
        {
            DAG[delete_number].node_name.remove(i);
            //printf("remove this node");
            break;
        }
    }
}
void Grammatical_Analyze::dfs_DAG(int now_node)
{
    if(node_vis[now_node]) return;
    node_vis[now_node]=true;
    if(DAG[now_node].child1==-1&&DAG[now_node].child2==-1)
    {
        if(DAG[now_node].opreate=="ie"||DAG[now_node].opreate=="end"||DAG[now_node].opreate=="start"||DAG[now_node].opreate=="el")
        {
            QT new_quaternaty;
            new_quaternaty.first_val="_";
            new_quaternaty.second_val="_";
            new_quaternaty.tmp_val="_";
            new_quaternaty.oprate_val=DAG[now_node].opreate;
            output_quaternary_tabel.push_back(new_quaternaty);
            return;
        }
        for(int i=1;i<DAG[now_node].node_name.size();++i)//测试等于四元式
        {
            if(tmp_symbol_tabel.find_identifiter(DAG[now_node].node_name.at(i))!=-1)
            {
                QT new_quaternaty;
                new_quaternaty.first_val=DAG[now_node].node_name[0];
                new_quaternaty.second_val="_";
                new_quaternaty.tmp_val=DAG[now_node].node_name.at(i);
                new_quaternaty.oprate_val="=";
                output_quaternary_tabel.push_back(new_quaternaty);
            }
            else break;
        }
        return ;
    }
    if(DAG[now_node].child1!=-1)
    {
        dfs_DAG(DAG[now_node].child1);
        if(DAG[now_node].child2==-1)//单目运算
        {
            QT new_quaternaty;
            new_quaternaty.first_val=DAG[DAG[now_node].child1].node_name[0];
            new_quaternaty.second_val="_";
            new_quaternaty.oprate_val=DAG[now_node].opreate;
            new_quaternaty.tmp_val=DAG[now_node].node_name[0];
            output_quaternary_tabel.push_back(new_quaternaty);
            return;
        }
    }
    if(DAG[now_node].child2!=-1)
    {
        dfs_DAG(DAG[now_node].child2);
    }
    QT new_quaternaty;
    new_quaternaty.first_val=DAG[DAG[now_node].child1].node_name[0];
    new_quaternaty.second_val=DAG[DAG[now_node].child2].node_name[0];
    new_quaternaty.oprate_val=DAG[now_node].opreate;
    new_quaternaty.tmp_val=DAG[now_node].node_name[0];
    output_quaternary_tabel.push_back(new_quaternaty);
    for(int i=1;i<DAG[now_node].node_name.size();++i)//测试等于四元式
    {
        if(tmp_symbol_tabel.find_identifiter(DAG[now_node].node_name.at(i))!=-1)
        {
            new_quaternaty.first_val=DAG[now_node].node_name[0];
            new_quaternaty.second_val="_";
            new_quaternaty.tmp_val=DAG[now_node].node_name.at(i);
            new_quaternaty.oprate_val="=";
            output_quaternary_tabel.push_back(new_quaternaty);
        }
        else break;
    }
    return;
}
void Grammatical_Analyze::recombine_QT()//重组四元式
{
    memset(node_vis,false,sizeof(node_vis));
    output_quaternary_tabel.clear();
    /*QT new_quaternaty;
    new_quaternaty.oprate_val=DAG[0].opreate;
    new_quaternaty.first_val="_";
    new_quaternaty.second_val="_";
    new_quaternaty.tmp_val="_";
    output_quaternary_tabel.push_back(new_quaternaty);*/
    for(int i=0;i<DAG.size();++i)
    {
        if(!node_vis[i])
        {
            dfs_DAG(i);
        }
    }
    /*new_quaternaty.oprate_val=DAG[DAG.size()-1].opreate;
    new_quaternaty.first_val="_";
    new_quaternaty.second_val="_";
    new_quaternaty.tmp_val="_";
    output_quaternary_tabel.push_back(new_quaternaty);*/
}
