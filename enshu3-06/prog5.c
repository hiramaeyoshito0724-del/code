/* 2026年度 情報領域演習第三 第6回 問題5
 * 問題4 と同じ掛け算を、10 進 4 桁を 1 つの節点にまとめた 10000 進数で行う。
 * 節点の個数が約 1/4 になるので、確保するメモリも計算時間も減らせる。
 * 入出力は問題4 と同様 (10 進表記)。
 * 不要になったメモリは free() で必ず解放する。
 */
#include <stdio.h>
#include <stdlib.h>
#include "/ced-home/staff/26jr3/include/jr3.h"

#define BASE   10000   /* 10 進 4 桁を 1 単位とする */
#define KETA       4

typedef int elementtype;

struct node {
  elementtype element;
  struct node *next;
};

typedef struct node* list;

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

void free_list(list l)
{
  list p = l, q;

  while (p != NULL) {
    q = p->next;
    free(p);
    p = q;
  }
}

int length(list l)
{
  int n = 0;
  list p;

  for (p = l; p != NULL; p = p->next) {
    ++n;
  }
  return n;
}

/* リストを破壊的に反転する */
list reverse_list(list l)
{
  list prev = NULL, cur = l, next;

  while (cur != NULL) {
    next = cur->next;
    cur->next = prev;
    prev = cur;
    cur = next;
  }
  return prev;
}

/* 1 行読み込み、10000 進で各桁を要素とするリストを返す (1 の位側が先頭)。
 * まず 10 進 1 桁ずつのリストを作り、下位から 4 桁ずつまとめ直す。
 * まとめ終えた節点はその場で free() するので、余分なメモリは残らない。 */
list read_number(void)
{
  int c, k, v;
  list d = NULL, p, q;
  list head = NULL, tail = NULL, n;

  while ((c = getchar()) != '\n' && c != EOF) {
    if (c >= '0' && c <= '9') {
      d = cons(c - '0', d);   /* 1 の位が先頭になる */
    }
  }

  p = d;
  while (p != NULL) {
    v = 0;
    for (k = 1; k < BASE && p != NULL; k *= 10) {  /* k = 1, 10, 100, 1000 */
      v += p->element * k;
      q = p->next;
      free(p);                /* 使い終わった 10 進 1 桁の節点は解放する */
      p = q;
    }
    n = cons(v, NULL);
    if (tail == NULL) { head = n; } else { tail->next = n; }
    tail = n;
  }
  if (head == NULL) {
    head = cons(0, NULL);
  }
  return head;
}

/* 要素が全て 0 で長さ n のリストを作る */
list make_zeros(int n)
{
  list l = NULL;
  int i;

  for (i = 0; i < n; ++i) {
    l = cons(0, l);
  }
  return l;
}

/* a と b の積のリストを返す (a, b は変更しない)。
 * 10000 進では各桁を足し込んでから最後に繰り上がりを処理すると int が溢れるため、
 * 内側のループの中で繰り上がりを処理する。
 * v の最大値は 9999 + 9999*9999 + 10000 = 100000000 程度で int に収まる。 */
list multiply(list a, list b)
{
  list c, cstart, pa, pb, pc;
  int carry, v;

  c = make_zeros(length(a) + length(b));

  cstart = c;
  for (pa = a; pa != NULL; pa = pa->next) {
    carry = 0;
    pc = cstart;
    for (pb = b; pb != NULL; pb = pb->next) {
      v = pc->element + pa->element * pb->element + carry;
      pc->element = v % BASE;
      carry = v / BASE;
      pc = pc->next;
    }
    while (carry != 0) {        /* c は十分な長さがあるので pc は NULL にならない */
      v = pc->element + carry;
      pc->element = v % BASE;
      carry = v / BASE;
      pc = pc->next;
    }
    cstart = cstart->next;
  }

  return c;
}

/* 末尾側 (上位の桁) にある余分な 0 の節点を取り除き、そのメモリを解放する */
void trim_zeros(list l)
{
  list p, keep = l, q;

  if (l == NULL) {
    return;
  }
  for (p = l; p != NULL; p = p->next) {
    if (p->element != 0) {
      keep = p;
    }
  }
  q = keep->next;
  keep->next = NULL;
  free_list(q);
}

/* 1 の位側が先頭の 10000 進リスト l を 10 進数として出力し、l を解放する。
 * 最上位の節点だけはそのまま、それ以降は 4 桁に 0 詰めして出力する。 */
void print_and_free(list l)
{
  list r, p;

  r = reverse_list(l);
  printf("%d", r->element);
  for (p = r->next; p != NULL; p = p->next) {
    printf("%0*d", KETA, p->element);
  }
  printf("\n");
  free_list(r);
}

int main(void) {
  list a, b, c;

  a = read_number();
  b = read_number();

  c = multiply(a, b);

  free_list(a);
  free_list(b);

  trim_zeros(c);
  print_and_free(c);

  return 0;
}
