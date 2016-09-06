#' Read a WARC entry from a WARC file
#'
#' Given the path to a WARC file (compressed or uncompressed) and the start
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
#' @export
#' @note \code{warcinfo}, \code{request} and \code{response} objects are currently
#'   supported.
#' @examples \dontrun{
#' cdx <- read_cdx(system.file("extdata", "20160901.cdx", package="warc"))
#' i <- 1
#' path <- file.path(cdx$warc_path[i], cdx$file_name[i])
#' start <- cdx$compressed_arc_file_offset[i]
#'
#' (read_warc_entry(path, start))
#' }
read_warc_entry <- function(path, start, size, compressed=grepl(".gz$", path),
                            compressed_buffer_multiplier=10) {

  path <- path.expand(path)

  fil <- file(path, "rb")
  seek(fil, start)
  buffer <- readBin(fil, "raw", size)
  if (compressed) buffer <- expand(buffer, length(buffer)*compressed_buffer_multiplier);
  close(fil)
  process_entry(buffer)

}
