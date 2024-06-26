#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

enum Symbol {
  // Terminal
  EMPTY = 0u,
  LBRACKET = 1u,
  RBRACKET = 2u,
  SUM = 3u,
  SUB = 4u,
  MUL = 5u,
  DIV = 6u,
  DIGIT = 7u,
  POINT = 8u,

  // Non-terminal
  EXPR = 9u,
  T_EXPR = 10u,
  TERM = 11u,
  P_TERM = 12u,
  NUM = 13u,
  ELEM = 14u,
  P_ELEM = 15u,

  // Default
  UNDEFINED = 16u
};

struct Table {
  Table();

  Symbol getSymbolFromChar(char val);

  void setRules();

  std::map<Symbol, std::map<Symbol, std::vector<Symbol>>> transitionTable;
  std::map<char, Symbol> charToSymbol = {
    {'(', LBRACKET},
    {')', RBRACKET},
    {'+', SUM},
    {'-', SUB},
    {'*', MUL},
    {'/', DIV},
    {'.', POINT},
    {'\0', EMPTY}
  };
};

struct SimpleTree {
  SimpleTree();

  ~SimpleTree();

  union values
  {
    // digit
    float fval;
    // operation
    char cval;
  } val;
  // anything to be parsed
  std::string any_val;

  SimpleTree *parent;
  std::vector<SimpleTree *> leaves;

  void addLeaf(SimpleTree* leaf);

  SimpleTree *addParentLeaf();

  static void print(
      SimpleTree* tree, 
      const std::string& prefix = "", 
      bool isLeft = false);

  static void calc(SimpleTree* tree);
};
