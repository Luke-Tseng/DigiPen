#include "pl.h"

CNF const operator|(Literal const &op1, Literal const &op2) { return CNF(op1) | CNF(op2); }
CNF const operator|(Literal const &op1, CNF const &op2) { return CNF(op1) | op2; }
CNF const operator&(Literal const &op1, Literal const &op2) { return CNF(op1) & CNF(op2); }
CNF const operator&(Literal const &op1, CNF const &op2) { return CNF(op1) & op2; }
CNF const operator>(Literal const &op1, Literal const &op2) { return CNF(op1) > CNF(op2); }
CNF const operator>(Literal const &op1, CNF const &op2) { return CNF(op1) > op2; }

bool const Clause::operator<(Clause const &op2)
{
    return literals < op2.literals;
}

bool Clause::operator<(Clause const &op2) const
{
    return literals < op2.literals;
}

// not
std::set<Clause> Clause::operator~() const
{
    std::set<Clause> result;
    for (const auto &literal : literals)
    {
        result.insert(~literal);
    }

    return result;
}

// or
Clause Clause::operator|(Clause const &op2) const
{
    // Check if either clause is empty
    if (literals.empty())
        return op2;
    if (op2.literals.empty())
        return *this;

    for (auto it1 = literals.begin(); it1 != literals.end(); ++it1)
    {
        for (auto it2 = op2.literals.begin(); it2 != op2.literals.end(); ++it2)
        {
            if (it1->Complementary(*it2))
            {
                // Combine both clauses
                Clause resolvent;
                // Add literals from this clause except it1
                for (auto it = literals.begin(); it != literals.end(); ++it)
                {
                    if (!((*it) == (*it1)))
                    {
                        resolvent.literals.insert(*it);
                    }
                }
                // Add literals from op2 except it2
                for (auto it = op2.literals.begin(); it != op2.literals.end(); ++it)
                {
                    if (!((*it) == (*it2)))
                    {
                        resolvent.literals.insert(*it);
                    }
                }
                return resolvent;
            }
        }
    }

    // If no complementary literals return disjunction
    Clause result(*this);
    result.literals.insert(op2.literals.begin(), op2.literals.end());
    return result;
}

// not
CNF const CNF::operator~() const
{
    // if CNF is made of a single clause: A | B | ~C,
    // negating it gives ~A & ~B & C (3 clauses)
    // otherwise
    // CNF = clause1 & clause2 & clause3,
    //~CNF = ~clause1 | ~clause2 | ~clause3
    //"or" is defined later
    if (clauses.empty())
        return CNF();

    CNF result;
    for (const auto &clause : clauses)
    {
        CNF n = ~clause;
        result = result | n;
    }

    return result;
}
////////////////////////////////////////////////////////////////////////
// and
CNF const CNF::operator&(CNF const &op2) const
{
    // CNF1 = clause1 & clause2 & clause3,
    // CNF2 = clause4 & clause5 & clause6,
    // CNF1 & CNF2 = clause1 & clause2 & clause3 & clause4 & clause5 & clause6
    if (op2.Empty())
        return *this;

    if (this->Empty())
        return op2;

    CNF res(*this);

    res.clauses.insert(op2.begin(), op2.end());
    return res;
}
///////////////////////////////////////////////////////////////////////
// or
CNF const CNF::operator|(CNF const &op2) const
{
    // CNF1 = clause1 & clause2 & clause3,
    // CNF2 = clause4 & clause5 & clause6,
    // CNF1 | CNF2 =
    //               c1|c4 & c1|c5 & c1|c6    &
    //               c2|c4 & c2|c5 & c2|c6    &
    //               c3|c4 & c3|c5 & c3|c6
    if (clauses.empty())
        return op2;
    if (op2.clauses.empty())
        return *this;

    CNF result;
    for (const auto &clause1 : clauses)
    {
        for (const auto &clause2 : op2.clauses)
        {
            Clause combined = clause1 | clause2; // Combine two clauses

            // Avoid duplicates
            if (result.clauses.find(combined) == result.clauses.end())
            {
                result.clauses.insert(combined);
            }
        }
    }

    return result;
}

KnowledgeBase::KnowledgeBase() : clauses() {}
////////////////////////////////////////////////////////////////////////////
KnowledgeBase &KnowledgeBase::operator+=(CNF const &cnf)
{
    for (std::set<Clause>::const_iterator it = cnf.begin(); it != cnf.end(); ++it)
    {
        clauses.insert(*it);
    }
    return *this;
}
////////////////////////////////////////////////////////////////////////
std::set<Clause>::const_iterator KnowledgeBase::begin() const { return clauses.begin(); }
std::set<Clause>::const_iterator KnowledgeBase::end() const { return clauses.end(); }
unsigned KnowledgeBase::size() const { return clauses.size(); }
////////////////////////////////////////////////////////////////////////////
bool KnowledgeBase::ProveByRefutation(CNF const &alpha) const
{
    // Negate the query and add it to the knowledge base
    CNF negated = ~alpha;
    if (negated.size() == 0)
    {
        return true;
    }

    KnowledgeBase current(*this);
    current += negated;

    KnowledgeBase result;
    return Resolve(result, current); // Start resolution
}

bool isSubset(const Clause &clause1, const Clause &clause2)
{
    if (clause1.getLiterals().size() > clause2.getLiterals().size())
    {
        return false;
    }

    // Check if every literal in clause1 is also in clause2
    for (const auto &literal : clause1.getLiterals())
    {
        bool found = false;
        for (const auto &otherLiteral : clause2.getLiterals())
        {
            if (literal == otherLiteral)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return false;
        }
    }
    return true;
}

bool KnowledgeBase::Resolve(KnowledgeBase &result, KnowledgeBase &newKB) const
{
    // Create a copy of all clauses
    std::set<Clause> allClauses;
    allClauses.insert(result.clauses.begin(), result.clauses.end());
    allClauses.insert(newKB.clauses.begin(), newKB.clauses.end());

    // Add all clauses from newKB to result
    result.clauses.insert(newKB.clauses.begin(), newKB.clauses.end());

    bool added = false;
    KnowledgeBase additionalKB;

    // Resolve all pairs of clauses
    for (auto it1 = allClauses.begin(); it1 != allClauses.end(); ++it1)
    {
        for (auto it2 = std::next(it1); it2 != allClauses.end(); ++it2)
        {
            Clause newClause = *it1 | *it2;

            // No resolution
            if (newClause == *it1 || newClause == *it2)
            {
                continue;
            }

            // Check contradiction
            if (newClause.getLiterals().empty())
            {
                return true;
            }

            // Subsumption check and duplicate detection
            bool isSubsumed = false;
            for (const auto &existingClause : allClauses)
            {
                if (isSubset(existingClause, newClause))
                {
                    isSubsumed = true;
                    break;
                }
            }

            if (!isSubsumed && allClauses.find(newClause) == allClauses.end())
            {
                additionalKB.clauses.insert(newClause);
                allClauses.insert(newClause);
                added = true;
            }
        }
    }

    if (!added)
    {
        return false;
    }

    return Resolve(result, additionalKB);
}
////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, KnowledgeBase const &kb)
{
    unsigned size = kb.clauses.size();
    for (std::set<Clause>::const_iterator it1 = kb.clauses.begin(); it1 != kb.clauses.end(); ++it1)
    {
        os << *it1 << ", ";
    }
    return os;
}
