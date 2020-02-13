#ifndef Node_H
#define Node_H
#include <vector>

using namespace std;
class Node {
   public:
    std::vector<int> point;
   public:
    bool operator==(const Node &p) const {
        return point == p.point;
    }
    bool operator< (const Node &a) const {
        for (int i = 0; i < this->point.size(); i++) {
            if (this->point[i] == a.point[i]) continue;
            return this->point[i] < a.point[i];
        }
        return false;
    }
};
#endif