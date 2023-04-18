#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>

using namespace std;

struct Point {
    int x, y, index;
};

Point centroid = {0, 0, -1};

int quad(const Point& p)
{
    if (p.x >= 0 && p.y >= 0)
        return 1;
    if (p.x <= 0 && p.y >= 0)
        return 2;
    if (p.x <= 0 && p.y <= 0)
        return 3;
    return 4;
}
 

bool compare_x(const Point& a, const Point& b)
{
    return a.x < b.x;
}

// compare function for sorting
bool compare(const Point& a, const Point& b)
{
    Point a0 = {a.x - centroid.x, a.y - centroid.y, -1};
    Point b0 = {b.x - centroid.x, b.y - centroid.y, -1};
 
    int one = quad(a0);
    int two = quad(b0);
 
    if (one != two)
        return (one < two);
    return (a.y * b.x < b.y * a.x);
}

int cross_product(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

int orientation(const Point& a, const Point& b, const Point& c)
{
    int res = cross_product(a, b, c);
 
    if (res == 0)
        return 0; // colinear
    if (res > 0)
        return 1; // clockwise

    return -1; // counterclockwise
}

vector<Point> brute_hull(const vector<Point>& points){
    set<pair<int, int> > hullPoints;
    int psize = points.size();

    for (int i = 0; i < psize; i++) {
        for (int j = i + 1; j < psize; j++) {
            int x1 = points[i].x, x2 = points[j].x;
            int y1 = points[i].y, y2 = points[j].y;
            
            int a = y1 - y2;
            int b = x2 - x1;
            int c = x1 * y2 - y1 * x2;
            int numPointsOnPositiveSide = 0, numPointsOnNegativeSide = 0;
            for (int k = 0; k < psize; k++) {
                if (a * points[k].x + b * points[k].y + c <= 0) {
                    numPointsOnNegativeSide++;
                }
                if (a * points[k].x + b * points[k].y + c >= 0) {
                    numPointsOnPositiveSide++;
                }
            }
            if (numPointsOnNegativeSide == (int)points.size() || numPointsOnPositiveSide == (int)points.size()) {
                hullPoints.insert(make_pair(points[i].x, points[i].y));
                hullPoints.insert(make_pair(points[j].x, points[j].y));
            }
        }
    }

    vector<Point> hullVector;

    for (auto hullPoint : hullPoints) {
        Point p = {hullPoint.first, hullPoint.second, -1};
        hullVector.push_back(p);
    }

    centroid.x = 0;
    centroid.y = 0;
    centroid.index = -1;

    int numPointsOnHull = hullVector.size();
    for (int i = 0; i < numPointsOnHull; i++) {
        centroid.x += hullVector[i].x;
        centroid.y += hullVector[i].y;
        hullVector[i].x *= numPointsOnHull;
        hullVector[i].y *= numPointsOnHull;
    }

    sort(hullVector.begin(), hullVector.end(), compare);

    for (int i = 0; i < numPointsOnHull; i++) {
        hullVector[i].x = hullVector[i].x / numPointsOnHull;
        hullVector[i].y = hullVector[i].y / numPointsOnHull;
        hullVector[i].index = -1;
    }

    for (int i = 0; i < numPointsOnHull; i++) { // fix indexes
        for (int j = 0; j < numPointsOnHull; j++) {
            if ((hullVector[i].x == points[j].x) && (hullVector[i].y == points[j].y)) {
                hullVector[i].index = points[j].index;
                break;
            }
        }
    }

    return hullVector;
}

pair<int, int> upper_tangent(const int& right_most, const int& left_most, const vector<Point>& left_hull, const vector<Point>& right_hull) {
    int lh_ptr = right_most, rh_ptr = left_most;
    int left_size = left_hull.size(), right_size = right_hull.size();

    bool done = false;
    while (!done){
        done = true;
        while (orientation(right_hull[rh_ptr], left_hull[lh_ptr], left_hull[(lh_ptr + 1) % left_size]) >= 0){
            lh_ptr = (lh_ptr + 1) % left_size; // move the left pointer counterclockwise until it is at the rightmost point
        }
        while (orientation(left_hull[lh_ptr], right_hull[rh_ptr], right_hull[(right_size + rh_ptr - 1) % right_size]) <= 0){
            rh_ptr = (right_size + rh_ptr - 1) % right_size; // move the right pointer clockwise until it is at the leftmost point
            done = false;
        }
    }
    return make_pair(lh_ptr, rh_ptr);
}

pair<int, int> lower_tangent(const int& right_most, const int& left_most, const vector<Point>& left_hull, const vector<Point>& right_hull) {
    int lh_ptr = right_most, rh_ptr = left_most;
    int left_size = left_hull.size(), right_size = right_hull.size();

    bool done = false;
    while (!done) {
        done = true;
        while (orientation(left_hull[lh_ptr], right_hull[rh_ptr], right_hull[(rh_ptr + 1) % right_size]) >= 0) {
            rh_ptr = (rh_ptr + 1) % right_size; // move the right pointer counterclockwise until it is at the leftmost point
        }
        while (orientation(right_hull[rh_ptr], left_hull[lh_ptr], left_hull[(left_size + lh_ptr - 1) % left_size]) <= 0) {
            lh_ptr = (left_size + lh_ptr - 1) % left_size; // move the left pointer clockwise until it is at the rightmost point
            done = false;
        }
    }
    return make_pair(lh_ptr, rh_ptr);
}

vector<Point> merge_hulls(vector<Point> left_hull, vector<Point> right_hull) {
    int right_most = 0, left_most = 0; // left and right pointers
    int left_size = left_hull.size(), right_size = right_hull.size(); // size of the left and right hulls

    for (int i = 0; i < left_size; ++i) {
        if (left_hull[i].x > left_hull[right_most].x) {
            right_most = i; // find the rightest point in the left hull
        }
    }

    for (int i = 0; i < right_size; ++i) {
        if (right_hull[i].x < right_hull[left_most].x) {
            left_most = i; // find the leftest point in the right hull
        }
    }

    pair<int, int> upair = upper_tangent(right_most, left_most, left_hull, right_hull);
    int upper_left = upair.first, upper_right = upair.second; // set the upper left and upper right points

    pair<int, int> lpair = lower_tangent(right_most, left_most, left_hull, right_hull);
    int lower_left = lpair.first, lower_right = lpair.second; // set the lower left and lower right points

    vector<Point> hull; // create a vector to store the points of the convex hull

    int idx = upper_left; // set the index to the upper left point

    hull.emplace_back(left_hull[idx]);

    while (idx != lower_left) {
        idx = (idx + 1) % left_size; // add the points of the left hull to the convex hull
        hull.emplace_back(left_hull[idx]);
    }

    idx = lower_right;

    while (idx != upper_right){
        idx = (idx + 1) % right_size; // add the points of the right hull to the convex hull
        hull.emplace_back(right_hull[idx]);
    }

    return hull; // return the convex hull
    
}

vector<Point> convex_hull(const vector<Point>& points) {
    if(points.size() <= 3) return brute_hull(points); // minimum 5 points required to form a convex hull
    int mid = points.size() / 2; // divide the points into two parts

    vector<Point> left_hull = convex_hull(vector<Point>(points.begin(), points.begin() + mid)); // left hull
    vector<Point> right_hull = convex_hull(vector<Point>(points.begin() + mid, points.end())); // right hull
    
    vector<Point> hull = merge_hulls(left_hull, right_hull); // merge the two hulls

    return hull; // return the merged hull
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Please provide the input file path." << endl;
        // return 1;
    }

    // ifstream input(argv[1]);
    ifstream input("./cases/case00/input00.txt");
    int n, k;
    float th;
    input >> n >> k >> th;

    vector<int> bakeries(k);
    for (int i = 0; i < k; ++i) {
        input >> bakeries[i];
    }

    vector<Point> cities(n);
    for (int i = 0; i < n; ++i) {
        input >> cities[i].x >> cities[i].y;
        cities[i].index = i;
        int a;
        input >> a;
    }
    input.close(); 

    std::sort(cities.begin(), cities.end(), compare_x); // sort by x coordinate

    vector<Point> hull = convex_hull(cities);
    hull.push_back(hull[0]); // Close the polygon.


    std::exit(0);
}
