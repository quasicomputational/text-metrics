/*
 * This file is part of ‘text-metrics’ package.
 *
 * Copyright © 2016 Mark Karpov
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name Mark Karpov nor the names of contributors may be used to
 *   endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS “AS IS” AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "text_metrics.h"

/* Levenshtein variants */

unsigned int tmetrics_levenshtein (unsigned int la, uint16_t *a, unsigned int lb, uint16_t *b)
{
  if (la == 0) return lb;
  if (lb == 0) return la;

  unsigned int v_len = lb + 1;
  unsigned int *v0 = malloc(sizeof(unsigned int) * v_len);
  unsigned int *v1 = malloc(sizeof(unsigned int) * v_len);

  for (unsigned int i = 0; i < v_len; i++)
    v0[i] = i;

  for (unsigned int i = 0; i < la; i++)
    {
      v1[0] = i + 1;

      for (unsigned int j = 0; j < lb; j++)
        {
          unsigned int cost = *(a + i) == *(b + j) ? 0 : 1;
          unsigned int x = *(v1 + j) + 1;
          unsigned int y = *(v0 + j + 1) + 1;
          unsigned int z = *(v0 + j) + cost;
          *(v1 + j + 1) = x > y ? (y > z ? z : y) : (x > z ? z : x);
        }

      for (unsigned int j = 0; j < v_len; j++)
        *(v0 + j) = *(v1 + j);
    }

  unsigned int result = *(v1 + lb);

  free(v0);
  free(v1);

  return result;
}

/* Other */

unsigned int tmetrics_hamming (unsigned int len, uint16_t *a, uint16_t *b)
{
  unsigned int acc = 0;
  for (unsigned int i = 0; i < len; i++)
    {
      if (*(a + i) != *(b + i)) acc++;
    }
  return acc;
}