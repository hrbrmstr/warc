// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "../inst/include/warc.h"
#include <Rcpp.h>

using namespace Rcpp;

// int_create_cdx_from_warc
void int_create_cdx_from_warc(std::string warc_path, std::string warc_record_types, std::string field_spec, std::string cdx_path);
RcppExport SEXP warc_int_create_cdx_from_warc(SEXP warc_pathSEXP, SEXP warc_record_typesSEXP, SEXP field_specSEXP, SEXP cdx_pathSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type warc_path(warc_pathSEXP);
    Rcpp::traits::input_parameter< std::string >::type warc_record_types(warc_record_typesSEXP);
    Rcpp::traits::input_parameter< std::string >::type field_spec(field_specSEXP);
    Rcpp::traits::input_parameter< std::string >::type cdx_path(cdx_pathSEXP);
    int_create_cdx_from_warc(warc_path, warc_record_types, field_spec, cdx_path);
    return R_NilValue;
END_RCPP
}
// gzip_inflate_from_pos
RawVector gzip_inflate_from_pos(std::string path, size_t raw_stream_pos);
RcppExport SEXP warc_gzip_inflate_from_pos(SEXP pathSEXP, SEXP raw_stream_posSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type path(pathSEXP);
    Rcpp::traits::input_parameter< size_t >::type raw_stream_pos(raw_stream_posSEXP);
    rcpp_result_gen = Rcpp::wrap(gzip_inflate_from_pos(path, raw_stream_pos));
    return rcpp_result_gen;
END_RCPP
}
// gzuncompress
SEXP gzuncompress(SEXP r_source, SEXP r_guess_size);
RcppExport SEXP warc_gzuncompress(SEXP r_sourceSEXP, SEXP r_guess_sizeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type r_source(r_sourceSEXP);
    Rcpp::traits::input_parameter< SEXP >::type r_guess_size(r_guess_sizeSEXP);
    rcpp_result_gen = Rcpp::wrap(gzuncompress(r_source, r_guess_size));
    return rcpp_result_gen;
END_RCPP
}
// find_sequence
int find_sequence(RawVector buffer, RawVector pattern);
RcppExport SEXP warc_find_sequence(SEXP bufferSEXP, SEXP patternSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< RawVector >::type buffer(bufferSEXP);
    Rcpp::traits::input_parameter< RawVector >::type pattern(patternSEXP);
    rcpp_result_gen = Rcpp::wrap(find_sequence(buffer, pattern));
    return rcpp_result_gen;
END_RCPP
}
// gz_open
XPtrGz gz_open(std::string path, std::string mode);
RcppExport SEXP warc_gz_open(SEXP pathSEXP, SEXP modeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type path(pathSEXP);
    Rcpp::traits::input_parameter< std::string >::type mode(modeSEXP);
    rcpp_result_gen = Rcpp::wrap(gz_open(path, mode));
    return rcpp_result_gen;
END_RCPP
}
// gz_offset
int gz_offset(XPtrGz gzfile);
RcppExport SEXP warc_gz_offset(SEXP gzfileSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtrGz >::type gzfile(gzfileSEXP);
    rcpp_result_gen = Rcpp::wrap(gz_offset(gzfile));
    return rcpp_result_gen;
END_RCPP
}
// gz_read_char
std::string gz_read_char(XPtrGz gzfile, unsigned len);
RcppExport SEXP warc_gz_read_char(SEXP gzfileSEXP, SEXP lenSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtrGz >::type gzfile(gzfileSEXP);
    Rcpp::traits::input_parameter< unsigned >::type len(lenSEXP);
    rcpp_result_gen = Rcpp::wrap(gz_read_char(gzfile, len));
    return rcpp_result_gen;
END_RCPP
}
// gz_write_raw
void gz_write_raw(XPtrGz gzfile, SEXP buffer);
RcppExport SEXP warc_gz_write_raw(SEXP gzfileSEXP, SEXP bufferSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtrGz >::type gzfile(gzfileSEXP);
    Rcpp::traits::input_parameter< SEXP >::type buffer(bufferSEXP);
    gz_write_raw(gzfile, buffer);
    return R_NilValue;
END_RCPP
}
// gz_write_char
void gz_write_char(XPtrGz gzfile, std::string buffer);
RcppExport SEXP warc_gz_write_char(SEXP gzfileSEXP, SEXP bufferSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtrGz >::type gzfile(gzfileSEXP);
    Rcpp::traits::input_parameter< std::string >::type buffer(bufferSEXP);
    gz_write_char(gzfile, buffer);
    return R_NilValue;
END_RCPP
}
// gz_flush
void gz_flush(XPtrGz gzfile);
RcppExport SEXP warc_gz_flush(SEXP gzfileSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtrGz >::type gzfile(gzfileSEXP);
    gz_flush(gzfile);
    return R_NilValue;
END_RCPP
}
// gz_close
void gz_close(XPtrGz gzfile);
RcppExport SEXP warc_gz_close(SEXP gzfileSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< XPtrGz >::type gzfile(gzfileSEXP);
    gz_close(gzfile);
    return R_NilValue;
END_RCPP
}
