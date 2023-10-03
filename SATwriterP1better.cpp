#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include <time.h>
#include<cstdlib>
using namespace std;

#define S(i,j) (size+2)*(i-1) + (j+1) + starting_index   
void ensure_clique(int vertices,int starting_index,int is_one,int size,vector<vector<pair<int,short>>>& clauses){
        vector<pair<int,short>> clause;
        clause.push_back(make_pair(is_one*vertices + 1,-1));
        clause.push_back(make_pair(S(1,1),1));
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(is_one*vertices + 1,1));
        clause.push_back(make_pair(S(1,1),-1));
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(S(1,0),1));
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
            clause.push_back(make_pair(S(i,j),-1));
            clauses.push_back(clause);
            clause.clear();
            }
            else{
        clause.push_back(make_pair(S(i,j),1));
        clause.push_back(make_pair(S(i-1,j),-1));
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(S(i,j),-1));
        clause.push_back(make_pair(S(i-1,j),1));
        clause.push_back(make_pair(is_one*vertices + i,1));
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(S(i,j),-1));
        clause.push_back(make_pair(S(i-1,j-1),1));
        clause.push_back(make_pair(S(i-1,j),1));
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(S(i,j),1));
        clause.push_back(make_pair(S(i-1,j-1),-1));
        clause.push_back(make_pair(is_one*vertices + i,-1));
        clauses.push_back(clause);
        clause.clear();

            }
        }
        }
        clause.push_back(make_pair(S(vertices,size),1));
        clauses.push_back(clause);
        clause.clear();
        clause.push_back(make_pair(S(vertices,size+1),-1));
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

int main(int argc, char** argv){
    
    string inputfilename (argv[1]);
    int vertices;
    int edges;
    int k1;
    int k2;
    get_params(inputfilename,vertices,edges,k1,k2);
    bool **data;
 data = new bool *[vertices];
 //edges stored as adjacency matrix
 for(int i = 0; i <vertices; i++){
     data[i] = new bool[vertices];
     }
    get_edges(inputfilename,vertices,edges,k1,k2,data);

    vector<vector<pair<int,short>>> clauses;
    for(int i = 1;i <= vertices;i++){

        vector<pair<int,short>> clause;
        clause.push_back(make_pair(i,1));
        clause.push_back(make_pair(i+vertices,1));
        clause.push_back(make_pair(i+2*vertices,1));
        clauses.push_back(clause);
        clause.clear();
    }
   for(int i = 1;i <= vertices;i++){

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

   for(int i = 1;i <= vertices;i++){
    for(int j = i+1;j <= vertices;j++){
            if (!data[i-1][j-1]){
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

    ofstream outputfile("test.satinput");
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
    return 0;
}