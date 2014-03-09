#include <iostream>
#include <ctime>
#include <vector>
#include <cstdlib>  // for RAND_MAX
#include "neuron.cpp"
#define LEARNING_RATE 0.2
#define MAX_ERROR 0.1 

using namespace std;

vector<vector<float> > tt; // stores input provided after all the required processing
vector<vector<float> > t; // stores the expected output for the input provided. t[x][y] represents output of yth perceptron on xth input.

vector<vector<float> > output; // vector required to store the output of perceptrons in the feedForward phase.
vector<vector<neuron> > network;
void finalOutput(vector<float>& input,int hidno,int totalLayers) {
    if(hidno==0) output.clear();
    output.push_back(input);
    if(hidno==totalLayers) return ;
    vector<float>* out = new vector<float>();
    for(int i=0;i<network[hidno].size();i++)
        out->push_back(network[hidno][i].output(&input,"sigmoid"));

    input.~vector();
    finalOutput(*out,++hidno,totalLayers);
}

void store(vector<float>& dest, vector<float>* src) {
    int i=src->size();
    for(int j=0;j<i;j++)
        dest.push_back(src->at(j));
}

int main() {
    srand(time(NULL));
    cout <<"No. of hidden layers : ";
    int h;
    cin >> h;
    vector<vector<float> > delta;
    network.resize(h+1);
    delta.resize(h+1);
    int hn,prev_hn;
    cout << "Enter the no. of inputs : ";
    cin >> prev_hn;
    int numinput = prev_hn;
    cout << "Enter the no. of perceptrons in the hidden layers : ";
    for(int i=0;i<h;i++) {
        cin >> hn;
        delta[i].resize(hn);
        while(hn>0) {
            neuron* n = new neuron(prev_hn);
            network[i].push_back(*n);
            hn--;
        }
        prev_hn=network[i].size();
    }
    cout << "Enter the no. of perceptrons in the output layer : ";
    cin >> hn;
    delta[h].resize(hn);
    for(int i=0;i<hn;i++) {
        neuron* n = new neuron(prev_hn);
        network[h].push_back(*n);
    }

    /* The structure of the neural network is as follows:
       2D vector of neurons with h no. of hidden layers and one output layer. */
    cout << "Enter the truth table\n";
    //fillTruthTable(1<<numinput,numinput,hn); // arrange the truth table in tt and t vector.
    for(int i=0;i<(1<<numinput);i++)  { /*9322073910*/
	    vector<float> o;
	    int temp;
        for(int j=0;j<=numinput+hn;j++) {
            if(j<hn) {
                cin >> temp;
                o.push_back(temp);
            }
            else if(j==hn) {
                t.push_back(o);
                o.clear();
                cin >> temp;
                o.push_back(temp);
            }
            else if(j==numinput+hn) {
                o.push_back(-1);
                tt.push_back(o);
                o.clear();
            }
            else {
                cin >> temp;
                o.push_back(temp);
            }
        }
    }

    bool success=false;

    //Initializing the weight vectors with a random value
    for(int m=0;m<5;m++) {
        for(int i=0;i<h+1;i++) {
            for(int j=0;j<network[i].size();j++) {
                int pos=0;
                bool notfilled=true;
                while(notfilled) {
                    cout << "pos" << pos << endl;
                    notfilled=network[i][j].fillWtComponent(-1+2*((float)rand())/RAND_MAX, pos);
                    pos++;
                }
            }
        }


        float err;
        for(int i=0;i<100000;i++) {
            err=0;
            for(int testCase=0; testCase<tt.size();testCase++) {
                output.clear();
                finalOutput(tt[testCase],0,h+1); //feedForward complete
                cout << "Output table::\n";
                for(int q=0;q<output.size();q++) {
                    for(int e=0;e<output[q].size();e++) {
                        cout << output[q][e] << " " ;
                    }
                    cout << endl;
                }

                //backpropagation for the last layer
                //First delta calculation
                for(int outp=0; outp<network[h].size(); outp++) {
                    delta[h][outp]=(t[testCase][outp]-output[h+1][outp])*output[h+1][outp]*(1-output[h+1][outp]);
                    //Now update the weight
                    int wtComp=0;
                    //the while loop runs and it will retuen false when all the weght components are updated
                    while(network[h][outp].updateWtComponent(LEARNING_RATE*delta[h][outp]*output[h][wtComp],wtComp++));
                }

                //backpropagation for the remaining hidden layers
                for(int layer=h-1;layer>=0;layer--) {
                    for(int outp=0;outp<network[layer].size();outp++) {
                        cout << delta[layer].size() << endl;
                        delta[layer][outp]=0;
                        cout << "hoho" <<endl;
                        for(int nextp=0;nextp<network[layer+1].size();nextp++) {
                            delta[layer][outp] += network[layer+1][nextp].wtComponent(outp)*delta[layer+1][nextp];
                        }
                        delta[layer][outp] *= output[layer+1][outp]*(1-output[layer+1][outp]);
                        int wtComp=0;
                        while(network[layer][outp].updateWtComponent(LEARNING_RATE*delta[layer][outp]*output[layer][wtComp],wtComp++));
                    }
                }

                //error calculation
                for(int outp=0;outp<network[h].size();outp++) {
                    err += 0.5 * (t[testCase][outp]-output[h+1][outp])*(t[testCase][outp]-output[h+1][outp]);
                }
            }
            if(err < MAX_ERROR) {
                cout << "Error is : " << err << ". Algorithm successfully terminated\n";
                success=true; break;
            }
            else if(i==0) {
                cout << "Error large enough. Trying for a different set of initial values.\n";
                for(int layer=0;layer<h+1;layer++) {
                    for(int nump=0;nump<network[layer].size();nump++) {
                        int prevp=0;
                        while(network[layer][nump].fillWtComponent(-1+2*rand()/RAND_MAX,prevp++));
                    }
                }
            }
        }
        if(success) break;
    }

    if(!success) { cout << "Unable to find suitable weights and theta values for the network\n"; return 0; }
    while(1) {
        cout << "Give input : "<<endl;
        vector<float>* inp=new vector<float>();

        float x;
        for(int i=0;i<numinput;i++) {
            cin >> x;
            inp->push_back(x);
        }
        finalOutput(*inp,0,h+1);
        for(int i=0;i<h+1;i++) {
            for(int j=0;j<network[i].size();j++) {
                cout << output[i+1][j] << " ";
            }
            cout << "\n";
        }
        cin >> x;
        if(x==-1) return 0;
    }
    return 0;
}
