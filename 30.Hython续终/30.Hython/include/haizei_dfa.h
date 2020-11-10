/*************************************************************************
	> File Name: haizei_dfa.h
	> Author: 
	> Mail: 
	> Created Time: 日  4/ 5 18:40:20 2020
 ************************************************************************/

#ifndef _HAIZEI_DFA_H
#define _HAIZEI_DFA_H
#include <vector>
#include <haizei_shared_type.h>
#include <stack>

namespace haizei {

class DFA {
public :
    DFA(ASTree *);
    void run(SParameter &);

private:
    static DFA *build(ASTree *);
    static std::stack<IDFANode *> breakPoint;
    static std::stack<IDFANode *> continuePoint;
    static std::stack<IDFANode *> returnPoint;
    static int blockPosition;
    DFA();
    IDFANode *head, *tail;
};

class IDFANode {
public :
    IDFANode(ASTree *, int, std::string);
    virtual IDFANode *next(SParameter &) = 0;
    IDFANode *&at(int);
    std::string type();

protected:
    ASTree *tree;
    std::vector<IDFANode *> childs;
    std::string __type;
};

class DFANode_1 : public IDFANode {
public :
    DFANode_1(ASTree *, std::string);
    virtual IDFANode *next(SParameter &) = 0;
};

class DFANode_2 : public IDFANode {
public :
    DFANode_2(ASTree *, std::string);
    virtual IDFANode *next(SParameter &) = 0;
};

class ExprDFANode : public DFANode_1 {
public :
    ExprDFANode(ASTree *);
    virtual IDFANode *next(SParameter &);
};

class BlockBeginDFANode : public DFANode_1 {
public :
    BlockBeginDFANode(int);
    virtual IDFANode *next(SParameter &);
    int pos();

private:
    int __pos;
};

class BlockEndDFANode : public DFANode_1 {
public :
    BlockEndDFANode(int);
    virtual IDFANode *next(SParameter &);
    int pos();

private :
    int __pos;
};

class ConditionDFANode : public DFANode_2 {
public :
    ConditionDFANode(ASTree *);
    virtual IDFANode *next(SParameter &);
};

class NopeDFANode : public DFANode_1 {
public :
    NopeDFANode(int = -100);
    void set_pos(int);
    virtual IDFANode *next(SParameter &);

private:
    int pos;
};

class JumpDFANode : public DFANode_1 {
public :
    JumpDFANode(IDFANode *);
    virtual IDFANode *next(SParameter &);

private:
    IDFANode *direct_node;
};

class DefFunctionDFANode : public DFANode_1 {
public :
    DefFunctionDFANode(ASTree *, DFA *);
    virtual IDFANode *next(SParameter &);

private:
    SFunctionValue func;
};

class ReturnDFANode : public JumpDFANode {
public :  
    ReturnDFANode(IDFANode *, ASTree *);
    virtual IDFANode *next(SParameter &);

private:
    ASTree *tree;
};

}
#endif
