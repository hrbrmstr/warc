#include <Rcpp.h>
using namespace Rcpp;

/* Shamelessly stolen from DTL's defunct Rcompression pkg */

#include <zlib.h>

#include <Rdefines.h>
#include <Rinternals.h>

#include "R_ext/Memory.h"
#include "R_ext/Utils.h"

static voidpf R_zlib_alloc(voidpf ptr, uInt items, uInt size) {
  return(R_alloc(items, size));
}

static void R_zlib_free(voidpf ptr, voidpf addr) {}

#define error Rf_error

// [[Rcpp::export]]
SEXP gzuncompress(SEXP r_source, SEXP r_guess_size) {

  z_stream stream;
  int err, len, guess_size = REAL(r_guess_size)[0];
  unsigned char *ans = (unsigned char *)R_alloc(guess_size, sizeof(unsigned char));
  SEXP r_ans;

  stream.next_in = RAW(r_source);
  stream.avail_in = GET_LENGTH(r_source);
  stream.next_out = (unsigned char *) ans;
  stream.avail_out = guess_size;

  stream.zalloc = R_zlib_alloc;
  stream.zfree = R_zlib_free;
  stream.opaque = NULL;

  /* The MAX_WBITS+16 incantation came from Pascal Scheffers' Tcl bindings for zlib. */
  err = inflateInit2( &stream, MAX_WBITS + 16);
  if(err != Z_OK) {
    PROBLEM  "cannot establish the uncompres/inflate stream on this data (%d)", err
    ERROR;
  }
  /* inflate the entire thing.  XXX Need to put this in a loop where we extend the output buffer size if
   it is too small at any point. Z_BUF_ERROR*/
  err = inflate(&stream, Z_FINISH);

  if(err < 0) {
    char *msg =  stream.msg; /*XXX should go away after next command and not be valid when we get to ERROR... but okay for now.*/
  inflateEnd(&stream);
  PROBLEM  "Failed to uncompress the raw data: (%d) %s", err, msg
    ERROR;
  }

  /* terminate our string. */
  len = stream.total_out;
  /* clean up after ourselves. */
  inflateEnd(&stream);

  r_ans = Rf_allocVector(RAWSXP, stream.total_out);
  memcpy(RAW(r_ans), ans, stream.total_out);

  return(r_ans);

}

//' Find the first occurrence (if any) of a sequence of raw bytes
//' (\code{pattern}) in \code{buffer}.
//'
//' @param buffer vector to search in
//' @param pattern sequence of bytes to look for
//' @return index in \code{buffer} or \code{-1} if not found
//' @export
// [[Rcpp::export]]
int find_sequence(RawVector buffer, RawVector pattern) {
  RawVector::iterator it;
  it = std::search(buffer.begin(), buffer.end(),
                   pattern.begin(), pattern.end());
  int pos = it - buffer.begin() + 1;
  if (pos > buffer.size()) pos = -1;
  return(pos);
}