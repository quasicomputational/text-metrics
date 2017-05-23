#include "text_metrics.h"

/* Levenshtein variants */

unsigned int tmetrics_levenshtein (unsigned int la, uint16_t *a, unsigned int lb, uint16_t *b)
{
  if (la == 0) return lb;
  if (lb == 0) return la;

  unsigned int v_len = lb + 1, *v0, *v1, i, j;

  if (v_len > VLEN_MAX)
    {
      v0 = malloc(sizeof(unsigned int) * v_len);
      v1 = malloc(sizeof(unsigned int) * v_len);
    }
  else
    {
      v0 = alloca(sizeof(unsigned int) * v_len);
      v1 = alloca(sizeof(unsigned int) * v_len);
    }

  for (i = 0; i < v_len; i++)
    v0[i] = i;

  for (i = 0; i < la; i++)
    {
      v1[0] = i + 1;

      for (j = 0; j < lb; j++)
        {
          unsigned int cost = *(a + i) == *(b + j) ? 0 : 1;
          unsigned int x = *(v1 + j) + 1;
          unsigned int y = *(v0 + j + 1) + 1;
          unsigned int z = *(v0 + j) + cost;
          *(v1 + j + 1) = MIN(x, MIN(y, z));
        }

      unsigned int *ptr = v0;
      v0 = v1;
      v1 = ptr;
    }

  unsigned int result = *(v0 + lb);

  if (v_len > VLEN_MAX)
    {
      free(v0);
      free(v1);
    }

  return result;
}

unsigned int tmetrics_damerau_levenshtein (unsigned int la, uint16_t *a, unsigned int lb, uint16_t *b)
{
  if (la == 0) return lb;
  if (lb == 0) return la;

  unsigned int v_len = lb + 1, *v0, *v1, *v2, i, j;

  if (v_len > VLEN_MAX)
    {
      v0 = malloc(sizeof(unsigned int) * v_len);
      v1 = malloc(sizeof(unsigned int) * v_len);
      v2 = malloc(sizeof(unsigned int) * v_len);
    }
  else
    {
      v0 = alloca(sizeof(unsigned int) * v_len);
      v1 = alloca(sizeof(unsigned int) * v_len);
      v2 = alloca(sizeof(unsigned int) * v_len);
    }

  for (i = 0; i < v_len; i++)
    v0[i] = i;

  for (i = 0; i < la; i++)
    {
      v1[0] = i + 1;

      for (j = 0; j < lb; j++)
        {
          unsigned int cost = *(a + i) == *(b + j) ? 0 : 1;
          unsigned int x = *(v1 + j) + 1;
          unsigned int y = *(v0 + j + 1) + 1;
          unsigned int z = *(v0 + j) + cost;
          *(v1 + j + 1) = MIN(x, MIN(y, z));
          unsigned int val = *(v2 + j - 1) + cost;
          if ( i > 0                    &&
               j > 0                    &&
               *(a + i) == *(b + j - 1) &&
               *(a + i - 1) == *(b + j) &&
               val < *(v1 + j + 1) )
            *(v1 + j + 1) = val;
        }

      unsigned int *ptr = v0;
      v0 = v1;
      v1 = v2;
      v2 = ptr;
    }

  unsigned int result = *(v0 + lb);

  if (v_len > VLEN_MAX)
    {
      free(v0);
      free(v1);
      free(v2);
    }

  return result;
}

/* Other */

unsigned int tmetrics_hamming (unsigned int len, uint16_t *a, uint16_t *b)
{
  unsigned int acc = 0, i;
  for (i = 0; i < len; i++)
    {
      if (*(a + i) != *(b + i)) acc++;
    }
  return acc;
}

void tmetrics_jaro (unsigned int *m, unsigned int *t, unsigned int la, uint16_t *a, unsigned int lb, uint16_t *b)
{
  unsigned int d = 0, i, j, tj = 0, from, to;
  char *v;

  *m = 0, *t = 0;

  if (la >= 2 && lb >= 2)
    d = MAX(lb, la) / 2 - 1;

  if (lb > VLEN_MAX) v = malloc(sizeof(char) * lb);
  else               v = alloca(sizeof(char) * lb);

  for (i = 0; i < lb; i++) *(v + i) = 0;

  for (i = 0; i < la; i++)
    {
      from = i < d ? 0 : i - d;
      to   = MIN(i + d + 1, lb);
      for (j = from; j < to; j++)
        {
          if (*(v + j)) continue;

          if (*(a + i) == *(b + j))
            {
              if (j < tj) (*t)++;
              else tj = j;
              *(v + j) = 1;
              (*m)++;
              break;
            }
        }
    }

  if (lb > VLEN_MAX) free(v);
}

unsigned int tmetrics_common_prefix (unsigned int la, uint16_t *a, unsigned int lb, uint16_t *b)
{
  unsigned int acc = 0, i, l = MIN(la, lb);
  for (i = 0; i < l; i++)
    {
      if (*(a + i) == *(b + i)) acc++;
      else break;
    }
  return acc;
}
