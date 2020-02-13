#include "FastMLCS.h"
#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;

void MLCS::init() {
    info("start init");
    char_num = 4;
    node_num = 0;
    camp['A'] = 1;
    camp['C'] = 2;
    camp['G'] = 3;
    camp['T'] = 4;
    indegree = vector<int>(number, 0);
    level = vector<int>(number, 0);
    Set = vector<Node>(number);
    G = vector<vector<int> >(number);
    info("init success");
}
void MLCS::info(string info) {
    for (int i = 1; i <= 30; i++) cout << "-";
    cout << "\n";
    int d = 30 - info.size();
    for (int i = 1; i <= d / 2; i++) cout << "-";
    cout << info;
    for (int i = 1; i <= d - d / 2; i++) cout << "-";
    cout << "\n";
    for (int i = 1; i <= 30; i++) cout << "-";
    cout << "\n";
}
void MLCS::construct_ST() {
    info("start construct ST");
    for (int i = 0; i < s.size(); i++) {
        for (int j = 0; j < s[i].size(); j++) {
            int d = ST[i][camp[s[i][j]]].size();
            for (int k = d; k <= j; k++) {
                ST[i][camp[s[i][j]]].emplace_back(j + 1);
            }
        }
    }
    info("construct ST success");
}
void MLCS::construct_PT() {
    info("start construct PT");
    for (int i = 0; i < s.size(); i++) {
        for (int j = 1; j <= char_num; j++) {
            PT[i][j] = vector<int>(s[i].size(), 0);
        }
    }
    for (int i = 0; i < s.size(); i++) {
        for (int j = s[i].size() - 1; j >= 0; j--) {
            for (int k = j + 1; k < s[i].size(); k++) {
                PT[i][camp[s[i][j]]][k] = j + 1;
                if (s[i][k] == s[i][j]) break;
            }
        }
    }
    info("construct PT success");
}
string MLCS::new_hash(Node &p) {
    string s = "";
    for (int x : p.point) {
        s += to_string(x);
    }
    return s;
}
bool MLCS::judge(int father, Node node) {
    int i, j;
    for (i = 0; i < s.size(); i++) {
        if (node.point[i] == Set[father].point[i] + 1) return true;
    }
    for (i = 1; i <= char_num; i++) {
        int p = 0;
        for (j = 0; j < s.size(); j++) {
            if (PT[j][i][node.point[j] - 1] <= Set[father].point[j]) {
                p = 1;
                break;
            }
        }
        if (p == 0) return false;
    }
    return true;
}
void MLCS::clear_precursor(int d) {
    queue<int> q;
    int p;
    q.push(d);
    while (!q.empty()) {
        p = q.front();
        q.pop();
        if (p != d) {
            auto it = mp.find(new_hash(Set[p]));
            if (it != mp.end()) mp.erase(it);
            Set[p].point.clear();
        }
        for (int x : G[p]) {
            indegree[x]--;
            if (indegree[x] == 0) q.push(x);
        }
        G[p].clear();
    }
}
void MLCS::construct_graph() {
    Node now;
    int flag = 0, success = 0, p, d, successor_num;
    info("start construct graph");
    while (!now_que.empty()) now_que.pop();
    Set[0].point = vector<int>(s.size(), 0);
    mp[new_hash(Set[0])] = 0;
    string ans;
    now_que.push(0);
    int sum = 100000;
    while (!success) {
        success = 0;
        while (!now_que.empty()) {
            p = now_que.front();
            now_que.pop();
            successor_num = 0;
            d = -1;
            for (int i = 1; i <= char_num; i++) {
                // info("sing");
                now.point.clear();
                flag = 0;
                d = -1;
                for (int j = 0; j < s.size(); j++) {
                    if (ST[j][i].size() > Set[p].point[j]) {
                        now.point.emplace_back(ST[j][i][Set[p].point[j]]);
                    } else {
                        flag = 1;
                        break;
                    }
                }
                if (flag == 0) {  //存在第i个字符的后集结点
                    successor_num++;
                    ans = new_hash(now);
                    if (judge(p, now)) {  //判断是不是直接后继
                        if (mp.find(ans) == mp.end()) {  //该结点未在图中出现
                            node_num++;
                            Set[node_num] = now;
                            mp[ans] = node_num;
                            d = node_num;
                        }
                        if (d == -1) d = mp[ans];
                        if (level[d] < level[p] + 1) {
                            //清空前驱，建边, +入度, 入队
                            G[d].clear();
                            clear_precursor(d);
                            G[d].emplace_back(p);
                            indegree[p]++;
                            if (G[d].size() == 1) next_que.push(d);
                            level[d] = level[p] + 1;
                        } else if (level[d] == level[p] + 1) {
                            //建边，+入度, 不入队
                            G[d].emplace_back(p);
                            indegree[p]++;
                            if (G[d].size() == 1) next_que.push(d);
                        }
                    } else {  //存储结点，标记level
                        if (mp.find(ans) == mp.end()) {  //该结点未在图中出现
                            node_num++;
                            Set[node_num] = now;
                            mp[ans] = node_num;
                            d = node_num;
                        }
                        if (d == -1) d = mp[ans];

                        if (level[d] < level[p] + 2) {
                            clear_precursor(d);
                            level[d] = level[p] + 2;
                        }
                    }
                    // info("basketball");
                }
            }
            if (successor_num == 0) {
                no_successor.push(p);
            }
        }
        if (next_que.empty()) {
            success = 1;
            node_num++;
            Set[node_num].point = vector<int>(s.size(), s[0].size() + 1);
            level[node_num] = level[no_successor.front()] + 1;
            while (!no_successor.empty()) {
                G[node_num].emplace_back(no_successor.front());
                indegree[no_successor.front()]++;
                no_successor.pop();
            }
        } else {
            while (!no_successor.empty()) {
                d = no_successor.front();
                no_successor.pop();
                clear_precursor(d);
            }
            while (!next_que.empty()) {
                now_que.push(next_que.front());
                next_que.pop();
            }
        }
    }
    cout << node_num << "\n";
    info("construct graph success");
}
void MLCS::solve() {
    this->construct_ST();
    this->construct_PT();
    this->construct_graph();
}
void MLCS::read() {
    string t;
    info("start read");
    while (cin >> t) {
        this->s.emplace_back(t);
    }
    info("read success");
}
void MLCS::dfs(int x, int length, vector<int> &ans) {
    if (x == 0) {
        for (int y : ans) {
            if (y == 0) break;
            cout << s[0][Set[y].point[0] - 1];
        }
        cout << "\n";
        return;
    }
    for (int y : G[x]) {
        ans[length] = y;
        dfs(y, length + 1, ans);
    }
}
void MLCS::output() {
    cout << "the length of MLCS is " << level[node_num] - 1 << "\n";
    return;
}
void MLCS::run() {
    int start_time = clock();
    this->init();
    int end_time = clock();
    cout << (double)(end_time - start_time) / CLOCKS_PER_SEC << "\n";
    this->read();
    start_time = clock();
    cout << (double)(start_time - end_time) / CLOCKS_PER_SEC << "\n";
    this->solve();
    this->output();
    end_time = clock();
    cout << (double)(end_time - start_time) / CLOCKS_PER_SEC << "\n";
}
