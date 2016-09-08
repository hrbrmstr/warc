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

struct node {
  gz_fp gzf;
  node *next;
};

node gzlist = *(new node);
node *lstail = &gzlist;

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

  lstail->gzf.wfp = wf;
  lstail->gzf.gzf = gzf;

  XPtrGz ptr(&(lstail->gzf));
  lstail->next = new node;
  lstail = lstail->next;

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

//' Read from a gz file
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
