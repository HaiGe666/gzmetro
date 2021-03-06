#include<iostream>
#include<cstdio>
#include<string>
#include<vector>
#include<climits>
#include<cstring>
#include<map>
#define INF 0x3fffffff
using namespace std;

struct Station {
	string name;
	vector<int> line;
	Station(string n = "fill") :name(n) {}
};

void transferWarshall(int min_transfer[][55])	//对换乘次数的warshall核心代码
{
	for (int k = 0; k<55; k++)
		for (int i = 0; i<55; i++)
			for (int j = 0; j<55; j++)
				if (min_transfer[i][j] > min_transfer[i][k] + min_transfer[k][j])
					min_transfer[i][j] = min_transfer[i][k] + min_transfer[k][j];
}

void pathWarshall(double shortest_path[][205], int priorMatrix[][205])	//对路径的的warshall核心代码
{
	for (int k = 1; k<205; k++)
		for (int i = 1; i<205; i++)
			for (int j = 1; j<205; j++)
				if (shortest_path[i][j] > shortest_path[i][k] + shortest_path[k][j]) {
                    shortest_path[i][j] = shortest_path[i][k] + shortest_path[k][j];
                    //track the path
                    priorMatrix[i][j] = priorMatrix[k][j];
                }

}

int main()
{
	int n, m;
	freopen("test.txt", "r", stdin);
	freopen("gzmetro.out", "w", stdout);
	scanf("%d%d", &n, &m);
	char station_name_c[75];	//站点名称
	char line_num[75];	//站点所在线路
	int min_transfer[55][55];   //save min_transfer data
	for (int i = 0; i<55; i++)	//初始化换乘邻接矩阵
		for (int j = 0; j<55; j++)
			min_transfer[i][j] = INF;
	for (int i = 0; i < 55; i++) {
		min_transfer[i][i] = 0;
	}
	Station station_set[205];   //set of all station，结构体数组
	int string_station = 54;
	map<string, int> string_station_num;
	for (int i = 1; i <= n; i++) {	//存储站点所在线路
		scanf("%s", station_name_c);
		station_set[i].name = station_name_c;
		while (scanf("%s", line_num) && line_num[0] != '0') {
			if (line_num[0] == 'A')	//APM line is seen as 0 line
				station_set[i].line.push_back(0);
			else if(atoi(line_num) == 0) {
				if(string_station_num.find(line_num) != string_station_num.end()) {
					station_set[i].line.push_back(string_station_num[line_num]);
				}
				else {	
					string_station_num[line_num] = string_station;
					station_set[i].line.push_back(string_station);
					string_station--;
				}
			}
			else
				station_set[i].line.push_back(atoi(line_num));
		}
		for (int j = 0; j<station_set[i].line.size(); j++)	//将换乘站点所在线路距离设为1
			for (int k = j + 1; k < station_set[i].line.size(); k++) {
				min_transfer[station_set[i].line[j]][station_set[i].line[k]] = min_transfer[station_set[i].line[k]][station_set[i].line[j]] = 1;
				/*if (station_set[i].line[k] == 0) {
					printf("%d %s ", i, station_set[i].name);
					for (int s = 0; s < station_set[i].line.size(); s++) {
						printf("%d ", station_set[i].line[s]);
					}
				}
				if (station_set[i].line[j] == 0) {
					printf("%d %s ", i, station_set[i].name);
					for (int s = 0; s < station_set[i].line.size(); s++) {
						printf("%d ", station_set[i].line[s]);
					}
				}*/
			}
	}
	double shortest_path[205][205];
	for (int i = 1; i<205; i++)		//初始化路径邻接矩阵
		for (int j = 1; j<205; j++)
			if(i != j)
                shortest_path[i][j] = 1000000;
            else
                shortest_path[i][j] = 0;
	int first, second;
	double dis;
	for (int i = 0; i<m; i++) {		//为邻接矩阵赋值
		scanf("%d%d%lf", &first, &second, &dis);
		shortest_path[first][second] = shortest_path[second][first] = dis;
	}
    // track the path
    int priorMatrix[205][205];
    for(int i=0; i<205; i++) {
        for(int j=0; j<205; j++) {
            if(shortest_path[i][j] != 1000000.d && i != j)
                priorMatrix[i][j] = i;
            else
                priorMatrix[i][j] = -1;
        }
    }
	pathWarshall(shortest_path, priorMatrix);
	transferWarshall(min_transfer);
	int q;
	scanf("%d", &q);
	int min;
	for (int i = 0; i<q; i++) {
		scanf("%d%d", &first, &second);
		if(shortest_path[first][second] >= 999999)
			printf("Not connected ");
		else
			printf("%.2lf ", shortest_path[first][second]);
		min = INF;
		for (int j = 0; j<station_set[first].line.size(); j++)	//从两站所在线路中找出次数最少的一个输出
			for (int k = 0; k<station_set[second].line.size(); k++)
				if (min > min_transfer[station_set[first].line[j]][station_set[second].line[k]])
					min = min_transfer[station_set[first].line[j]][station_set[second].line[k]];
		if(min >= INF)
			printf("Not connected\n");
		else
			printf("%d\n", min);
        //track the path
        printf("%d", second);
        second = priorMatrix[first][second];
        while(second != -1) {
            printf("<-%d", second);
            second = priorMatrix[first][second];
        }
        printf("\n");
	}
	return 0;
}
