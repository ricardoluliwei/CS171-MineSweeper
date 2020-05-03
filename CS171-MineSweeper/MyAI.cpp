// ======================================================================
// FILE:        MyAI.cpp
//
// AUTHOR:      Jian Li
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================

#include "MyAI.hpp"

MyAI::MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY ) : Agent()
{
    this->rowDimension = _rowDimension;
    this->colDimension = _colDimension;
    this->totalMines = _totalMines;
    this->agentX = _agentX;
    this->agentY = _agentY;
    this->remainCovered = rowDimension * colDimension;
    initializeBoard();
    
};

Agent::Action MyAI::getAction( int number )
{
    update(number);
    
    if (shouldExit())
        return {LEAVE,-1,-1};
    
    for (auto it = innerFrontier.begin(); it != innerFrontier.end(); ++it) {
        Action act = thumbsRule(*it);
        if (act.action == actions[0])
            continue;
        if (act.action == actions[3]) {
            innerFrontier.erase(it);
            continue;
        }
        agentX = act.x;
        agentY = act.y;
        return act;
    }
    
    vector<Tile*> neighbors = getBlankNeighbors(innerFrontier[0]);
    
    return randomMove(set<Tile*>(neighbors.begin(), neighbors.end()));


}

//initialize board
void MyAI::initializeBoard(){
    board = new Tile*[colDimension];
    for ( int index = 0; index < colDimension; ++index ){
        board[index] = new Tile[rowDimension];
        for (int j = 0; j < rowDimension; j++) {
            board[index][j].x = index;
            board[index][j].y = j;
        }
    }
}


// update the information we got from the perceived number
bool MyAI::update(int number){
    if (number < 0 || number > 8)
        return false;

    int x = agentX;
    int y = agentY;
    board[x][y].number = number;
    board[x][y].effective += number;
    board[x][y].uncovered = true;
    remainCovered--;
    
    innerFrontier.push_back(&board[x][y]);
    
    return true;
}

// use it when the tile's effective label is 0 or equal the number of
// uncoverd neighbors, return leave if we can't use thumbsRule
// return unflag if the tile has no neighbors
Agent::Action MyAI::thumbsRule(const Tile* t){
    vector<Tile*> neighbors = getBlankNeighbors(t);
    if (t->effective == 0){
        if (!neighbors.empty())
            return {actions[1], neighbors[0]->x, neighbors[0]->y};
        return {actions[3], -1, -1};
    }
    
    if (t->effective == neighbors.size()) {
        for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
            flag(*it);
        }
    }
    
    return {actions[0], -1, -1};
}


// return a vector of covered and unflaged neighbor of t
vector<MyAI::Tile*> MyAI::getBlankNeighbors(const Tile* t){
    vector<Tile*> neighbors;
    int x = t->x;
    int y = t->y;
    for (int i = x - 1; i < x + 2; i++) {
        if (i < 0 || i >= colDimension)
            continue;
        for (int j = y - 1; j < y + 2; j++) {
            if (j < 0 || j >= rowDimension || (i == x && j == y))
                continue;
            if (!board[i][j].uncovered && !board[i][j].flag)
                neighbors.push_back(&board[i][j]);
        }
    }
    
    return neighbors;
}


// flag a tile and the effective label of its neighbor minus 1
bool MyAI::flag(Tile* t){
    if (t->flag)
        return false;
    
    t->flag = true;
    int x = t->x;
    int y = t->y;
    for (int i = x - 1; i < x + 2; i++) {
        if (i < 0 || i >= colDimension)
            continue;
        for (int j = y - 1; j < y + 2; j++) {
            if (j < 0 || j >= rowDimension || (i == x && j == y))
                continue;
            board[i][j].effective--;
        }
    }
    return true;
}

// use it at the beginning to check exit or not
bool MyAI::shouldExit(){
    return remainCovered == totalMines;
}

// random uncover an covered tile outside of outerFrontier
Agent::Action MyAI::randomMove(set<Tile*> exception){
    for (int i = 0; i < colDimension; i++) {
        for (int j = 0; j < rowDimension; j++) {
            if (exception.find(&board[i][j]) != exception.end())
                continue;
            agentX = i;
            agentY = j;
            return {actions[1], i, j};
        }
    }
    
    return {actions[0], -1, -1};
}
