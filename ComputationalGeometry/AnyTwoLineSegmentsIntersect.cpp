/*
ANY-SEGMENTS-INTERSECT.S/
sweepLineIntersection(Points[0..2n-1]):
1. Sort Points[] from left to right (according to x coordinate)

2. Create an empty Self-Balancing BST T. It will contain 
  all active line Segments ordered by y coordinate.

// Process all 2n points 
3. for i = 0 to 2n-1

    // If this point is left end of its line  
    if (Points[i].isLeft) 
       T.insert(Points[i].line())  // Insert into the tree

       // Check if this points intersects with its predecessor and successor
       if ( doIntersect(Points[i].line(), T.pred(Points[i].line()) )
         return true
       if ( doIntersect(Points[i].line(), T.succ(Points[i].line()) )
         return true

    else  // If it's a right end of its line
       // Check if its predecessor and successor intersect with each other
       if ( doIntersect(T.pred(Points[i].line(), T.succ(Points[i].line()))
         return true
       T.delete(Points[i].line())  // Delete from tree

4. return False
*/

#include <iostream>
#include <vector>
#include <set>
#include <ostream>


struct Point {
    int x = 0;
    int y = 0;

    Point operator - (Point p) const {
        return Point {this->x - p.x, this->y - p.y};
    }

    int crossProduct(Point p) const {
        return this->x * p.y - this->y * p.x;
    }

    friend std::ostream& operator << (std::ostream &os, const Point& p ) ;

};

std::ostream& operator << ( std::ostream &os, const Point& p)  {
    os << "x: " << p.x << ", y: " << p.y;
    return os;
}

struct Segment {
    Point left;
    Point right;


    bool intersect(Segment &segment) const {
        Point p1 = this->left;
        Point p2 = this->right;
        Point p3 = segment.left;
        Point p4 = segment.right;

        int d1 = direction(p1,p2,p3);
        int d2 = direction(p1,p2,p4);
        int d3 = direction(p3,p4,p1);
        int d4 = direction(p3,p4,p2);

        if (d1 < 0 && d2 > 0)
            return true;

        if (d1 >0 && d2 < 0)
            return true;

        if (d3 > 0 && d4 < 0)
            return true;

        if (d3 < 0  && d4 > 0)
            return true;

        if (d1 == 0 && this->onSegment(p3))
            return true;

        if (d2 == 0 && this->onSegment(p4))
            return true;

        if (d3 == 0 && segment.onSegment(p1))
            return true;

        if (d4 == 0 && segment.onSegment(p2))
            return true;

        return false;
    }

    int direction(Point p1, Point p2, Point p3) const{
        Point pi = p2-p1;
        Point pj = p3-p1;
        return pi.crossProduct(pj);
    }

    bool onSegment (Point p3) const {
        Point p1 = this->left;
        Point p2 = this->right;
        if (
            std::min(p1.x, p2.x) <= p3.x 
            && p3.x <= std::max(p1.x, p2.x) 
            && std::min(p1.y, p2.y) <= p3.y
            && p3.y <= std::max(p1.y, p2.y)
            )
            return true;
        
        return false;
    }
};
using Segments = std::vector<Segment>;

struct Event {
    Point point;
    bool isLeft = false;
    int segmentIndex = -1;
    
    bool operator < (const Event &e) const {
        if ( this->point.y == e.point.y ) {
            return this->point.x < e.point.x;
        }
        return this->point.y < e.point.y;
    }
};

class AnyTwoLineSegmentsIntersect {
public:
    AnyTwoLineSegmentsIntersect(const Segments& segments) : segments(segments) {}

    bool check() {
        std::vector<Event> events = getEvents();

        std::set<Event> preOrderT;

        for (auto &e : events){
            Point p = e.point;
            int currentSegmentIndex = e.segmentIndex;

            std::cout << "SegmentIndex : " << currentSegmentIndex << ", Point: " << p << ", isLeft: " << e.isLeft << std::endl;
            if (e.isLeft) {
                //In left point of segment
                preOrderT.emplace(e);

                if (hasPredecessor(preOrderT, e)) {
                    Event pred = predecessor(preOrderT, e);
                    if (intersect(e.segmentIndex, pred.segmentIndex)) {
                        return true;
                    }
                }
                if (hasSuccessor(preOrderT, e)) {
                    Event succ = successor(preOrderT, e);
                    if (intersect(e.segmentIndex, succ.segmentIndex)) {
                        return true;
                    }
                }

               
            }
            else {
                //In right point of segment.
                //Get event of segment when it is inserted in preOrderT;
                Event event {p, true, currentSegmentIndex};

                if (hasPredecessor(preOrderT, event) && hasSuccessor(preOrderT, event)) {
                    Event pred = predecessor(preOrderT, event);
                    Event succ = successor(preOrderT, event);
                    if(intersect(pred.segmentIndex, succ.segmentIndex)) {
                        return true;
                    }
                }
                preOrderT.erase(event);
            }
        }

        return false;
    }

    bool intersect(int segmentIndex1, int segmentIndex2) {
        if (segmentIndex1 == segmentIndex2) return false;

        Segment segment1 = segments[segmentIndex1];
        Segment segment2 = segments[segmentIndex2];

        return segment1.intersect(segment2);
        
    }

    Event predecessor(const std::set<Event> &T, const Event &e) {
        auto itr = T.find(e);
        --itr;
        return *itr;
    }

    Event successor(const std::set<Event> &T, const Event &e) {
        auto itr = T.find(e);
        ++itr;
        return *itr;
    }

    bool hasPredecessor(const std::set<Event> &T, const Event &e) {
        auto itr = T.find(e);
        if (itr == T.end()) {
            return false;
        }

        if(itr == T.begin()) {
            return false;
        }

        --itr;
        if (itr->segmentIndex == e.segmentIndex) {
            return false;
        }
        return true;
    }

    bool hasSuccessor(const std::set<Event> &T, const Event &e) {
        auto itr = T.find(e);
        if (itr == T.end()) {
            return false;
        }

        ++itr;
        if(itr == T.end()) {
            return false;
        }

        if (itr->segmentIndex == e.segmentIndex) {
            return false;
        }

        return true;
    }

    std::vector<Event> getEvents() {
        std::vector<Event> events;

        for (int i = 0; i <= segments.size()-1; ++i) {
            Point left = segments[i].left;
            Point right = segments[i].right;
            events.emplace_back(Event{left, true, i});
            events.emplace_back(Event{right, false, i});
        }

        auto cmp = [](auto &a, auto &b) {
            if (a.point.x == b.point.x) {
                if (a.isLeft && !b.isLeft) {
                    return true;
                }

                return a.point.y < b.point.y;
            }

            return a.point.x < b.point.x;
        };

        std::sort(std::begin(events), std::end(events), cmp);
        return events;
    }

    const Segments &segments;
};

int main() {
    std::cout << "AnyTwoLineSegmentsIntersect" << std::endl;
    Segments segments;
    segments.emplace_back(Segment{Point{1, 5}, Point{4, 5}}); 
    segments.emplace_back(Segment{Point{2, 5}, Point{10, 1} });
    segments.emplace_back(Segment{Point{3, 2}, Point{10, 3}});
    segments.emplace_back(Segment{Point{6, 4}, Point{9, 4}});
    segments.emplace_back(Segment{Point{7, 1}, Point{8, 1}});

    
    std::cout << std::boolalpha << AnyTwoLineSegmentsIntersect(segments).check() << std::endl;

    return 0;
}