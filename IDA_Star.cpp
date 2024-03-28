#include "IDA_Star.h"

IDA_Star::IDA_Star(shared_ptr<vector<PUZZLE>> _puzzles)
{
    puzzles = _puzzles;
    numberOfPuzzles = puzzles->size();

    //Reserve memory for the linear vector.
    linearPuzzle.reserve(PUZZLE_SIZE * PUZZLE_SIZE);
}

IDA_Star::~IDA_Star()
{
    //dtor
}

void IDA_Star::solvePuzzles()
{
    vector<PossibleMove> possibleMoves;
    for(int puzzleNumber = 0; puzzleNumber < numberOfPuzzles; puzzleNumber++){
        Position spacePosition = locateSpace(puzzleNumber);
        if(isPuzzleSolvable(puzzleNumber, spacePosition)){
            shared_ptr<PUZZLE> puzzle = make_shared<PUZZLE>(puzzles->at(puzzleNumber));
            solvePuzzle(puzzle, spacePosition);
        }
    }
}

bool IDA_Star::solvePuzzle(shared_ptr<PUZZLE> puzzle, Position spacePosition)
{
    vector<PossibleMove> possibleMoves = legalMoves(spacePosition);
    cout << move(possibleMoves[2]) << "\n";
    play(puzzle, possibleMoves[2], spacePosition);
    printPuzzle(puzzle);

    return true;
}

void IDA_Star::play(shared_ptr<PUZZLE> puzzle, PossibleMove _move, Position spacePos)
{
    int tempNumber;
    switch(_move){
        case PossibleMove::LEFT:
            tempNumber = ((*puzzle))[spacePos.row][spacePos.column - 1];
            ((*puzzle))[spacePos.row][spacePos.column - 1] = SPACE;
            ((*puzzle))[spacePos.row][spacePos.column] = tempNumber;
            return;
        case PossibleMove::RIGHT:
            tempNumber = (*puzzle)[spacePos.row][spacePos.column + 1];
            (*puzzle)[spacePos.row][spacePos.column + 1] = SPACE;
            (*puzzle)[spacePos.row][spacePos.column] = tempNumber;
            return;
        case PossibleMove::UP:
            tempNumber = (*puzzle)[spacePos.row - 1][spacePos.column];
            (*puzzle)[spacePos.row - 1][spacePos.column] = SPACE;
            (*puzzle)[spacePos.row][spacePos.column] = tempNumber;
            return;
        case PossibleMove::DOWN:
            tempNumber = (*puzzle)[spacePos.row + 1][spacePos.column];
            (*puzzle)[spacePos.row + 1][spacePos.column] = SPACE;
            (*puzzle)[spacePos.row][spacePos.column] = tempNumber;
            return;
        default:
            return;
    }
}

string IDA_Star::move(PossibleMove _move)
{
    switch(_move)
    {
        case PossibleMove::DOWN: return "Down";
        case PossibleMove::LEFT: return "Left";
        case PossibleMove::RIGHT: return "Right";
        case PossibleMove::UP: return "Up";
        default: return "";
    }
}

vector<PossibleMove> IDA_Star::legalMoves(Position spacePos)
{
    vector<PossibleMove> moves;
    if((spacePos.column == 0) && (spacePos.row == 0)){
        moves.push_back(PossibleMove::RIGHT);
        moves.push_back(PossibleMove::DOWN);
    }
    else if((spacePos.column == 3) && (spacePos.row == 0)){
        moves.push_back(PossibleMove::LEFT);
        moves.push_back(PossibleMove::DOWN);
    }
    else if((spacePos.column == 0) && (spacePos.row == 3)){
        moves.push_back(PossibleMove::RIGHT);
        moves.push_back(PossibleMove::UP);
    }
    else if((spacePos.column == 3) && (spacePos.row == 3)){
        moves.push_back(PossibleMove::LEFT);
        moves.push_back(PossibleMove::UP);
    }
    else if(spacePos.column == 0){
        moves.push_back(PossibleMove::RIGHT);
        moves.push_back(PossibleMove::UP);
        moves.push_back(PossibleMove::DOWN);
    }
    else if(spacePos.column == 3){
        moves.push_back(PossibleMove::LEFT);
        moves.push_back(PossibleMove::UP);
        moves.push_back(PossibleMove::DOWN);
    }
    else if(spacePos.row == 0){
        moves.push_back(PossibleMove::RIGHT);
        moves.push_back(PossibleMove::LEFT);
        moves.push_back(PossibleMove::DOWN);
    }
    else if(spacePos.row == 3){
        moves.push_back(PossibleMove::RIGHT);
        moves.push_back(PossibleMove::LEFT);
        moves.push_back(PossibleMove::UP);
    }
    else{
        moves.push_back(PossibleMove::RIGHT);
        moves.push_back(PossibleMove::LEFT);
        moves.push_back(PossibleMove::UP);
        moves.push_back(PossibleMove::DOWN);
    }
    return moves;
}

Position IDA_Star::locateSpace(int puzzleIndex)
{

    linearPuzzle.clear();  //Clear the linear puzzle vector before re-using it.
    Position spacePos;
    for(int row = 0; row < PUZZLE_SIZE; row++){
        for(int column = 0; column < PUZZLE_SIZE; column++){
            if(puzzles->at(puzzleIndex)[row][column] == SPACE){
                spacePos.row = row;
                spacePos.column = column;
                //std::cout << "(" << spacePos.row << "," << spacePos.column << ") ";
            }
            linearPuzzle.push_back(puzzles->at(puzzleIndex)[row][column]);
        }
     }
     return spacePos;
}

bool IDA_Star::isPuzzleSolvable(int puzzleIndex, Position spacePos)
{
    /// A puzzle will be solvable if one of the following conditions is met
    /// 1) Total number of inversions is EVEN and the space is in an ODD row
    /// 2) Total number of inversions in ODD and the space is in an EVEN row

    int numberOfInversions = 0;
    for(int i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE; i++){
        for(int j = i + 1; j < PUZZLE_SIZE * PUZZLE_SIZE; j++){
            if((linearPuzzle[i] > linearPuzzle[j]) && (linearPuzzle[j] != SPACE)){
                numberOfInversions++;
            }
        }
    }

    if(isEven(numberOfInversions) != isEven(spacePos.row)) return true;
    else return false;
}

bool IDA_Star::isEven(int number)
{
    if(number % 2 == 0) return true;
    else return false;
}

void IDA_Star::printPuzzle(shared_ptr<PUZZLE> puzzle)
{
    for(int x = 0; x < PUZZLE_SIZE; x++){
        for(int y = 0; y < PUZZLE_SIZE; y++){
            cout << (*puzzle)[x][y] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}



