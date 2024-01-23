#ifndef KRUSKAL_H
#define KRUSKAL_H
#include "disjoint_sets.h"
#include "graph.h"

template <typename Vertex, typename Edge>
std::vector<Edge> kruskal(Graph<Vertex, Edge> const &g)
{
    std::vector<Edge> mst;
    std::vector<Edge> edges(g.GetEdges().begin(), g.GetEdges().end());
    std::sort(edges.begin(), edges.end());
    DisjointSets sets(g.Size());
    for (size_t i = 0; i < g.Size(); ++i)
        sets.Make();

    for (size_t i = 0; i < edges.size(); i++)
    {
        size_t rep1 = sets.GetRepresentative(edges[i].ID1());
        size_t rep2 = sets.GetRepresentative(edges[i].ID2());
        if (rep1 != rep2)
        {
            sets.Join(rep1, rep2);
            mst.push_back(edges[i]);
        }
    }
    return mst;
}

#endif
