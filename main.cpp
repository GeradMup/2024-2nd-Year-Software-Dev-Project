#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <sstream>
#include <string>

#include "IDA_Star.h"

using namespace std;

const string inputFileName = "15_puzzles.txt";

/// PUZZLE is defined in IDA_Star.h ==> typedef vector<vector<int>> PUZZLE;
/// PUZZLE_SIZE is defined in IDA_Star.h ==> const int PUZZLE_SIZE = 4;

void printPuzzles(shared_ptr<vector<PUZZLE>> puzzles);
shared_ptr<vector<PUZZLE>> getPuzzles();

int main()
{
    shared_ptr<vector<PUZZLE>> puzzles = getPuzzles();
    printPuzzles(puzzles);

    IDA_Star idaStar = IDA_Star(puzzles);
    idaStar.solvePuzzles();

    printPuzzles(puzzles);
    return 0;
}

///Function for reading all the puzzles from the input file.
///Function will store and return all the puzzles in a vector
shared_ptr<vector<PUZZLE>> getPuzzles()
{
    ///Pre-Allocate memory for each puzzle to ensure contigious memory, Thus faster execuation!
    PUZZLE puzzle(PUZZLE_SIZE, vector<int>(PUZZLE_SIZE,0));

    shared_ptr<vector<PUZZLE>> allPuzzles = make_shared<vector<PUZZLE>>();
    string line;
    stringstream sstream;
    ifstream inputFile(inputFileName, std::ios::in);
    if(inputFile.is_open()){
        while(!inputFile.eof()){
            for(int x = 0; x < PUZZLE_SIZE; x++){
                for(int y = 0; y < PUZZLE_SIZE; y++){
                    inputFile >> puzzle[x][y];
                }
            }
            allPuzzles->push_back(puzzle);
        }
    }
    inputFile.close();
    return allPuzzles;
}

void printPuzzles(shared_ptr<vector<PUZZLE>> puzzles)
{
    for(PUZZLE puzzle : *puzzles){
        for(int x = 0; x < PUZZLE_SIZE; x++){
            for(int y = 0; y < PUZZLE_SIZE; y++){
                cout << puzzle[x][y] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
}



