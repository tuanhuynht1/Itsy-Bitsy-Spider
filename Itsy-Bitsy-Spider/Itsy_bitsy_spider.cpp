/*      Itsy-Bitsy Spider
        by Tuan Huynh       
        
This program will traverse a 3D maze outine in thw “Itsy-Bitsy Spider”
maze problem from “MAD MAZES: Intriguing Mind Twisters for Puzzle Buffs, 
Game Nuts and Other Smart People” by Robert Abbott.       */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Location{
    public:
    //location coordinate and degree of openings
    int lvl, row, col;
    int degree;
    //adj list and string representing all possible directions to move to  
    vector<Location*> next;
    string openings = "";
    
    //constructor
    Location(int l, int r, int c) : lvl(l), row(r), col(c) {}
    //set referenced variables to location coordinates
    void setVariables(int &l, int &r, int &c) { l=lvl; r=row; c=col; }
    //initialize adjacency list using the 6-digit possible moves code from the map
    void decodeNext(string code);
};

class Maze{
    public:
    //dimensions
    int lvls, rows, cols;
    //auxillary data structures
    vector<vector<vector<string>>> openings;
    vector<vector<vector<bool>>> discovered;
    //boolean to break out of backtracking algorithm
    bool solved = false;

    //constructor
    Maze(vector<vector<vector<string>>> openings);
    //backtracking algorithm
    string solve(Location* current, Location* end, string path);
    //helper functions
    void print();
};


int main(int argc, char* argv[]){
    int mazes, lvls, rows, cols, l, r, c;
    Location* u;
    Location* v;
    ifstream inFile("input.txt");
    if(inFile){
        ofstream outFile("output.txt");
        //read in number of mazes
        inFile >> mazes;

        for(int m = 0; m < mazes; m++){
            //read in dimensions of maze and initialize size of map accordingly
            inFile >> lvls >> rows >> cols;
            vector<vector<vector<string>>> map_of_openings (lvls,vector<vector<string>>(rows,vector<string>(cols,string(6,'0'))));
            //read in start location 
            inFile >> l >> r >> c;
            Location* u = new Location(l,r,c);
            //read in end location
            inFile >> l >> r >> c;
            Location* v = new Location(l,r,c);
            //populate map with openings using string code in file
            for(int i = 0; i < lvls; i++){
                for(int j = 0; j < rows; j++){
                    for(int k = 0; k < cols; k++){
                        inFile >> map_of_openings[i][j][k];
                    }
                }
            }

            //create maze
            Maze maze(map_of_openings);
            //find path from u to v
            string path = maze.solve(u,v,"");
            //maze.print();

            //write to output file
            for(int i = 0; i < path.size(); i++){
                outFile << path[i];
                outFile << " ";
            }
            outFile << "\n";
        }

        inFile.close();
        outFile.close();
        cout << mazes << " mazes completed!\n" << "output.txt created!" << endl;
    }
    else{
        cout << "ERROR in opening file" << endl;
    }
}

void Location::decodeNext(string code){
    degree = 0;
    //north
    if(code[0] == '1') {
        next.push_back(new Location(lvl, row-1 , col));
        openings += 'N';
        degree++;
    }
    //east
    if(code[1] == '1') {
        next.push_back(new Location(lvl, row , col+1));
        openings += 'E';
        degree++;
    }
    //south
    if(code[2] == '1') {
        next.push_back(new Location(lvl, row+1 , col));
        openings += 'S';
        degree++;
    }
    //west
    if(code[3] == '1') {
        next.push_back(new Location(lvl, row , col-1));
        openings += 'W';
        degree++;
    }
    //up 
    if(code[4] == '1'){
        next.push_back(new Location(lvl + 1, row, col));
        openings += 'U';
        degree++;
    }
    //down
    if(code[5] == '1') {
        next.push_back(new Location(lvl - 1, row , col));
        openings += 'D';
        degree++;
    }
}

Maze::Maze(vector<vector<vector<string>>> openings){
    this->openings = openings;
    lvls = openings.size();
    rows = openings[0].size();
    cols = openings[0][0].size();
    vector<vector<vector<bool>>> buffer(lvls, vector<vector<bool>>(rows, vector<bool>(cols, false)));
    discovered = buffer;
}

void Maze::print(){
    cout << "Dim: " << lvls << "x" << rows << "x" << cols << endl << endl;
    for(int i = 0; i < lvls; i++){
        for(int j = 0; j < rows; j++){
            for(int k = 0; k < cols; k++){
                cout << openings[i][j][k] << "("<< discovered[i][j][k] << ") ";
            }
            cout << endl;
        }
        cout << endl << endl;
    }
}

string Maze::solve(Location* current, Location* end, string path){
    int l, r, c;
    //set local variables to current location's coordinates
    current->setVariables(l,r,c);

    //if path found, mark end of search by setting solved flag
    if(l == end->lvl && r == end->row && c == end->col){
        solved = true;
        return path;
    }

    //set output as current path
    string output = path;
    //mark current location as discovered
    discovered[l][r][c] = true;

    //set adj list of open paths by decoding 6-digit string in map
    current->decodeNext(openings[l][r][c]);

    //recurse through all undiscovered open paths
    for(int i = 0; i < current->degree; i++){
        current->next[i]->setVariables(l,r,c);
        if(discovered[l][r][c] != true){
            output = solve(current->next[i], end, path + current->openings[i]);
        }
        else{
            //free up memory by removing discovered location
            delete current->next[i];
        }

        //path found
        if(solved){
            return output;
        }
    }
    //if not found, backtrack
    return output;
}