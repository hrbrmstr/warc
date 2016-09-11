#include <Rcpp.h>
using namespace Rcpp;

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

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#include <fcntl.h>
#include <io.h>
#define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#define SET_BINARY_MODE(file)
#endif

#define CHUNK (16*1024*2)

//' Inflate a gzip stream from a file
//'
//' Given a gzip file that was built with concatenated individual gzip streams,
//' this function will expand the contents of the stream into a \code{raw} vector
//' and return it.
//'
//' @param path path to gzip individual stream compressed WARC file
//' @param raw_stream_pos position in the raw file at \code{path} (not the "gzip
//'   stream position")
//' @note Since this is working with compressed files, the memory size of the returned
//'   value may be quite large.
//' @export
// [[Rcpp::export]]
RawVector gzip_inflate_from_pos(std::string path, size_t raw_stream_pos) {

  // zlib zpipe.c FTW

  int ret;
  unsigned have;
  z_stream strm;
  unsigned char in[CHUNK];
  unsigned char out[CHUNK];

  FILE *source = fopen(path.c_str(), "rb");

  std::vector<unsigned char> rv;
  rv.reserve(CHUNK);

  SET_BINARY_MODE(source);

  strm.zalloc = R_zlib_alloc;
  strm.zfree = R_zlib_free;
  strm.opaque = Z_NULL;
  strm.avail_in = 0;
  strm.next_in = Z_NULL;

  ret = inflateInit2(&strm, 16+MAX_WBITS);
  if (ret != Z_OK) Rcpp::stop("Error in gzip stream");

  fseek(source, raw_stream_pos, SEEK_SET);

  do {

    strm.avail_in = fread(in, 1, CHUNK, source);
    if (ferror(source)) {
      (void)inflateEnd(&strm);
      Rcpp::stop("Error reading from file");
    }

    if (strm.avail_in == 0) break;

    strm.next_in = in;

    do {
      strm.avail_out = CHUNK;
      strm.next_out = out;
      ret = inflate(&strm, Z_NO_FLUSH);
      assert(ret != Z_STREAM_ERROR);
      switch (ret) {
      case Z_NEED_DICT:
        ret = Z_DATA_ERROR;
        (void)inflateEnd(&strm);
        Rcpp::stop("gzip deflate need dict error");
        break;
      case Z_DATA_ERROR:
        (void)inflateEnd(&strm);
        Rcpp::stop("gzip deflate data error");
        break;
      case Z_MEM_ERROR:
        Rcpp::stop("gzip deflate memory error");
        break;
      }
      have = CHUNK - strm.avail_out;
      std::vector<unsigned char> tmp(out, out+have);
      rv.insert(rv.end(), tmp.begin(), tmp.end());
    } while (strm.avail_out == 0);

  } while (ret != Z_STREAM_END);

  (void)inflateEnd(&strm);

  return(wrap(rv));

}

/* Shamelessly stolen from DTL's defunct Rcompression pkg */

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

  err = inflateInit2(&stream, MAX_WBITS+16);
  if(err != Z_OK) {
    Rcpp::stop("cannot establish the uncompres/inflate stream on this data");
  }

  err = inflate(&stream, Z_FINISH);

  if (err < 0) {
    inflateEnd(&stream);
    Rcpp::stop("Failed to uncompress the raw data");
  }

  len = stream.total_out;

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
  it = std::search(buffer.begin(), buffer.end(), pattern.begin(), pattern.end());
  int pos = it - buffer.begin() + 1;
  if (pos > buffer.size()) pos = -1;
  return(pos);
}