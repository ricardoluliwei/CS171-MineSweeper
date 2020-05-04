// ======================================================================
// FILE:        MyAI.hpp
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

#ifndef MINE_SWEEPER_CPP_SHELL_MYAI_HPP
#define MINE_SWEEPER_CPP_SHELL_MYAI_HPP

#include "Agent.hpp"
#include <iostream> // temporary use
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <cstdlib>

using namespace std;



class MyAI : public Agent
{
typedef pair<int, int> position;
    
public:
    MyAI ( int _rowDimension, int _colDimension, int _totalMines, int _agentX, int _agentY );

    Action getAction ( int number ) override;

private:
    class Tile{
    public:
        int x = 0;
        int y = 0;
        bool uncovered  = false; // the tile uncovered or not
        bool flag       = false; // the tile has been flag or not
        int  number     = 0;     // records number of bombs around
        int effective = 0; // recods the number of bombs around - flaged tiles around
        
        Tile(int x, int y){
            this->x = x;
            this->y = y;
        }
        
        Tile(){}
        
        // because pq's top is the greatest element, so when effectice is greater, Tile is smaller
        bool operator () (const Tile &t1, const Tile &t2){return t1.effective > t2.effective;}
    };
    
    const Action_type actions[4] =
    {
            LEAVE,
            UNCOVER,
            FLAG,
            UNFLAG,
    };
    
    vector<vector<Tile>> board;
    int flagedMine = 0;
    int remainCovered = 0;
    vector<Tile*> innerFrontier;
    vector<Tile> outerFrontier;
    
    void initializeBoard();
    // update the information we got from the perceived number
    bool update(int number);
    
    // use it when the top of innerFrontier's effective label is 0 or equal the number of
    // uncoverd neighbors
    Action thumbsRule();
    // return a covered neighbor of (x,y), return NULL if all
    // neighbors are uncovered
    vector<Tile> getBlankNeighbors(const Tile &t);
    // flag a tile and let the effective label of its neighbor minus 1
    bool flag(const Tile &t);
    
    // use it at the beginning to check exit or not
    bool shouldExit();
    
    // random uncover an covered tile
    Action randomMove(set<Tile, Tile> exception);

    
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
