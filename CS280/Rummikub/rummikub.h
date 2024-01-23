#ifndef RUMMIKUB_H
#define RUMMIKUB_H
#include <fstream>
#include <vector>
#include <iostream>

enum Color
{
    Red,
    Green,
    Blue,
    Yellow
};

struct Tile
{
    int denomination;
    Color color;
};

std::ostream &operator<<(std::ostream &os, Tile const &t);

class RummiKub
{
public:
    RummiKub();             // empty hand
    void Add(Tile const &); // add a tile to hand

    void Solve(); // solve

    // get solution - groups
    std::vector<std::vector<Tile>> GetGroups() const;
    // get solution - runs
    std::vector<std::vector<Tile>> GetRuns() const;
    // if both vectors are empty - no solution possible
private:
    std::vector<Tile> hand;
    std::vector<std::vector<Tile>> runs;
    std::vector<std::vector<Tile>> groups;

    bool solveRecursion(std::vector<Tile> currHand);

    bool addRun(Tile tile, int index);
    bool addGroup(Tile tile, int index);
    void undoRun(int index);
    void undoGroup(int index);

    bool checkGroups();
    bool checkGroup(std::vector<Tile> const &group);
    bool checkRuns();
    bool checkRun(std::vector<Tile> const &run);
};

#endif
