#include <Rcpp.h>
using namespace Rcpp;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <zlib.h>

#ifndef __WARC_TYPES__
#define __WARC_TYPES__

typedef struct _gz_fp gz_fp;
typedef gz_fp *gz_fp_ptr;
struct _gz_fp {
  FILE *wfp;
  gzFile gzf;
};


inline void finaliseGz(gz_fp* gzfp) {
  if (gzfp) free(gzfp);
}

typedef Rcpp::XPtr<gz_fp,Rcpp::PreserveStorage,finaliseGz> XPtrGz;

#endif