#ifndef AGENT_H
#define AGENT_H
#include "definitions.h"
#include <vector>
#include <set>

#define MAX_STEPS_WITHOUT_DIRT 30

class Agent
{
public:
    Agent(int random_seed);
    Action GetAction(Percept p);
    void UpdatePosition();
    Action MoveTowardPosition(std::pair<int, int> target);
    std::pair<int, int> FindEarliest();

private:
    int dir;        // 0 = up, 1 = right, 2 = down, 3 = left for direction
    int dx;         // relative x distance from home
    int dy;         // relative y distance from home
    bool wasHome;   // to detect when we first leave home
    bool returning; // true when we're trying to return home
    int stepsSinceLastDirt;
    bool bumped;
    bool turningRight;
    int turnCount;
    std::set<std::pair<int, int>> walls;
    std::vector<std::pair<int, int>> visited;
};

#endif
