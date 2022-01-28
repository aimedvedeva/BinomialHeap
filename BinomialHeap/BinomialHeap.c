#include <stdlib.h>
#include <math.h>
#include "BinomialHeap.h"
#define MAXKEY 1000000000
#define SIZE 40

void f_DestroyBinomialHeapHead(BinomialHeap *heap);
void f_BinomialTreeLink(BinomialTree *futureChild, BinomialTree *futureParent);
BinomialHeap *f_BinomialHeapMerge(BinomialHeap *heap1, BinomialHeap *heap2);
BinomialHeap *f_BinomialHeapUnion(BinomialHeap *heap1, BinomialHeap *heap2);

BinomialHeap *CreateBinomialHeap(){
  BinomialHeap *heap = (BinomialHeap*)malloc(sizeof(BinomialHeap));
  heap->head = NULL;
  return heap;
}

BinomialTree *BinomialHeapMinimum(BinomialHeap *heap){
  BinomialTree *result = NULL;
  BinomialTree *tmp = heap->head;
  int minKey = MAXKEY; 
  while (tmp != NULL){
    if (tmp->key < minKey){
      minKey = tmp->key;
      result = tmp;
    }
    tmp = tmp->sibling;
  }
  return result;
}

void f_DestroyBinomialHeapHead(BinomialHeap *heap){
  free(heap);
}

void f_BinomialTreeLink(BinomialTree *futureChild, BinomialTree *futureParent){
  futureChild->parent = futureParent;
  futureChild->sibling = futureParent->child;
  futureParent->child = futureChild;
  futureParent->degree += 1;
}

BinomialHeap *f_BinomialHeapMerge(BinomialHeap *heap1, BinomialHeap *heap2){
  //создаем новую кучу
  BinomialHeap *heap = CreateBinomialHeap();
  BinomialTree *tmp = NULL;

  //создаем итераторы для каждой из сливаемых в одну куч
  BinomialTree *tmp1 = heap1->head;
  BinomialTree *tmp2 = heap2->head;

  bool isFirstIteration = true;
  
  while (tmp1 != NULL && tmp2 != NULL){
    if (tmp1->degree < tmp2->degree){
      if (isFirstIteration){
        isFirstIteration = false;
        heap->head = tmp1;
        tmp = heap->head;
        tmp->sibling = NULL;
      }
      else{
        tmp->sibling = tmp1;
        tmp = tmp->sibling;
      }
      tmp1 = tmp1->sibling;
    }
    else if (tmp1->degree > tmp2->degree || tmp1->degree == tmp2->degree){
      if (isFirstIteration){
        isFirstIteration = false;
        heap->head = tmp2;
        tmp = heap->head;
      }
      else{
        tmp->sibling = tmp2;
        tmp = tmp->sibling;
      }
      tmp2 = tmp2->sibling;
    }
  }

  if (tmp1 != NULL){
    if (isFirstIteration){
      isFirstIteration = false;
      heap->head = tmp1;
      tmp = heap->head;
    }
    else{
      tmp->sibling = tmp1;
    }
  }


  if (tmp2 != NULL){
    if (isFirstIteration){
      isFirstIteration = false;
      heap->head = tmp2;
      tmp = heap->head;
    }
    else{
      tmp->sibling = tmp2;
    }
  }

  return heap;
}

BinomialTree *f_CreateBinomialTree(){
  BinomialTree *tree = (BinomialTree*)malloc(sizeof(BinomialTree));
  tree->child = tree->parent = tree->sibling = NULL;
  tree->degree = 0;
  return tree;
}

BinomialHeap *f_BinomialHeapUnion(BinomialHeap *heap1, BinomialHeap *heap2){
  BinomialHeap *heap = f_BinomialHeapMerge(heap1, heap2);
  f_DestroyBinomialHeapHead(heap1);
  f_DestroyBinomialHeapHead(heap2);

  if (heap->head == NULL){
    return NULL;
  }

  BinomialTree *prev = NULL;
  BinomialTree *current = heap->head;
  BinomialTree *next = current->sibling;

  while (next != NULL){
    if ((current->degree != next->degree) || (next->sibling != NULL && next->sibling->degree == current->degree)){
      prev = current;
      current = next;
    }
    else if (current->key <= next->key){
      current->sibling = next->sibling;
      f_BinomialTreeLink(next, current);
    }
    else{
      if (prev == NULL){
        heap->head = next;
      }
      else{
        prev->sibling = next;
      }
      f_BinomialTreeLink(current, next);
      current = next;
    }
    next = current->sibling;
  }
  return heap;
}

void BinomialHeapInsert(BinomialHeap **heap, int key){
  BinomialHeap *newHeap = CreateBinomialHeap();
  BinomialTree *newNode = f_CreateBinomialTree();
  newNode->key = key;
  newHeap->head = newNode;
  
  *heap = f_BinomialHeapUnion(*heap, newHeap);
}

BinomialHeap *BinomialHeapExtractMin(BinomialHeap *heap){
  BinomialTree *result = NULL;
  BinomialTree *tmp = heap->head;
  BinomialTree *prev = NULL;
  BinomialTree *prevMin = NULL;

  int minKey = MAXKEY;
  while (tmp != NULL){
    if (tmp->key < minKey){
      prevMin = prev;
      minKey = tmp->key;
      result = tmp;
    }
    prev = tmp;
    tmp = tmp->sibling;
   }

  if (prevMin->sibling != NULL){
    prevMin->sibling = prevMin->sibling->sibling;
  }
  else{
    prevMin->sibling = NULL;
  }
  

  BinomialHeap *newHeap = CreateBinomialHeap();

  BinomialTree *tmp1 = result->child;
  BinomialTree **mas = (BinomialTree**)malloc(sizeof(BinomialTree*) * result->degree);

  for (int i = 0; i < result->degree; i++){
    mas[i] = tmp1;
    mas[i]->parent = NULL;
    tmp1 = tmp1->sibling;
    mas[i]->sibling = NULL;
  }

  newHeap->head = mas[result->degree - 1];
  BinomialTree *tmp2 = newHeap->head;
  for (int i = result->degree - 2; i >= 0; i--){
    tmp2->sibling = mas[i];

    tmp2 = tmp2->sibling;
  }

  heap = f_BinomialHeapUnion(heap, newHeap);
  return heap;
}

void BinomialDecreaseKey(BinomialHeap *heap, BinomialTree *node, int key){
  if (key > node->key){
    return;
  }
  node->key = key;
  BinomialTree *y = node;
  BinomialTree *z = y->parent;

  while (z != NULL && y->key < z->key){
    int tmp = z->key;
    z->key = y->key;
    y->key = tmp;
    y = z;
    z = y->parent;
  }
}

void f_InitMatrixForPrint(int y, int *x, BinomialTree *node, int **matrixForPrint){
  if (node != NULL){
    for (int i = 0; i < node->degree; i++){
      BinomialTree *tmpNode = node->child;
      int c = i;
      while (c){
        tmpNode = tmpNode->sibling;
        c--;
      }
      *x += i;
      f_InitMatrixForPrint(y + 1, x, tmpNode, matrixForPrint);
    }

    *x += 1;
    matrixForPrint[y][*x] = node->key;
  }
}

void f_PrintHeap(int y, int *x, BinomialTree *node, int **matrixForPrint){
  BinomialTree *tmp = node;
  while (tmp != NULL){
    f_InitMatrixForPrint(y, x, tmp, matrixForPrint);
    *x += 2;
    tmp = tmp->sibling;
  }
}



void f_PrintTreeMatrix(FILE *Output, int **matrixForPrint){
  for (int i = 0; i < SIZE; i++){
    for (int j = 0; j < SIZE; j++){
      if (matrixForPrint[i][j] == 0){
        fprintf(Output, "  ");
      }
      else{
        fprintf(Output, "%i", matrixForPrint[i][j]);
      }
    }
    fprintf(Output, "\n");
  }
  fprintf(Output, "\n------------------------------\n");
}

void f_InitMatrixByZero(int **matrixForPrint){
  for (int i = 0; i < SIZE; i++){
    for (int j = 0; j < SIZE; j++){
      matrixForPrint[i][j] = 0;
    }
  }
}

int **f_CreateMatrix(){
  int **MatrixForPrint = (int**)malloc(sizeof(int*) * SIZE);
  for (int i = 0; i < SIZE; i++){
    MatrixForPrint[i] = (int*)malloc(sizeof(int) * SIZE);
  }
  return MatrixForPrint;
}

void f_DeleteMatrix(int **Matrix){
  for (int i = 0; i < SIZE; i++){
    free(Matrix[i]);
  }
  free(Matrix);
}

void PrintTree(FILE *F, BinomialHeap *heap){
  int **MatrixForPrint = f_CreateMatrix();
  f_InitMatrixByZero(MatrixForPrint);

  int x = 0;
  f_PrintHeap(0, &x, heap->head, MatrixForPrint);
  //f_InitMatrixForPrint(0, &x, heap->head, MatrixForPrint);
  f_PrintTreeMatrix(F, MatrixForPrint);
  f_DeleteMatrix(MatrixForPrint);
}
