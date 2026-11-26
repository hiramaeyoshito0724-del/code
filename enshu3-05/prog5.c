/* 2026年度 情報領域演習第三 第5回 問題5
 * 入力文字列を連結リストに格納し、
 *   ( と ) で挟まれた部分を先頭へ、[ と ] で挟まれた部分を末尾へ移動する。
 * 括弧そのものの節点はリストから外す。
 * 注意: リストに格納した後は、節点を新たに導入したり element を書き換えたりせず、
 *       節点の移動は next の繋ぎ換えのみで実現する。
 */
#include <stdio.h>
#include <stdlib.h>

typedef char elementtype;

struct node {
  elementtype element;
  struct node *next;
};

typedef struct node* list;

/* 頭のみからなる空リストを作る (入力を格納する前に用意する) */
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

/* p の次に e を含む節点を挿入する */
void insert(list p, elementtype e)
{
  list n;

  n = (list)malloc(sizeof(struct node));
  if (n == NULL) {
    exit(1);
  }
  n->element = e;
  n->next = p->next;
  p->next = n;
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
  char c, ctx, buf[128];
  int i, sp;
  char stack[128];          /* 入れ子になっている括弧の種類を保持する作業用スタック */
  list l, p, next, tail;
  list fh, ft;              /* ( ) で挟まれた部分の列 (先頭へ移動する) */
  list mh, mt;              /* どの括弧にも挟まれていない部分の列 */
  list th, tt;              /* [ ] で挟まれた部分の列 (末尾へ移動する) */

  l = make_list();
  tail = l;

  /* 1 行目の入力を (改行を除いて) そのままリストに格納する */
  if (fgets(buf, sizeof(buf), stdin) == NULL) {
    buf[0] = '\n';
    buf[1] = '\0';
  }
  for (i = 0; (c = buf[i]) != '\n' && c != '\0'; ++i) {
    insert(tail, c);
    tail = tail->next;
  }

  /* ここから先は節点を新たに作らず、next の繋ぎ換えだけで並べ替える */
  fh = ft = NULL;
  mh = mt = NULL;
  th = tt = NULL;
  sp = 0;

  p = l->next;
  l->next = NULL;
  while (p != NULL) {
    next = p->next;
    c = p->element;
    if (c == '(' || c == '[') {
      stack[sp++] = c;       /* 括弧の節点はリストから外す */
      free(p);
    } else if (c == ')' || c == ']') {
      if (sp > 0) {
        --sp;
      }
      free(p);
    } else {
      /* 一番内側の括弧の種類で行き先を決める */
      ctx = (sp > 0) ? stack[sp - 1] : '\0';
      p->next = NULL;
      if (ctx == '(') {
        if (ft == NULL) { fh = p; } else { ft->next = p; }
        ft = p;
      } else if (ctx == '[') {
        if (tt == NULL) { th = p; } else { tt->next = p; }
        tt = p;
      } else {
        if (mt == NULL) { mh = p; } else { mt->next = p; }
        mt = p;
      }
    }
    p = next;
  }

  /* ( ) の部分 -> 括弧の外の部分 -> [ ] の部分 の順に繋ぎ直す */
  tail = l;
  if (fh != NULL) { tail->next = fh; tail = ft; }
  if (mh != NULL) { tail->next = mh; tail = mt; }
  if (th != NULL) { tail->next = th; tail = tt; }
  tail->next = NULL;

  print_list(l);

  /* 後始末: 残っている節点をすべて解放する */
  p = l;
  while (p != NULL) {
    next = p->next;
    free(p);
    p = next;
  }
  return 0;
}
