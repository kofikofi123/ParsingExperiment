#ifndef PARSE_TREE
#define PARSE_TREE

#include <vector>
#include <memory>

/*Tree ascii pic broke*/

class TreeNode {
protected:
	const char* name;
public:
	TreeNode(const char* n): name(n){}
	virtual void printNode(int ident = 1) = 0;
};

class NonTerminalNode : public TreeNode{
	std::vector<std::shared_ptr<TreeNode>> children;
public:
	NonTerminalNode(const char* n): TreeNode(n){}
	NonTerminalNode(): TreeNode("NonTerminalNode"){}
	void appendChild(std::shared_ptr<TreeNode>);
	void printNode(int ident = 1);
};

class TerminalNode : public TreeNode {
public:
	TerminalNode(const char* n): TreeNode(n){}
	TerminalNode(): TreeNode("Terminal"){}

	void printNode(int ident = 1);
};
#endif
