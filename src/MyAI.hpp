// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Liwei Lu, Xinyi Ai
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
#include <unordered_set>
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
        
        bool operator==(const Tile& t) const
        {
            return (this->x * 1000 + y == t.x * 1000 + y);
        }
    };
    
    class modelCheckingNode{
    public:
        
    };
    
    class TileHashFunction{
    public:
        size_t operator () (const Tile &t) const{
            return t.x * 1000 + t.y;
        }
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
    vector<Tile*> uncoveredFrontier;
    vector<Tile> coveredFrontier;
    
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
    Action randomMove();
    
    // model checking function
    void modelChecking(unsigned &totalModels, vector<int> coveredFrontier, vector<Tile> unassignedTile, vector<unsigned> &minePossibility);
    
    Tile getNextModelTile(const vector<int> &coveredFrontier, const vector<Tile> &unassignedTile);
    
    
};

#endif //MINE_SWEEPER_CPP_SHELL_MYAI_HPP
