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
    
    Action act = thumbsRule();
    if (act.action != LEAVE)
        return act;

    return randomMove();
}

//initialize board
void MyAI::initializeBoard(){
    for (int i = 0; i < colDimension; i++) {
        board.push_back(vector<Tile>());
        for (int j = 0; j < rowDimension; j++) {
            board[i].push_back(Tile(i,j));
        }
    }
}


// update the information we got from the perceived number
bool MyAI::update(int number){
    if (number < 0 || number > 8)
        return false;

    int x = agentX;
    int y = agentY;
    
    if (board[x][y].uncovered)
        return false;
    
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
Agent::Action MyAI::thumbsRule(){
    for (int i = 0; i < innerFrontier.size(); i++) {
        Tile* temp = innerFrontier[i];
        vector<Tile> neighbors = getBlankNeighbors(*temp);
        if (temp->effective == 0){
            if (!neighbors.empty()){
                agentX = neighbors[0].x;
                agentY = neighbors[0].y;
                return {UNCOVER, agentX, agentY};
            }
            innerFrontier.erase(innerFrontier.begin() + i);
            return thumbsRule();
        }
        
        if (temp->effective == neighbors.size()) {
            for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
                flag(*it);
            innerFrontier.erase(innerFrontier.begin() + i);
            return thumbsRule();
        }
    }
    
    return {LEAVE, -1, -1};
}


// return a vector of covered and unflaged neighbor of t
vector<MyAI::Tile> MyAI::getBlankNeighbors(const Tile &t){
    vector<Tile> neighbors;
    int x = t.x;
    int y = t.y;
    for (int i = x - 1; i < x + 2; i++) {
        if (i < 0 || i >= colDimension)
            continue;
        for (int j = y - 1; j < y + 2; j++) {
            if (j < 0 || j >= rowDimension || (i == x && j == y))
                continue;
            if (!board[i][j].uncovered && !board[i][j].flag)
                neighbors.push_back(board[i][j]);
        }
    }
    
    return neighbors;
}


// flag a tile and the effective label of its neighbor minus 1
bool MyAI::flag(const Tile &t){
    if (t.flag)
        return false;
    
    flagedMine++;
    int x = t.x;
    int y = t.y;
    
    board[x][y].flag = true;
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
Agent::Action MyAI::randomMove(){
    std::cout<<"ramdon"<<std::endl;
    set<Tile, Tile> exception;
    for (auto it = innerFrontier.begin(); it != innerFrontier.end(); ++it) {
        vector<Tile> neighbors = getBlankNeighbors(*(*it));
        exception.insert(neighbors.begin(), neighbors.end());
    }
    
    for (int i = 0; i < colDimension; i++) {
        for (int j = 0; j < rowDimension; j++) {
            if (exception.find(board[i][j]) != exception.end())
                continue;
            if (board[i][j].flag || board[i][j].uncovered)
                continue;
            agentX = i;
            agentY = j;
            cout << "first" << endl;
            return {actions[1], agentX, agentY};
        }
    }
    
    for (int i = 0; i < colDimension; i++) {
           for (int j = 0; j < rowDimension; j++) {
               if (board[i][j].flag || board[i][j].uncovered)
                   continue;
               agentX = i;
               agentY = j;
               cout << "second" << endl;
               return {actions[1], agentX, agentY};
           }
       }
    
    return {actions[0], -1, -1};
}
