#include <iostream>
#include <vector>
#include <queue>

using namespace std;


typedef pair<int, int> pii;

int main(){

    priority_queue<pii, vector<pii>, greater<pii> > pq;

    pq.push(make_pair(50, 1));
    pq.push(make_pair(30, 2));
    pq.push(make_pair(60, 3));
    pq.push(make_pair(40, 4));

    cout << pq.top().first << " " << pq.top().second << endl;
    pq.pop();
    cout << pq.top().first << " " << pq.top().second << endl;
    pq.pop();
    cout << pq.top().first << " " << pq.top().second << endl;
    pq.pop();
    cout << pq.top().first << " " << pq.top().second << endl;
    pq.pop();
    return 0;
}