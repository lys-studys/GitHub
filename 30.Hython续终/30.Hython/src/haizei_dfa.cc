#include <iostream>
#include <haizei_dfa.h>
#include <haizei_runtime.h>
#include <haizei_parameter.h>
#include <haizei_type.h>
#include <haizei_tree.h>

namespace haizei {

    std::stack<IDFANode *> DFA::breakPoint;
    std::stack<IDFANode *> DFA::continuePoint;
    std::stack<IDFANode *> DFA::returnPoint;

    int DFA::blockPosition = 100;
    
    DFA::DFA() : head(nullptr), tail(nullptr) {}
    DFA::DFA(ASTree *tree) {
        DFA *ret = DFA::build(tree);
        head = ret->head, tail = ret->tail;
    }
    void DFA::run(SParameter &p) {
        IDFANode *now = this->head;
        while (now) {
            now = now->next(p);
        }
        return ;
    }
    DFA *DFA::build(ASTree *tree) {
        DFA *ret = new DFA();
        switch (tree->type()) {
            case FUNCTION: {
                NopeDFANode *return_node = new NopeDFANode();
                
                returnPoint.push(return_node);
                DFA *dfa = DFA::build(tree->at(2));
                returnPoint.pop();
                
                return_node->set_pos(
                    dynamic_cast<BlockBeginDFANode *>(dfa->head)->pos()
                );
                return_node->at(0) = dfa->tail;
                
                ret->head = new DefFunctionDFANode(tree, dfa);
                blockPosition += 1;
                ret->tail = new BlockBeginDFANode(blockPosition);
                ret->head->at(0) = ret->tail;
            } break;
            case BREAK: {
                if (breakPoint.empty()) {
                    std::string msg = "\nbreak statement is wrong! there is no loop.";
                    throw std::runtime_error(msg);
                }
                ret->head = ret->tail = new JumpDFANode(breakPoint.top());
            } break;
            case RETURN: {
                if (returnPoint.empty()) {
                    std::string msg = "\nreturn statement is wrong! there is no function.";
                    throw std::runtime_error(msg);
                }
                ret->head = ret->tail = new ReturnDFANode(
                    returnPoint.top(), 
                    tree->size() ? tree->at(0) : nullptr
                );
            } break;
            case CONTINUE: {
                if (continuePoint.empty()) {
                    std::string msg = "\ncontinue statement is wrong! there is no loop.";
                    throw std::runtime_error(msg);
                }
                ret->head = ret->tail = new JumpDFANode(continuePoint.top());
            } break;
            case IF: {
                DFA *temp;
                ret->head = new ConditionDFANode(tree->at(0));
                ret->tail = new NopeDFANode();
                temp = DFA::build(tree->at(1));
                ret->head->at(0) = temp->head;
                temp->tail->at(0) = ret->tail;
                if (tree->size() == 3) {
                    temp = DFA::build(tree->at(2));
                    ret->head->at(1) = temp->head;
                    temp->tail->at(0) = ret->tail;
                } else {
                    ret->head->at(1) = ret->tail;
                }
            } break;
            case FOR: {
                blockPosition += 1;
                ret->head = new BlockBeginDFANode(blockPosition);
                ret->tail = new BlockEndDFANode(blockPosition);
                
                IDFANode *break_node = new NopeDFANode(blockPosition);
                IDFANode *continue_node = new NopeDFANode(blockPosition);
                breakPoint.push(break_node);
                continuePoint.push(continue_node);
                
                IDFANode *init_node = new ExprDFANode(tree->at(0));
                IDFANode *cond_node = new ConditionDFANode(tree->at(1));
                IDFANode *do_node = new ExprDFANode(tree->at(2));
                
                DFA *stmt = DFA::build(tree->at(3));
                ret->head->at(0) = init_node;
                init_node->at(0) = cond_node;
                cond_node->at(0) = stmt->head;
                cond_node->at(1) = ret->tail;
                stmt->tail->at(0) = do_node;
                do_node->at(0) = cond_node;
                
                break_node->at(0) = ret->tail;
                continue_node->at(0) = do_node;
                breakPoint.pop();
                continuePoint.pop();
            } break;
            case BLOCK: {
                DFA *temp;
                blockPosition += 1;
                ret->head = new BlockBeginDFANode(blockPosition);
                ret->tail = new BlockEndDFANode(blockPosition);
                IDFANode *p = ret->head;
                for (int i = 0; i < tree->size(); i++) {
                    temp = DFA::build(tree->at(i));
                    p->at(0) = temp->head;
                    p = temp->tail;
                }
                p->at(0) = ret->tail;
            } break;
            case WHILE: {
                blockPosition += 1;
                ret->head = new BlockBeginDFANode(blockPosition);
                ret->tail = new BlockEndDFANode(blockPosition);

                IDFANode *break_node = new NopeDFANode(blockPosition);
                IDFANode *continue_node = new NopeDFANode(blockPosition);
                breakPoint.push(break_node);
                continuePoint.push(continue_node);

                IDFANode *cond_node = new ConditionDFANode(tree->at(0));
                DFA *stmt = DFA::build(tree->at(1));
                ret->head->at(0) = cond_node;
                cond_node->at(0) = stmt->head;
                cond_node->at(1) = ret->tail;
                stmt->tail->at(0) = cond_node;
                
                break_node->at(0) = ret->tail;
                continue_node->at(0) = cond_node;
                breakPoint.pop();
                continuePoint.pop();
            } break;
            case DOWHILE: {
                blockPosition += 1;
                ret->head = new BlockBeginDFANode(blockPosition);
                ret->tail = new BlockEndDFANode(blockPosition);
                
                IDFANode *break_node = new NopeDFANode(blockPosition);
                IDFANode *continue_node = new NopeDFANode(blockPosition);
                breakPoint.push(break_node);
                continuePoint.push(continue_node);
                
                IDFANode *cond_node = new ConditionDFANode(tree->at(0));
                DFA *stmt = DFA::build(tree->at(1));
                ret->head->at(0) = stmt->head;
                stmt->tail->at(0) = cond_node;
                cond_node->at(0) = stmt->head;
                cond_node->at(1) = ret->tail;

                break_node->at(0) = ret->tail;
                continue_node->at(0) = cond_node;
                breakPoint.pop();
                continuePoint.pop();
            } break;
            default: {
                ret->head = ret->tail = new ExprDFANode(tree);
            } break;
        }
        return ret;
    }

    // DFA node constructor
    IDFANode::IDFANode(ASTree *tree, int n, std::string __type) : 
        tree(tree), childs(n), __type(__type) {}
    DFANode_1::DFANode_1(ASTree *tree, std::string __type) : 
        IDFANode(tree, 1, __type) {}
    DFANode_2::DFANode_2(ASTree *tree, std::string __type) : 
        IDFANode(tree, 2, __type) {}
    ExprDFANode::ExprDFANode(ASTree *tree) : 
        DFANode_1(tree, "ExprDFANode") {}
    BlockBeginDFANode::BlockBeginDFANode(int pos) : 
        DFANode_1(nullptr, "BlockBeginDFANode"), __pos(pos) {}
    BlockEndDFANode::BlockEndDFANode(int pos) : 
        DFANode_1(nullptr, "BlockEndDFANode"), __pos(pos) {}
    ConditionDFANode::ConditionDFANode(ASTree *tree) : 
        DFANode_2(tree, "ConditionDFANode") {}
    NopeDFANode::NopeDFANode(int pos) : 
        DFANode_1(nullptr, "NopeDFANode"), pos(pos) {}
    JumpDFANode::JumpDFANode(IDFANode *direct_node) : 
        DFANode_1(nullptr, "JumpDFANode"), direct_node(direct_node) {}
    DefFunctionDFANode::DefFunctionDFANode(ASTree *tree, DFA *dfa) : 
        DFANode_1(nullptr, "DefFunctionDFANode"),
        func(std::make_shared<FunctionValue>(tree, dfa)) {}
    ReturnDFANode::ReturnDFANode(IDFANode *direct_node, ASTree *tree) :
        JumpDFANode(direct_node), tree(tree) {}

    IDFANode *&IDFANode::at(int ind) {
        return this->childs[ind];
    }
    
    std::string IDFANode::type() {
        return this->__type;
    } 

    IDFANode *ExprDFANode::next(SParameter &p) {
        RunTimeEnv::GetValue(this->tree, p);
        return this->at(0);
    }
    
    IDFANode *BlockBeginDFANode::next(SParameter &p) {
        p = std::make_shared<Parameter>(p, this->pos());
        return this->at(0);
    }
    int BlockBeginDFANode::pos() { return this->__pos; }

    IDFANode *BlockEndDFANode::next(SParameter &p) {
        while (p->pos() != this->pos()) p = p->next();
        p = p->next();
        return this->at(0);
    }
    int BlockEndDFANode::pos() { return this->__pos; }
    
    IDFANode *ConditionDFANode::next(SParameter &p) {
        SIValue val = RunTimeEnv::GetValue(this->tree, p);
        if (val->isTrue()) return this->at(0);
        return this->at(1);
    }
    
    IDFANode *NopeDFANode::next(SParameter &p) {
        if (this->pos > 0) {
            while (p->pos() != this->pos) { p = p->next(); }
        }
        return this->at(0);
    }
    void NopeDFANode::set_pos(int pos) {
        this->pos = pos;
        return ;
    }
    
    IDFANode *JumpDFANode::next(SParameter &p) {
        return this->direct_node;
    }

    IDFANode *DefFunctionDFANode::next(SParameter &p) {
        SFunctionValue copy_func = std::make_shared<FunctionValue>(*(this->func));
        p->define_param(copy_func->name());
        copy_func->set_init_param(p); 
        p->set(copy_func->name(), copy_func);
        return this->at(0);
    }
    
    IDFANode *ReturnDFANode::next(SParameter &p) {
        SIValue ret = haizei::null_val;
        if (this->tree) ret = RunTimeEnv::GetValue(this->tree, p);
        p->set(haizei::ReturnValueName, ret);
        return JumpDFANode::next(p);
    }

// end of namespace haizei
}
