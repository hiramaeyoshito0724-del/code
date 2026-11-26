/* 2026年度 情報領域演習第三 第6回 問題4
 * 2 つの 0 以上の整数 (桁数の制限なし) の掛け算を連結リストで行う。
 * リストは 1 の位が先頭 (上位の桁ほど末尾側の節点)。
 *   (Σ a_i 10^i)(Σ b_j 10^j) = Σ_k (Σ_i a_i b_{k-i}) 10^k
 * を利用し、各桁を繰り上がりを無視して計算してから最後に繰り上がりを処理する。
 * 不要になったメモリは free() で必ず解放する。
 */
#include <stdio.h>
#include <stdlib.h>
#include "/ced-home/staff/26jr3/include/jr3.h"

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

/* 1 行読み込み、各桁を要素とするリストを返す (1 の位が先頭) */
list read_number(void)
{
  int c, d;
  list l = NULL;

  while ((c = getchar()) != '\n' && c != EOF) {
    if (c >= '0' && c <= '9') {
      d = c - '0';
      l = cons(d, l);
    }
  }
  if (l == NULL) {
    l = cons(0, NULL);
  }
  return l;
}

/* 要素が全て 0 で長さ n のリストを作る (必要な長さだけ確保する) */
list make_zeros(int n)
{
  list l = NULL;
  int i;

  for (i = 0; i < n; ++i) {
    l = cons(0, l);
  }
  return l;
}

/* a と b の積のリストを返す (a, b は変更しない) */
list multiply(list a, list b)
{
  list c, cstart, pa, pb, pc;
  int carry, v;

  c = make_zeros(length(a) + length(b));   /* 積は高々 la+lb 桁 */

  /* 繰り上がりを無視して各桁を計算する。
   * c[k] の最大値は min(la,lb)*81 程度なので int で十分。
   * cstart を 1 つずつ進めることで、添字計算なしに c[i+j] を辿れる。 */
  cstart = c;
  for (pa = a; pa != NULL; pa = pa->next) {
    pc = cstart;
    for (pb = b; pb != NULL; pb = pb->next) {
      pc->element += pa->element * pb->element;
      pc = pc->next;
    }
    cstart = cstart->next;
  }

  /* 最後に繰り上がりの処理を行う */
  carry = 0;
  for (pc = c; pc != NULL; pc = pc->next) {
    v = pc->element + carry;
    pc->element = v % 10;
    carry = v / 10;
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

/* 1 の位が先頭のリスト l を 10 進数として出力し、l のメモリを解放する */
void print_and_free(list l)
{
  list r, p;

  r = reverse_list(l);
  for (p = r; p != NULL; p = p->next) {
    printf("%d", p->element);
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
