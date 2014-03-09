#include <iostream>
#include <vector>
#include <cmath>
#include "neuron.cpp"
#include "tt.cpp"
using namespace std;

vector<vector<int> > tt;
int main() {
    vector<neuron> player;
    vector<int> iw;
    int n=7;
    iw.push_back(0);
    for(int i=1;i<n;i++) {
        iw.push_back(1);
    }
    for(int i=0;i<10;i++) {
        neuron* P = new neuron(7);
        P->fillWeight(&iw,0);
        player.push_back(*P);
    }

    int p = pow(2,7);
    for(int m=0; m<10; m++) {
        fillTruthTable(p,n);
        cout << tt.size() << endl;
        for(int i=0;i<128;i++) {
            if(player[m].output(&tt[i])==0) {
                player[m].updateWeight(&tt[i]);
                i=0;
            }
        }
    }
    vector<int> input;
    int k;

    while(k!=-1) {
        for(int i=0;i<7;i++) {
            cin >> k;
            input.push_back(k);
        }
        input.push_back(-1);
        for(int i=0;i<10;i++)
            cout << player[i].output(&input) << endl;
        cin >> k;
    }
    return 0;
}


