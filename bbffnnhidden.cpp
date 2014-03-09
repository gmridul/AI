#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>

#define LEARNING_RATE 0.5
#define MAX_ERROR 0.02

//input.txt -> XOR
//input2.txt -> NAND
//input3.txt -> palindrome
//input4.txt -> parity
using namespace std;

vector<vector<float> > tt; 
vector<vector<float> > t;

float sigmoid(float net) {return 1/(1+exp(net*(-1)));}

int main(){
//    float LEARNING_RATE, MAX_ERROR;
//    cin >> LEARNING_RATE >> MAX_ERROR;

    srand(time(NULL));

    vector<vector<vector<float> > > weights;
    vector<vector<float> > theta;
    vector<vector<float> > delta;
    vector<vector<float> > output;
    cout<<"Enter the no. of hidden layers"<<endl;
    int layers;
    cin >> layers;
    layers++;
    weights.resize(layers);
    theta.resize(layers);
    delta.resize(layers);
    output.resize(layers+1);

    cout<<"No. of perceptrons in each layer with the number of inputs to the perceptrons in that layer"<<endl;
    int per,inputper;
    for(int i=0;i<layers;i++) {
        cin >> per >> inputper;
        theta[i].resize(per);
        weights[i].resize(per);
        delta[i].resize(per);
        output[i+1].resize(per+1);
        while(per-->0) {
            weights[i][per].resize(inputper);
            for(int j=0;j<inputper;j++) {
                weights[i][per][j]=-1+2*((float)rand())/RAND_MAX;
            }
            delta[i][per]=0;
        }
    }

    cout<<"No. of outputs"<<endl;
    int outp;
    cin >> outp;

    cout<<"No. of inputs"<<endl;
    int inp;
    cin >> inp;
    output[0].resize(inp);

    //Now, we need the basic neural net structure

    float net, error_bound, error=0;

    // theta : 2D vector with [layer][perceptron_no] storing the 'theta' value at that place
    // output : 2D vector with [layer][perceptron_no] storing the input at that perceptron, or the output from [layer-1][perceptron_no]
    // delta : 2D vector with [layer][perceptron_no] storing the delta value at that perceptron needed in BP phase
    // weights : 3D vector with [layer][perceptron_no] storing the vector of weights for inputs at that place

    //Now I have a 2D representation of perceptrons in x.y coordinate form
    //Neural nets is actually the theta value of that perceptron, with weights keeping how many weights are needed for that perceptron.

    
    tt.resize(1<<inp);
    t.resize(1<<inp);
    for(int i=0; i<1<<inp; i++) {
        t[i].resize(outp);
    }

    //Filling the truth table and the expected output table
    for(int i=0; i<(1<<inp); i++) {
        tt[i].resize(inp);
        for(int j=0; j<outp; j++) {
            cin >> t[i][j];
        }
        for(int j=0; j<inp; j++) {
            cin >> tt[i][j];
        }

    }


    for(int trials=0;trials<16;trials++) {
        for(int iter=0;iter<100000;iter++) {
            cout << error << "," << iter << endl;
            error = 0;
            for(int i=0; i<(1<<inp); i++) {
                for(int j=0; j<tt[i].size(); j++) {
                    output[0][j] = tt[i][j];
                }			

                //Feed-forward on the network to generate outputs for all the perceptrons
                for(int j=0; j<layers; j++) {
                    for(int k=0; k<theta[j].size() ; k++) {
                        net = 0;
                        for(int l=0; l<weights[j][k].size() ;l++) {
                            net += output[j][l]*weights[j][k][l];
                        }
                        net += theta[j][k];
                        output[j+1][k] = sigmoid(net);
                    }
                }
                //Outputs of layer j are now stored in layer j+1

                //Now the back propagation needs to be done
                //Do it for last layer
                for(int j=0; j<theta[layers-1].size() ;j++) {
                    delta[layers-1][j] = (t[i][j] - output[layers][j])*output[layers][j]*(1 - output[layers][j]);
                    for(int k=0; k<weights[layers-1][j].size(); k++) {
                        weights[layers-1][j][k] += LEARNING_RATE*delta[layers-1][j]*output[layers-1][k];
                    }
                    theta[layers-1][j] += LEARNING_RATE*delta[layers-1][j];
                }

                //Now do it for all the other layers using last layer's values
                //For all perceptrons in a certain layer
                for(int j=layers-2; j>=0; j--) {
                    for(int k=0; k<theta[j].size(); k++) {
                        delta[j][k] = 0;
                        for(int l=0; l<theta[j+1].size(); l++) {
                            delta[j][k] += weights[j+1][l][k]*delta[j+1][l];
                        }
                        delta[j][k] *= output[j+1][k]*(1 - output[j+1][k]);
                        //Weight update here
                        for(int l=0; l<weights[j][k].size(); l++) {
                            weights[j][k][l] += LEARNING_RATE*delta[j][k]*output[j][l];
                        }
                        //Theta update here
                        theta[j][k] += LEARNING_RATE*delta[j][k];
                    }
                }

                for(int j=0; j<outp; j++) {
                    error += 0.5*(t[i][j] - output[layers][j])*(t[i][j] - output[layers][j]);
                }
            }
            cout << "Error is " << error << endl;
            if(error < MAX_ERROR) {
                cout << "Completed operation!"<<endl;	
                int x=0;
                while(x>=0) {
                    for(int testinp=0;testinp<inp;testinp++) {
                        cin>>output[0][testinp];
                    }
                    for(int j=0; j<layers; j++) {
                        for(int k=0; k<theta[j].size() ; k++) {
                            net = 0;
                            for(int l=0; l<weights[j][k].size() ;l++) {
                                net += output[j][l]*weights[j][k][l];
                            }
                            net += theta[j][k];
                            output[j+1][k] = sigmoid(net);
                            cout << output[j+1][k] << " ";
                        }
                        cout << "\n";
                    }    
                    cout << "\n";
                    cin >> x;
                }

                return 0;
            }
            if(iter == 99999) {
                cout << "Could not converge. Resetting initial weights..."<<endl;
                sleep(2);
                for(int u=0; u<layers; u++)
                    for(int v=0; v<weights[u].size(); v++)
                        for(int w=0; w<weights[u][v].size(); w++)
                            weights[u][v][w] = -1+2*((float)rand())/RAND_MAX;
            }
        }
    }
    cout << "Could not find suitable weights\n";
    return 0;
}

