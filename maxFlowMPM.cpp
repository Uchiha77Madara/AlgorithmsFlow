#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
#include<climits>

using namespace std;

int maxFlow = 0;

void printGraph(vector<vector<pair<int,int>>> level_graph, int n, string kaunsa)
{
	cout << endl << kaunsa << " Graph: " << endl;
	for(int i=0;i<n-1; ++i)
	{
		int size = level_graph[i].size();

		cout << i << " --> ";		
		for(int j=0;j<size;++j)
			cout << level_graph[i][j].second << "(" << level_graph[i][j].first << ")  ";
		cout << endl;
	}
}

void levelGraph(vector<vector<pair<int,int>>> &level_graph, vector<vector<pair<int,int>>> graph, int n, int m)
{
	//vector<vector<pair<int,int>>> level_graph(n);
	queue<int> list;

	list.push(0);
	//level_graph.push_back(vector<pair<int,int>>());

	vector<int> vislevels(n);
	vislevels[0] = 0;
	vector<bool> visited(n);
	visited[0] = true;

	while(!list.empty())
	{
		int current = list.front();
		list.pop();
		int size = graph[current].size();

		for(int i=0; i<size; ++i)
		{
			if(graph[current][i].first != 0 && (!visited[graph[current][i].second] || vislevels[current] < vislevels[graph[current][i].second]))
			{	
				level_graph[current].push_back(graph[current][i]);

				if(!visited[graph[current][i].second])
				{
					vislevels[graph[current][i].second] = vislevels[current] + 1;
					visited[graph[current][i].second] = true;
					list.push(graph[current][i].second);
				}
			}
		}
	}


	cout << endl << "Level Graph: " << endl;
	for(int i=0;i<n-1; ++i)
	{
		int size = level_graph[i].size();

		cout << i << " --> ";		
		for(int j=0;j<size;++j)
			cout << level_graph[i][j].second << "(" << level_graph[i][j].first << ")  ";
		cout << endl;
	}

}

void dfs(vector<vector<pair<int,int>>> &level_graph, int n, int current, int &sum, int vertex, int weight, bool &flag, bool &flagend)
{
	if(current == vertex)
	{
		flag = true;
		sum = weight;
	}

	if(current == n-1)
	{
		if(flag)
			flagend = true;

		maxFlow += sum;
		return;
	}

	int size = level_graph[current].size();
	for(int i=0;i<size; ++i)
	{
		if(flag && flagend)
			return;

		dfs(level_graph, n, level_graph[current][i].second, sum, vertex, weight, flag, flagend);
		level_graph[current][i].first -= sum;
	}
}

pair<int,int> minPhi(vector<vector<pair<int,int>>> level_graph, int n)
{
	vector<int> phi_in(n);
	vector<int> phi_out(n);

	for(int i=0;i<n; ++i)
	{
		int size = level_graph[i].size();
		for(int j=0;j<size; ++j)
		{
			phi_in[level_graph[i][j].second] += level_graph[i][j].first;
			phi_out[i] += level_graph[i][j].first;
		}
	}

	cout << endl << "PhiIn -- PhiOut" << endl;
	for(int i=0;i<n;++i)
		cout << phi_in[i] << " " << phi_out[i] << endl;

	int index = -1;
	int mini = INT_MAX;

	for(int i=1;i<n-1; ++i)
	{
		if(phi_in[i] == 0 || phi_out[i] == 0)
			continue;

		if(phi_in[i] < mini)
		{
			mini = phi_in[i];
			index = i;
		}

		if(phi_out[i] < mini)
		{
			mini = phi_out[i];
			index = i;
		}
	}

	cout << endl << "mini PhiIn/PhiOut: " << index << " with weight: " << mini << endl;

	return make_pair(index, mini);
}

void makeChangesToOrginal(vector<vector<pair<int,int>>> level_graph, vector<vector<pair<int,int>>> &graph, int n)
{
	for(int i=0;i<n-1; ++i)
	{
		int levelsize = level_graph[i].size();

		if(levelsize == 0)
			continue;

		int size = graph[i].size();

		int index = 0;
		for(int j=0;j<size && index<levelsize; ++j)
		{
			if(level_graph[i][index].second == graph[i][j].second)
			{
				graph[i][j].first = level_graph[i][index].first;
				++ index;
			}		
		}
	}
}

void removeFromLevelGraph(vector<bool> &visited, vector<vector<pair<int,int>>> &level_graph, int n)
{

	vector<int> phi_in(n);
	vector<int> phi_out(n);
 
	for(int i=0;i<n; ++i)
	{
		int size = level_graph[i].size();
		for(int j=0;j<size; ++j)
		{
			phi_in[level_graph[i][j].second] += level_graph[i][j].first;
			phi_out[i] += level_graph[i][j].first;
		}
	}

	bool flag = false;
	for(int i=1;i<n-1; ++i)
	{
		if(!visited[i] && (phi_in[i] == 0 || phi_out[i] == 0))
		{
			visited[i] = true;
			flag = true;
			for(int j=0; j<n-1; ++j)
			{
				int size = level_graph[j].size();
				for(int k=0;k<size; ++k)
				{
					if(level_graph[j][k].second == i)
						level_graph[j].erase(level_graph[j].begin()+k);

				}
			}
		}
	}

	if(flag)
		removeFromLevelGraph(visited, level_graph, n);
}

void removeStart(vector<vector<pair<int,int>>> &level_graph, int n)
{
	for(int i=0;i<n-1; ++i)
	{
		int size = level_graph[i].size();
		for(int j=0;j<size;++j)
		{
			if(level_graph[i][j].first == 0)
				level_graph[i].erase(level_graph[i].begin()+j);
		}
	}

	vector<bool> visited(n);
	removeFromLevelGraph(visited, level_graph, n);
}

bool reachability(vector<vector<pair<int,int>>> graph, vector<bool> &visited, int current, int n)
{
	visited[current] = true;
	if(current == n-1)
		return true;

	int size = graph[current].size();
	for(int i=0;i<size; ++i)
	{
		if(!visited[graph[current][i].second] && graph[current][i].first !=0 && reachability(graph, visited, graph[current][i].second, n))	
			return true;
	}

	return false;
}

bool isReachable(vector<vector<pair<int,int>>> graph, int n)
{
	vector<bool> visited(n);
	return reachability(graph, visited, 0, n);
}

void MPM(vector<vector<pair<int,int>>> &graph, int n, int m)
{
	vector<vector<pair<int,int>>> level_graph(n);
	levelGraph(level_graph, graph, n, m);

	removeStart(level_graph, n);

	while(isReachable(level_graph, n))
	{
		pair<int,int> minindex = minPhi(level_graph, n);

		int sum = 0;
		bool flag = false, flagend = false;
		dfs(level_graph, n, 0, sum, minindex.first, minindex.second, flag, flagend);

		printGraph(level_graph, n, "Level");

		makeChangesToOrginal(level_graph, graph, n);

		printGraph(graph, n, "Original");

		removeStart(level_graph, n);
	}
}

int main(int argc,char* argv[])
{
	ifstream file;
	file.open(argv[1]);
	
	int n, m;
	
	file >> n >> m;

	vector<vector<pair<int,int>>> graph(n);

	int u, v, w;
	for(int i=0;i<m;++i)
	{
		file >> u >> v >> w;
		graph[u-1].push_back(make_pair(w, v-1));
	}

	printGraph(graph, n, "Original");

	int count = 2;
	while(isReachable(graph, n))
	{
		MPM(graph, n, m);
		printGraph(graph, n, "Originalmain");
	}

	cout << endl << "The value of max flow is : " << maxFlow << endl;

	file.close();
	return 0;
}