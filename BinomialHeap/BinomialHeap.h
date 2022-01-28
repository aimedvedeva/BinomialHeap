#pragma once
#include <stdio.h>

typedef enum{
  false,
  true,
}bool;

typedef struct tag_BinomialTree{
  int key;
  int degree;                         //кол-во дочерних узлов
  struct tag_BinomialTree *parent;    
  struct tag_BinomialTree *child;     //крайний левый ребенок
  struct tag_BinomialTree *sibling;   //правй сестринский узел
}BinomialTree;

typedef struct tag_BinomialHeap{
  BinomialTree *head;
}BinomialHeap;

BinomialHeap *CreateBinomialHeap();
BinomialTree *BinomialHeapMinimum(BinomialHeap *heap);
void BinomialHeapInsert(BinomialHeap **heap, int key);
BinomialHeap *BinomialHeapExtractMin(BinomialHeap *heap);
void BinomialDecreaseKey(BinomialHeap *heap, BinomialTree *node, int key);
void PrintTree(FILE *F, BinomialHeap *heap);