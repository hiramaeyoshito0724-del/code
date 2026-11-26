/* 2026年度 情報領域演習第三 第6回 問題2
 * 2 つの 0 以上の整数 (最大 126 桁) の掛け算を配列で行う。
 *   (Σ a_i 10^i)(Σ b_j 10^j) = Σ_k (Σ_i a_i b_{k-i}) 10^k
 * の等式を利用し、各桁を繰り上がりを無視して計算してから、
 * 最後に繰り上がりの処理を行う。
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
  len = i;
  for (i = 0; i < len; ++i) {
    arr[i] = (int)(buf[len - 1 - i] - '0');
  }
  return len;
}

int main(void) {
  int a[DIGITS + 1] = {0};
  int b[DIGITS + 1] = {0};
  int c[2 * DIGITS + 2] = {0};   /* 積は最大 252 桁 */
  int la, lb, lc, i, j, carry, v;

  la = read_number(a);
  lb = read_number(b);

  if (la == 0 || lb == 0) {   /* 入力が無い場合は 0 とみなす */
    printf("0\n");
    return 0;
  }

  lc = la + lb;

  /* 繰り上がりを無視して各桁を計算する
   * (c[k] の最大値は 126*9*9 = 10206 なので int で十分) */
  for (i = 0; i < la; ++i) {
    for (j = 0; j < lb; ++j) {
      c[i + j] += a[i] * b[j];
    }
  }

  /* 最後に繰り上がりの処理を行う */
  carry = 0;
  for (i = 0; i < lc; ++i) {
    v = c[i] + carry;
    c[i] = v % 10;
    carry = v / 10;
  }

  /* 余分な上位の 0 を取り除く (結果が 0 のときは 1 桁残す) */
  while (lc > 1 && c[lc - 1] == 0) {
    --lc;
  }

  for (i = lc - 1; i >= 0; --i) {
    printf("%d", c[i]);
  }
  printf("\n");

  return 0;
}
