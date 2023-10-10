#include <bits/stdc++.h>
#define int long long
using namespace std;
const string minisat_init = "./minisat ";
#define S(i,j) (size+2)*(i-1) + (j+1) + starting_index   //S(i,j) denotes if the sum of variables upto index i is greater than or equal to j. j may range from 0 to k+1 for our purposes, so this adds a total of (numVertices)*(k+2) to the size of the clauses
void ensureClique(int vertices, int starting_index, int size, vector<vector<pair<int,bool>>>& clauses){
    //cout<<"currently running for "<<starting_index<<" "<<size<<"\n";
    vector<pair<int,bool>> clause;
    clause.push_back(make_pair(vertices + 1,false)); 
    clause.push_back(make_pair(S(1,1),true)); //if S(1,1), then the first vertex must be assigned to our clique, else not.
    clauses.push_back(clause);
    clause.clear();
    clause.push_back(make_pair(vertices + 1,true));
    clause.push_back(make_pair(S(1,1),false));
    clauses.push_back(clause);
    clause.clear();
    clause.push_back(make_pair(S(1,0),true)); //Base Case 
    clauses.push_back(clause);
    clause.clear();
    for(int i = 2;i <= size+1;i++){
        clause.push_back(make_pair(S(1,i),false)); 
        clauses.push_back(clause);
        clause.clear();
    }
    for(int i = 2;i <= vertices;i++){
        clause.push_back(make_pair(S(i,0),true));
        clauses.push_back(clause);
        clause.clear();
        for(int j = 1;j <= size+1;j++)
        {
            if (j > i){
                clause.push_back(make_pair(S(i,j),false)); //S(i,j) can't be true if j>i
                clauses.push_back(clause);
                clause.clear();
            }
            else
            {   
                //S(i,j) occurs in 2 cases only: (P(i,j) and S(i-1, j-1)) or (S(i-1,j)). So we wanna encode 
                //S(i,j) <=> (P(i,j) and S(i-1, j-1)) or (S(i-1,j))
                clause.push_back(make_pair(S(i,j),true)); 
                clause.push_back(make_pair(S(i-1,j),false)); //S(i-1,j)=>S(i,j) 
                clauses.push_back(clause);
                clause.clear();
                clause.push_back(make_pair(S(i,j),false)); //S(i,j)=>S(i-1,j) or P(i,j)
                clause.push_back(make_pair(S(i-1,j),true));
                clause.push_back(make_pair(vertices + i,true));
                clauses.push_back(clause);
                clause.clear();
                clause.push_back(make_pair(S(i,j),false)); //S(i,j)=>S(i-1,j-1) or S(i-1,j)
                clause.push_back(make_pair(S(i-1,j-1),true));
                clause.push_back(make_pair(S(i-1,j),true));
                clauses.push_back(clause);
                clause.clear();
                clause.push_back(make_pair(S(i,j),true)); //S(i-1,j-1) and P(i,j) => S(i,j)
                clause.push_back(make_pair(S(i-1,j-1),false));
                clause.push_back(make_pair(vertices + i,false));
                clauses.push_back(clause);
                clause.clear();
            }
        }
    }
    clause.push_back(make_pair(S(vertices,size),true)); //if S(vertices,size) means that among all vertices, exactly size-many should be assigned to the clique
    clauses.push_back(clause);
    clause.clear();
    clause.push_back(make_pair(S(vertices,size+1),false)); //if S(vertices,size+1) means that among all vertices, exactly size-many should be assigned to the clique
    clauses.push_back(clause);
    clause.clear();
}
void writeToFile(vector<vector<pair<int,bool>>>& clauses, string fileName, int maximumClique, int vertices)
{
    ofstream outputfile(fileName);
    if (!outputfile.is_open()) {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }
    outputfile <<  "p cnf " << 2*vertices + vertices*(maximumClique+2)<< " " << clauses.size() << "\n";
        for(int i = 0;i < clauses.size();i++){
        for(int j = 0;j < clauses[i].size();j++){
            if(clauses[i][j].second)
            {
                outputfile << clauses[i][j].first << " ";
            }
            else
            {
                outputfile << "-" << clauses[i][j].first << " ";
            }
        }
        outputfile << "0\n";
    }
    outputfile.close();
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
string readMapping(string outputFileName)
{
    ifstream opfile;
    opfile.open(outputFileName, ios::in);
    if (!opfile) {
        cout << "No such file\n";
        opfile.close();
        exit( 0 );
    }
    string line;
    getline(opfile,line);
    getline(opfile,line);
    return line;
    opfile.close();
}
void parseMapping(string line, string outputFile, int vertices)
{
    stringstream ss(line);
    string token;
    vector<int> clique;
    while(getline(ss,token,' '))
    {
        if(token=="0" || token=="-"+to_string(vertices+1) || token==to_string(vertices+1))
        {
            break;
        }
        if(token[0]=='-' )
        {
            clique.push_back(stoll(token.substr(1)));
        }
    }
    string cliqueString = "#1\n";
    for(int i=0; i<clique.size()-1; i++)
    {
        cliqueString += to_string(clique[i]) + " ";
    }
    cliqueString += to_string(clique[clique.size()-1]);
    ofstream ofs(outputFile);
    ofs<<cliqueString;
    ofs.close();
}
pair<int, string> iterativeDeepening(int vertices, vector<vector<pair<int,bool>>>& clauses)
{
    vector<vector<pair<int,bool>>> originalClauseSet(clauses);
    string readFile = "";
    int maxCliqueSize = 1;
    int maxCliqueSizeFound = 1;
    while(maxCliqueSizeFound<=vertices)
    {
        ensureClique(vertices,2*vertices,maxCliqueSizeFound,clauses);
        writeToFile(clauses, "part2input.txt", maxCliqueSizeFound, vertices);
        clauses = originalClauseSet;
        if(runMiniSAT("part2input.txt", "part2output.txt"))
        {  
            readFile = readMapping("part2output.txt");
            maxCliqueSize = maxCliqueSizeFound;
            maxCliqueSizeFound = maxCliqueSizeFound + 1;
        }
        else
        {
            break;
        }
    }
    return make_pair(maxCliqueSize, readFile);
}
int binarySearchDeepening(int vertices, int edges, vector<vector<pair<int,bool>>>& clauses)
{
    vector<vector<pair<int,bool>>> originalClauseSet(clauses);
    int maxCliqueSize = 1;
    int maxCliqueSizeFound = 1;
    int low = 1;
    int high = vertices;
    while(low<high)
    {
        int mid = low + (high - low + 1)/2;
        cout<<"right now, mid is "<<mid<<" and num edges is "<<edges<<endl;
        if(edges < (mid*(mid-1))/2)
        {
            high = mid - 1;
            continue;
        }
        ensureClique(vertices,2*vertices,mid,clauses);
        writeToFile(clauses, "part2input.txt", mid, vertices);
        clauses = originalClauseSet;
        if(runMiniSAT("part2input.txt", "part2output.txt"))
        {
            maxCliqueSize = mid;
            low = mid;
        }
        else
        {
            high = mid - 1;
        }
    }
    return maxCliqueSize;
}
signed main(int argc, char** argv){
    //I/O functions
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
    std::ifstream ipfile;
    ipfile.open(inputfilename, ios::in);
    vector<set<int>> adjacencyList;
    if (!ipfile) {
        cout << "No such file\n";
        ipfile.close();
        exit( 0 );
    }
    ipfile >> vertices;
    adjacencyList.resize(vertices+1);
    ipfile >> edges;
    for(int i=0;i<edges;i++)
    {
        int v1,v2;
        ipfile >> v1;
        ipfile >> v2;
        adjacencyList[v1].insert(v2);
        adjacencyList[v2].insert(v1);
    }
    ipfile.close();
    //end of I/O functions
    vector<vector<pair<int,bool>>> clauses; 
    //Notation: (x, 1) denotes x, and (x, 0) denotes not x;
    //Consistency checks for vertices
    /**
        Type 1: All vertices must be assigned a colour
        Let variable i (from 1 to vertices) represent if variable was in gang 0 (not a part of the clique)
        and vertices + i (from vertices + 1 to 2*vertices) represent if variable was in gang 1 (a part of the clique)
    **/
    for(int i=0;i<vertices;i++)
    {
        vector<pair<int,bool>> clause;
        clause.push_back(make_pair(i+1,true));
        clause.push_back(make_pair(i+1 + vertices,true));
        clauses.push_back(clause);
        clause.clear();
    }
    /**
        Type 2: Each vertex should have a different colour
    **/
    for(int i=0;i<vertices;i++)
    {
        vector<pair<int,bool>> clause;
        clause.push_back(make_pair(i+1,false));
        clause.push_back(make_pair(i+1 + vertices,false));
        clauses.push_back(clause);
        clause.clear();
    }
    /**
     * Edge Constraint Encoding: If 2 vertices don't have an edge between them, then they can't both belong to gang 1
    */
    for(int i = 1;i <= vertices;i++)
    {
        for(int j = i+1;j <= vertices;j++)
        {
            if (adjacencyList[i].find(j) == adjacencyList[i].end())
            {   
                //if no edge between them, they can't both be in gang 1
                vector<pair<int,bool>> clause;
                clause.push_back(make_pair(i+vertices,false));
                clause.push_back(make_pair(j+vertices,false));
                clauses.push_back(clause);
                clause.clear();
            }
        }
    } 
    /**
     * These were the constraints that would be uniform across all clique sizes. 
    */
    pair<int, string> maxClique = iterativeDeepening(vertices, clauses);
    //cout << "Maximum Clique Size: " << maxCliqueSize << "\n";
    ensureClique(vertices,2*vertices,maxClique.first,clauses);
    //writeToFile(clauses, satinputfile, maxClique.first, vertices);
    //runMiniSAT(satinputfile, satoutputfile);
    parseMapping(maxClique.second, outputfilename, vertices);
}