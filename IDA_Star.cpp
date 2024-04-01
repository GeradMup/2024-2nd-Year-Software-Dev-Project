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
    vector<Move> Moves;
    int numberOfMoves = 1;
    bool solved = false;
    string output = "";

    for(int puzzleNumber = 0; puzzleNumber < numberOfPuzzles; puzzleNumber++){
        Position spacePosition = locateSpace(puzzleNumber);
        minimumCost = INT_MAX;
        if(isPuzzleSolvable(puzzleNumber, spacePosition)){
            shared_ptr<PUZZLE> puzzle = make_shared<PUZZLE>(puzzles->at(puzzleNumber));

            boundaryStateScore = calculateHeuristic(puzzle);
            minimumCost = INT_MAX;

            while(solved == false){
                solved = solvePuzzle(puzzle, spacePosition, numberOfMoves, Move::NONE);
                if(solved == false) {
                    boundaryStateScore = minimumCost;
                    minimumCost = INT_MAX;
                }else{
                    //If solved is equal to true, we have found the solution for the puzzle
                    output = to_string(puzzleNumber) + ", " + "solvable, solved, " + to_string(puzzleOutput.size()) + ":";
                    allPuzzlesOutput.push_back(output);
                    reverse(puzzleOutput.begin(), puzzleOutput.end());
                    allPuzzlesOutput.insert(end(allPuzzlesOutput), begin(puzzleOutput), end(puzzleOutput));
                    allPuzzlesOutput.push_back(" ");
                }
            }
            //After each puzzle is solved, set solved == false so that we can solve the next one
            solved = false;
            puzzleOutput.clear();
        }else{
            //Let's handle the case where a puzzle is not solvable
            output = to_string(puzzleNumber) + ", " + "not solvable";
            allPuzzlesOutput.push_back(output);
            allPuzzlesOutput.push_back(" ");
        }
    }
}

bool IDA_Star::solvePuzzle(shared_ptr<PUZZLE> _puzzle, Position spacePosition, int numberOfMoves, Move previousMove)
{
    vector<Move> Moves = legalMoves(spacePosition, previousMove);
    //cout << move(Moves[2]) << "\n";
    //play(puzzle, Moves[2], spacePosition);
    //printPuzzle(puzzle);
    int heuristic = 0;
    int solvingCost = 0;
    Position newSpacePosition;
    bool puzzleSolved = false;

    for(Move _move : Moves){
        shared_ptr<PUZZLE> puzzle;

        //First check that the puzzle is not already solved before playing.
        //If the puzzle is solved, return true
        //The puzzle is solved if the heuristic of the current state is equal to 0
        heuristic = calculateHeuristic(_puzzle);

        if(heuristic == 0) {
            cout << "Hurray, You have solved the puzzle" << endl;
            printPuzzle(_puzzle);
            //cout << movesTaken << endl;
            return true;
        }

        //cout << "\n" << stringMove(_move) << "\n";
        tie(puzzle, newSpacePosition) = play(_puzzle, _move, spacePosition);

        //printPuzzle(puzzle);
        //cout << endl;
        heuristic = calculateHeuristic(puzzle);
        solvingCost = heuristic + numberOfMoves;

        //cout << "Space Position: (" << newSpacePosition.row << "," << newSpacePosition.column << ")\n";
        //cout << "Heuristic: " << heuristic << endl;
        //cout << "Boundary Value: " << boundaryStateScore << endl;
        //cout << "Moves: " << numberOfMoves << endl;
        //cout << "Total Cost: " << solvingCost << endl;
        //cout << "Minimum Cost: " << minimumCost << endl;

        if(solvingCost  <= boundaryStateScore){
            puzzleSolved = solvePuzzle(puzzle, newSpacePosition, numberOfMoves + 1, _move);
        }else{
            //When the solving cost of this path is greater than our boundary,
            //we stop exploring that path but we still take note of the total cost
            //if we had to continue down than path.
            //This will help us to increase our boundary value if we completely fail to
            //solve the puzzle within the initial boundary value
            if(solvingCost < minimumCost){
                minimumCost = solvingCost;
            }
        }
        if(puzzleSolved){
            //cout << "Solved: " << formatMove(_move, newSpacePosition) << "\n";
            puzzleOutput.push_back(formatMove(_move, newSpacePosition));
            return true; //We need this part in order to exit the for loop!

        }
    }


    return false;
}

int IDA_Star::calculateHeuristic(shared_ptr<PUZZLE> puzzle)
{
    ///Let's first calculate the Manhatten/Taxicab Distance for the puzzle.
    int manhattenDistance = 0;
    int number = 0;

    vector<int> possibleLinearConflicts(2*PUZZLE_SIZE);

    Position destinationPos;
    Position currentPosition;
    for(int row = 0; row < PUZZLE_SIZE; row++){
        for(int col = 0; col < PUZZLE_SIZE; col++){
            number = (*puzzle)[row][col];

            if(number != -1){
                //Manhatten Distance Calc
                destinationPos = destination(number);
                manhattenDistance += abs(destinationPos.row - row);
                manhattenDistance += abs(destinationPos.column - col);
                currentPosition = Position(row,col);

                /// Check all linear conflicts
                // A linear conflict happens when two tiles are in their destination row/column but not their actual final location
                if((row == 0) && (destinationPos.row == 0) && (destinationPos != currentPosition)){
                    possibleLinearConflicts[0] = possibleLinearConflicts[0] + 1;
                }
                else if((row == 1) && (destinationPos.row == 1) && (destinationPos != currentPosition)){
                    possibleLinearConflicts[1] = possibleLinearConflicts[1] + 1;
                }
                else if((row == 2) && (destinationPos.row == 2) && (destinationPos != currentPosition)){
                    possibleLinearConflicts[2] = possibleLinearConflicts[2] + 1;
                }
                else if((row == 3) && (destinationPos.row == 3) && (destinationPos != currentPosition)){
                    possibleLinearConflicts[3] = possibleLinearConflicts[3] + 1;
                }

                if((col == 0) && (destinationPos.column == 0) && (destinationPos != currentPosition)){
                    possibleLinearConflicts[4] = possibleLinearConflicts[4] + 1;
                }
                else if((col == 1) && (destinationPos.column == 1) && (destinationPos != currentPosition)){
                    possibleLinearConflicts[5] = possibleLinearConflicts[5] + 1;
                }
                else if((col == 2) && (destinationPos.column == 2) && (destinationPos != currentPosition)){
                    possibleLinearConflicts[6] = possibleLinearConflicts[6] + 1;
                }
                else if((col == 3) && (destinationPos.column == 3) && (destinationPos != currentPosition)){
                    possibleLinearConflicts[7] = possibleLinearConflicts[7] + 1;
                }
            }
        }
    }

    //Sum up all the linear conflicts
    int linearConflicts = 0;
    for(int index = 0; index < 2 * PUZZLE_SIZE; index++){
        if(isEven(possibleLinearConflicts[index])){
            linearConflicts = linearConflicts + possibleLinearConflicts[index];
        }
    }

    int heuristic = manhattenDistance + linearConflicts;
    return heuristic;
}

Position IDA_Star::destination(int number)
{
    Position destinationPosition;
    destinationPosition.row = (number - 1) / 4;
    destinationPosition.column = (number + 3) % 4;
    return destinationPosition;
}

tuple<shared_ptr<PUZZLE>, Position> IDA_Star::play(shared_ptr<PUZZLE> _puzzle, Move _move, Position spacePos)
{
    int tempNumber;
    shared_ptr<PUZZLE> puzzle = make_shared<PUZZLE>(*_puzzle);
    switch(_move){
        case Move::RIGHT:
            tempNumber = ((*puzzle))[spacePos.row][spacePos.column - 1];
            ((*puzzle))[spacePos.row][spacePos.column - 1] = SPACE;
            ((*puzzle))[spacePos.row][spacePos.column] = tempNumber;
            spacePos.column = spacePos.column - 1;
            return { puzzle, spacePos };

        case Move::LEFT:
            tempNumber = (*puzzle)[spacePos.row][spacePos.column + 1];
            (*puzzle)[spacePos.row][spacePos.column + 1] = SPACE;
            (*puzzle)[spacePos.row][spacePos.column] = tempNumber;
            spacePos.column = spacePos.column + 1;
            return { puzzle, spacePos };

        case Move::DOWN:
            tempNumber = (*puzzle)[spacePos.row - 1][spacePos.column];
            (*puzzle)[spacePos.row - 1][spacePos.column] = SPACE;
            (*puzzle)[spacePos.row][spacePos.column] = tempNumber;
            spacePos.row = spacePos.row - 1;
            return { puzzle, spacePos };

        case Move::UP:
            tempNumber = (*puzzle)[spacePos.row + 1][spacePos.column];
            (*puzzle)[spacePos.row + 1][spacePos.column] = SPACE;
            (*puzzle)[spacePos.row][spacePos.column] = tempNumber;
            spacePos.row = spacePos.row + 1;
            return { puzzle, spacePos };

        default:
            return { puzzle, spacePos };
    }
}

string IDA_Star::stringMove(Move _move)
{
    switch(_move)
    {
        case Move::DOWN: return "D";
        case Move::LEFT: return "L";
        case Move::RIGHT: return "R";
        case Move::UP: return "U";
        case Move::NONE: return "";
        default: return "";
    }
}

vector<Move> IDA_Star::legalMoves(Position spacePos, Move previousMove)
{
    vector<Move> moves;
    if((spacePos.column == 0) && (spacePos.row == 0)){
        if(previousMove != Move::RIGHT) moves.push_back(Move::LEFT);
        if(previousMove != Move::DOWN) moves.push_back(Move::UP);
    }
    else if((spacePos.column == 3) && (spacePos.row == 0)){
        if(previousMove != Move::LEFT) moves.push_back(Move::RIGHT);
        if(previousMove != Move::DOWN) moves.push_back(Move::UP);
    }
    else if((spacePos.column == 0) && (spacePos.row == 3)){
        if(previousMove != Move::RIGHT) moves.push_back(Move::LEFT);
        if(previousMove != Move::UP) moves.push_back(Move::DOWN);
    }
    else if((spacePos.column == 3) && (spacePos.row == 3)){
        if(previousMove != Move::LEFT) moves.push_back(Move::RIGHT);
        if(previousMove != Move::UP) moves.push_back(Move::DOWN);
    }
    else if(spacePos.column == 0){
        if(previousMove != Move::RIGHT) moves.push_back(Move::LEFT);
        if(previousMove != Move::UP) moves.push_back(Move::DOWN);
        if(previousMove != Move::DOWN) moves.push_back(Move::UP);
    }
    else if(spacePos.column == 3){
        if(previousMove != Move::LEFT) moves.push_back(Move::RIGHT);
        if(previousMove != Move::UP) moves.push_back(Move::DOWN);
        if(previousMove != Move::DOWN) moves.push_back(Move::UP);
    }
    else if(spacePos.row == 0){
        if(previousMove != Move::RIGHT) moves.push_back(Move::LEFT);
        if(previousMove != Move::LEFT) moves.push_back(Move::RIGHT);
        if(previousMove != Move::DOWN) moves.push_back(Move::UP);
    }
    else if(spacePos.row == 3){
        if(previousMove != Move::RIGHT) moves.push_back(Move::LEFT);
        if(previousMove != Move::LEFT) moves.push_back(Move::RIGHT);
        if(previousMove != Move::UP) moves.push_back(Move::DOWN);
    }
    else{
        if(previousMove != Move::RIGHT) moves.push_back(Move::LEFT);
        if(previousMove != Move::LEFT) moves.push_back(Move::RIGHT);
        if(previousMove != Move::UP) moves.push_back(Move::DOWN);
        if(previousMove != Move::DOWN) moves.push_back(Move::UP);
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

string IDA_Star::formatMove(Move _move, Position spacePosition){
    return to_string(spacePosition.column) + " " + to_string(spacePosition.row) + " " + stringMove(_move);
}

vector<string> IDA_Star::getPuzzleSolutions()
{
    return allPuzzlesOutput;
}



