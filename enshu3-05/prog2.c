/* 2026年度 情報領域演習第三 第5回 問題2
 * 「頭」(ダミーの節点) のある連結リスト
 * length と print_list は頭を数えない/出力しないように定義し直す
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

/* 頭のあるリスト l の長さ (頭を除いた要素の数) を返す */
int length(list l)
{
  int n = 0;
  list p;

  for (p = l->next; p != NULL; p = p->next) {
    ++n;
  }
  return n;
}

/* 頭のあるリスト l の要素を先頭から順に ( と ) で挟んで出力する
 * 頭の節点の element は参照しない */
void print_list(list l)
{
  list p;

  for (p = l->next; p != NULL; p = p->next) {
    printf("(%d)", p->element);
  }
  printf("\n");
}

/* 以下の main 関数は 5 行目のコメント部分のみを埋めたもの */
int main() {
  int i;
  char buf[128];
  list l, last;
  l = cons(0, NULL); /* 頭のみからなる空リストを作る (element は使わない) */
  last = l;
  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    sscanf(buf, "%d", &i);
    last->next = cons(i, NULL);
    last = last->next;
  }
  printf("length=%d\n", length(l));
  print_list(l);
  return 0;
}
