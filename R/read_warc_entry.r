#' Read a WARC entry from a WARC file
#'
#' Given the path to a WARC file (compressed or uncompressed) and the start/end
#' positions of the WARC record, this function will produce an R object from the
#' WARC record.
#'
#' WARC \code{warinfo} objects are returned classed both \code{warc} and
#' \code{info}.
#'
#' WARC \code{response} objects are returned classed both \code{warc} and
#' \code{httr::response} and the standard \code{httr} content functions
#' will work with the object.
#'
#' WARC \code{request} objects are returned classed both \code{warc} and
#' \code{httr::request}.
#'
#' @param path path to WARC file
#' @param start starting offset of WARC record
#' @param size total size of WARC record
#' @param compressed is WARC file compressed? (guessed by file name suffix)
#' @param compressed_buffer_multipler if the WARC file is compressed the record
#'   will be inflated which requires guessing the buffer size for the expanded
#'   data. The default (after numerous tests) of \code{10x} the size of the
#'   compressed record seems to be the sweet spot, but you can override this
#'   value if you run into issues. The buffer is only temporary but is allocated
#'   across each call (just be aware that you're allocating moderately sized
#'   memory chunks each time you call this).
#' @export
#' @note \code{warcinfo}, \code{request} and \code{response} objects are currently
#'   supported.
#' @examples \dontrun{
#' cdx <- read_cdx(system.file("extdata", "20160901.cdx", package="warc"))
#' i <- 1
#' p <- file.path(cdx$warc_path[i], cdx$file_name[i])
#' st <- cdx$compressed_arc_file_offset[i]
#' sz <- cdx$calc_compressed_warc_rec_size[i]
#'
#' (read_warc_entry(p, st, sz))
#' }
read_warc_entry <- function(path, start, size, compressed=grepl("gz$", path),
                            compressed_buffer_multiplier=10) {

  fil <- file(path, "rb")
  seek(fil, start)
  buffer <- readBin(fil, "raw", size)
  if (compressed) buffer <- expand(buffer, length(buffer)*compressed_buffer_multiplier);
  close(fil)
  process_entry(buffer)

}
