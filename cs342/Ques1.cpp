#include <bits/stdc++.h>
using namespace std;

#define MAX_ROUTERS 10
#define no_parent = -1;

class router
{
public:
	int router_id;

	vector<pair<router *, int>> neighbours;

	unordered_map<int, int> routing_table;

	void add_neighbour(router *new_neighbour, int weight)
	{
		neighbours.push_back(make_pair(new_neighbour, weight));
	}
	vector<int> dist, parent;
	void update_routing_table(router *src, int V)
	{
		priority_queue<pair<int, router *>, vector<pair<int, router *>>, greater<pair<int, router *>>> pq;

		for (int i = 0; i < V; i++)
		{
			dist.push_back(INT_MAX);
			parent.push_back(-1);
		}

		pq.push(make_pair(0, src));
		dist[src->router_id] = 0;

		while (!pq.empty())
		{

			router *u = pq.top().second;
			pq.pop();
			for (auto i = u->neighbours.begin(); i != u->neighbours.end(); ++i)
			{
				router *v = (*i).first;
				int weight = (*i).second;

				if (dist[v->router_id] > dist[u->router_id] + weight)
				{
					dist[v->router_id] = dist[u->router_id] + weight;
					parent[v->router_id] = u->router_id;
					pq.push(make_pair(dist[v->router_id], v));
				}
			}
		}
	}

	void print_parents(int a)
	{
		if (a == -1)
			return;
		cout << " <- " << a ;
		print_parents(parent[a]);
	}

	void print_routing_table(router *src, int n)
	{
		cout << "Routing table of R" << src->router_id << "	Shortest Distance	Path" << endl;
		for (int i = 0; i < n; ++i)
		{	if(dist[i] == INT_MAX){
				cout << "R" << i << "	" << "Not Reachable" << endl;
				
			}
			else{
			cout << "R" << i << "			" << dist[i] << "			";
			cout << i ;
			print_parents(parent[i]);
			cout << endl;
			}
		}
	}
};

int main()
{

	int n;
	cout << "Enter no.of routers" << endl;
	cin >> n;
	router *arr[n];
	for (int i = 0; i < n; i++)
	{
		arr[i] = new router;
		arr[i]->router_id = i;
	}
	int k;
	cout << "no. of edges" << endl;
	cin >> k;

	cout << "give the two routers and weight b/w them" << endl;

	for (int i = 0; i < k; i++)
	{
		int x, y, w;
		cin >> x >> y >> w;
		arr[x]->add_neighbour(arr[y], w);
		arr[y]->add_neighbour(arr[x], w);
	}

	for (int i = 0; i < n; i++)
	{

		arr[i]->update_routing_table(arr[i], n);
		arr[i]->print_routing_table(arr[i], n);
		cout << endl;
	}
	return 0;
}
