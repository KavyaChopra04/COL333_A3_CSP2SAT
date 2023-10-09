#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include <time.h>
#include<cstdlib>
using namespace std;
const string minisat_init = "minisat ";
#define S(i,j) (size+2)*(i-1) + (j+1) + starting_index   //S(i,j) denotes if the sum of variables upto index i is greater than or equal to j
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
void ensure_clique(int vertices,int starting_index,int is_one,int size,vector<vector<pair<int,short>>>& clauses){
        vector<pair<int,short>> clause;
        clause.push_back(make_pair(is_one*vertices + 1,-1)); 
        clause.push_back(make_pair(S(1,1),1)); //if S(1,1), then the first vertex must be assigned to our clique, else not.
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(is_one*vertices + 1,1));
        clause.push_back(make_pair(S(1,1),-1));
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(S(1,0),1)); //Base Case 
        clauses.push_back(clause);
        clause.clear();
        for(int i = 2;i <= size+1;i++){
            clause.push_back(make_pair(S(1,i),-1)); 
            clauses.push_back(clause);
            clause.clear();
        }
        for(int i = 2;i <= vertices;i++){
            clause.push_back(make_pair(S(i,0),1));
            clauses.push_back(clause);
            clause.clear();
            for(int j = 1;j <= size+1;j++)
            {
                if (j > i){
                    clause.push_back(make_pair(S(i,j),-1)); //S(i,j) can't be true if j>i
                    clauses.push_back(clause);
                    clause.clear();
                }
                else
                {   
                    //S(i,j) occurs in 2 cases only: (P(i,j) and S(i-1, j-1)) or (S(i-1,j)). So we wanna encode 
                    //S(i,j) <=> (P(i,j) and S(i-1, j-1)) or (S(i-1,j))
                    clause.push_back(make_pair(S(i,j),1)); 
                    clause.push_back(make_pair(S(i-1,j),-1)); //S(i-1,j)=>S(i,j)
                    clauses.push_back(clause);
                    clause.clear();
                    clause.push_back(make_pair(S(i,j),-1)); //S(i,j)=>S(i-1,j) or P(i,j)
                    clause.push_back(make_pair(S(i-1,j),1));
                    clause.push_back(make_pair(is_one*vertices + i,1));
                    clauses.push_back(clause);
                    clause.clear();
                    clause.push_back(make_pair(S(i,j),-1)); //S(i,j)=>S(i-1,j-1) or S(i-1,j)
                    clause.push_back(make_pair(S(i-1,j-1),1));
                    clause.push_back(make_pair(S(i-1,j),1));
                    clauses.push_back(clause);
                    clause.clear();
                    clause.push_back(make_pair(S(i,j),1)); //S(i-1,j-1) and P(i,j) => S(i,j)
                    clause.push_back(make_pair(S(i-1,j-1),-1));
                    clause.push_back(make_pair(is_one*vertices + i,-1));
                    clauses.push_back(clause);
                    clause.clear();
                }
            }
        }
        clause.push_back(make_pair(S(vertices,size),1)); //if S(vertices,size) means that among all vertices, exactly size-many should be assigned to the clique
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(S(vertices,size+1),-1)); //if S(vertices,size+1) means that among all vertices, exactly size-many should be assigned to the clique
        clauses.push_back(clause);
        clause.clear();
}
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
void get_edges(string inputfilename,int& vertices,int& edges,int& k1,int& k2,bool** data)
    {
            std::ifstream ipfile;
	        ipfile.open(inputfilename, ios::in);
            if (!ipfile) {
                cout << "No such file\n";
                exit( 0 );
            }
            ipfile >> vertices;
            ipfile >> edges;
            ipfile >> k1;
            ipfile >> k2;
            int e1,e2;
            for(int i =0;i < vertices;i++){
                for(int j = 0;j < vertices;j++){
                    data[i][j] = false;
                }
            }
            for(int i =0;i < edges;i++){
                ipfile >> e1;
                ipfile >> e2;
                data[e1-1][e2-1] = true;
                data[e2-1][e1-1] = true;
            }   
            ipfile.close();
    }
bool runMiniSAT(string inputFileName, string outputFileName)
{
    string command = minisat_init + inputFileName + " " + outputFileName;
    system(command.c_str());
    ifstream opfile;
    opfile.open(outputFileName, ios::in);
    if (!opfile) {
        cout << "No such file\n";
        opfile.close();
        exit( 0 );
    }
    string line;
    getline(opfile,line);
    if(line == "UNSAT")
    {
        opfile.close();
        return false;
    }
    opfile.close();
    return true;
}
int main(int argc, char** argv){
    
    string inputfilename (argv[1]);
    inputfilename+=".graphs";
    string outputfilename (argv[1]);
    outputfilename+=".mapping";
    string satinputfile(argv[1]);
    satinputfile+=".satinput";
    string satoutputfile(argv[1]);
    satoutputfile+=".satoutput";
    int vertices;
    int edges;
    int k1;
    int k2;
    get_params(inputfilename,vertices,edges,k1,k2); //TO-DO: incorporate all File I/O in one function/pass. How much of a difference does it make anyway?
    bool **data;
    data = new bool *[vertices];
 //edges stored as adjacency matrix
 for(int i = 0; i <vertices; i++){
     data[i] = new bool[vertices];
     }
    get_edges(inputfilename,vertices,edges,k1,k2,data);

    vector<vector<pair<int,short>>> clauses; //second parameter denotes positive or negative literal (change to bool for less memory consumption?)
    for(int i = 1;i <= vertices;i++){ //ensure each vertex is assigned a colour. Might wanna optimise on 3 to 2 SAT ratio?

        vector<pair<int,short>> clause;
        clause.push_back(make_pair(i,1)); //C i,0
        clause.push_back(make_pair(i+vertices,1)); // C i,1
        clause.push_back(make_pair(i+2*vertices,1)); // C i,2
        clauses.push_back(clause);
        clause.clear();
    }
   for(int i = 1;i <= vertices;i++){ //consistency check: ensure no vertex is assigned 2 colours in a 2-SAT formulation. Nice.

        vector<pair<int,short>> clause;
        clause.push_back(make_pair(i,-1));  
        clause.push_back(make_pair(i+vertices,-1)); 
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(i,-1));
        clause.push_back(make_pair(i+2*vertices,-1));
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(i+2*vertices,-1));
        clause.push_back(make_pair(i+vertices,-1));
        clauses.push_back(clause);
        clause.clear();
    }

   for(int i = 1;i <= vertices;i++)
    {   int degree = 0;
        for(int j = i+1;j <= vertices;j++)
        {
            if (!data[i-1][j-1])
            {   //if no edge between them, they can't be in the same clique
                vector<pair<int,short>> clause;
                clause.push_back(make_pair(i,-1)); 
                clause.push_back(make_pair(j,-1));
                clauses.push_back(clause);
                clause.clear();
                clause.push_back(make_pair(i+vertices,-1));
                clause.push_back(make_pair(j+vertices,-1));
                clauses.push_back(clause);
                clause.clear();
            }
            else{
                degree += 1;
            }
        }
    if (degree < min(k1-1,k2-1)){
        vector<pair<int,short>> clause;
        clause.push_back(make_pair((2*vertices)+i,1));
        clause.clear();
    }
    } 
   for(int i = 1;i <= vertices;i++)
    {   int degree = 0;
        for(int j = 1;j <= vertices;j++)
        {
            if (data[i-1][j-1])
            {   
                degree += 1;
            }
        }
    if (degree < min(k1-1,k2-1)){
        vector<pair<int,short>> clause;
        clause.push_back(make_pair((2*vertices)+i,1));
        clauses.push_back(clause);
        clause.clear();
    }
    } 







   ensure_clique(vertices,3*vertices,0,k1,clauses);
   ensure_clique(vertices,3*vertices+ vertices*(k1+2),1,k2,clauses);
    // for(int i = 0;i < clauses.size();i++){
    //     for(int j = 0;j < clauses[i].size();j++){
    //         cout << clauses[i][j].first* clauses[i][j].second << " ";
    //     }
    //     cout << "\n";
    // }

    ofstream outputfile(satinputfile);
    if (!outputfile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }
    outputfile <<  "p cnf " << 3*vertices + vertices*(k1+2)+vertices*(k2+2) << " " << clauses.size() << "\n";
        for(int i = 0;i < clauses.size();i++){
        for(int j = 0;j < clauses[i].size();j++){
            outputfile << clauses[i][j].first* clauses[i][j].second << " ";
        }
        outputfile << "0\n";
    }
    outputfile.close();
    // runMiniSAT(satinputfile, satoutputfile);
    // reconstructGraph(satoutputfile, vertices, k1, k2, outputfilename);
    return 0;
}