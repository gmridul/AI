#include <iostream>
#include <vector>
#include "neuron.cpp"
#include "tt.cpp"
#define LEARNING_RATE 0.2
using namespace std;

vector<vector<float> > tt;
vector<vector<float> > t;
neuron*** network;
vector<int> layerSize;
vector<vector<float> > output; // vector required to store the output of perceptrons in the feedForward phase.
void finalOutput(vector<float>& input,int hidno) {
    cout << "hidno = " << hidno << endl;
    if(hidno==layerSize.size()-1) return ;
    vector<float>* out = new vector<float>();
    for(int i=0;i<layerSize[hidno];i++) {
        out->push_back(network[hidno][i]->output(&input));
        output[hidno][i]=out->at(i);
    }
    input.~vector();
    finalOutput(*out,++hidno);
}

void store(vector<float>& dest, vector<float>* src) {
    int i=src->size();
    for(int j=0;j<i;j++)
        dest.push_back(src->at(j));
}

int main() {
    cout << "No. of hidden layers : ";
    int h;
    output.resize(h+1);
    cin >> h;
    network = new neuron**[h+1];
    int hn,prev_hn;
    cout << "Enter the no. of inputs : ";
    cin >> prev_hn;
    int numinput = prev_hn;
    layerSize.push_back(prev_hn);
    cout << "Enter the no. of perceptrons in the hidden layers : ";
    for(int i=0;i<h;i++) {
        cin >> hn;
        output[i].resize(hn);
        layerSize.push_back(hn);
        network[i] = new neuron*[hn];
        for(int j=0;j<hn;j++)
            network[i][j] = new neuron(prev_hn);

        prev_hn = hn;
    }
    cout << "Enter the no. of perceptrons in the output layer : ";
    cin >> hn;
    output[h].resize(hn);
    layerSize.push_back(hn);
    for(int i=0;i<hn;i++) {
        network[h] = new neuron*[hn];
        network[h][i] = new neuron(prev_hn);
    }

    /* The structure of the neural network is as follows:
       2D array of neurons with h no. of hidden layers and one output layer. */
    cout << "Enter the truth table\n";
    fillTruthTable(1<<prev_hn,prev_hn,hn); // arrange the truth table in tt and t vector.

    // t vector is the desired output and o vector is the actual output

    vector<vector<float> > delta; // used to store the delta values in the backpropagation phase.
    int insize = tt.size(); //No. of total inputs
    float errDiff=0;
    vector<float> dw;
    delta.resize(h+1);
    for(int i=0;i<h+1;i++) {
        delta[i].resize(layerSize[i+1]);
    }
    //for(int i=0;i<100000;i++) {
    for(int ttentry=0;ttentry<insize ;ttentry++) {
        finalOutput(tt[ttentry],1);
        //For the output layer
        for(int pno=0;pno<layerSize[h+1];pno++) {
            dw.clear();
            delta[h][pno] = (t[ttentry][pno]-output[h][pno])*output[h][pno]*(1-output[h][pno]);
            for(int prevlayer=0;prevlayer<layerSize[h];prevlayer++) {
                cout << "AERA" <<endl;
                cout << delta[h][pno] << endl;
                cout << "AERA" <<endl;
                cout << h << endl;
                cout << output.size() << endl;
                cout << output[h-1].size() << endl;
                cout << output[h-1][prevlayer] << endl;
                dw.push_back(LEARNING_RATE*delta[h][pno]*output[h-1][prevlayer]);
            }
            network[h][pno]->updateWeight(&dw);
        }

        // For the remaining layers
        for(int layer=h;layer>0;layer--) {
            dw.clear();
            dw.resize(layerSize[layer-1]);
            for(int pno=0;pno<layerSize[layer];pno++) {
                float temp=0;
                for(int nextLayer=0;nextLayer<layerSize[layer+1];nextLayer++) {
                    temp += (network[layer+1][nextLayer]->wtComponent(pno)) * (delta[layer][nextLayer]);
                }
                delta[layer-1][pno] = temp*output[layer][pno]*(1-output[layer][pno]);
                for(int prevlayer=0;prevlayer<layerSize[layer-1];prevlayer++) {
                    if(layer-2>-1)
                        dw.push_back(LEARNING_RATE*delta[layer-1][pno]*output[layer-2][prevlayer]);
                    else
                        dw.push_back(LEARNING_RATE*delta[layer-1][pno]*tt[ttentry][prevlayer+layerSize[h+1]]);
                }
                network[h][pno]->updateWeight(&dw);
            }
        }
    }



    return 0;
}
