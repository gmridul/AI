#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

class neuron {
    float* w;
    int n;

    public:
    neuron(int n) {
        w = new float[n+1];
        this->n = n;
        for(int i=0;i<n+1;i++)
            w[i] = 0;

    }

    void fillWeight(vector<float>* q, float theta) {
        for(int i=0;i<n;i++)
            w[i] = (*q)[i];

        w[n] = theta;
    }

    void updateWeight(vector<float>* dw) {
        for(int i=0;i<n+1;i++)
            w[i] += (*dw)[i];
    }

    int output(vector<float>* x) {
        int res=0;
        int temp;
        for(int i=0;i<n+1;i++) {
            temp = (*x)[i];
            res+=temp*(w[i]);
        }

        if(res > 0) return 1;
        else return 0;
    }

    float output(vector<float>* x, string s) {
        cout << "x.size" << x->size() << endl;
        cout << "n"<<n<<endl;
        if(s!="sigmoid") return -1;
        float res=0;
        for(int i=0;i<n+1;i++) 
            res+=w[i]*(*x)[i];
        return (1/(1+exp(-res)));
    }

    bool fillWtComponent(float x, int pos) {
        if(pos==n+1) return false;
        w[pos]=x;
        return true;
    }

    bool updateWtComponent(float x, int pos) {
        if(pos==n+1) return false;
        w[pos]+=x;
        return true;
    }

    float wtComponent(int i) {
        return  w[i];
    }

    void printWeight() {
        for(int i=0;i<=n;i++) cout << w[i] << " ";
        cout << "\n";
    }
};
