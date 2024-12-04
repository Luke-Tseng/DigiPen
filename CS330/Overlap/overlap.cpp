#include "overlap.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

int MOD = 7001; // for modulo counting

struct R
{
    int x1, y1, x2, y2;
};

struct Line
{
    int x1, x2, y;
    bool start; // true - start. false - end
    bool operator<(Line const &other) const { return y < other.y; }
};

////////////////////////////////////////////////////////////////////////////////
//   START BRUTE FORCE
////////////////////////////////////////////////////////////////////////////////
struct BruteForce
{
    std::vector<std::vector<int>> field;
    std::map<int, int> freq;

    BruteForce(int dim) : field(dim, std::vector<int>(dim, 0)), freq() {}

    void AddRectangle(int x1, int y1, int x2, int y2)
    {
        for (int i = x1; i <= x2; ++i)
        { // note "<=" !!!
            for (int j = y1; j <= y2; ++j)
            { // note "<=" !!!
                ++field[i][j];
            }
        }
    }

    void Process()
    {
        for (unsigned i = 0; i < field.size(); ++i)
        {
            for (unsigned j = 0; j < field.size(); ++j)
            {
                ++freq[field[i][j]];
                freq[field[i][j]] %= MOD; // modulo so no overflow
            }
        }
    }
};

std::map<int, int> brute_force(char const *filename)
{
    std::ifstream in(filename); // closed automatically
    if (in.fail())
        throw "Cannot open input file";

    std::vector<R> rects;
    int dim, N;
    in >> dim >> N;
    for (int r = 0; r < N; ++r)
    {
        int x1, y1, x2, y2;
        in >> x1 >> y1 >> x2 >> y2;
        rects.push_back(R{x1, y1, x2, y2});
    }

    BruteForce brf(dim);

    for (auto const &r : rects)
    {
        brf.AddRectangle(r.x1, r.y1, r.x2, r.y2);
    }

    brf.Process();

    return brf.freq;
}
////////////////////////////////////////////////////////////////////////////////
//   END BRUTE FORCE
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   YOUR IMPLEMENTATION
////////////////////////////////////////////////////////////////////////////////

std::map<int, int> overlap_coverage(char const *filename)
{
    std::ifstream in(filename); // closed automatically
    if (in.fail())
        throw "Cannot open input file";

    // Create lines
    std::vector<Line> lines;
    std::vector<R> rects;
    int dim, N;
    in >> dim >> N;
    for (int r = 0; r < N; ++r)
    {
        int x1, y1, x2, y2;
        in >> x1 >> y1 >> x2 >> y2;
        R rect{x1, y1, x2, y2};
        rects.push_back(rect);

        Line newLineStart{rect.x1, rect.x2, rect.y1, true};
        Line newLineEnd{rect.x1, rect.x2, rect.y2, false};

        lines.push_back(newLineStart);
        lines.push_back(newLineEnd);
    }

    // Sort the lines by y (ascending order)
    std::sort(lines.begin(), lines.end(), [](const Line &l1, const Line &l2)
              {
        if (l1.y == l2.y)
        {
            return l1.start;
        }
        return l1.y < l2.y; });

    // Initialize variables
    std::vector<int> lineCount(dim, 0);
    std::map<int, int> coverage; // Final result: coverage count -> total area

    int prevY = -1;

    for (const auto &line : lines)
    {
        int currY = line.y;

        if (line.start)
        {
            for (int i = line.x1; i <= line.x2; ++i)
            {
                lineCount[i]++;
            }
        }

        // std::cout << "y: " << currY << std::endl;
        // std::cout << "lineCount: ";
        // for (int count : lineCount)
        // {
        //     std::cout << count << " ";
        // }
        // std::cout << std::endl;

        if (currY != prevY)
        {
            int diff = prevY == -1 ? 1 : currY - prevY;
            for (int i : lineCount)
            {
                if (i > 0)
                {
                    coverage[i] += diff;
                    coverage[i] %= MOD;
                }
            }
        }

        if (!line.start)
        {
            for (int i = line.x1; i <= line.x2; ++i)
            {
                lineCount[i]--;
            }
        }

        // std::cout << "coverage: ";
        // for (const auto &pair : coverage)
        // {
        //     std::cout << "{" << pair.first << ": " << pair.second << "} ";
        // }
        // std::cout << std::endl;

        prevY = currY;
    }

    // Return the coverage map
    return coverage;
}

////////////////////////////////////////////////////////////////////////////////
std::map<int, int> overlap(char const *filename)
{
    return overlap_coverage(filename); // change to your function
}

// for debugging - see pictures
// in0
// in1
// in2
//
// simple tests ( less than 0.1s)
// in3    100   rectangles  gen.exe 1000 100 50 > in3
// in4    500   rectangles  gen.exe 2000 500 50 > in4
// in5    1000  rectangles  gen.exe 5000 1000 50 > in5
//
// algorithm correctness test: (80% of the grade)
// in_scan    500  rectangles  gen.exe 1000 500 50 > in_scan
// in_scan_10 is exactly as the above but scaled 10 times
// brute-force can solve in_scan in 0.1s
// but it will take brute-force 10^2 times longer to solve in_scan_10
// if scan is implemented for only Y coordinates (as described in the handout), it will be 10 times slower
// if scan is implemented for both coordinates, it will be the same time as in_scan
//
// bigger test
// in6    5000  rectangles  gen.exe 50000 5000 50 > in6
//
// stress test
// in7    10000 rectangles  gen.exe 100000 10000 50 > in7
// in8    10000 rectangles  gen.exe 100000 10000 1 > in8
