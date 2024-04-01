#ifndef IDA_STAR_H
#define IDA_STAR_H

#include <memory>
#include <vector>
#include <iostream>
#include <cmath>
#include <tuple>
#include <algorithm>

using namespace std;

const int PUZZLE_SIZE = 4;
const int SPACE = -1;
typedef vector<vector<int>> PUZZLE;

struct Position
{
    int row;
    int column;

    Position(){}
    Position(int _row, int _col){
        this->row = _row;
        this->column = _col;
    }

    Position& operator=(const Position& pos){
        this->row = pos.row;
        this->column = pos.column;
        return *this;
    }

    bool operator==(const Position& otherPosition){
        if((this->row == otherPosition.row) && (this->column == otherPosition.column)) return true;
        else return false;
    }

    bool operator != (const Position& otherPosition){
        if(this->row != otherPosition.row) { return true; }
        else if(this->column != otherPosition.column) { return true; }
        else return false;
    }
};

enum class Move
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

class IDA_Star
{
    public:
        /** Default constructor */
        IDA_Star(shared_ptr<vector<PUZZLE>> _puzzles);
        /** Default destructor */
        ~IDA_Star();

        void solvePuzzles();
        vector<string> getPuzzleSolutions();

    protected:

    private:

        ///PRIVATE VARIABLES
        shared_ptr<vector<PUZZLE>> puzzles;
        //We will build a linear puzzle in order to do a parity test on the puzzles to check if they are solvable?
        vector<int> linearPuzzle;
        int numberOfPuzzles;
        //Stores the value against which the heuristics of each puzzle state will be compared!
        int boundaryStateScore;
        //Stores the minimum cost of solving the puzzle found at any given state;
        int minimumCost;
        vector<string> allPuzzlesOutput;
        vector<string> puzzleOutput;

        ///PRIVATE FUNCTIONS
        //Locates the position of the space in a given puzzle as indicated by the space.
        Position locateSpace(int puzzleIndex);
        bool isPuzzleSolvable(int puzzleIndex, Position spacePosition);
        vector<Move> legalMoves(Position spacePos, Move previousMove);
        bool solvePuzzle(shared_ptr<PUZZLE> puzzle, Position spaceLocation, int numberOfMoves, Move previousMove);

        //Function returns a new puzzle state after moving
        tuple<shared_ptr<PUZZLE>, Position> play(shared_ptr<PUZZLE> puzzle, Move _move, Position spacePos);

        ///Private Helper functions
        bool isEven(int number);
        string stringMove(Move _move);
        string formatMove(Move _move, Position spacePosition);
        void printPuzzle(shared_ptr<PUZZLE> puzzles);
        int calculateHeuristic(shared_ptr<PUZZLE> puzzle);
        Position destination(int number);


};

#endif // IDA_STAR_H
