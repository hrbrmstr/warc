#include "warc.h"

#include <Rdefines.h>
#include <Rinternals.h>

#include "R_ext/Memory.h"
#include "R_ext/Utils.h"

gz_fp GZP = *(new gz_fp);

//' @export
// [[Rcpp::export]]
XPtrGz gz_open(std::string path) {

  FILE *wf = fopen(path.c_str(), "wb+");
  gzFile gzf = gzdopen(dup(fileno(wf)), "a9");

  GZP.wfp = wf;
  GZP.gzf = gzf;

  XPtrGz p(&GZP);

  return(p);

}

//' @export
// [[Rcpp::export]]
int gz_offset(XPtrGz GZP) {
  return(gzoffset(GZP->gzf));
}

//' @export
// [[Rcpp::export]]
void gz_write(XPtrGz GZP, SEXP r_source) {
  gzwrite(GZP->gzf, RAW(r_source), GET_LENGTH(r_source));
}

//' @export
// [[Rcpp::export]]
void gz_flush(XPtrGz GZP) {
  gzflush(GZP->gzf, Z_FINISH);
}

//' @export
// [[Rcpp::export]]
void gz_close(XPtrGz GZP) {
  gzclose(GZP->gzf);
  fclose(GZP->wfp);
}
