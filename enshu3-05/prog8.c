/* 2026年度 情報領域演習第三 第5回 問題8
 * 整数の列を読み込み、絶対値の大きい順に出力する。
 * 絶対値が同じ場合は正の数を先に、同じ数値は 1 つしか出力しない。
 * 問題1・問題2で使った list 型の連結リスト (ここでは頭あり) を用い、
 *   1. 空リスト l を用意する
 *   2. 入力行ごとに、l の先頭から順に比較して適切な位置に挿入する
 *      (既に同じ値があれば追加しない)
 *   3. l の中身を先頭から順に出力する
 * というアルゴリズムで実装する。
 */
#include <stdio.h>
#include <stdlib.h>

typedef int elementtype;

struct node {
  elementtype element;
  struct node *next;
};

typedef struct node* list;

/* リスト l の先頭に e を追加したリストを返す */
list cons(elementtype e, list l)
{
  list n;

  n = (list)malloc(sizeof(struct node));
  if (n == NULL) {
    exit(1);
  }
  n->element = e;
  n->next = l;
  return n;
}

/* 絶対値 (int の下限でも溢れないよう long long で求める) */
long long absll(int x)
{
  long long v = (long long)x;

  return (v < 0) ? -v : v;
}

/* a を b より先に出力すべきなら 1 を返す
 * (絶対値が大きい方が先、絶対値が同じなら正の数が先 = 値が大きい方が先) */
int before(int a, int b)
{
  long long aa = absll(a);
  long long bb = absll(b);

  if (aa != bb) {
    return (aa > bb);
  }
  return (a > b);
}

int main() {
  char buf[128];
  int i;
  list l, p;

  l = cons(0, NULL);   /* 頭のみからなる空リスト */

  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    if (sscanf(buf, "%d", &i) != 1) {
      continue;
    }
    /* 先頭から順に比較し、i を置くべき位置の直前の節点 p を求める */
    p = l;
    while (p->next != NULL && before(p->next->element, i)) {
      p = p->next;
    }
    /* 既に同じ値がリストにあれば追加しない */
    if (p->next != NULL && p->next->element == i) {
      continue;
    }
    p->next = cons(i, p->next);
  }

  for (p = l->next; p != NULL; p = p->next) {
    printf("%d\n", p->element);
  }

  return 0;
}
