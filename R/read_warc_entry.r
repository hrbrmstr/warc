#' Read a WARC entry from a WARC file
#'
#' @param path path to WARC file
#' @param start starting offset of WARC record
#' @param size total size of WARC record
#' @param compressed is WARC file compressed? (guessed by file name suffix)
#' @export
#' @examples \dontrun{
#' cdx <- read_cdx(system.file("extdata", "20160901.cdx", package="warc"))
#' p <- file.path(cdx$warc_path[i], cdx$file_name[i])
#' st <- cdx$compressed_arc_file_offset[i]
#' sz <- cdx$calc_compressed_warc_rec_size[i]
#'
#' cat(read_warc_entry(p, st, sz))
#' }
read_warc_entry <- function(path, start, size, compressed=grepl("gz$", path)) {

  fil <- file(path, "rb")

  (seek(fil, start))

  buffer <- readBin(fil, "raw", size)

  if (compressed) buffer <- Rcompression::gunzip(buffer)

  close(fil)

  buffer

}
