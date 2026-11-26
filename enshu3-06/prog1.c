/* 2026年度 情報領域演習第三 第6回 問題1
 * 2 つの 0 以上の整数 (最大 126 桁) の足し算を配列で行う。
 * 1 の位を配列の 0 番目に、10 の位を 1 番目に ... と格納する。
 */
#include <stdio.h>

#define DIGITS 126

char buf[DIGITS + 2];   /* 改行文字とナル文字の 2 文字分だけ多く用意 */

/* 1 行読み込み、各桁を arr に (1 の位が 0 番目になるように) 格納して桁数を返す */
int read_number(int *arr)
{
  int i, len;

  if (fgets(buf, sizeof(buf), stdin) == NULL) {
    return 0;
  }
  i = 0;
  while (buf[i] != '\n' && buf[i] != '\0') {
    ++i;
  }
  len = i;                                  /* 改行の位置が入力の長さ */
  for (i = 0; i < len; ++i) {
    arr[i] = (int)(buf[len - 1 - i] - '0'); /* 下の位から順に格納 */
  }
  return len;
}

int main(void) {
  int a[DIGITS + 1] = {0};
  int b[DIGITS + 1] = {0};
  int c[DIGITS + 2] = {0};   /* 和は最大 127 桁 */
  int la, lb, lc, i, carry, v;

  la = read_number(a);
  lb = read_number(b);

  lc = (la > lb) ? la : lb;

  /* 各桁の足し算 (繰り上がりは無視して加算しておく) */
  for (i = 0; i < lc; ++i) {
    c[i] = a[i] + b[i];
  }

  /* 繰り上がりをあとで処理する */
  carry = 0;
  for (i = 0; i < lc; ++i) {
    v = c[i] + carry;
    c[i] = v % 10;
    carry = v / 10;
  }
  if (carry > 0) {
    c[lc] = carry;
    ++lc;
  }

  /* 余分な上位の 0 を取り除く (結果が 0 のときは 1 桁残す) */
  while (lc > 1 && c[lc - 1] == 0) {
    --lc;
  }
  if (lc == 0) {
    lc = 1;
  }

  for (i = lc - 1; i >= 0; --i) {
    printf("%d", c[i]);
  }
  printf("\n");

  return 0;
}
