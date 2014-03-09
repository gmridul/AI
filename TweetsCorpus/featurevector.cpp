#include <vector>
#include <limits>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int no_of_feature_vectors = 1;
int wc=0;
map<string,int> distinct_words;
vector<pair<string,int> > indices;
map<string,int> stop_words;

char easytolower(char in){
  if(in<='Z' && in>='A')
    return in-('Z'-'z');
  return in;
}

void add_stop_words(string line){
	stop_words.insert(make_pair(line,1));
}

bool is_present(string line){
	if(stop_words.find(line) != stop_words.end()) return false;
	else return true;
}

void delete_useless_words(){
	map<string,int>::iterator it;
	vector<pair<string,int> > tmp_vec;

	for (it=distinct_words.begin(); it!=distinct_words.end(); ++it){
		if(it->second <= 1){
		}
		else{
			tmp_vec.push_back(make_pair(it->first,0));
			indices.push_back(make_pair(it->first,wc));
			wc++;
		}
		// cout<<it->first<<"   "<<it->second<<endl;
	}
	distinct_words.clear();
	for(int i=0;i<tmp_vec.size();i++){
		distinct_words.insert(make_pair(tmp_vec[i].first,0));
	}
}

vector<int> generatevector(string line){
	vector<int> ret;
	ret.resize(distinct_words.size());
	
	for(int i=0;i<ret.size();i++){
		ret[i] = 0;
	}

	char * tokens;
	char * tmp = const_cast<char*>(line.c_str());
	tokens = strtok(tmp," ");
	while (tokens != NULL){
		if(strlen(tokens) >= 2){
			for(int i=0;i<indices.size();i++){
				if(indices[i].first == (string)(tokens)){
					ret[indices[i].second] = 1;
				}
			}
		}
	    tokens = strtok (NULL, " ");
	}
	return ret;
}

string replace_useless(string line){
	replace( line.begin(), line.end(), ',', ' ');
	replace( line.begin(), line.end(), '!', ' ');
	replace( line.begin(), line.end(), '@', ' ');
	replace( line.begin(), line.end(), '#', ' ');
	replace( line.begin(), line.end(), '$', ' ');
	replace( line.begin(), line.end(), '%', ' ');
	replace( line.begin(), line.end(), '^', ' ');
	replace( line.begin(), line.end(), '&', ' ');
	replace( line.begin(), line.end(), '*', ' ');
	replace( line.begin(), line.end(), '?', ' ');
	replace( line.begin(), line.end(), '.', ' ');
	replace( line.begin(), line.end(), '-', ' ');
	replace( line.begin(), line.end(), '=', ' ');
	replace( line.begin(), line.end(), '+', ' ');
	replace( line.begin(), line.end(), '/', ' ');
	replace( line.begin(), line.end(), '\'', ' ');
	replace( line.begin(), line.end(), '\"', ' ');
	replace( line.begin(), line.end(), '|', ' ');
	return line;
}

void parse(string s){
	transform(s.begin(), s.end(), s.begin(), easytolower);
	char * tokens;
	char * tmp = const_cast<char*>(s.c_str());
	tokens = strtok(tmp," ");
	while (tokens != NULL){
		if(strlen(tokens) >= 2){
			if(is_present((string)tokens)){

				if(distinct_words.find((string)tokens) == distinct_words.end()){
					distinct_words.insert(make_pair((string)tokens,1));
				}
				else{
					map<string,int>::iterator it;
					it = distinct_words.find((string)tokens);
					distinct_words.erase(it);
					distinct_words.insert(make_pair((string)tokens,(it->second)+1));
				}
			}
		}
	    tokens = strtok (NULL, " ");
	}
}

void print_vector(vector<int> v,string file){
	// cout<<v.size()<<endl;
	ofstream outfile;
	outfile.open(file.c_str(), ios::out | ios::app);

	for(int i=0;i<v.size();i++){
		outfile << v[i] << " ";
	}
	outfile << "\n";
	outfile.close();
}

void print_string(string s,string file){
	ofstream outfile;
	outfile.open(file.c_str(),ios::out | ios::app);
	outfile << s;
	outfile.close();	
}

int get_no_of_lines(string file){
	int number_of_lines = 0;
	string line;  
	ifstream myfile(file.c_str());
	    if(myfile.is_open()){
	        while(!myfile.eof()){
	            getline(myfile,line);
	            // cout<< line << endl;
	            number_of_lines++;
	        }
	        myfile.close();
	    }    
	return number_of_lines;          
}

int main(){
	int a,b;
	cin>>a>>b;
	vector<pair<string,string> > input_files;
	input_files.push_back(make_pair("twitter_positive","1 0 0 "));
	input_files.push_back(make_pair("twitter_negative","0 0 1 "));
	input_files.push_back(make_pair("twitter_objective","0 1 0 "));	

	ifstream myfile;
	string line;

	
	//stop words

	myfile.open ("stop_words");
	while (getline (myfile,line) )
	    {
	      add_stop_words(line);
	    }
	    myfile.close();

	for(int i=a;i<b;i++){
		
		 ostringstream strs;
		 strs << i;
		 string buffer = strs.str();

		for(int j=0;j<input_files.size();j++){
			int lines = get_no_of_lines(input_files[j].first);
			int l =	(float)i * (0.20) * (float)lines;
			int h = (float)(i+1) * (0.20) * (float)lines;
			// cout<<input_files[j].first<<endl;
			// cout<<lines<<"   "<<l<<"   "<<h<<endl;
			int cnt=0;
			myfile.open (input_files[j].first.c_str());
			while (getline (myfile,line))
			    {
			    	if(cnt<=l || cnt >= h){
			    		parse(replace_useless(line));
			    	}

			    	cnt++;
			    }
			    myfile.close();
		}

		delete_useless_words();
		
		for(int j=0;j<input_files.size();j++){	
			int lines = get_no_of_lines(input_files[j].first);
			int l =	(float)i * (0.20) * (float)lines;
			int h = (float)(i+1) * (0.20) * (float)lines;
			int cnt =0;
			myfile.open (input_files[j].first.c_str());
		  	while (getline (myfile,line) ){
		  	  vector<int> v;	
		      v = generatevector(replace_useless(line));

		  	  if(cnt>l && cnt <h){
		  	  print_string(input_files[j].second,"test"+buffer); 
		  		print_vector(v,"test"+buffer);
			  }
			  else{
			  	print_string(input_files[j].second,"input"+buffer);
			  	print_vector(v,"input"+buffer);	
			  }
			  cnt++;	
		  	}
		  	myfile.close();  		
		}

		distinct_words.clear();
		indices.clear();
	}    
	    
	// for(int i=0;i<input_files.size();i++){
	// 	myfile.open (input_files[i].first.c_str());
	// 	while (getline (myfile,line) )
	// 	    {
	// 	      parse(replace_useless(line));
	// 	    }
	// 	    myfile.close();			
	// }	

 //  	delete_useless_words();
  	
 //  	vector<int> v;

 //  	for(int i=0;i<input_files.size();i++){

	// 		myfile.open (input_files[i].first.c_str());
	// 	  	while (getline (myfile,line) ){
	// 	      cout<<input_files[i].second;
	// 	      v = generatevector(replace_useless(line));
	// 	  		print_vector(v);
	// 	  	}
	// 	  	myfile.close();  		
 //  	}

}
