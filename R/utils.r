#' Expand a compressed raw buffer
#'
#' @param buffer gz compressed buffer to expand
#' @param size estimated size of the inflated buffer
#' @note Guess \code{size} well since this will fail if \code{size}
#'     is not large enough to hold the uncompressed output.
#' @export
expand <- function(buffer, size=length(buffer)*20) {
  if (typeof(buffer) == "raw") {
    .Call('warc_gzuncompress', PACKAGE='warc', buffer, size)
  } else {
    stop("Not a raw vector", call.=FALSE)
  }
}
