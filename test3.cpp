#include <vector>
#include <iostream>
#include <set>

using namespace std;

// stores the centre of polygon (It is made
// global because it is used in compare function)
pair<int, int> centroid;
 
// determines the quadrant of a point
// (used in compare())
int quad(pair<int, int> p)
{
    if (p.first >= 0 && p.second >= 0)
        return 1;
    if (p.first <= 0 && p.second >= 0)
        return 2;
    if (p.first <= 0 && p.second <= 0)
        return 3;
    return 4;
}
 
// compare function for sorting
bool compare(pair<int, int> p1, pair<int, int> q1)
{
    pair<int, int> p = make_pair(p1.first - centroid.first,
                                p1.second - centroid.second);
    pair<int, int> q = make_pair(q1.first - centroid.first,
                                q1.second - centroid.second);
 
    int one = quad(p);
    int two = quad(q);
 
    if (one != two)
        return (one < two);
    return (p.second*q.first < q.second*p.first);
}

// Brute force algorithm to find convex hull for a set
// of less than 6 points
vector<pair<int, int>> bruteHull(vector<pair<int, int>> points)
{
    // Take any pair of points from the set and check
    // whether it is the edge of the convex hull or not.
    // if all the remaining points are on the same side
    // of the line then the line is the edge of convex
    // hull otherwise not
    set<pair<int, int>> hullPoints;
 
    for (int i = 0; i < points.size(); i++)
    {
        for (int j = i + 1; j < points.size(); j++)
        {
            int x1 = points[i].first, x2 = points[j].first;
            int y1 = points[i].second, y2 = points[j].second;
 
            int a = y1 - y2;
            int b = x2 - x1;
            int c = x1 * y2 - y1 * x2;
            int numPointsOnPositiveSide = 0, numPointsOnNegativeSide = 0;
            for (int k = 0; k < points.size(); k++)
            {
                if (a * points[k].first + b * points[k].second + c <= 0)
                {
                    numPointsOnNegativeSide++;
                }
                if (a * points[k].first + b * points[k].second + c >= 0)
                {
                    numPointsOnPositiveSide++;
                }
            }
            if (numPointsOnPositiveSide == points.size() || numPointsOnNegativeSide == points.size())
            {
                hullPoints.insert(points[i]);
                hullPoints.insert(points[j]);
            }
        }
    }
 
    vector<pair<int, int>> hullVector;
    for (auto hullPoint : hullPoints)
    {
        hullVector.push_back(hullPoint);
    }
 
    // Sorting the points in the anti-clockwise order
    centroid = {0, 0};
    int numPointsOnHull = hullVector.size();
    for (int i = 0; i < numPointsOnHull; i++)
    {
        centroid.first += hullVector[i].first;
        centroid.second += hullVector[i].second;
        hullVector[i].first *= numPointsOnHull;
        hullVector[i].second *= numPointsOnHull;
    }
    sort(hullVector.begin(), hullVector.end(), compare);
    for (int i = 0; i < numPointsOnHull; i++)
    {
        hullVector[i] = make_pair(hullVector[i].first / numPointsOnHull, hullVector[i].second / numPointsOnHull);
    }
 
    return hullVector;
}


vector<pair<int, int>> merger(vector<pair<int, int> > a,
                            vector<pair<int, int> > b)
{
    // n1 -> number of points in polygon a
    // n2 -> number of points in polygon b
    int n1 = a.size(), n2 = b.size();
 
    int ia = 0, ib = 0;
    for (int i=1; i<n1; i++)
        if (a[i].first > a[ia].first)
            ia = i;
 
    // ib -> leftmost point of b
    for (int i=1; i<n2; i++)
        if (b[i].first < b[ib].first)
            ib=i;
 
    // finding the upper tangent
    int inda = ia, indb = ib;
    bool done = 0;
    while (!done)
    {
        done = 1;
        while (orientation(b[indb], a[inda], a[(inda+1)%n1]) >=0)
            inda = (inda + 1) % n1;
 
        while (orientation(a[inda], b[indb], b[(n2+indb-1)%n2]) <=0)
        {
            indb = (n2+indb-1)%n2;
            done = 0;
        }
    }
 
    int uppera = inda, upperb = indb;
    inda = ia, indb=ib;
    done = 0;
    int g = 0;
    while (!done)//finding the lower tangent
    {
        done = 1;
        while (orientation(a[inda], b[indb], b[(indb+1)%n2])>=0)
            indb=(indb+1)%n2;
 
        while (orientation(b[indb], a[inda], a[(n1+inda-1)%n1])<=0)
        {
            inda=(n1+inda-1)%n1;
            done=0;
        }
    }
 
    int lowera = inda, lowerb = indb;
    vector<pair<int, int>> ret;
 
    //ret contains the convex hull after merging the two convex hulls
    //with the points sorted in anti-clockwise order
    int ind = uppera;
    ret.push_back(a[uppera]);
    while (ind != lowera)
    {
        ind = (ind+1)%n1;
        ret.push_back(a[ind]);
    }
 
    ind = lowerb;
    ret.push_back(b[lowerb]);
    while (ind != upperb)
    {
        ind = (ind+1)%n2;
        ret.push_back(b[ind]);
    }
    return ret;
 
}