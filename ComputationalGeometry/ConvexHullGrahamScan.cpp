/*

*/

#include <iostream>
#include <vector>
#include <stack>
#include <ostream>
#include <set>

struct Point {
    int x = 0;
    int y = 0;

    Point operator-(const Point &p) const{
        return Point{this->x-p.x, this->y-p.y};
    }

    int crossProduct(const Point& p) const {
        return this->x*p.y - this->y*p.x;
    }

    int distSqaure (const Point &p) const {
        return  (p.x-this->x)*(p.x-this->x) + (p.y-this->y) * (p.y-this->y);
    }

    bool operator < (const Point& p) const {
        return std::pair<int, int> {this->x, this->y} < std::pair<int, int>{p.x, p.y};
    }
    friend std::ostream& operator << (std::ostream& os, const Point &p);
};

std::ostream& operator << (std::ostream& os, const Point &p) {
    os << "x: " << p.x << ", y: " << p.y << std::endl;
    return os;
}

using PointVector = std::vector<Point>;
using PointStack = std::stack<Point>;
using PointSet = std::set<Point>;

class ConvexHullGrahamScan {
public:
    ConvexHullGrahamScan(const PointVector& pointVector) : pointVector(pointVector) {
        if (pointVector.size() < 3) {
            throw std::runtime_error("Point size should be atleast 3");
        }
    }

    PointVector compute() const {
        int p0Index = getLowestLeftMostPointIndex();
        Point p0 = pointVector[p0Index];
        std::cout << "p0: " << p0;

        PointVector remainingPoints = getRemainingSortedPoints(p0Index);
        std::cout << "remainingPoints: " << std::endl;
        for (auto &p: remainingPoints) {
            std::cout << p << std::endl;
        }

        PointStack pointStack;
        pointStack.push(p0);
        pointStack.push(remainingPoints[0]);
        pointStack.push(remainingPoints[1]);

        auto angle = [](const Point &p0, const Point &p1, const Point& p2) {
                Point pi = p1-p0;
                Point pj = p2-p0;
                return pi.crossProduct(pj);
            };

        for (int i = 2; i <= remainingPoints.size()-1; ++i) {
            Point top = pointStack.top();
            pointStack.pop();
            Point nexToTop = pointStack.top();
            Point pi = remainingPoints[i];

            int angleVal = angle(nexToTop, top, pi);
            if (angleVal < 0) {
                pointStack.push(pi);
            }
            else if (angleVal >= 0) {
                pointStack.push(top);
                pointStack.push(pi);
            }

        }

        PointVector convexHull;

        while(!pointStack.empty()) {
            convexHull.emplace_back(pointStack.top());
            pointStack.pop();
        }
        return convexHull;
    }

private:
    PointVector getRemainingSortedPoints(int p0Index) const {
        PointVector pv;
        for (int i = 0; i <= pointVector.size()-1; ++i) {
            if (i != p0Index) {
                pv.emplace_back(pointVector[i]);
            }
        }

        Point p0 = pointVector[p0Index];
        PointSet pointsToBeRemoved;
        auto cmp = [&] (auto &p1, auto &p2) {

            auto angle = [](const Point &p0, const Point &p1, const Point& p2) {
                Point pi = p1-p0;
                Point pj = p2-p0;
                return pi.crossProduct(pj);
            };

            int angleVal = angle(p0, p1, p2);
            if (angleVal > 0) {
                return true;
            }
            else if (angleVal < 0) {
                return false;
            }
            else {
                //angle  === 0
                int distP1 = p0.distSqaure(p1);
                int distP2 = p0.distSqaure(p2);
                if (distP1 > distP2) {
                    pointsToBeRemoved.emplace(p2);
                    return true;
                }
                pointsToBeRemoved.emplace(p1);
                return false;

            }
        };

        std::sort(std::begin(pv), std::end(pv), cmp );

        PointVector out; 
        for (auto &p : pv) {
            if (pointsToBeRemoved.find(p) == pointsToBeRemoved.end() ){
                out.emplace_back(p);
            }
        }
        return out;
    }

    

    
    int getLowestLeftMostPointIndex() const {
        int p0Index = 0;
        Point p0 = pointVector[0];

        for (int i = 1; i <= pointVector.size()-1; ++i) {
            Point p = pointVector[i];
            if (p.y < p0.y) {
                p0 = p;
                p0Index = i;
            }
            else if (p.y == p0.y) {
                if (p.x < p0.x) {
                    p0 = p;
                    p0Index = i;
                }
            }
        }
        return p0Index;
    }
    const PointVector& pointVector;

};

int main() {
    std::cout << "Convex Hull Graham Scan" << std::endl;
    try {
        {
            PointVector pointVector{{0, 3}, {1, 1}, {2, 2}, {4, 4}, {0, 0}, {1, 2}, {3, 1}, {3, 3}};
            PointVector convexHull = ConvexHullGrahamScan(pointVector).compute();
            std::cout << "convexHull: " << std::endl;
            for (auto &point : convexHull) {
                std::cout << point << std::endl;
            }
        }
        {
            PointVector pointVector{{0, 3}, {2, 2}, {1, 1}, {2, 1},{3, 0}, {0, 0}, {3, 3}};
            PointVector convexHull = ConvexHullGrahamScan(pointVector).compute();
            std::cout << "convexHull: " << std::endl;
            for (auto &point : convexHull) {
                std::cout << point << std::endl;
            }
        }
    }
    catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown error" << std::endl;
    }
    return 0;
}