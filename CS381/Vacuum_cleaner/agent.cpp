#include <ctime> //for random seed
#include <cmath>
#include <algorithm>
#include "agent.h"

Agent::Agent(int random_seed)
{
    if (random_seed == 0)
        std::srand(static_cast<unsigned>(std::time(0)));
    else
        std::srand(random_seed);

    // Initialize state variables
    dir = 0; // 0: up, 1: right, 2: down, 3: left
    dx = 0;
    dy = 0;
    wasHome = true;
    returning = false;
    stepsSinceLastDirt = 0;
    bumped = true;
    turningRight = true;
    turnCount = 0;
    visited.push_back({0, 0});
}

Action Agent::GetAction(Percept p)
{
    if (p.dirt)
    {
        // Clean dirt immediately
        stepsSinceLastDirt = 0;
        returning = false;
        return SUCK;
    }
    else
    {
        stepsSinceLastDirt++;
    }

    if (p.home && !wasHome)
    {
        // Shut off if back at home after cleaning
        return SHUTOFF;
    }

    if (stepsSinceLastDirt > MAX_STEPS_WITHOUT_DIRT)
    {
        // Trigger returning home if no dirt is found for a while
        if (!returning)
        {
            if (!bumped)
            {
                UpdatePosition();
                turnCount = 0;
            }
            bumped = false;
        }
        returning = true;
    }

    if (returning)
    {
        // Find the earliest visited adjacent position and move toward it
        std::pair<int, int> target = FindEarliest();
        return MoveTowardPosition(target);
    }

    // // Check if cell visited before
    // if (std::find(visited.begin(), visited.end(), std::make_pair(dx, dy)) != visited.end() - 1)
    // {
    //     std::pair<int, int> adjacentPositions[4] = {
    //         {dx, dy + 1}, // Up
    //         {dx + 1, dy}, // Right
    //         {dx, dy - 1}, // Down
    //         {dx - 1, dy}  // Left
    //     };

    //     // Check if left and right cell has not been visited or a wall
    //     if (std::find(visited.begin(), visited.end(), adjacentPositions[(dir + 3) % 4]) == visited.end() ||
    //         walls.find(adjacentPositions[(dir + 3) % 4]) == walls.end())
    //     {
    //         // Left unchecked
    //         dir = (dir + 3) % 4;
    //         return LEFT;
    //     }
    //     else if (std::find(visited.begin(), visited.end(), adjacentPositions[(dir + 1) % 4]) == visited.end() ||
    //              walls.find(adjacentPositions[(dir + 1) % 4]) == walls.end())
    //     {
    //         // Right unchecked
    //         dir = (dir + 1) % 4;
    //         return RIGHT;
    //     }
    // }

    turningRight = turnCount < 1 ? true : false;

    if (p.bump)
    {
        // Add wall into set
        std::pair<int, int> adjacentPositions[4] = {
            {dx, dy + 1}, // Up
            {dx + 1, dy}, // Right
            {dx, dy - 1}, // Down
            {dx - 1, dy}  // Left
        };

        walls.insert(adjacentPositions[dir]);

        // Handle wall collision
        if (turningRight)
        {
            dir = (dir + 1) % 4; // Turn right
            bumped = true;
            turnCount++;
            return RIGHT;
        }
        else
        {
            dir = (dir + 3) % 4; // Turn left
            bumped = true;
            turnCount++;
            if (turnCount == 4)
            {
                turnCount = 0;
            }
            return LEFT;
        }
    }
    else
    {
        // Move forward and update position
        if (!bumped)
        {
            UpdatePosition();
            turnCount = 0;
        }
        bumped = false;
    }

    return FORWARD;
}

void Agent::UpdatePosition()
{
    // Update relative position based on current direction
    switch (dir)
    {
    case 0:
        dy++;
        break; // Up
    case 1:
        dx++;
        break; // Right
    case 2:
        dy--;
        break; // Down
    case 3:
        dx--;
        break; // Left
    }
    wasHome = false;

    // Add current position to visited vector
    visited.push_back({dx, dy});
}

std::pair<int, int> Agent::FindEarliest()
{
    std::pair<int, int> adjacentPositions[4] = {
        {dx, dy + 1}, // Up
        {dx + 1, dy}, // Right
        {dx, dy - 1}, // Down
        {dx - 1, dy}  // Left
    };

    // Search the visited vector for the earliest adjacent position
    for (const auto &pos : visited)
    {
        for (int i = 0; i < 4; i++)
        {
            if (pos == adjacentPositions[i])
            {
                return pos; // Return the earliest adjacent position
            }
        }
    }

    return {0, 0};
}

Action Agent::MoveTowardPosition(std::pair<int, int> target)
{
    // Move toward the target position
    int targetDx = target.first;
    int targetDy = target.second;

    if (targetDy > dy)
    {
        // Move up
        if (dir == 0)
        {
            // dy++;
            UpdatePosition();
            return FORWARD;
        }
        else if (dir == 3)
        {
            dir = (dir + 1) % 4;
            return RIGHT;
        }
        else
        {
            dir = (dir + 3) % 4;
            return LEFT;
        }
    }
    else if (targetDy < dy)
    {
        // Move down
        if (dir == 2)
        {
            // dy--;
            UpdatePosition();
            return FORWARD;
        }
        else if (dir == 1)
        {
            dir = (dir + 1) % 4;
            return RIGHT;
        }
        else
        {
            dir = (dir + 3) % 4;
            return LEFT;
        }
    }
    else if (targetDx > dx)
    {
        // Move right
        if (dir == 1)
        {
            // dx++;
            UpdatePosition();
            return FORWARD;
        }
        else if (dir == 0)
        {
            dir = (dir + 1) % 4;
            return RIGHT;
        }
        else
        {
            dir = (dir + 3) % 4;
            return LEFT;
        }
    }
    else if (targetDx < dx)
    {
        // Move left
        if (dir == 3)
        {
            // dx--;
            UpdatePosition();
            return FORWARD;
        }
        else if (dir == 2)
        {
            dir = (dir + 1) % 4;
            return RIGHT;
        }
        else
        {
            dir = (dir + 3) % 4;
            return LEFT;
        }
    }

    // Default action
    return NOOP;
}
