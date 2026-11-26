/* 2026年度 情報領域演習第三 第6回 問題3
 * 2 つの 0 以上の整数 (桁数の制限なし) の足し算を連結リストで行う。
 * リストは「上位の桁ほど末尾側の節点」、つまり 1 の位が先頭になるように持つ。
 * 入力の桁数が事前にわからないので、buf は使わず getchar() で 1 文字ずつ読む。
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

/* リスト全体のメモリを解放する */
void free_list(list l)
{
  list p = l, q;

  while (p != NULL) {
    q = p->next;
    free(p);
    p = q;
  }
}

/* リストを破壊的に反転する (節点を新たに作らず next を繋ぎ換えるだけ) */
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

/* 1 行読み込み、各桁を要素とするリストを返す (1 の位が先頭)。
 * 入力の途中で桁を 1 つずつ cons していくと、自然に 1 の位が先頭になる。 */
list read_number(void)
{
  int c, d;
  list l = NULL;

  while ((c = getchar()) != '\n' && c != EOF) {
    if (c >= '0' && c <= '9') {
      d = c - '0';        /* 読み込んだ数字を int 型として d に格納 */
      l = cons(d, l);     /* 読み込んだ数字 d をリストに挿入する */
    }
  }
  if (l == NULL) {        /* 空行は 0 とみなす */
    l = cons(0, NULL);
  }
  return l;
}

/* a と b の和のリストを返す (a, b は変更しない) */
list add(list a, list b)
{
  list head = NULL, tail = NULL, n;
  int carry = 0, s;

  while (a != NULL || b != NULL || carry != 0) {
    s = carry;
    if (a != NULL) { s += a->element; a = a->next; }
    if (b != NULL) { s += b->element; b = b->next; }
    carry = s / 10;
    n = cons(s % 10, NULL);
    if (tail == NULL) { head = n; } else { tail->next = n; }
    tail = n;
  }
  if (head == NULL) {
    head = cons(0, NULL);
  }
  return head;
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
      keep = p;           /* 最も末尾側にある 0 でない節点 */
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

  r = reverse_list(l);    /* 上位の桁が先頭になる */
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

  c = add(a, b);

  free_list(a);           /* 不要になったリストはすぐに解放する */
  free_list(b);

  trim_zeros(c);
  print_and_free(c);

  return 0;
}
