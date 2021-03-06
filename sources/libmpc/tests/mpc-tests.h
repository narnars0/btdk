/* Tests helper functions.

Copyright (C) 2008, 2009 Philippe Th\'eveny, Andreas Enge, Paul Zimmermann

This file is part of the MPC Library.

The MPC Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPC Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPC Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#ifndef MPC_TESTS_H
#define MPC_TESTS_H

#include <stdio.h>
#include <ctype.h>

#include "mpc-impl.h"

/** OUTPUT HELPER MACROS */
#define MPFR_OUT(X) \
  printf (#X" [%ld]=", MPFR_PREC (X));\
  mpfr_out_str (stdout, 2, 0, (X), GMP_RNDN);\
  printf ("\n");


#define MPC_INEX_STR(inex)                      \
  (inex) == 0 ? "(0, 0)"                        \
    : (inex) == 1 ? "(+1, 0)"                   \
    : (inex) == 2 ? "(-1, 0)"                   \
    : (inex) == 4 ? "(0, +1)"                   \
    : (inex) == 5 ? "(+1, +1)"                  \
    : (inex) == 6 ? "(-1, +1)"                  \
    : (inex) == 8 ? "(0, -1)"                   \
    : (inex) == 9 ? "(+1, -1)"                  \
    : (inex) == 10 ? "(-1, -1)" : "unknown"

#define QUOTE(X) NAME(X)
#define NAME(X) #X

/** RANDOM FUNCTIONS **/
/* the 3 following functions handle seed for random numbers. Usage:
   - add test_start at the beginning of your test function
   - use test_default_random (or use your random functions with
   gmp_randstate_t rands) in your tests
   - add test_end at the end the test function */
extern gmp_randstate_t  rands;
extern char             rands_initialized;

void test_start (void);
void test_end (void);

void test_default_random (mpc_ptr, mp_exp_t, mp_exp_t, unsigned int, unsigned int);
unsigned long urandomm_ui (unsigned long);


/** COMPARISON FUNCTIONS **/
/* some sign are unspecified in ISO C99, thus we record in struct known_signs_t
   whether the sign has to be checked */
typedef struct
{
  int re;  /* boolean value */
  int im;  /* boolean value */
} known_signs_t;

/* same_mpfr_value returns 1:
   - if got and ref have the same value and known_sign is true,
   or
   - if they have the same absolute value, got = 0 or got = inf, and known_sign is
   false.
   returns 0 in other cases.
   Unlike mpfr_cmp, same_mpfr_value(got, ref, x) return 1 when got and
   ref are both NaNs. */
int same_mpfr_value (mpfr_ptr got, mpfr_ptr ref, int known_sign);
int same_mpc_value (mpc_ptr got, mpc_ptr ref, known_signs_t known_signs);


/** GENERIC TESTS **/

typedef int (*CC_func_ptr) (mpc_t, mpc_srcptr, mpc_rnd_t);
typedef int (*CCC_func_ptr) (mpc_t, mpc_srcptr, mpc_srcptr, mpc_rnd_t);
typedef int (*CCU_func_ptr) (mpc_t, mpc_srcptr, unsigned long, mpc_rnd_t);
typedef int (*CCS_func_ptr) (mpc_t, mpc_srcptr, long, mpc_rnd_t);
typedef int (*CCI_func_ptr) (mpc_t, mpc_srcptr, int, mpc_rnd_t);
typedef int (*CCF_func_ptr) (mpc_t, mpc_srcptr, mpfr_srcptr, mpc_rnd_t);
typedef int (*CFC_func_ptr) (mpc_t, mpfr_srcptr, mpc_srcptr, mpc_rnd_t);
typedef int (*CUC_func_ptr) (mpc_t, unsigned long, mpc_srcptr, mpc_rnd_t);
typedef int (*CUUC_func_ptr) (mpc_t, unsigned long, unsigned long, mpc_srcptr,
                              mpc_rnd_t);
typedef int (*FC_func_ptr) (mpfr_t, mpc_srcptr, mpfr_rnd_t);

typedef union
{
  FC_func_ptr FC;     /* output: mpfr_t, input: mpc_t */
  CC_func_ptr CC;     /* output: mpc_t, input: mpc_t */
  CCC_func_ptr CCC;   /* output: mpc_t, inputs: (mpc_t, mpc_t) */
  CCU_func_ptr CCU;   /* output: mpc_t, inputs: (mpc_t, unsigned long) */
  CCS_func_ptr CCS;   /* output: mpc_t, inputs: (mpc_t, long) */
  CCI_func_ptr CCI;   /* output: mpc_t, inputs: (mpc_t, int) */
  CCF_func_ptr CCF;   /* output: mpc_t, inputs: (mpc_t, mpfr_t) */
  CFC_func_ptr CFC;   /* output: mpc_t, inputs: (mpfr_t, mpc_t) */
  CUC_func_ptr CUC;   /* output: mpc_t, inputs: (unsigned long, mpc_t) */
  CUUC_func_ptr CUUC; /* output: mpc_t, inputs: (ulong, ulong, mpc_t) */
} func_ptr;

/* the rounding mode is implicit */
typedef enum
  {
    FC,   /* output: mpfr_t, input: mpc_t */
    CC,   /* output: mpc_t, input: mpc_t */
    CCC,  /* output: mpc_t, inputs: (mpc_t, mpc_t) */
    CCU,  /* output: mpc_t, inputs: (mpc_t, unsigned long) */
    CCS,  /* output: mpc_t, inputs: (mpc_t, long) */
    CCI,  /* output: mpc_t, inputs: (mpc_t, int) */
    CCF,  /* output: mpc_t, inputs: (mpc_t, mpfr_t) */
    CFC,  /* output: mpc_t, inputs: (mpfr_t, mpc_t) */
    CUC,  /* output: mpc_t, inputs: (unsigned long, mpc_t) */
    CUUC  /* output: mpc_t, inputs: (ulong, ulong, mpc_t) */
  } func_type;

/* properties */
#define FUNC_PROP_NONE     0
#define FUNC_PROP_SYMETRIC 1

typedef struct
{
  func_ptr  pointer;
  func_type type;
  char *    name;
  int       properties;
} mpc_function;

#define DECL_FUNC(_ftype, _fvar, _func)         \
  mpc_function _fvar;                           \
  _fvar.pointer._ftype = _func;                 \
  _fvar.type = _ftype;                          \
  _fvar.name = QUOTE (_func);                   \
  _fvar.properties = FUNC_PROP_NONE;


/* tgeneric(mpc_function, prec_min, prec_max, step, exp_max) checks rounding
 with random numbers:
 - with precision ranging from prec_min to prec_max with an increment of
   step,
 - with exponent between -prec_max and prec_max.

 It also checks parameter reuse (it is assumed here that either two mpc_t
 variables are equal or they are different, in the sense that the real part of
 one of them cannot be the imaginary part of the other). */
void tgeneric (mpc_function, mpfr_prec_t, mpfr_prec_t, mpfr_prec_t, mp_exp_t);


/* GMP_RND_MAX is used is tgeneric.c.
   It was renamed MPFR_RND_MAX in mpfr-3.0 */
#if MPFR_VERSION_MAJOR > 2
#  define GMP_RND_MAX MPFR_RND_MAX
#endif


/** READ FILE WITH TEST DATA SET **/
/* data_check (function, "data_file_name") checks function results against
   precomputed data in a file.*/
void data_check (mpc_function, const char *);

FILE * open_data_file (const char *file_name);
void close_data_file (FILE *fp);

/* helper file reading functions */
void skip_whitespace_comments (FILE *fp);
void read_ternary (FILE *fp, int* ternary);
void read_mpfr_rounding_mode (FILE *fp, mpfr_rnd_t* rnd);
void read_mpc_rounding_mode (FILE *fp, mpc_rnd_t* rnd);
mpfr_prec_t read_mpfr_prec (FILE *fp);
void read_int (FILE *fp, int *n, const char *name);
size_t read_string (FILE *fp, char **buffer_ptr, size_t buffer_length, const char *name);
void read_mpfr (FILE *fp, mpfr_ptr x, int *known_sign);
void read_mpc (FILE *fp, mpc_ptr z, known_signs_t *ks);
void tests_memory_start (void);
void tests_memory_end (void);

#define TERNARY_NOT_CHECKED 255
   /* special value to indicate that the ternary value is not checked */
#define TERNARY_ERROR 254
   /* special value to indicate that an error occurred in an mpc function */

#endif /* MPC_TESTS_H */
