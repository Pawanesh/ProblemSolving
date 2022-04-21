
#include <iostream>
#include <algorithm>

struct Point {
    int x = 0;
    int y = 0;

    Point operator - (const Point &p) const {
        return Point{ this->x - p.x, this->y - p.y};
    }

    int crossProduct(const Point &p) const {
        return this->x * p.y - this->y * p.x;
    }
};

class TwoLineSegmentsIntersect {

public:

    TwoLineSegmentsIntersect(
        const Point &p1,
        const Point &p2,
        const Point &p3,
        const Point &p4
    ) :
        p1(p1),
        p2(p2),
        p3(p3),
        p4(p4) {

    }

    bool check() {
        int d1 = direction(p1, p2, p3);
        int d2 = direction(p1, p2, p4);
        int d3 = direction(p3, p4, p1);
        int d4 = direction(p3, p4, p2);

        if (d1 < 0 && d2 > 0)
            return true;

        if (d1 > 0 && d1 < 0)
            return 0;

        if (d3 < 0 && d4 > 0)
            return 0;
        
        if (d3 > 0 && d4 < 0)
            return true;

        if (d1 == 0 && onSegment(p1, p2, p3) )
            return true;

        if (d2 == 0 && onSegment(p1,p2,p4) )
            return true;

        if (d3 == 0 && onSegment(p3,p4,p1))
            return true;

        if (d4 == 0 && onSegment(p3, p4, p2))
            return true;

        return false;
    }

    int direction(const Point &p1, const Point &p2, const Point &p3) {
        const Point &pi = p3 - p1;
        const Point &pj = p2 - p1;
        return pi.crossProduct(pj);
    }

    bool onSegment(const Point &p1, const Point &p2, const Point &p3) {
        if (
            std::min(p1.x, p2.x) <= p3.x 
            && p3.x <= std::max(p1.x, p2.x) 
            && std::min(p1.y, p2.y) <= p3.y
            && p3.y <= std::max(p1.y, p2.y)
            )
            return true;
        
        return false;
    }

    const Point &p1;
    const Point &p2;
    const Point &p3;
    const Point &p4;
};

int main() {
    std::cout << "TwoLineSegmentsIntersect" << std::endl;

    Point p1 = {1, 1}, p2 = {10, 1};
    Point p3 = {1, 2}, p4 = {10, 2};
    
    std::cout << std::boolalpha << TwoLineSegmentsIntersect(p1,p2,p3,p4).check() << std::endl;

    p1 = {10, 0}, p2 = {0, 10};
    p3 = {0, 0}, p4 = {10, 10};
    std::cout << std::boolalpha << TwoLineSegmentsIntersect(p1,p2,p3,p4).check() << std::endl;

    p1 = {-5, -5}, p2 = {0, 0};
    p4 = {1, 1}, p3 = {10, 10};
    std::cout << std::boolalpha << TwoLineSegmentsIntersect(p1,p2,p3,p4).check() << std::endl;
/*
    Output: 
    false
    true
    false
*/
    return 0;
}