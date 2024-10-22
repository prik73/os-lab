#include<iostream>
#include<vector>
using namespace std;

bool isSafe(vector<vector<int>>&max, vector<vector<int>>&alloc, vector<int>&avail, int process, int numResources){
    vector<int> need(numResources);

    //calculating need matrix
    for(int i = 0; i < numResources;i++ ){
        need[i] = max[process][i] - alloc[process][i];
    }

    for(int i = 0; i< numResources; i++){
        if(need[i] > avail[i]){
            return false;
        }
    }
}
int main(void){
    
}
