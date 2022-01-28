#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "BinomialHeap.h"


int main(){
  BinomialHeap *heap = CreateBinomialHeap();
  BinomialHeapInsert(&heap, 18);
  BinomialHeapInsert(&heap, 12);
  BinomialHeapInsert(&heap, 37);
  BinomialHeapInsert(&heap, 25);
  BinomialHeapInsert(&heap, 41);
  BinomialHeapInsert(&heap, 6);
  BinomialHeapInsert(&heap, 8);
  BinomialHeapInsert(&heap, 14);
  BinomialHeapInsert(&heap, 29);
  BinomialHeapInsert(&heap, 11);
  BinomialHeapInsert(&heap, 17);
  BinomialHeapInsert(&heap, 77);



  BinomialHeapInsert(&heap, 10);
  BinomialHeapInsert(&heap, 96);
  BinomialHeapInsert(&heap, 120);

  BinomialHeapInsert(&heap, 1);
  BinomialHeapInsert(&heap, 7);
  BinomialHeapInsert(&heap, 78);

  BinomialTree *min = BinomialHeapMinimum(heap);
  int minKey = min->key;

  heap = BinomialHeapExtractMin(heap);
  //heap = BinomialHeapExtractMin(heap);

  //BinomialDecreaseKey(heap, min->child->child, 0);
  //heap = BinomialHeapExtractMin(heap);

  FILE *out;

  out = fopen("out.txt", "w");

  PrintTree(out, heap);

  fclose(out);

  return 0;
}