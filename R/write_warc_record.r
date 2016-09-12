#' Write a WARC record to a file
#'
#' @param warc_rec a WARC Record (as created by \code{as_warc})
#' @param warc_path path to WARC file
#' @param compressed if \code{TRUE} a gz compressed version will be appended to
#'   \code{warc_path}. This inherently means that \code{warc_path} should, therefore, be
#'   an existing file comprised of invividually compressed streams of WARC records.
#' @param append if \code{TRUE} then the data will be appended to \code{warc_path}
#' @export
write_warc_record <- function(warc_rec, warc_path, compressed=FALSE, append=TRUE) {

  warc_path <- path.expand(warc_path)

  rv <- c(charToRaw(warc_rec$rendered_warc_header),
          charToRaw(warc_rec$rendered_response_header))
  if (!is.null(warc_rec$content)) rv <- c(rv, warc_rec$content)
  rv <- c(rv, charToRaw("\r\n\r\n"))

  return(rv)

}

# library(warc)
# library(httr)
#
# sites <- c("http://rud.is/", "http://www.zlib.net/manual.html",
#            "http://rud.is/b", "https://iipc.github.io/warc-specifications/specifications/cdx-format/cdx-2015/",
#            "http://rud.is/a", "http://rud.is/a/", "http://warc.readthedocs.io/en/latest/",
#            "http://www.netpreserve.org/projects/warc-tools-project", "http://www.perceptualedge.com/blog/?p=2503",
#            "https://ianmilligan.ca/2012/12/13/warc-files-part-two-using-warc-tools/")
#
# got <- map(sites, GET)
#
# wf <- "/tmp/a.warc.gz"
#
# gzf <- gz_open(wf, "write")
#
# map(got, as_warc) %>%
#   flatten() %>%
#   map(~write_warc_record(., warc_path=wf)) %>%
#   walk(function(x) {
#     gz_write_raw(gzf, x)
#     gz_flush(gzf)
#   })
#
# gz_close(gzf)
