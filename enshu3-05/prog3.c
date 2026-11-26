/* 2026年度 情報領域演習第三 第5回 問題3
 * 頭のある連結リストに対し、先頭に節点を挿入する関数 insert
 * 要素の型は char
 */
#include <stdio.h>
#include <stdlib.h>

typedef char elementtype;

struct node {
  elementtype element;
  struct node *next;
};

typedef struct node* list;

/* 頭のみからなる空リストを作る */
list make_list(void)
{
  list l;

  l = (list)malloc(sizeof(struct node));
  if (l == NULL) {
    exit(1);
  }
  l->next = NULL;
  return l;
}

/* l の指す節点の次 (= l を頭とみなしたときの先頭) に e を含む節点を挿入する */
void insert(list l, elementtype e)
{
  list n;

  n = (list)malloc(sizeof(struct node));
  if (n == NULL) {
    exit(1);
  }
  n->element = e;
  n->next = l->next;
  l->next = n;
}

/* 頭のあるリスト l の i 番目 (0 番目が先頭) に e を挿入する。
 * i がその時点でのリストの長さより大きい場合は末尾に挿入する。 */
void insert_at(list l, int i, elementtype e)
{
  list p = l;
  int k;

  for (k = 0; k < i && p->next != NULL; ++k) {
    p = p->next;
  }
  insert(p, e);
}

/* 頭のあるリスト l の中身を先頭から順に文字列として出力し、改行する */
void print_list(list l)
{
  list p;

  for (p = l->next; p != NULL; p = p->next) {
    putchar(p->element);
  }
  putchar('\n');
}

int main() {
  char c, buf[128];
  int i;
  list l;
  l = make_list();                               /* リスト l を空リストに初期化 */
  if (fgets(buf, sizeof(buf), stdin) == NULL) {  /* 1 行目を buf に読み込み */
    buf[0] = '\n';
    buf[1] = '\0';
  }
  for (i = 0; (c = buf[i]) != '\n' && c != '\0'; ++i) { /* 1 文字ずつ処理 */
    insert_at(l, i, c);                          /* c をリスト l の末尾に挿入 */
  }
  print_list(l);                                 /* リスト l の中身を出力 */
  while (fgets(buf, sizeof(buf), stdin) != NULL) { /* 2 行目以降を順に読み込み */
    sscanf(buf, "%d %c", &i, &c);
    insert_at(l, i, c);                          /* リスト l の i 番目に c を挿入 */
    print_list(l);                               /* リスト l の中身を出力 */
  }
  return 0;
}
