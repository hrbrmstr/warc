#' @export
expand <- function(buffer, size=length(buffer)*5) {
  if (typeof(buffer) == "raw") {
    .Call('warc_gzuncompress', PACKAGE='warc', buffer, size)
  } else {
    stop("Not a raw vector", call.=FALSE)
  }
}
