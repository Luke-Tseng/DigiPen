#ifndef ASTAR
#define ASTAR

#include <algorithm>
#include <queue>
#include <vector>

// callback object for Astar
template <typename GraphType, typename AstarType>
class Callback
{
protected:
    GraphType const &g;

public:
    Callback(GraphType const &_g) : g(_g) {}
    virtual ~Callback() {}
    virtual void OnIteration(AstarType const &) {}
    virtual void OnFinish(AstarType const &) {}
};

template <typename GraphType, typename Heuristic>
class Astar
{
public:
    ////////////////////////////////////////////////////////////
    Astar(GraphType const &_graph, Callback<GraphType, Astar> &cb) : graph(_graph),
                                                                     openlist(),
                                                                     closedlist(),
                                                                     solution(),
                                                                     callback(cb),
                                                                     start_id(0),
                                                                     goal_id(0)
    {
    }
    ////////////////////////////////////////////////////////////
    // this function should not be used in the actual code
    void sample_function(size_t s, size_t g)
    {
        start_id = s;
        goal_id = g;
        openlist.clear();
        closedlist.clear();
        solution.clear();
        Heuristic heuristic;
        // note "const&", since Graph returns const references, we save a
        // temporary
        typename GraphType::Vertex const &vertex_start = graph.GetVertex(start_id);
        typename GraphType::Vertex const &vertex_goal = graph.GetVertex(goal_id);
        // heuristic from start to goal
        typename Heuristic::ReturnType h = heuristic(graph, vertex_start, vertex_goal);
        std::cout << "Heuristic at start " << h << std::endl;

        // note "const&", since Graph returns const references, we save a
        // temporary
        std::vector<typename GraphType::Edge> const &outedges = graph.GetOutEdges(vertex_goal);
        size_t outedges_size = outedges.size();
        for (size_t i = 0; i < outedges_size; ++i)
        {
            std::cout << "goal has a neighbor " << outedges[i].GetID2() << " at distance " << outedges[i].GetWeight() << std::endl;
        }
    }
    ////////////////////////////////////////////////////////////
    std::vector<typename GraphType::Edge> search(size_t s, size_t g)
    {
        start_id = s;
        goal_id = g;
        openlist = OpenListContainer(); // clear
        closedlist.clear();
        solution.clear();
        Heuristic heuristic;

        typename Heuristic::ReturnType h_start = heuristic(graph, graph.GetVertex(start_id), graph.GetVertex(goal_id));
        // add start to openlist
        openlist.push(Node(start_id, start_id, 0.0f, h_start));

        while (!openlist.empty())
        {
            // get node with lowest cost
            Node curr = openlist.top();
            openlist.pop();

            // check if in closedlist with better cost
            bool skip = false;
            for (const auto &node : closedlist)
            {
                if (node.id == curr.id && node.g <= curr.g)
                {
                    skip = true;
                    break;
                }
            }
            if (skip)
            {
                continue;
            }
            // add to closed list
            closedlist.push_back(curr);

            // if current is at goal
            if (curr.id == goal_id)
            {
                // recontruct path for solution going backwards
                size_t curr_id = curr.id;
                while (curr_id != start_id)
                {
                    // find node in closed list
                    auto it = std::find_if(closedlist.begin(), closedlist.end(),
                                           [curr_id](const Node &n)
                                           { return n.id == curr_id; });
                    // node not found
                    if (it == closedlist.end())
                    {
                        break;
                    }

                    // find edge for next node
                    const auto &edges = graph.GetOutEdges(it->parent);
                    for (const auto &edge : edges)
                    {
                        if (edge.GetID2() == curr_id)
                        {
                            solution.push_back(edge);
                            break;
                        }
                    }
                    curr_id = it->parent;
                }
                // reverse solution
                std::reverse(solution.begin(), solution.end());
                break;
            }

            // check each neighbor
            const auto &outEdges = graph.GetOutEdges(curr.id);
            for (const auto &edge : outEdges)
            {
                size_t neighborId = edge.GetID2();
                float g = curr.g + edge.GetWeight();
                typename Heuristic::ReturnType h_neighbor = heuristic(graph, graph.GetVertex(neighborId), graph.GetVertex(goal_id));

                // check if neighbor is in closed list
                bool skip = false;
                for (const auto &node : closedlist)
                {
                    if (node.id == neighborId && node.g <= g)
                    {
                        skip = true;
                        break;
                    }
                }
                if (skip)
                {
                    continue;
                }

                openlist.push(Node(neighborId, curr.id, g, h_neighbor));
            }

            callback.OnIteration(*this);
        }

        callback.OnFinish(*this);
        return solution;
    }

    ////////////////////////////////////////////////////////////////////////
private:
    struct Node
    {
        size_t id;
        size_t parent;
        float g, h, f;

        Node(size_t id, size_t parent, float g, float h) : id(id), parent(parent), g(g), h(h), f(g + h)
        {
        }

        bool operator<(const Node &other) const
        {
            return f > other.f;
        }
    };

    typedef std::priority_queue<Node> OpenListContainer;
    typedef std::vector<Node> ClosedListContainer;
    typedef std::vector<typename GraphType::Edge> SolutionContainer;

    const GraphType &graph;
    OpenListContainer openlist;
    ClosedListContainer closedlist;
    SolutionContainer solution;
    Callback<GraphType, Astar> &callback;
    size_t start_id, goal_id;
};

#endif
