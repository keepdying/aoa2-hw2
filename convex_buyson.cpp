#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <functional>

using namespace std;

int INF = 2147483647;

struct Input{
    int Ncity;
    int Nbakery;
    float th; // threshold
};
struct Point{
    int x;
    int y;
    int a; // pount number
    int index;
    Point()
    {
        
    }
    Point(int in1, int in2, int in3)
    {
        x = in1;
        y = in2;
        a = in3;
    }
};
int mod(int dividend, int divisor) {
    int remainder = dividend % divisor;
    return (remainder >= 0) ? remainder : remainder + abs(divisor);
}

float y(Point a, Point b, float at)
{
    float slope =((float)b.y - (float)a.y)/((float)b.x - (float)a.x);
    return slope*(at-a.x) + a.y;
}

pair<int, int> UpperTangent(const vector<Point>& left, const vector<Point>& right, int rightmost, int leftmost)
{
    int i = rightmost; // iterator for left convex hull
    int j = leftmost; // iterator for right convex hull

    float at = ((float)right[j].x + (float)left[i].x)/2.0;

    while((y(left[i], right[mod(j+1, right.size())], at) > y(left[i], right[j], at)) || (y(left[mod(i-1, left.size())], right[j], at) > y(left[i], right[j], at)))
    {
        if(y(left[i], right[mod(j+1, right.size())], at) > y(left[i], right[j], at))
        {
            j = mod(j+1, right.size()); // right convex hull moving clockwise
        } else
        {
            i = mod(i-1, left.size()); // left convex hull moving anticlockwise
        }
        //at = ((float)right[j].x + (float)left[i].x)/2.0;
       
    }

    cout << left[i].x << " " << left[i].y << "\n";
    cout << right[j].x << " " << right[j].y << "\n";
    return make_pair(i, j);
}

pair<int, int> LowerTangent(const vector<Point>& left, const vector<Point>& right, int rightmost, int leftmost)
{
    int i = rightmost; // iterator for left convex hull
    int j = leftmost; // iterator for right convex hull

    float at = ((float)right[j].x + (float)left[i].x)/2.0;

    while((y(left[i], right[mod(j-1, right.size())], at) < y(left[i], right[j], at)) || (y(left[mod(i+1, left.size())], right[j], at) < y(left[i], right[j], at)))
    {
        if(y(left[i], right[mod(j-1, right.size())], at) < y(left[i], right[j], at))
        {
            j = mod(j-1, right.size()); // right convex hull moving anticlockwise
        } else
        {
            i = mod(i+1, left.size()); // left convex hull moving clockwise
        }
        //at = ((float)right[j].x + (float)left[i].x)/2.0;
       
    }
    cout << left[i].x << " " << left[i].y << "\n";
    cout << right[j].x << " " << right[j].y << "\n";
    return make_pair(i, j);
}
int cross(const Point& O, const Point& A, const Point& B) {
    int cross_product = (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
    if (cross_product == 0) {
        return 0; // points are collinear
    } else if (cross_product < 0) {
        return -1; // right turn
    } else {
        return 1; // left turn
    }
}


vector<Point> convex_hull2(vector<Point>& P) {
    int n = P.size();
    if (n <= 1) return P;

    /*
    sort(P.begin(), P.end(), [](const Point& a, const Point& b) {
        return a.y < b.y || (a.y == b.y && a.x < b.x);
    });
    */

    vector<Point> hull;
    hull.reserve(n);

    auto ccw = [](const Point& a, const Point& b, const Point& c) -> int {
        int area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        return area == 0 ? 0 : area > 0 ? 1 : -1;
    };

    hull.emplace_back(P[0]);
    hull.emplace_back(P[1]);
    for (int i = 2; i < n; i++) {
        while (hull.size() >= 2) {
            const Point& a = hull[hull.size() - 2];
            const Point& b = hull[hull.size() - 1];
            if (ccw(a, b, P[i]) <= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.emplace_back(P[i]);
    }

    int hull_size = hull.size();
    hull.emplace_back(P[n - 2]);
    for (int i = n - 3; i >= 0; i--) {
        while (hull.size() > hull_size) {
            const Point& a = hull[hull.size() - 2];
            const Point& b = hull[hull.size() - 1];
            if (ccw(a, b, P[i]) <= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.emplace_back(P[i]);
    }

    hull.pop_back();
    reverse(hull.begin(), hull.end());
    return hull;
}

vector<Point> convex_hull(vector<Point>& P) {
    int n = P.size();
    if (n <= 1)
        return P;

    sort(P.begin(), P.end(), [](Point& a, Point& b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    vector<Point> hull(2 * n);
    int k = 0;

    // Build lower hull
    for (int i = 0; i < n; i++) {
        while (k >= 2 && cross(hull[k-2], hull[k-1], P[i]) <= 0) k--;
        hull[k++] = P[i];
    }

    // Build upper hull
    for (int i = n-2, t = k+1; i >= 0; i--) {
        while (k >= t && cross(hull[k-2], hull[k-1], P[i]) <= 0) k--;
        hull[k++] = P[i];
    }

    hull.resize(k-1);
    reverse(hull.begin(), hull.end());

    return hull;
}

vector<Point> bitangent_hull_merge(vector<Point>& P) {
    int n = P.size();
    if (n <=5) return convex_hull(P);
    //if (n <= 5) return P;
    //if (n <= 2) return P;

    int mid = n / 2;
    vector<Point> left(P.begin(), P.begin() + mid);
    vector<Point> right(P.begin() + mid, P.end());

    vector<Point> L = bitangent_hull_merge(left);
    vector<Point> R = bitangent_hull_merge(right);

    int rightmost = 0, leftmost = 0;
    for (int i = 0; i < L.size(); i++) {
        if (L[i].x > L[rightmost].x) rightmost = i;
    }
    for (int i = 0; i < R.size(); i++) {
        if (R[i].x < R[leftmost].x) leftmost = i;
    }
    pair up = UpperTangent(L, R, rightmost, leftmost);

    int utl = up.first, utr = up.second;

    // Compute lower tangent
    pair down = LowerTangent(L, R, rightmost, leftmost);
    int ltl = down.first, ltr = down.second;


    // Remove points inside the tangents
    vector<Point> res;
    res.emplace_back(L[utl]);
    res.emplace_back(R[utr]);

    int i = mod((utr + 1), R.size()); // start at the next point in R after utr
    while (i != ltr) {
        res.emplace_back(R[i]);
        i = mod((i + 1), R.size()); // move to the next point in R
    }

    if (utr != ltr) {
        res.emplace_back(R[ltr]);
    }
    if(utl != ltl)
        res.emplace_back(L[ltl]);

    i = mod((ltl + 1), L.size()); // start at the next point in L after ltl
    while (i != utl) {
        res.emplace_back(L[i]);
        i = mod((i + 1), L.size()); // move to the next point in L
    }

    // Add the upper tangent point to close the hull
    //res.emplace_back(L[utl]);

    return res;
}

int main() {
    
    Input input;

    cin >> input.Ncity >> input.Nbakery >> input.th;

    vector<int> bakery;
    int val;
    for(int i = 0; i<input.Nbakery;i++)
    {
        cin >> val; 
        bakery.emplace_back(val);
    }
    vector<Point> cities;
    Point city;
    
    for(int i = 0; i<input.Ncity; i++)
    {
        cin >> city.x >> city.y >> city.a;
        city.index = i;
        cities.emplace_back(city);
    }

    sort(cities.begin(), cities.end(), [](const Point& c1, const Point& c2){
        return c1.x < c2.x;
    });

    vector<Point> H = bitangent_hull_merge(cities);
    // find leftmost point
    int leftmost = 0;
    for (int i = 0; i < H.size(); i++) {
        if (H[i].x < H[leftmost].x) leftmost = i;
    }
    int siz = H.size();
    vector<float> slope(H.size());
    for(int i = 0; i< H.size()-1; i++)
    {
        slope[i] = ((float)H[i+1].y - (float)H[i].y)/((float)H[i+1].x - (float)H[i].x);
        if(i > 0 && slope[i] == slope[i-1]) siz--;
        
    }
    // slide the window until reaching back to leftmost point
    cout << siz <<"\n";
    cout << H[leftmost].index << "->";
    //Point prev = H[leftmost];
    for (int i = H.size()-1; i >0; i--) {
        int index = mod(leftmost+i, H.size());
        if(slope[index] != slope[mod(index-1, H.size())])
        {
            //Point next = H[index];
            cout << H[index].index << "->";
        }
        
        //if(!isCollinear(prev, next, slope))
        //{
        //    
        //    slope = ((float)prev.y - (float)next.y)/((float)prev.x - (float)next.x);
        //    prev = next;
        //}
        
    }
    cout << H[leftmost].index << "\n"; 
   
  
    Point a, b;
    vector<Point> left, right;
    
    a.x = 2; a.y = 3;
    left.emplace_back(a);
    a.x = 4; a.y = 9;
    left.emplace_back(a);
    a.x = 6; a.y = 5;
    left.emplace_back(a);

    a.x = 7; a.y = 6;
    right.emplace_back(a);
    a.x = 8; a.y = 17;
    right.emplace_back(a);
    a.x = 14; a.y = 2;
    right.emplace_back(a);
    a.x = 18; a.y = 4;
    right.emplace_back(a);
    a.x = 19; a.y = 8;
    right.emplace_back(a);

   //vector<Point> ch = convex_hull(right);
   //for(auto a: ch)
   //{
   //    cout << a.x << " " << a.y << "\n";
   //}



   pair p1 = UpperTangent(left, right, 2, 0);
    pair p2 = LowerTangent(left, right, 2, 0);
    


    return 0;
}