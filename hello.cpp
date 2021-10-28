#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

vector<vector<int>> bicycleYard(vector<int>& position, vector<vector<int>>& terrain, vector<vector<int>>& obstacle) {
        int n = terrain.size();
        int m = terrain[0].size();
        vector<vector<vector<int>>> vis(n,vector<vector<int>> (m,vector<int> (4,0)));
        vector<vector<int>> res;
        int r = position[0],c = position[1];
        queue<vector<int>> neighbors;//x,y,speed
        neighbors.push({r, c, 1});
        vis[r][c][0] = 1;
        vis[r][c][1] = 1;
        vis[r][c][2] = 1;
        vis[r][c][3] = 1;
        while(!neighbors.empty()){
            auto rc = neighbors.front();
            neighbors.pop();
            int row = rc[0], col = rc[1];
            int h1 = terrain[row][col];
            if (row - 1 >= 0) {
                if(!vis[row-1][col][0]){
                    int h2 = terrain[row-1][col];
                    int o2 = obstacle[row-1][col];
                    int speed = rc[3] - (h1 - h2 - o2);
                    vis[row-1][col][0] = 1;
                    if(speed > 0){
                        if(speed == 1){
                            res.push_back({row-1,col});
                        }
                        neighbors.push({row-1,col,speed});
                    }
                }
            }
            if (row + 1 < n) {
                if(!vis[row+1][col][2]){
                    int h2 = terrain[row+1][col];
                    int o2 = obstacle[row+1][col];
                    int speed = rc[3] - (h1 - h2 - o2);
                    vis[row+1][col][2] = 1;
                    if(speed > 0){
                        if(speed == 1){
                            res.push_back({row+1,col});
                        }
                        neighbors.push({row+1,col,speed});
                    }
                }
            }
            if (col - 1 >= 0) {
                if(!vis[row][col-1][1]){
                    int h2 = terrain[row][col-1];
                    int o2 = obstacle[row][col-1];
                    int speed = rc[3] - (h1 - h2 - o2);
                    vis[row][col-1][1] = 1;
                    if(speed > 0){
                        if(speed == 1){
                            res.push_back({row,col-1});
                        }
                        neighbors.push({row,col-1,speed});
                    }       
                }
            }
            if (col + 1 < m) {
                if(!vis[row][col+1][3]){
                    int h2 = terrain[row][col+1];
                    int o2 = obstacle[row][col+1];
                    int speed = rc[3] - (h1 - h2 - o2);
                    vis[row][col+1][3] = 1;
                    if(speed > 0){
                        if(speed == 1){
                            res.push_back({row-1,col});
                        }
                        neighbors.push({row,col+1,speed});
                    }
                }
            }
        }
        return res;
    }


int main()
{
    vector<int> position = {0,0};
    vector<vector<int>> terrain = {{0,0},{0,0}};
    vector<vector<int>> obstacle = {{0,0},{0,0}};
    vector<vector<int>> res;
    res = bicycleYard(position,terrain,obstacle);
    return 0;
}