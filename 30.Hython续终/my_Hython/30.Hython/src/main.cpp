/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 15:49:12 2020
 ************************************************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <hlLexer.h>
#include <hlParser.h>
#include <haizei_tree.h>
#include <haizei_runtime.h>
#include <logger.h>
#include <thread_pool.h>

haizei::ASTree init(int argc, char *argv[]) {
    pANTLR3_INPUT_STREAM input;
    phlLexer lex;
    pANTLR3_COMMON_TOKEN_STREAM tokens;
    phlParser parser;

    assert(argc > 1);
    input = antlr3FileStreamNew((pANTLR3_UINT8)argv[1],ANTLR3_ENC_8BIT);
    lex = hlLexerNew(input);
    tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT,
                                            TOKENSOURCE(lex));
    parser = hlParserNew(tokens);
    hlParser_prog_return r = parser->prog(parser);

    return r.tree;
}
void func1() {
    std::cout << "1" << "2" << "3" << "4" << "5" << "6" << std::endl;
    return ;
}

void func2(haizei::Logger &log) {
 //   LOG_SET(haizei::logLevel::DEBUG | haizei:;logLevel::ERRPR);

    LOG_INFO << "1" << "2" << "3" << "4" << "5" << "6";
    LOG_WARN << "1" << "2" << "3" << "4" << "5" << "6";
    LOG_ERROR << "1" << "2" << "3" << "4" << "5" << "6";
    LOG_DEBUG << "1" << "2" << "3" << "4" << "5" << "6";
  //  log() << "1" << "2" << "3" << "4" << "5" << "6";
    return ;
}
void run_(){
    haizei::thread_pool tp(10);
   /* tp.start();
    for (int i = 1; i <= 20; i++) {
        tp.add_one_task(func1);
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    tp.stop(); 
    std::cout << "cout done" << std::endl;
    std::cout.flush();*/
    tp.start();
    haizei::Logger log;
    for (int i = 1; i <= 20; i++) {
        tp.add_one_task(func2,std::ref(log));
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    tp.stop();
    std::cout << "log done" << std::endl;
   
   // cout << "error"<<endl;
}

void check(haizei::ASTree *tree) {
    std::cout << tree->text() << " " << tree->type() << std::endl;
    for (int i = 0; i < tree->size(); i++) {
        check(tree->at(i));
    }
    return ;
}

int main(int argc, char *argv[]) {
    haizei::ASTree *tree = new haizei::ASTree(init(argc, argv));
    haizei::RunTimeEnv env(tree);
    env.run();
	run_();
    return 0;
}
