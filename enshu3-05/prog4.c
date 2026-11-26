/* 2026年度 情報領域演習第三 第5回 問題4
 * 頭のある連結リストに対し、先頭の節点を削除する関数 delete
 * 削除した節点のメモリは free で必ず解放する (メモリリークを起こさない)
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

/* l を頭とみなしたときの先頭の節点を削除する。
 * 空リスト (l->next == NULL) に対しては何も削除しない。
 * 不要になった節点のメモリは free で解放する。 */
void delete(list l)
{
  list p = l->next;

  if (p == NULL) {
    return;
  }
  l->next = p->next;
  free(p);
}

/* 頭のあるリスト l の i 番目 (0 番目が先頭) の節点を削除する。
 * i がその時点でのリストの長さ以上の場合は何も削除しない。 */
void delete_at(list l, int i)
{
  list p = l;
  int k;

  for (k = 0; k < i && p != NULL; ++k) {
    p = p->next;
  }
  if (p != NULL) {
    delete(p);  /* p->next == NULL なら delete は何もしない */
  }
}

/* 頭のあるリスト l の i 番目に e を挿入する (1 行目の格納に使用) */
void insert_at(list l, int i, elementtype e)
{
  list p = l;
  int k;

  for (k = 0; k < i && p->next != NULL; ++k) {
    p = p->next;
  }
  insert(p, e);
}

/* リスト全体 (頭を含む) のメモリを解放する */
void free_list(list l)
{
  list p = l, q;

  while (p != NULL) {
    q = p->next;
    free(p);
    p = q;
  }
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
  for (i = 0; (c = buf[i]) != '\n' && c != '\0'; ++i) {
    insert_at(l, i, c);                          /* c をリスト l の末尾に挿入 */
  }
  print_list(l);
  while (fgets(buf, sizeof(buf), stdin) != NULL) { /* 2 行目以降を順に読み込み */
    sscanf(buf, "%d", &i);
    delete_at(l, i);                             /* リスト l の i 番目を削除 */
    print_list(l);
  }
  free_list(l);
  return 0;
}
