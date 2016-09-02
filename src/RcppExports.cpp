// This file was generated by Rcpp::compileAttributes
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// gzuncompress
SEXP gzuncompress(SEXP r_source, SEXP r_guess_size);
RcppExport SEXP warc_gzuncompress(SEXP r_sourceSEXP, SEXP r_guess_sizeSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< SEXP >::type r_source(r_sourceSEXP);
    Rcpp::traits::input_parameter< SEXP >::type r_guess_size(r_guess_sizeSEXP);
    __result = Rcpp::wrap(gzuncompress(r_source, r_guess_size));
    return __result;
END_RCPP
}
// find_sequence
int find_sequence(RawVector buffer, RawVector pattern);
RcppExport SEXP warc_find_sequence(SEXP bufferSEXP, SEXP patternSEXP) {
BEGIN_RCPP
    Rcpp::RObject __result;
    Rcpp::RNGScope __rngScope;
    Rcpp::traits::input_parameter< RawVector >::type buffer(bufferSEXP);
    Rcpp::traits::input_parameter< RawVector >::type pattern(patternSEXP);
    __result = Rcpp::wrap(find_sequence(buffer, pattern));
    return __result;
END_RCPP
}
