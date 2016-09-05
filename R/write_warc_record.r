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

  mode <- if (append) "ab" else "wb"
  wf <- if (compressed) gzfile(warc_path, mode) else file(warc_path, mode)

  cat(warc_rec$rendered_warc_header, file=wf, append=append)
  cat(warc_rec$rendered_response_header, file=wf, append=TRUE)

  if (!is.null(warc_rec$content)) writeBin(warc_rec$content, wf, useBytes=TRUE)

  cat("\r\n\r\n", file=wf, append=TRUE) # two CRLFs after WARC record

  flush(wf)

  close(wf)

}