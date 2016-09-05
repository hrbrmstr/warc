#' Convert an \code{httr::respone} object to WARC response objects
#'
#' @param x \code{httr} \code{response} object
#' @export
as_warc <- function(x) {

  if (inherits(x, "response")) {

    ah <- x$all_headers

    purrr::map(ah, function(h) {

      r_status <- sprintf("%s %s %s\r\n", h$version, h$status, http_statuses[as.character(h$status)])
      r_headers <- h$headers
      names(r_headers) <- stri_trans_totitle(names(h$headers))
      ro <- map2_chr(names(r_headers), r_headers, function(n, v) {
        if (h$status != "200") {
          if (n == "Content-Length") v <- 0
        }
        sprintf("%s: %s\r\n", n, v)
      })
      ro <- c(r_status, ro, "\r\n")
      ro <- paste0(ro, collapse="")

      cl <- nchar(ro)
      cnt <- NULL
      if (h$status == "200") {
        cl <- cl + length(x$content)
        cnt <- x$content
      }

      warc_version <- "WARC/1.0\r\n"
      w_headers <- list(`WARC-Tye`="response",
                        `WARC-Date`=strftime(x$date, "%Y-%m-%dT-%H:%M:%sZ", td="GMT"),
                        `Content-Type`="application/http;msgtype=response",
                        `WARC-Identified-Payload-Type`=x$headers$`content-type`,
                        `WARC-Record-ID`=sprintf("urn:uuid:%s", uuid::UUIDgenerate()),
                        `Content-Length`=cl)

      wo <- map2_chr(names(w_headers), w_headers, function(n, v) {
        sprintf("%s: %s\r\n", n, v)
      })
      wo <- c(warc_version, wo, "\r\n")
      wo <- paste0(wo, collapse="")

      ret <- list(rendered_warc_header=wo, rendered_response_header=ro, content=cnt)
      class(ret) <- c("warc", "warc_record", class(ret))

      ret

    })

  }

}

#' Write a WARC record to a file
#'
#' @param warc_rec a WARC Record (as created by \code{as_warc})
#' @param warc_path path to WARC file
#' @param append if \code{TRUE} then the data will be appended to \code{warc_path}
#' @export
write_warc_record <- function(warc_rec, warc_path, append=TRUE) {

  warc_path <- path.expand(warc_path)

  cat(warc_rec$rendered_warc_header, file=warc_path, append=append)
  cat(warc_rec$rendered_response_header, file=warc_path, append=TRUE)
  if (!is.null(warc_rec$content)) {
    wf <- file(warc_path, "ab")
    writeBin(warc_rec$content, wf, useBytes=TRUE)
    close(wf)
  }
  cat("\r\n\r\n", file=warc_path, append=TRUE)

}