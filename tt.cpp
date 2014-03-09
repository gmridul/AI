extern  vector<vector<float> > tt;
extern  vector<vector<float> > t;
void fillTruthTable(int p, int n, int numOut) {
    for(int i=0;i<p;i++) {
        int label;
        vector<float> subt;
        for(int i=0;i<numOut;i++) {
            cin >> label; 
            subt.push_back(label); 
        }
        t.push_back(subt);
        vector<float> dataline;
        for(int j=0; j<n; j++) {
            int k;
            cin >> k;
            dataline.push_back(k);
            if(label == 0) {
                dataline[j] = -dataline[j];
            }
        }
        if(label == 0) dataline.push_back(1);
        else dataline.push_back(-1);
        tt.push_back(dataline);
    }
}
