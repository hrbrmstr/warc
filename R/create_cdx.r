#' Create a CDX from a WARC file
#'
#' Takes as input an optionally compressed WARC file and creates a CDX file
#' of \code{warc_record_types} with the specified fields (if available) and
#' writes it to \code{cdx_path}. If the WARC file is compressed the CDX/WARC
#' specification expects each WARC record to be in it's own "gzstream" (i.e you
#' can't just \code{gzip} a plaintext WARC file and expect any CDX indexer
#' to work.)
#'
#' Use an atomic character vector of single character CDX field specifications
#' in the order you want them in the CDX file. The default value
#' "\code{abmsrVgu}" is taken from the defaults used by \code{wget} in
#' "WARC mode" and will output the:
#'
#' \itemize{
#' \item{original url}
#' \item{date}
#' \item{mime type of original document}
#' \item{response code}
#' \item{redirect}
#' \item{compressed arc file offset}
#' \item{file name}
#' \item{URN (warc record id)}
#' }
#'
#' in that order.
#'
#' @param warc_path path to the WARC file to index
#' @param warc_record_types the WARC record types to index in \code{cdx_file}. Should be a
#'   character vector of field names or "\code{all}" to index all records. NOTE: Most CDX
#'   files index WARC \code{response} records.
#' @param field_spec (See \code{Description})
#' @param cdx_path where to output the CDX file
#' @note Only "\code{response}" is the currently supported value for
#'   \code{warc_record_types} and it only indexes gz WARC files...hey, it's alpha s/w.
#' @references \url{https://iipc.github.io/warc-specifications/specifications/cdx-format/cdx-2015/}
#' @export
create_cdx <- function(warc_path, warc_record_types="response",
                       field_spec="abmsrVgu", cdx_path) {

  warc_path <- path.expand(warc_path)
  cdx_path <- path.expand(cdx_path)

  invisible(.Call('warc_int_create_cdx_from_warc', PACKAGE='warc',
                  warc_path=warc_path, warc_record_types=warc_record_types,
                  field_spec=field_spec, cdx_prefix=cdx_path))

}