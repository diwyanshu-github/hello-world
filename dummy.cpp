#include<iostream>
using namespace std;
#include<vector>
#include<algorithm>

void printSubsets(vector<int>&ip, vector<vector<int>>&op,vector<int>temp, int i){
    if(i == ip.size()){
        op.push_back(temp);
        return;
    }


    temp.push_back(ip[i]);

    // include
    printSubsets(ip,op,temp,i+1);

    //not include
    temp.pop_back();
    int idx = upper_bound(ip.begin()+i,ip.end(),ip[i]) - ip.begin();
    printSubsets(ip,op,temp,idx);

}

void balance(int open, int close, string temp, vector<string>&res){
    if(close == 0){
        res.push_back(temp);
        return;
    }

    if(close >= open)
    balance(open,  close -1, temp + ')', res);
    if(open > 0)
    balance(open -1, close, temp + '(', res);
}

int main(){

    vector<int>ip = {1,2,2};
    vector<vector<int>>res;
    vector<int>temp;
    string s;
    vector<string>res2;
    balance(3,3,s,res2);
    // printSubsets(ip,res,temp,0);
    for(auto it:res2){
        for(auto i2: it){
            cout<<i2<<" ";
        }
        // cout<<it[1];
        cout<<endl;
    }
    cout<<"hello";
    return 0;
}