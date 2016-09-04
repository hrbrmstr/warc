#' Read a WARC CDX index file
#'
#' CDX files are used to index the content of WARC files.
#'
#' The returned object is a \code{tbl_df} but is also classed
#' \code{cdx}.
#'
#' @param path path to CDX file
#' @param warc_path path to the WARC files referenced in \code{path}. Defaults to
#'     to the location of the CDX file
#' @references \url{https://iipc.github.io/warc-specifications/specifications/cdx-format/cdx-2015/}
#' @export
#' @examples \dontrun{
#' cdx <- read_cdx(system.file("extdata", "20160901.cdx", package="warc"))
#' }
read_cdx <- function(path, warc_path=dirname(path)) {

  lines <- readr::read_lines(path.expand(path))

  delim <- substr(lines[1], 1, 1)

  if (substr(lines[1], 2, 4) != "CDX") {
    stop("Not a CDX file", call.=FALSE)
  }

  if (substr(lines[1], 5, 5) != delim) {
    stop("Malformed CDX file header", call.=FALSE)
  }

  header <- stri_split_fixed(substr(lines[1], 6, nchar(lines[1])), delim)[[1]]
  header <- data_frame(field=header)
  header <- left_join(header, field_trans, by="field")

  map_df(lines[-1], function(x) {
    bits <- stri_split_fixed(x, delim)[[1]]
    record <- setNames(as.list(bits), header$short_name)
  }) -> df

  df <- mutate(df, date=as.POSIXct(date, "%Y%m%d%H%M%S", tz="GMT"))
  df <- mutate(df, warc_path=warc_path)
  df <- suppressMessages(readr::type_convert(df))

  if ("compressed_arc_file_offset" %in% colnames(df)) {
    if (!("compressed_record_size" %in% colnames(df))) {
      group_by(df, file_name) %>%
        do(calc_size(., warc_path)) %>%
        ungroup() -> df
    }
  }

  if ("response_code" %in% colnames(df)) {
    df <- mutate(df, response_code=ifelse(response_code==0, NA, response_code))
  }

  if ("original_url" %in% colnames(df)) {
    df <- mutate(df, original_url=remove_brackets(original_url))
  }

  class(df) <- c("cdx", class(df))
  df

}

#' Display a CDX object
#' @export
print.cdx <- function(x, ...) {

  fields <- left_join(data_frame(short_name=colnames(x)), field_trans, by="short_name")
  fields <- mutate(fields, description=ifelse(is.na(description), "", description))

  cat(sprintf("# A CDX WARC index with %s records and the following fields:\n\n",
              scales::comma(nrow(x))))

  max_sn <- max(map_int(fields$short_name, nchar))

  for (i in 1:nrow(fields)) {
    cat(sprintf("%s: %s\n",
            stri_pad_left(fields$short_name[i], max_sn),
            fields$description[i]))
  }

  cat("\n\n")

  dplyr::glimpse(x)

}

#' Calculate the size of the final WARC record in the file
#' @noRd
calc_size <- function(x, warc_path) {

  fsiz <- file.size(file.path(warc_path, x$file_name[1]))

  x$calc_compressed_warc_rec_size <-
    c(x$compressed_arc_file_offset[-1], 0) -
    x$compressed_arc_file_offset + 1

  x$calc_compressed_warc_rec_size[nrow(x)] <-
    fsiz - x$compressed_arc_file_offset[nrow(x)] + 1

  x

}