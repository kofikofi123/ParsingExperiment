#include "ParseTree.h"

#include <iostream>
#include <iomanip>
#include "ParseTree.h"
void NonTerminalNode::appendChild(std::shared_ptr<TreeNode> leaf){
	children.push_back(leaf);
} 

void NonTerminalNode::printNode(int ident){
	std::cout << std::setw(ident) << "-" <<  name << std::endl;
	//auto oi = std::cout.width;
	for (auto i : children){
		i->printNode(ident + 4);
	}

}

void TerminalNode::printNode(int ident){
	std::cout << std::setw(ident) << "-" << name << std::endl;
}