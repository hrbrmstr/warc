#include <Rcpp.h>
using namespace Rcpp;

#include "warc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <zlib.h>

#include <Rdefines.h>
#include <Rinternals.h>

#include "R_ext/Memory.h"
#include "R_ext/Utils.h"

//' Open a gzip file for reading or writing
//'
//' @param path path to file
//' @param mode "\code{write}" or "\code{read}"
//' @return handle to the file
//' @export
// [[Rcpp::export]]
XPtrGz gz_open(std::string path, std::string mode) {

  std::string fmode, gmode;

  if (mode == "append") {
    fmode = "w+";
    gmode = "a9";
  } else if (mode == "write") {
    fmode = "w";
    gmode = "w9";
  } else if (mode == "read") {
    fmode = "r";
    gmode = "r";
  } else {
    Rcpp::stop("Invalid 'mode' specified");
  }

  FILE *wf = fopen(path.c_str(), fmode.c_str());
  gzFile gzf = gzdopen(dup(fileno(wf)), gmode.c_str());
  gzbuffer(gzf, 16*1024*1024);

  gz_fp *fp = new gz_fp;

  fp->wfp = wf;
  fp->gzf = gzf;

  XPtrGz ptr(fp);

  return(ptr);

}

//' Return the current raw compressed offset in the file
//'
//' @param gzfile file handle
//' @return offset position (integer)
//' @export
// [[Rcpp::export]]
int gz_offset(XPtrGz gzfile) {
  return(gzoffset(gzfile->gzf));
}

//' Sets the starting position for the next \code{gz_read()} or \code{gz_write()}
//'
//' @param offset represents a number of bytes in the uncompressed data stream and the
//' @param from either "\code{start}" or "\code{current}"
//' @return the resulting offset location as measured in bytes from the beginning of the
//'   uncompressed stream, or –1 in case of error, in particular if the file is opened
//'   for writing and the new starting position would be before the current position.
//' @export
// [[Rcpp::export]]
z_off_t gz_seek(XPtrGz gzfile, z_off_t offset, std::string from) {
  if (!gzfile) return(-1);
  int whence = (from == "start") ? SEEK_SET : SEEK_CUR;
  return(gzseek(gzfile->gzf, offset, whence));
}

//' Read from a gz file
//'
//' @export
// [[Rcpp::export]]
std::string gz_read_char(XPtrGz gzfile, unsigned len) {
  if (!gzfile) return("");
  unsigned char *buf = (unsigned char *)malloc(len);
  int res = gzread(gzfile->gzf, buf, len);
  if (res > 0) {
    std::string ret(buf, buf+res);
    free(buf);
    return(ret);
  } else {
    return("");
  }
}

//' Write a raw vector to a gz file
//'
//' @param gzfile file handle
//' @param buffer raw vector to write
//' @export
// [[Rcpp::export]]
void gz_write_raw(XPtrGz gzfile, SEXP buffer) {
  if (!gzfile) return;
  gzwrite(gzfile->gzf, RAW(buffer), GET_LENGTH(buffer));
}

//' Write an atomic character vector to a file
//'
//' @param gzfile file handle
//' @param buffer atomic character vector
//' @export
// [[Rcpp::export]]
void gz_write_char(XPtrGz gzfile, std::string buffer) {
  if (!gzfile) return;
  gzwrite(gzfile->gzf, buffer.c_str(), buffer.size());
}

//' Flush currenzt gzip stream
//'
//' This will flush all zlib output buffers for the current file
//' and terminate the gzip stream. The next \code{gz_write()} will
//' start a new gzip stream.
//'
//' @param gzfile file handle
//' @export
// [[Rcpp::export]]
void gz_flush(XPtrGz gzfile) {
  if (!gzfile) return;
  gzflush(gzfile->gzf, Z_FINISH);
}

//' Close the gz file
//'
//' @param gzfile file handle
//' @export
// [[Rcpp::export]]
void gz_close(XPtrGz gzfile) {
  if (gzfile) {
    gzclose(gzfile->gzf);
    fclose(gzfile->wfp);
  }
}
