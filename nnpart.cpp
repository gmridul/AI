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

float sigmoid(float net) {return 1/(1+exp(-net));}

int main(){

    srand(time(NULL));

    cout<<"Enter the number of layers"<<endl;
    cout<<"Enter the number of perceptrons in each layer"<<endl;
    /*cout<<"For example, a sample input structure for XOR training is : "<<endl;
    cout<<"\t3"<<endl;
    cout<<"\t2 2"<<endl;
    cout<<"\t2 2"<<endl;
    cout<<"\t1 2"<<endl;*/

    //Note this is basic FFNN with proper hierarchy maintained and no links allowed from say layer 1 to layer 3, i.e. over jumps of more than 1

    //Now, we need the basic neural net structure

    int input, inputs, input2, layers, outputs;
    float net, err;
    cin >> input;
    layers = input;

    // theta : 2D vector with [layer][perceptron_no] storing the 'theta' value at that place
    // output     : 2D vector with [layer][perceptron_no] storing the input at that perceptron, or the output from [layer-1][perceptron_no]
    // delta      : 2D vector with [layer][perceptron_no] storing the delta value at that perceptron needed in BP phase
    // weight    : 3D vector with [layer][perceptron_no] storing the vector of weight for inputs at that place

    vector< vector<float> > theta;
    vector< vector<float> > delta;
    vector< vector<float> > output;
    theta.resize(input);
    output.resize(input+1);
    delta.resize(input);
    vector< vector< vector<float> > > weight;
    weight.resize(input);

    int temp = input;
    //Input is here, initializing the vectors
    for(int i=0; i<temp; i++)
    {
        cin >> input;
        theta[i].resize(input);
        delta[i].resize(input);
        output[i+1].resize(input);
        for(int j=0; j<input; j++)
            theta[i][j] = 0;
        weight[i].resize(input);
        cin >> input2;
        for(int j=0; j<input; j++)
        {
            weight[i][j].resize(input2);
            for(int k=0; k<input2; k++)
            {
                weight[i][j][k] = -1+2*((float)rand())/RAND_MAX;
            }
        }
    }

    //Now I have a 2D representation of perceptrons in x.y coordinate form
    //Neural nets is actually the theta value of that perceptron, with weight keeping how many weight are needed for that perceptron.

    //Now for some test cases storage

    vector< vector<float> > tests; //1st coordinate is the number of examples, 2nd coordinate is the example input vals, 
    vector< vector<float> > exp_output; //Expected outputs stored here

    cin >> input;           //Number of test cases, this is
    tests.resize(input);
    exp_output.resize(input);
    cin >> inputs;          //Number of inputs, this is
    output[0].resize(inputs);
    cin >> outputs;         //Number of outputs, this is (example : the LED one has 10 inputs XOXO)
    for(int i=0; i<input; i++)
    {
        exp_output[i].resize(outputs);
    }

    //For each test case do
    for(int i=0; i<input; i++)
    {
        tests[i].resize(inputs);
        for(int j=0; j<inputs; j++)
        {
            cin >> tests[i][j];
        }
        for(int j=0; j<outputs; j++)
        {
            cin >> exp_output[i][j];
        }
    }

    //Now, basic data is all available
    //Note that currently this just trains the data for 1 run of each input
    //Need to include err bounds and multiple iterations 
    //^Maybe done that now

    int iter = 100000;
    int trials = 16;
    while(iter>0)
    {
        err = 0;
        for(int i=0; i<input; i++)
        {
            for(int j=0; j<tests[i].size(); j++)
            {
                output[0][j] = tests[i][j];
            }           
            //Firstly, feed-forward part of the algo is done, now
            //Here, I will feed-forward on the network to generate outputs for all the perceptrons
            for(int j=0; j<layers; j++)
            {
                for(int k=0; k<theta[j].size() ; k++)
                {
                    net = 0;
                    for(int l=0; l<weight[j][k].size() ;l++)
                    {
                        net += output[j][l]*weight[j][k][l];
                    }
                    net += theta[j][k];
                    output[j+1][k] = sigmoid(net);
                }
            }
            //Outputs of layer j are now stored in layer j+1

            //Now the back propagation needs to be done
            //Do it for last layer
            for(int j=0; j<theta[layers-1].size() ;j++)
            {
                delta[layers-1][j] = (exp_output[i][j] - output[layers][j])*output[layers][j]*(1 - output[layers][j]);
                for(int k=0; k<weight[layers-1][j].size(); k++)
                {
                    weight[layers-1][j][k] += LEARNING_RATE*delta[layers-1][j]*output[layers-1][k];
                }
                theta[layers-1][j] += LEARNING_RATE*delta[layers-1][j];
            }

            //Now do it for all the other layers using last layer's values
            //For all perceptrons in a certain layer
            for(int j=layers-2; j>=0; j--)
            {
                for(int k=0; k<theta[j].size(); k++)
                {
                    delta[j][k] = 0;
                    for(int l=0; l<theta[j+1].size(); l++)
                    {
                        delta[j][k] += weight[j+1][l][k]*delta[j+1][l];
                    }
                    delta[j][k] *= output[j+1][k]*(1 - output[j+1][k]);
                    //Weight update here
                    for(int l=0; l<weight[j][k].size(); l++)
                    {
                        weight[j][k][l] += LEARNING_RATE*delta[j][k]*output[j][l];
                    }
                    //Theta update here
                    theta[j][k] += LEARNING_RATE*delta[j][k];
                }
            }

            for(int j=0; j<outputs; j++)
            {
                err += 0.5*(exp_output[i][j] - output[layers][j])*(exp_output[i][j] - output[layers][j]);
            }
        }
        cout << "err is " << err << endl;
        if(err < MAX_ERROR)
        {
            cout << "Completed operation!"<<endl;   
            return 0;
        }
        iter--;
        if(trials<0)
        {
            cout << "Did not converge. Abort trials!" << endl;
            return 0;       
        }
        if(iter == 0)
        {
            trials--;
            iter = 100000;
            cout << "Could not converge. Resetting initial weight..."<<endl;
            sleep(2);
            for(int u=0; u<layers; u++)
                for(int v=0; v<weight[u].size(); v++)
                    for(int w=0; w<weight[u][v].size(); w++)
                        weight[u][v][w] = -1+2*((float)rand())/RAND_MAX;
        }
    }
}
