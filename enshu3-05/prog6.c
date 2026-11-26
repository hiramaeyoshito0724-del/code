/* 2026年度 情報領域演習第三 第5回 問題6
 * 循環リスト (頭のないリスト、要素は 1 個以上)
 * 最大値を要素とする節点のうち最も後に現れたものから順に出力する。
 * 終了判定は回数ではなく「同じ節点に戻ったかどうか」で行う。
 */
#include <stdio.h>
#include <stdlib.h>

typedef int elementtype;

struct node {
  elementtype element;
  struct node *next;
};

typedef struct node* list;

int main() {
  char buf[128];
  int i;
  list head = NULL;  /* 循環リストの先頭 (最初に入力された節点) */
  list tail = NULL;  /* 循環リストの末尾 (最後に入力された節点) */
  list maxp = NULL;  /* 最大値を要素とする節点のうち最も後に現れたもの */
  list n, p;

  while (fgets(buf, sizeof(buf), stdin) != NULL) {
    if (sscanf(buf, "%d", &i) != 1) {
      continue;
    }
    n = (list)malloc(sizeof(struct node));
    if (n == NULL) {
      exit(1);
    }
    n->element = i;
    if (head == NULL) {
      head = n;
      tail = n;
      n->next = n;        /* 要素が 1 つだけの循環リスト */
    } else {
      n->next = head;     /* 末尾の next は先頭を指す */
      tail->next = n;
      tail = n;
    }
    /* 等しい場合も更新することで「最も後に現れた」節点が残る */
    if (maxp == NULL || i >= maxp->element) {
      maxp = n;
    }
  }

  if (maxp == NULL) {
    return 0;
  }

  p = maxp;
  do {
    printf("(%d)", p->element);
    p = p->next;
  } while (p != maxp);   /* 同じ節点に戻ったかで終了を判定する */
  printf("\n");

  return 0;
}
