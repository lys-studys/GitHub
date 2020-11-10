/*************************************************************************
	> File Name: ASTree.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 15:36:27 2020
 ************************************************************************/

#ifndef _ASTREE_H
#define _ASTREE_H
#include <string>
#include <hlLexer.h>
#include <hlParser.h>
#include <haizei_master.h>
#include <vector>

namespace haizei {

class ASTree {
public :
    ASTree(pANTLR3_BASE_TREE tree);
    int size() const;
    ASTree *at(int ind);
    std::string text() const;
    int type() const;
    IMaster::IFactory *factory;

private:
    pANTLR3_BASE_TREE tree;
    int __type, __size;
    std::vector<ASTree *> child;
};

}
#endif
