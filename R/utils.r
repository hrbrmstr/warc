#' Expand a compressed raw buffer
#'
#' @param buffer gz compressed buffer to expand
#' @param size estimated size of the inflated buffer
#' @note Guess \code{size} well since this will fail if \code{size}
#'   is not large enough to hold the uncompressed output.
#' @export
expand <- function(buffer, size=length(buffer)*20) {
  if (typeof(buffer) == "raw") {
    .Call('warc_gzuncompress', PACKAGE='warc', buffer, size)
  } else {
    stop("Not a raw vector", call.=FALSE)
  }
}

#' Extract WARC headers from a WARC response object
#'
#' @param x WARC response object
#' @return list of WARC headers
#' @export
warc_headers <- function(x) { x$warc_header }

#' @export
remove_brackets <- function(x) {
  map_chr(x, function(y) {
    if ((substr(y, 1, 1) == "<") &
        (substr(y, nchar(y), nchar(y)) == ">")) {
      gsub("(^<|>$)", "", y)
    } else {
      y
    }
  })
}