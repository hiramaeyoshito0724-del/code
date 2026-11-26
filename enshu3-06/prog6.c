/* 2026年度 情報領域演習第三 第6回 問題6
 * 2 つの整数 n, m (0 <= n, m <= 10000) に対し n^m を求めて出力する。
 * 結果は基本型で表せないので、問題5 と同じ 10000 進の連結リストで数を表す。
 * 計算回数を減らすため繰り返し二乗法 (第3回 問題8 と同様の工夫) を用いる:
 *   n^m = (n^2)^(m/2)          (m が偶数)
 *       = n * (n^2)^((m-1)/2)  (m が奇数)
 * これにより掛け算の回数は O(m) ではなく O(log m) になる。
 * 途中で不要になったリストは free() で必ず解放する。
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

/* int 型の値 v (0 以上) を 10000 進のリストにする (1 の位側が先頭) */
list from_int(int v)
{
  list head = NULL, tail = NULL, n;

  do {
    n = cons(v % BASE, NULL);
    if (tail == NULL) { head = n; } else { tail->next = n; }
    tail = n;
    v /= BASE;
  } while (v > 0);
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

/* 末尾側 (上位の桁) にある余分な 0 の節点を取り除き、そのメモリを解放する。
 * 必要以上のメモリを持ち続けないために、掛け算のたびに呼び出す。 */
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

/* a と b の積のリストを返す (a, b は変更しない)。
 * 内側のループの中で繰り上がりを処理するので int が溢れることはない。 */
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
    while (carry != 0) {
      v = pc->element + carry;
      pc->element = v % BASE;
      carry = v / BASE;
      pc = pc->next;
    }
    cstart = cstart->next;
  }

  trim_zeros(c);   /* 余分な上位の 0 はすぐに解放する */
  return c;
}

/* 第 1 引数の第 2 引数乗をリストとして返す関数 */
list power_list(int n, int m)
{
  list result = from_int(1);   /* n^0 = 1 */
  list base = from_int(n);
  list t;

  while (m > 0) {
    if (m % 2 == 1) {          /* m が奇数なら result に base を掛ける */
      t = multiply(result, base);
      free_list(result);       /* 古い result はもう不要 */
      result = t;
    }
    m /= 2;
    if (m > 0) {               /* 次の段のために base を 2 乗する */
      t = multiply(base, base);
      free_list(base);
      base = t;
    }
  }
  free_list(base);
  return result;
}

/* 1 の位側が先頭の 10000 進リスト l を 10 進数として出力し、l を解放する */
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
  int n, m;
  list l;

  if (scanf("%d %d", &n, &m) != 2) {
    return 0;
  }

  l = power_list(n, m);
  print_and_free(l);

  return 0;
}
