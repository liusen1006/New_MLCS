#ifndef FastMLCS_H
#define FastMLCS_H

#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_map>
#include <vector>
#include "Node.h"
using namespace std;

const int number = 500000005;

class MLCS {
   public:
    int camp[256], char_num, node_num;  //标记序列中的字符、节点个数
    vector<string> s;                            //存储输入的序列
    vector<int> ST[10005][5], PT[10005][5];      //构建前驱表和后继表
    vector<vector<int> > G;                      //存储节点的前驱
    unordered_map<string, int> mp;            //计算节点的编号
    vector<int> indegree, level;                 //记录节点的入度和层
    vector<Node> Set;                            //存储点集
    queue<int> now_que, no_successor, next_que;  //辅助层次遍历
   public:
    void read();                        //读取数据
    void init();                        //初始化
    void solve();                       //求解MLCS
    void output();                      //输出所有MLCS
    void construct_ST();                //构建后继表
    void construct_PT();                //构建前驱表
    void run();                         //方法入口
    void info(string info);             //打印
    void construct_graph();             //构建子序列图
    bool judge(int father, Node node);  //判断father是不是node的直接前驱
    void clear_precursor(int d);        //从d结点开始清除前驱
    void dfs(int x, int length, vector<int>& ans);
    string new_hash(Node& p);
};
#endif