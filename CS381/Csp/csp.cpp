#include "csp.h"

#ifdef INLINE_CSP
// #warning "INFO - inlining CSP methods"
#define INLINE inline
#else
// #warning "INFO - NOT inlining CSP methods"
#define INLINE
#endif

////////////////////////////////////////////////////////////
// CSP constructor
template <typename T>
CSP<T>::CSP(T &cg) : arc_consistency(),
					 cg(cg),
					 solution_counter(0),
					 recursive_call_counter(0),
					 iteration_counter(0)
{
}

////////////////////////////////////////////////////////////
// CSP solver, brute force - no forward checking
template <typename T>
bool CSP<T>::SolveDFS(unsigned level)
{
	++recursive_call_counter;
	// std::cout << "entering SolveDFS (level " << level << ")\n";

	if (cg.AllVariablesAssigned())
	{
		return true;
	}

	// choose a variable by MRV
	Variable *var = MinRemVal();
	
	// save the current state of all unassigned variables
	std::map<Variable*, std::set<Value> > state = SaveState(var);

	typename std::set<Value>::const_iterator b_vals = var->GetDomain().begin();
	typename std::set<Value>::const_iterator e_vals = var->GetDomain().end();
	for ( ;b_vals!=e_vals;++b_vals) {
		++iteration_counter;
		Value val = *b_vals;

		// assign the value to the variable
		var->Assign(val);

		// check if the assignment is consistent
		if (AssignmentIsConsistent(var)) {
			// recursive call
			bool result = SolveDFS(level + 1);
			if (result) {
				return true;
			}
		}
	
		// undo the assignment
		var->UnAssign();
		LoadState(state);
	}

	return false;
}

////////////////////////////////////////////////////////////
// CSP solver, uses forward checking
template <typename T>
bool CSP<T>::SolveFC(unsigned level)
{
	++recursive_call_counter;
	// std::cout << "entering SolveDFS (level " << level << ")\n";

	if (cg.AllVariablesAssigned())
	{
		return true;
	}

	// choose a variable by MRV
	Variable *var = MinRemVal();
	
	// save the current state of all unassigned variables
	std::map<Variable*, std::set<Value> > state = SaveState(var);
	
	typename std::set<Value>::const_iterator b_vals = var->GetDomain().begin();
	typename std::set<Value>::const_iterator e_vals = var->GetDomain().end();
	for ( ;b_vals!=e_vals;++b_vals) {
		++iteration_counter;
		Value val = *b_vals;

		// assign the value to the variable
		var->Assign(val);
	
		// forward check
		if (ForwardChecking(var)) {
			// recursive call
			bool result = SolveFC(level + 1);
			if (result) {
				return true;
			}
		}
	
		// undo the assignment
		var->UnAssign();
		LoadState(state);
	}

	return false;
}
////////////////////////////////////////////////////////////
// CSP solver, uses arc consistency
template <typename T>
bool CSP<T>::SolveARC(unsigned level)
{
	(void) level;
	return false;
}

template <typename T>
INLINE bool CSP<T>::ForwardChecking(Variable *x)
{
    // get all neighbors
    const std::set<Variable*>& vars = cg.GetNeighbors(x);

	typename std::set<Variable*>::const_iterator b_vars = vars.begin();
	typename std::set<Variable*>::const_iterator e_vars = vars.end();
	for (; b_vars != e_vars; ++b_vars) {
		Variable* neighbor = *b_vars;

		const std::set<const Constraint*>& constrs = cg.GetConnectingConstraints(x, neighbor);

		// store values to remove outside of for loop
		std::vector<Value> removeValues;

		typename std::set<Value>::const_iterator b_vals = neighbor->GetDomain().begin();
		typename std::set<Value>::const_iterator e_vals = neighbor->GetDomain().end();
		for (; b_vals != e_vals; ++b_vals) {
			Value val = *b_vals;

			if (neighbor->IsAssigned()) {
				continue;
			}

			// temporarily assign to neight and check if satisfiable
			neighbor->Assign(val);
			bool satisfiable = true;
			typename std::set<const Constraint*>::const_iterator b_constrs = constrs.begin();
			typename std::set<const Constraint*>::const_iterator e_constrs = constrs.end();
			for (; b_constrs != e_constrs; ++b_constrs) {
				const Constraint* constr = *b_constrs;
                if (!constr->Satisfiable()) {
                    satisfiable = false;
                    break;
                }
            }

			neighbor->UnAssign();
			
			if (!satisfiable) {
				removeValues.push_back(val);
			}
		}

		for (size_t i = 0; i < removeValues.size(); ++i) {
            neighbor->RemoveValue(removeValues[i]);
        }

		if (neighbor->SizeDomain() == 0) {
			return false;
		}
	}

	return true;
}
////////////////////////////////////////////////////////////
// load states (available values) of all unassigned variables
template <typename T>
void CSP<T>::LoadState(
	std::map<Variable *,
			 std::set<typename CSP<T>::Variable::Value> > &saved) const
{
	typename std::map<Variable *, std::set<typename Variable::Value> >::iterator
		b_result = saved.begin();
	typename std::map<Variable *, std::set<typename Variable::Value> >::iterator
		e_result = saved.end();

	for (; b_result != e_result; ++b_result)
	{
		// std::cout << "loading state for "
		//<< b_result->first->Name() << std::endl;
		(*b_result).first->SetDomain((*b_result).second);
	}
}

////////////////////////////////////////////////////////////
// save states (available values) of all unassigned variables
// except the current
template <typename T>
INLINE
	std::map<typename CSP<T>::Variable *, std::set<typename CSP<T>::Variable::Value> >
	CSP<T>::SaveState(typename CSP<T>::Variable *x) const
{
	std::map<Variable *, std::set<typename Variable::Value> > result;

	const std::vector<Variable *> &all_vars = cg.GetAllVariables();
	typename std::vector<Variable *>::const_iterator
		b_all_vars = all_vars.begin();
	typename std::vector<Variable *>::const_iterator
		e_all_vars = all_vars.end();
	for (; b_all_vars != e_all_vars; ++b_all_vars)
	{
		if (!(*b_all_vars)->IsAssigned() && *b_all_vars != x)
		{
			// std::cout << "saving state for "
			//<< (*b_all_vars)->Name() << std::endl;
			result[*b_all_vars] = (*b_all_vars)->GetDomain();
		}
	}
	return result;
}
////////////////////////////////////////////////////////////
// check the current (incomplete) assignment for satisfiability
template <typename T>
INLINE bool CSP<T>::AssignmentIsConsistent(Variable *p_var) const
{
	const std::vector<const Constraint *>& constrs = cg.GetConstraints(p_var);

	typename std::vector<const Constraint*>::const_iterator b_constrs = constrs.begin();
	typename std::vector<const Constraint*>::const_iterator e_constrs = constrs.end();
	for (; b_constrs != e_constrs; ++b_constrs) {
		const Constraint* constr = *b_constrs;
		if (!constr->Satisfiable()) {
			return false;
		}
	}

	return true;
}
////////////////////////////////////////////////////////////
// insert pair
//(neighbors of the current variable, the current variable)
// current variable is th variable that just lost some values
//  for all y~x insert (y,x)
// into arc-consistency queue
template <typename T>
INLINE void CSP<T>::InsertAllArcsTo(Variable *cv)
{
}
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// AIMA p.146 AC-3 algorithm
template <typename T>
INLINE bool CSP<T>::CheckArcConsistency(Variable *x)
{
	return false;
}
////////////////////////////////////////////////////////////
// CHECK that for each value of x there is a value of y
// which makes all constraints involving x and y satisfiable
template <typename T>
INLINE bool CSP<T>::RemoveInconsistentValues(Variable *x, Variable *y, const Constraint *c)
{
	return false;
}
////////////////////////////////////////////////////////////
// choose next variable for assignment
// choose the one with minimum remaining values
template <typename T>
INLINE
	typename CSP<T>::Variable *
	CSP<T>::MinRemVal()
{
	Variable * minVar = NULL;
	int minDomainSize = std::numeric_limits<int>::max();

	std::vector<Variable *> vars = cg.GetAllVariables();
	// iterate through all variables in the CSP
	typename std::vector<Variable*>::const_iterator b_vars = vars.begin();
	typename std::vector<Variable*>::const_iterator e_vars = vars.end();
	for (; b_vars != e_vars; ++b_vars) {
		Variable* var = *b_vars;

		// skip variables that are already assigned
		if (var->IsAssigned()) {
			continue;
		}

		int domainSize = var->SizeDomain();

		if (domainSize < minDomainSize) {
			minVar = var;
			minDomainSize = domainSize;
		}
	}

	return minVar;
}
////////////////////////////////////////////////////////////
// choose next variable for assignment
// choose the one with max degree
template <typename T>
typename CSP<T>::Variable *CSP<T>::MaxDegreeHeuristic()
{
}
#undef INLINE
