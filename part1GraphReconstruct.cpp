#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include <time.h>
#include<cstdlib>
using namespace std;
void get_params(string inputfilename,int& vertices,int& edges,int& k1,int& k2)
    {        
            std::ifstream ipfile;
	        ipfile.open(inputfilename, ios::in);
            if (!ipfile) {
                cout << "No such file\n";
                exit( 0 );
            }
            else{
            ipfile >> vertices;
            ipfile >> edges;
            ipfile >> k1;
            ipfile >> k2;
            }
            ipfile.close();
    }
void reconstructGraph(string inputfilename, int vertices, int clique1, int clique2, string outputfilename)
{
    ifstream ipfile;
    ipfile.open(inputfilename, ios::in);
    if (!ipfile) {
        cout << "No such file\n";
        ipfile.close();
        exit( 0 );
    }
    ofstream outputfile(outputfilename);
    if (!outputfile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }
    string line;
    getline(ipfile,line);
    if(line=="UNSAT")
    {
        outputfile << "0 \n";
        ipfile.close();
        outputfile.close();
        exit(0);
    }
    getline(ipfile,line);
    stringstream ss(line);
    string token;
    vector<int> clique;
    while(getline(ss,token,' '))
    {
        if(token==to_string(2*vertices+1) ||  token=='-' + to_string(2*vertices+1))
        {
            break;
        }
        if(token[0]!='-' )
        {
            clique.push_back(stoll(token));
        }
    }
    outputfile<<"#1\n";
    for(int i=0; i<clique1-1; i++)
    {
        outputfile << clique[i] << " ";
    }
    outputfile<<clique[clique1-1];
    outputfile << "\n";
    outputfile<<"#2\n";
    for(int i=clique1; i<clique1+clique2-1; i++)
    {
        outputfile << clique[i] - vertices << " ";
    }
    outputfile<<clique[clique1+clique2-1] - vertices;
    outputfile << "\n";
    ipfile.close();
    outputfile.close();
}
int main(int argc, char** argv)
{
    string inputfilename (argv[1]);
    inputfilename+=".graphs";
    string satoutputfile(argv[1]);
    satoutputfile+=".satoutput";
    string outputfilename (argv[1]);
    outputfilename+=".mapping";
    int vertices;
    int edges;
    int k1;
    int k2;
    get_params(inputfilename,vertices,edges,k1,k2);
    reconstructGraph(satoutputfile,vertices,k1,k2,outputfilename);
}