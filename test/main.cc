#include <iostream>
#include <vector>
#include <climits>

using namespace std;
class Solution {
private:
  int count;
  vector<int> disabled;
  int getMaxDistance(vector<vector<int>> &graph) {
    auto V = graph.size();
    if (V <= 1) {
      return 0;
    }
    auto INF = INT_MAX;
    int dist[V][V], i, j, k;

    for (i = 0; i < V; i++)
      for (j = 0; j < V; j++)
        dist[i][j] = graph[i][j];

    for (k = 0; k < V; k++) {
      for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
          if (dist[i][k] != INF && dist[k][j] != INF &&
              dist[i][k] + dist[k][j] < dist[i][j])
            dist[i][j] = dist[i][k] + dist[k][j];
        }
      }
    }

    int maxDist = 0;
    for (i = 0; i < V; i++) {
      for (j = 0; j < V; j++) {
        if (i == j) continue;
        if (dist[i][j] == INF) return -1;
        if (dist[i][j] > maxDist && dist[i][j] != INF)
          maxDist = dist[i][j];
      }
    }

    return maxDist;
  }
  void dfs(int n, int maxDistance, vector<vector<int>> &map) {
    if (n == map.size()) {
      int newMapSize = map.size();
      for (auto t : disabled) {
        newMapSize -= t;
      }
      auto copy =
          vector<vector<int>>(newMapSize, vector<int>(newMapSize, INT_MAX));
      for (int i = 0, _i = 0; i < map.size(); i++) {
        if (disabled[i])
          continue;
        for (int j = 0, _j = 0; j < map.size(); j++) {
          if (disabled[j])
            continue;
          copy[_i][_j] = map[i][j];
          _j++;
        }
        _i++;
      }
      int r = getMaxDistance(copy);
      if (r <= maxDistance && r >= 0) {

        count++;
      }
      return;
    }
    disabled[n] = 0;
    dfs(n + 1, maxDistance, map);
    disabled[n] = 1;
    dfs(n + 1, maxDistance, map);
  }

public:
  int numberOfSets(int n, int maxDistance, vector<vector<int>> &roads) {
    auto map = vector<vector<int>>(n, vector<int>(n, INT_MAX));
    for (auto road : roads) {
      map[road[0]][road[1]] = min(map[road[0]][road[1]], road[2]);
      map[road[1]][road[0]] = min(map[road[1]][road[0]], road[2]);
    }
    count = 0;
    disabled = vector<int>(n, 0);
    dfs(0, maxDistance, map);
    return count;
  }
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  Solution s;
  vector<vector<int>> roads = {{0,1,1},{1,2,1},{2,0,1}};
  cout << s.numberOfSets(3, 2, roads) << endl;
  return 0;
}
