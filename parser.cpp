#include <cstdint>
#include <cstring>
#include <iostream>
#include <map>
#include <stack>
#include <string>

#include "basic_classes.hpp"


int main(int argc, char **argv) {
  char *cur;
  char *forValue;
  // we always need str, so we use cin if there was no str
  if (argc == 1) {
    std::string values;
    getline(std::cin, values);

    forValue = new char[values.length() + 1];
    std::strcpy(forValue, values.c_str());
    forValue[values.size() + 1] = 0;
    cur = forValue;
  }

  // init required values
  if (argc > 1) {
    cur = argv[1];
  }

  // set the rules
  Table table;
  table.setRules();

  std::stack<Symbol> countDepth;
  std::stack<Symbol> toFind;

  SimpleTree* tree = new SimpleTree();
  toFind.push(EXPR);

  // main cycle
  while (toFind.size()) {
    // ignore spaces
    if (*cur == ' ') {
      ++cur;
      continue;
    }

    // skipping all empty
    if (toFind.top() == EMPTY) {
      toFind.pop();
      continue;
    }

    // update cur symbol
    Symbol curSymbol = table.getSymbolFromChar(*cur);

    if (curSymbol == toFind.top()) {
      // when we need just to remove value from stack
      // used only for right bracket in fact
      if (countDepth.size() && countDepth.top() == toFind.top()) {
        countDepth.pop();
      }

      ++cur;
      toFind.pop();
    }
    else { 
      auto key = toFind.top();

      // when there is no such transition, we have an incorrect value
      if (! (table.transitionTable.count(key) 
          && table.transitionTable[key].count(curSymbol)) 
      ) {
        std::cerr << "Incorrect input " << *cur << " " << key << std::endl;
        if (argc == 1) {
          delete[] forValue;
        }
        return 1;
      }

      toFind.pop();

      // when we need to go upper
      if (countDepth.size() && countDepth.top() == key && tree->parent) {
        tree = tree->parent;
        countDepth.pop();
      }

      // if we have "-a ...", we change it to "0 - a ..."
      if (key == EXPR && curSymbol == SUB) {
        tree->val.cval = *cur;

        SimpleTree* leaf1 = new SimpleTree();
        SimpleTree* leaf2 = new SimpleTree();
        tree->addLeaf(leaf1);
        tree->addLeaf(leaf2);

        leaf1->any_val = "0";
        tree = leaf2;
      }
      // when we add a sign, we need to add common tree
      // for further number addition (a.k.a. leaf) and current tree
      else if (key == T_EXPR && (curSymbol == SUM || curSymbol == SUB) 
          || key == P_TERM && (curSymbol == MUL || curSymbol == DIV)) {
        // └── tree *

        // └── newTree
        //            └── tree *
        SimpleTree* newTree = tree->addParentLeaf();
        // set sign to current parent 
        newTree->val.cval = *cur;

        // └── newTree
        //            ├── tree *
        //            └── leaf
        SimpleTree* leaf = new SimpleTree();
        newTree->addLeaf(leaf);

        // set pointer to the new leaf
        // └── newTree
        //            ├── tree
        //            └── leaf *
        tree = leaf;

        countDepth.push(key);
      }
      // in case of bracket, we just add value in stack, which would be poped later
      else if (key == NUM && curSymbol == LBRACKET) {
        // └── tree
        countDepth.push(RBRACKET);
      }
      else if ((key == ELEM || key == P_ELEM) 
          && (curSymbol == DIGIT || curSymbol == POINT)) {
        tree->any_val += *cur;
      }

      // when we are at the empty symbol, just skip adding other symbols
      if (curSymbol == EMPTY) {
        continue;
      }
      // add values to be parsed
      for (auto val : table.transitionTable[key][curSymbol]) {
        toFind.push(val);
      }
    }
  }

  // when we haven't parsed whole string, it means that there was a mistake
  // in the expression and it should not be calculated
  if (*cur != '\0') {
    std::cerr << "Incorrect input " << *cur << std::endl;

    if (argc == 1) {
      delete[] forValue;
    }
    delete tree;

    return 1;
  }

  std::cout << "Parsing was finished" << std::endl;

  for (;tree->parent != nullptr; tree = tree->parent);

  SimpleTree::print(tree);
  SimpleTree::calc(tree);
  std::cout << "The result is: " << tree->val.fval << '\n';

  if (argc == 1) {
    delete[] forValue;
  }
  delete tree;

  return 0;
}
