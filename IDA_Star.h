#ifndef IDA_STAR_H
#define IDA_STAR_H

#include <memory>
#include <vector>
#include <iostream>

using namespace std;

const int PUZZLE_SIZE = 4;
const int SPACE = -1;
typedef vector<vector<int>> PUZZLE;

struct Position
{
    int row;
    int column;

    Position& operator=(const Position& pos){
        this->row = pos.row;
        this->column = pos.column;
        return *this;
    }
};

enum class PossibleMove
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class IDA_Star
{
    public:
        /** Default constructor */
        IDA_Star(shared_ptr<vector<PUZZLE>> _puzzles);
        /** Default destructor */
        ~IDA_Star();

        void solvePuzzles();

    protected:

    private:

        ///PRIVATE VARIABLES
        shared_ptr<vector<PUZZLE>> puzzles;
        //We will build a linear puzzle in order to do a parity test on the puzzles to check if they are solvable?
        vector<int> linearPuzzle;
        int numberOfPuzzles;
        int heuristic;
        int fscore;
        int gscore;
        //Position spacePos;

        ///PRIVATE FUNCTIONS
        //Locates the position of the space in a given puzzle as indicated by the space.
        Position locateSpace(int puzzleIndex);
        bool isPuzzleSolvable(int puzzleIndex, Position spacePosition);
        vector<PossibleMove> legalMoves(Position spacePos);
        bool solvePuzzle(shared_ptr<PUZZLE> puzzle, Position spaceLocation);
        void play(shared_ptr<PUZZLE> puzzle, PossibleMove _move, Position spacePos);

        ///Private Helper functions
        bool isEven(int number);
        string move(PossibleMove _move);
        void printPuzzle(shared_ptr<PUZZLE> puzzles);



};

#endif // IDA_STAR_H
