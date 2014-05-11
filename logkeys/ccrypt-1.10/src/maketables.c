/* Copyright (C) 2000-2012 Peter Selinger.
   This file is part of ccrypt. It is free software and it is covered
   by the GNU general public license. See the file COPYING for details. */

/* maketables.c: create lookup tables for Rijndael cipher */
/* $Id: maketables.c 305 2012-10-11 12:41:22Z selinger $ */

#include <stdio.h>

typedef unsigned char word8;

/* does declaring these "static" fix gcc-3.3 reserved word warnings? */
static word8 exp[256];
static word8 log[256];
static int explog_init = 0;

static word8 sbox[256];
static word8 sibox[256];
static int sbox_init = 0;

/* ---------------------------------------------------------------------- */

/* multiply two elements a and b of GF(2^8) modul0 0x11b */
static word8 multiply(word8 a, word8 b)
{
  unsigned int acc = a;
  unsigned int res = 0;
  while (b != 0) {	/* invariant: acc*b + res = const (mod 0x11b) */
    if (b & 1) {
      res ^= acc;
    }
    acc <<= 1;
    b >>= 1;		/* invariant holds */
    if (acc >= 0x100) {
      acc ^= 0x11b;	/* invariant holds */
    }
  }
  return res;
}

static void makeexplog()
{
  int i, p;		/* p = 3^i in GF(2^8) */

  for (i = 0, p = 1; i < 256; i++, p = multiply(3, p)) {
    exp[i] = p;
    log[p] = i % 255;
  }
  log[0] = 0;
  explog_init = 1;
}

/* ---------------------------------------------------------------------- */

/* find the inverse of a in GF(2^8) or 0 */
static word8 xinv(word8 a)
{
  if (!explog_init) {
    makeexplog();
  }
  if (a) {
    return exp[255 - log[a]];
  } else {
    return 0;
  }
}

/* apply an affine transformation over GF(2) */
static word8 xaff(word8 a)
{
  unsigned int acc = a;
  acc ^= acc << 1;
  acc ^= acc << 2;
  acc <<= 1;
  acc ^= a;
  return acc ^ acc >> 8;
}

static void makesboxes()
{
  int i, p;

  for (i = 0; i < 256; i++) {
    p = 99 ^ xaff(xinv(i));
    sbox[i] = p;
    sibox[p] = i;
  }
  sbox_init = 1;
}

/* ---------------------------------------------------------------------- */

static void multrot2113(int rot, word8 m, word8 r[4])
{
  r[rot] = multiply(2, m);
  r[(rot + 1) % 4] = m;
  r[(rot + 2) % 4] = m;
  r[(rot + 3) % 4] = multiply(3, m);
}

static void multrote9db(int rot, word8 m, word8 r[4])
{
  r[rot] = multiply(0xe, m);
  r[(rot + 1) % 4] = multiply(0x9, m);
  r[(rot + 2) % 4] = multiply(0xd, m);
  r[(rot + 3) % 4] = multiply(0xb, m);
}

/* ---------------------------------------------------------------------- */

static void printbox8(word8 b[256], char *name)
{
  int i;

  printf("xword8 %s[256] = {", name);
  for (i = 0; i < 256; i++) {
    if (i % 14 == 0) {
      printf("\n  ");
    }
    printf("%3d, ", b[i]);
  }
  printf("\n};\n\n");
}

/* ---------------------------------------------------------------------- */

int main()
{
  int rot, m, i, rcon;
  word8 r[4];

  printf("/* Copyright (C) 2000-2012 Peter Selinger.\n");
  printf("   This file is part of ccrypt. It is free software and it is covered\n");
  printf("   by the GNU general public license. See the file COPYING for details. */\n\n");

  printf("/* generated by maketables.c */\n\n");

  printf("#include \"rijndael.h\"\n\n");

  printf("xword8x4 M0[4][256] = {");
  for (rot = 0; rot < 4; rot++) {
    printf("\n {");
    for (m = 0; m < 256; m++) {
      if (m % 3 == 0) {
	printf("\n  ");
      }
      multrot2113(rot, m, r);
      printf("{{%3d, %3d, %3d, %3d}}, ", r[0], r[1], r[2], r[3]);
    }
    printf("\n },");
  }
  printf("\n};\n\n");

  printf("xword8x4 M1[4][256] = {");
  for (rot = 0; rot < 4; rot++) {
    printf("\n {");
    for (m = 0; m < 256; m++) {
      if (m % 3 == 0) {
	printf("\n  ");
      }
      multrote9db(rot, m, r);
      printf("{{%3d, %3d, %3d, %3d}}, ", r[0], r[1], r[2], r[3]);
    }
    printf("\n },");
  }
  printf("\n};\n\n");

  rcon = 0x01;
  printf("int xrcon[30] = {");
  for (i = 0; i < 30; i++) {
    if (i % 10 == 0) {
      printf("\n  ");
    }
    printf("0x%02x, ", rcon);
    rcon = multiply(2, rcon);
  }
  printf("\n};\n\n");

  makesboxes();

  printbox8(sbox, "xS");
  printbox8(sibox, "xSi");

  return 0;
}