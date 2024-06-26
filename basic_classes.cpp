#include "basic_classes.hpp"

Table::Table() {
  for (int i = 0; i < 10; ++i) {
    charToSymbol['0' + i] = DIGIT;
  }
}

Symbol Table::getSymbolFromChar(char val) {
  if (charToSymbol.count(val)) {
    return charToSymbol[val];
  }

  return UNDEFINED;
}

void Table::setRules() {
  // EXPR -> TERM T_ERXR
  transitionTable[EXPR][DIGIT] = { T_EXPR, TERM };
  transitionTable[EXPR][POINT] = { T_EXPR, TERM };
  // EXPR -> NUM T_EXPR
  transitionTable[EXPR][LBRACKET] = { T_EXPR, TERM };
  // EXPR -> T_EXPR
  transitionTable[EXPR][SUB] = { T_EXPR, TERM, SUB };
  // T_EXPR -> + TERM T_EXPR
  transitionTable[T_EXPR][SUM] = { T_EXPR, TERM, SUM };
  // T_EXPR -> - TERM T_EXPR
  transitionTable[T_EXPR][SUB] = { T_EXPR, TERM, SUB };
  // T_EXPR -> ""
  transitionTable[T_EXPR][EMPTY] = {  };
  transitionTable[T_EXPR][RBRACKET] = {  };
  // TERM -> NUM P_TERM
  transitionTable[TERM][DIGIT] = { P_TERM, NUM };
  transitionTable[TERM][POINT] = { P_TERM, NUM };
  transitionTable[TERM][LBRACKET] = { P_TERM, NUM };
  // P_TERM -> * TERM T_EXPR
  transitionTable[P_TERM][MUL] = { P_TERM, NUM, MUL };
  // P_TERM -> / TERM T_EXPR
  transitionTable[P_TERM][DIV] = { P_TERM, NUM, DIV };
  // P_TERM -> ""
  transitionTable[P_TERM][DIGIT] = {  };
  transitionTable[P_TERM][EMPTY] = {  };
  transitionTable[P_TERM][RBRACKET] = {  };
  transitionTable[P_TERM][SUB] = {  };
  transitionTable[P_TERM][SUM] = {  };
  // NUM -> ELEM
  transitionTable[NUM][DIGIT] = { ELEM };
  transitionTable[NUM][POINT] = { ELEM };
  // NUM -> (EXPR)
  transitionTable[NUM][LBRACKET] = { RBRACKET, EXPR, LBRACKET };
  // ELEM -> [0:9] ELEM
  transitionTable[ELEM][DIGIT] = { ELEM, DIGIT };
  // ELEM -> .P_ELEM
  transitionTable[ELEM][POINT] = { P_ELEM, POINT };
  // ELEM -> ""
  transitionTable[ELEM][EMPTY] = {  };
  transitionTable[ELEM][RBRACKET] = {  };
  transitionTable[ELEM][SUB] = {  };
  transitionTable[ELEM][SUM] = {  };
  transitionTable[ELEM][MUL] = {  };
  transitionTable[ELEM][DIV] = {  };
  // P_ELEM -> [0:9] P_ELEM
  transitionTable[P_ELEM][DIGIT] = { P_ELEM, DIGIT };
  // P_ELEM -> ""
  transitionTable[P_ELEM][EMPTY] = {  };
  transitionTable[P_ELEM][RBRACKET] = {  };
  transitionTable[P_ELEM][SUB] = {  };
  transitionTable[P_ELEM][SUM] = {  };
  transitionTable[P_ELEM][MUL] = {  };
  transitionTable[P_ELEM][DIV] = {  };
}

SimpleTree::SimpleTree() {
  val.fval = 0;
  val.cval = 0;
}

SimpleTree::~SimpleTree() {
  for (auto value : leaves) {
    delete value; 
  }
}

void SimpleTree::addLeaf(SimpleTree* leaf) {
  leaves.push_back(leaf);
  leaf->parent = this;
}

SimpleTree *SimpleTree::addParentLeaf() {
    SimpleTree* newTree = new SimpleTree();

  // └── newTree
    newTree->parent = parent;
    // when we have more than 1 leaf, we need to change it
    if (parent && parent->leaves.size()) {
      parent->leaves[parent->leaves.size() - 1] = newTree;
    }

    // └── newTree
    //            └── tree
    newTree->addLeaf(this);

    return newTree;
}

void SimpleTree::print(
    SimpleTree* tree, 
    const std::string& prefix, 
    bool isLeft) {
  std::cout << prefix;

  std::cout << (isLeft ? "├──" : "└──" );

  if (tree->any_val.empty()) {
    std::cout << tree->val.cval << std::endl;
  } else {
    std::cout << tree->any_val << std::endl;
  }

  for (int i = 0; i < tree->leaves.size(); ++i) {
    print(
        tree->leaves[i],
        prefix + (isLeft ? "│   " : "    "),
        i + 1 != tree->leaves.size()
    );
  }
}

void SimpleTree::calc(SimpleTree *tree) {
  float res = 0;
  if (tree->val.cval) {
    if (tree->leaves.size()) {
      auto iter = tree->leaves.begin();

      calc(*iter);
      res = (*iter)->val.fval;
      ++iter;

      for (;iter != tree->leaves.end(); ++iter) {
        calc(*iter);

        switch (tree->val.cval)
        {
        case '+':
          res += (*iter)->val.fval;
          break;
        case '-':
          res -= (*iter)->val.fval;
          break;
        case '*':
          res *= (*iter)->val.fval;
          break;
        case '/':
          res /= (*iter)->val.fval;
          break;
        case '(':
          break;
        default:
          std::cerr << "Unknown symbol " << tree->val.cval << '\n';
          break;
        }
      }
      tree->val.fval = res;
    }
    return;
  }

  tree->val.fval = atof(tree->any_val.c_str());
}
