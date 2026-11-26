/* 2026年度 情報領域演習第三 第5回 問題1
 * 頭のない連結リスト: cons / length / print_list
 * 要素の型は int (elementtype を typedef で設定)
 */
#include <stdio.h>
#include <stdlib.h>

typedef int elementtype;

struct node {
  elementtype element;
  struct node *next;
};

typedef struct node* list;

/* リスト l の先頭に e を追加したリストを返す (malloc で節点を確保) */
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

/* リスト l の長さ (要素の数) を返す */
int length(list l)
{
  int n = 0;
  list p;

  for (p = l; p != NULL; p = p->next) {
    ++n;
  }
  return n;
}

/* リスト l の要素を先頭から順に ( と ) で挟んで出力する
 * (elementtype が int なので %d を用いる) */
void print_list(list l)
{
  list p;

  for (p = l; p != NULL; p = p->next) {
    printf("(%d)", p->element);
  }
  printf("\n");
}

/* ここから下の main 関数は変更していない */
int main() {
  int i;
  char buf[128];
  list l = NULL;
  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    sscanf(buf, "%d", &i);
    l = cons(i, l);
  }
  printf("length=%d\n", length(l));
  print_list(l);
  return 0;
}
