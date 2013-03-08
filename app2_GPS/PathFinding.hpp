#pragma once

#include <queue>
#include <deque>
#include "Node.hpp"
#include "Singleton.hpp"

class PathFinding : public Singleton<PathFinding>
{
public:
	~PathFinding();

	enum PathMode
	{
		SHORTEST,
		FASTEST
	};

	void	setBegin(Node *node);
	void	setEnd(Node *node);
	void	resolve(PathMode mode);
    std::deque<Link *> &getResult();

private:
	Node	*_begin;
	Node	*_end;
	std::deque<Link *> _result;


    struct ptr_less {
        template<class T>
        bool operator()(const T& left, const T& right) const {
            return ((*right) < (*left));
        }
    };
    typedef std::priority_queue<Node *, /*std::vector<Node*>, std::less<std::vector<Node *>::value_type>*/std::vector<Node *>, ptr_less> NodeQueue;

	void	algo(NodeQueue &queue, PathMode mode) const;
	void	createNodeList();
	void	reset(Node *node);

	// class only accessible from Singleton
	PathFinding();
	PathFinding(PathFinding const &);
	friend Singleton<PathFinding>;
};

