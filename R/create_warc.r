#' Use wget to create a WARC archive for a URL list
#'
#' \code{wget} must be available, on the system \code{PATH} and be compiled with
#' WARC support.
#'
#' @param url_list character vector of URLs or a \code{connection} to a file
#'   with one URL per line
#' @param warc_path path where to store WARC archive output
#' @param user_agent,max_redirects,tries,waitretry,timeout,warc_header,warc_cdx options for \code{wget}
#' @param warc_file,nowarc_digests,no_warc_keep_log,warc_max_size,warc_tempdir options for \code{wget}
#' @param .opts a \code{list} of other options for \code{wget}. Use long-argument names replacing
#'   \code{-} with \code{_} and using \code{TRUE} or \code{FALSE} for boolean arguments.
#' @export
#' @note \code{Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US) AppleWebKit/533.20.25 (KHTML, like Gecko) Version/5.0.4 Safari/533.20.27}
create_warc <- function(url_list, warc_path, user_agent="r-warc", max_redirects=5,
                        tries=2, waitretry=1, timeout=5, warc_header="r-warc",
                        warc_cdx=TRUE, warc_file="r-warc", no_warc_digests=TRUE,
                        no_warc_keep_log=TRUE, warc_max_size="1G", warc_tempdir=tempdir(),
                        .opts=NULL) {

  args <- sprintf('--user-agent="%s"', user_agent)
  args <- c(args, sprintf('--max-redirects=%s', max_redirects))
  args <- c(args, sprintf('--tries=%s', tries))
  args <- c(args, sprintf('--waitretry=%s', waitretry))
  args <- c(args, sprintf('--timeout=%s', timeout))
  args <- c(args, sprintf('--warc-header="%s"', warc_header))
  args <- c(args, sprintf('--warc-file="%s"', warc_file))
  args <- c(args, sprintf('--warc-max-size=%s', warc_max_size))
  args <- c(args, sprintf('--warc-tempdir=%s', warc_tempdir))

  if (warc_cds) args <- c(args, '--warc-cdx')
  if (no_warc_digests) args <- c(args, '--no-warc-digests')
  if (no_warc_keep_log) args <- c(args, '--no-warc-keep-log')

  if (!inherits(url_list, "connection")) {
    tf <- tempfile()
    writeLines(url_list, tf)
  } else {
    tf <- unlist(summary(url_list))[["description"]]
  }

  args <- c(args, sprintf('--input-file="%s"', tf))

  system2("wget", args)

}