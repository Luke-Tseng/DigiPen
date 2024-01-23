#include "rummikub.h"
#include <algorithm>
#include <unordered_map>

RummiKub::RummiKub()
{
}

void RummiKub::Add(Tile const &tile)
{
	hand.push_back(tile);
}

void RummiKub::Solve()
{
	std::sort(hand.begin(), hand.end(), [](Tile const &a, Tile const &b)
			  { return a.denomination > b.denomination; });
	solveRecursion(hand);
}

// get solution - groups
std::vector<std::vector<Tile>> RummiKub::GetGroups() const
{
	return groups;
}

// get solution - runs
std::vector<std::vector<Tile>> RummiKub::GetRuns() const
{
	return runs;
}
// if both vectors are empty - no solution possible

bool RummiKub::solveRecursion(std::vector<Tile> currHand)
{
	// ends when hand size is 0
	if (currHand.size() == 0)
	{
		return checkRuns() && checkGroups();
	}

	Tile currTile = currHand.back();
	currHand.pop_back();

    std::cout << "\n=====================================================\n";
    std::cout << "table: current tile " << currTile << "\n";
    for( auto const& g : groups ) {
      std::cout << "(G( ";
      for( auto const& t : g  ) { std::cout << t << " "; }
      std::cout << " )))\n";
    }
    for( auto const& r : runs ) {
      std::cout << "(R( ";
      for( auto const& t : r  ) { std::cout << t << " "; }
      std::cout << " )))\n";
    }


	for (int run = 0; run <= static_cast<int>(runs.size()); run++)
	{
		if (addRun(currTile, run))
		{
			if (solveRecursion(currHand))
			{
				return true;
			}
			undoRun(run);
		}
	}

	for (int group = 0; group <= static_cast<int>(groups.size()); group++)
	{
		if (addGroup(currTile, group))
		{
			if (solveRecursion(currHand))
			{
				return true;
			}
			undoGroup(group);
		}
	}

	return false;
}

bool RummiKub::addRun(Tile tile, int index)
{
	if (runs.empty() || static_cast<int>(runs.size()) == index)
	{
		runs.push_back({tile});
		return true;
	}

	if (static_cast<int>(runs.size()) > index && tile.color == runs[index][0].color && tile.denomination == static_cast<int>(runs[index][0].denomination + runs[index].size()))
	{
		runs[index].push_back(tile);
		return true;
	}

	return false;
}

bool RummiKub::addGroup(Tile tile, int index)
{
	if (groups.empty() || static_cast<int>(groups.size()) == index)
	{
		groups.push_back({tile});
		return true;
	}

	if (static_cast<int>(groups.size()) > index && groups[index].size() < 4 && tile.color != groups[index][0].color && tile.denomination == groups[index][0].denomination)
	{
		groups[index].push_back(tile);
		return true;
	}

	return false;
}

void RummiKub::undoRun(int index)
{
	if (static_cast<int>(runs.size()) > index)
	{
		runs[index].pop_back();
		if (runs[index].size() == 0)
		{
			runs.erase(runs.begin() + index);
		}
	}
}

void RummiKub::undoGroup(int index)
{
	if (static_cast<int>(groups.size()) > index)
	{
		groups[index].pop_back();
		if (groups[index].size() == 0)
		{
			groups.erase(groups.begin() + index);
		}
	}
}

bool RummiKub::checkGroup(std::vector<Tile> const &group)
{
	if (group.size() < 3)
	{
		return false;
	}
	return true;
}

bool RummiKub::checkRun(std::vector<Tile> const &run)
{
	if (run.size() < 3)
	{
		return false;
	}

	return true;
}

bool RummiKub::checkGroups()
{
	if (static_cast<int>(groups.size()) == 0)
	{
		return true;
	}
	for (const std::vector<Tile> &group : groups)
	{
		if (!checkGroup(group))
		{
			return false;
		}
	}
	return true;
}

bool RummiKub::checkRuns()
{
	if (static_cast<int>(runs.size()) == 0)
	{
		return true;
	}
	for (const std::vector<Tile> &run : runs)
	{
		if (!checkRun(run))
		{
			return false;
		}
	}
	return true;
}

std::ostream &operator<<(std::ostream &os, Tile const &t)
{
	os << "{ " << t.denomination << ",";
	switch (t.color)
	{
	case Red:
		os << "R";
		break;
	case Green:
		os << "G";
		break;
	case Blue:
		os << "B";
		break;
	case Yellow:
		os << "Y";
		break;
	}
	os << " }";
	return os;
}