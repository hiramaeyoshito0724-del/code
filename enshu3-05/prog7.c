/* 2026年度 情報領域演習第三 第5回 問題7
 * 双方向リスト (ダミーの節点を持つ循環型)
 * d->next->prev == d かつ d->prev->next == d が挿入・削除後も常に成り立つようにする。
 * 要素の型は int
 */
#include <stdio.h>
#include <stdlib.h>

typedef int elementtype;

struct dlnode {
  elementtype element;
  struct dlnode *prev, *next;
};

/* ダミーの節点のみからなる空の双方向リストを作る
 * (空リストではダミーの next も prev もダミー自身を指す) */
struct dlnode *make_dllist(void)
{
  struct dlnode *d;

  d = (struct dlnode *)malloc(sizeof(struct dlnode));
  if (d == NULL) {
    exit(1);
  }
  d->prev = d;
  d->next = d;
  return d;
}

/* p の指している節点の prev に、element として e を含む節点を挿入する
 * (ダミーを p に渡すと末尾への追加になる) */
void insert(struct dlnode *p, elementtype e)
{
  struct dlnode *n;

  n = (struct dlnode *)malloc(sizeof(struct dlnode));
  if (n == NULL) {
    exit(1);
  }
  n->element = e;
  n->prev = p->prev;
  n->next = p;
  p->prev->next = n;
  p->prev = n;
}

/* p の指している節点のみを削除する (前後のポインタを繋ぎ変えて free する) */
void delete(struct dlnode *p)
{
  p->prev->next = p->next;
  p->next->prev = p->prev;
  free(p);
}

/* d の指している節点から next を順にたどって自分自身に戻るまで element を [ ] で挟んで出力し、
 * 続いて prev を順にたどって自分自身に戻るまで element を { } で挟んで出力する (最後に改行)。
 * ダミーの節点の element は参照しない。 */
void print_dllist(struct dlnode *d)
{
  struct dlnode *p;

  for (p = d->next; p != d; p = p->next) {
    printf("[%d]", p->element);
  }
  for (p = d->prev; p != d; p = p->prev) {
    printf("{%d}", p->element);
  }
  printf("\n");
}

/* d1 の末尾に d2 の内容を連結する。2 つめのダミー d2 のメモリは free で解放する。 */
void append_dllist(struct dlnode *d1, struct dlnode *d2)
{
  struct dlnode *first, *last, *d1last;

  if (d2->next != d2) {        /* d2 が空でないときだけ繋ぎ変える */
    first = d2->next;
    last = d2->prev;
    d1last = d1->prev;

    d1last->next = first;
    first->prev = d1last;
    last->next = d1;
    d1->prev = last;
  }
  free(d2);
}

int main() {
  char buf[128];
  int i;
  struct dlnode *d1, *d2;
  d1 = make_dllist();  /* d1 と d2 が空の双方向リストのダミーの節点を指すように初期化 */
  d2 = make_dllist();
  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    sscanf(buf, "%d", &i);
    insert(d1, i); /* ← d1 の末尾に i を要素として挿入 */
    insert(d2, i); /* ← d2 の末尾に i を要素として挿入 */
  }
  print_dllist(d1); /* ← d1 の内容を出力 */
  print_dllist(d2); /* ← d2 の内容を出力 */

  {
    struct dlnode *p, *q;
    /* delete 関数を繰り返し用いて、d1 に含まれる奇数要素を削除 */
    for (p = d1->next; p != d1; p = q) {
      q = p->next;
      if (p->element % 2 != 0) {  /* 負の奇数でも 0 にならないので != 0 で判定 */
        delete(p);
      }
    }
    /* delete 関数を繰り返し用いて、d2 に含まれる偶数要素を削除 */
    for (p = d2->next; p != d2; p = q) {
      q = p->next;
      if (p->element % 2 == 0) {
        delete(p);
      }
    }
  }
  print_dllist(d1); /* ← d1 の内容を出力 */
  print_dllist(d2); /* ← d2 の内容を出力 */

  append_dllist(d1, d2); /* ← d1 の末尾に d2 を連結 */
  print_dllist(d1);      /* ← d1 の内容を出力 */
  return 0;
}
