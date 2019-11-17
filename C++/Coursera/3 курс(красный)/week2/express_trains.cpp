#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <set>

using namespace std;

class RouteManager
{
	map<int, set<int>> reachable_lists_;
public: 
	void AddRoute(int start, int finish) // log(N)
	{
		reachable_lists_[start].insert(finish);
		reachable_lists_[finish].insert(start);
	}
	int FindNearestFinish(int start, int finish) const  // log(N)
	{
		int distance = abs(finish - start); // расстояние между точками не на экспрессе
		if (reachable_lists_.count(start) == 1)
		{
			const set<int>& reachable_stations = reachable_lists_.at(start);
			auto nearest = reachable_stations.lower_bound(finish);
			if (nearest != reachable_stations.end())
			{ // если есть станция справа от finish
				distance = min(distance, abs(*nearest - finish));
			}
			if (nearest != reachable_stations.begin())
			{ /// если есть станция слева от finish
				distance = min(distance, abs(*prev(nearest) - finish));
			}
		}
		return distance;
	}
};


int main()
{
	RouteManager routes;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {		// Q *  log(N)
		string query_type;
		cin >> query_type;
		int start, finish;
		cin >> start >> finish;
		if (query_type == "ADD") {
			routes.AddRoute(start, finish);								// logN
		}
		else if (query_type == "GO") {
			cout << routes.FindNearestFinish(start, finish) << "\n";	//  log(N)
		}
	}

	return 0;
}
