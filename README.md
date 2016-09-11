
`warc` : Tools to Work with the Web Archive Ecosystem

[WARC](http://www.digitalpreservation.gov/formats/fdd/fdd000236.shtml) files (and the metadata files that usually follow them) are the de facto method of archiving web content. There are tools in Python & Java to work with this data and there are many "big data" tools that make working with large-scale data from sites like [Common Crawl](http://commoncrawl.org/) and [The Internet Archive](https://archive.org/index.php) very straightforward.

Now there are tools to create and work with the WARC ecosystem in R.

Possible use-cases:

-   If you need to scrape data from many URLs and would like to make the analyses on that data reproducible but are concerned that the sites may change format or may be offline but also don't want to manage individual HTML (etc) files
-   Analyzing Common Crawl data (etc) natively in R
-   Saving the entire state of an `httr` request (`warc` can turn `httr` responses into WARC files and turns WARC `response` records into `httr::response` objects)

`warc` can work with WARC files that are composed of individual gzip streams or on plaintext WARC files and can also read & generate CDX files. Support for more file types (e.g. WET, WAT, etc) are planned.

The following functions are implemented:

-   `as_warc`: Convert an `httr::respone` object to WARC response objects
-   `create_cdx`: Create a CDX from a WARC file
-   `create_warc_wget`: Use wget to create a WARC archive for a URL list
-   `find_sequence`: Find the first occurrence (if any) of a sequence of raw bytes (`pattern`) in `buffer`
-   `read_cdx`: Read a WARC CDX index file
-   `read_warc_entry`: Read a WARC entry from a WARC file
-   `warc_headers`: Extract WARC headers from a WARC response object
-   `create_warc` Use wget to create a WARC archive for a URL list
-   `write_warc_record`: Write a WARC record to a file

### Installation

You need `wget` on your system `PATH`. Folks on real operating systems can do the `apt-get`, `yum install` or `brew install` (et al) dance for your particular system. Version 1.18+ is recommended, but any version with support for WARC extensions should do.

Windows folks will need to grab the statically linked [32-bit](https://eternallybored.org/misc/wget/current/wget.exe) or [64-bit](https://eternallybored.org/misc/wget/current/wget64.exe) binaries from [here](https://eternallybored.org/misc/wget/) and put them on your system `PATH` somewhere if you want to create WARC files in bulk using `wget`.

``` r
devtools::install_git("https://gitlab.com/hrbrmstr/warc.git")
```

### Usage

``` r
library(warc)
library(httr)

# current verison
packageVersion("warc")
```

    ## [1] '0.1.0'

``` r
cdx <- read_cdx(system.file("extdata", "20160901.cdx", package="warc"))

i <- 5

path <- file.path(cdx$warc_path[i], cdx$file_name[i])
start <- cdx$compressed_arc_file_offset[i]

entry <- read_warc_entry(path, start)

print(entry)
```

    ## Response [https://r-project.org/]
    ##   Date: 2016-09-01 18:03
    ##   Status: 301
    ##   Content-Type: text/html; charset=iso-8859-1
    ##   Size: 314 B
    ## <!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
    ## <html><head>
    ## <title>301 Moved Permanently</title>
    ## </head><body>
    ## <h1>Moved Permanently</h1>
    ## <p>The document has moved <a href="https://www.r-project.org/">here</a>.</p>
    ## <hr>
    ## <address>Apache/2.4.10 (Debian) Server at r-project.org Port 443</address>
    ## </body></html>

``` r
print(warc_headers(entry))
```

    ## $`warc-type`
    ## [1] "response"
    ## 
    ## $`warc-record-id`
    ## [1] "<urn:uuid:9EB1B94E-A929-4B5E-AE10-FB54C2E7308B>"
    ## 
    ## $`warc-warcinfo-id`
    ## [1] "<urn:uuid:7D7DC0CA-8FC7-4FFB-9D5A-4187EB50ED74>"
    ## 
    ## $`warc-concurrent-to`
    ## [1] "<urn:uuid:592019F2-C037-4899-8DB2-CC04266B9E29>"
    ## 
    ## $`warc-target-uri`
    ## [1] "https://r-project.org/"
    ## 
    ## $`warc-date`
    ## [1] "2016-09-01 18:03:46 UTC"
    ## 
    ## $`warc-ip-address`
    ## [1] "137.208.57.37"
    ## 
    ## $`warc-block-digest`
    ## [1] "\020"
    ## 
    ## $`warc-payload-digest`
    ## [1] ":\001"
    ## 
    ## $`content-type`
    ## [1] "application/http;msgtype=response"
    ## 
    ## $`content-length`
    ## [1] 578
    ## 
    ## attr(,"class")
    ## [1] "insensitive" "list"

``` r
print(status_code(entry))
```

    ## [1] 301

``` r
print(http_type(entry))
```

    ## [1] "text/html"

### Creating + reading

``` r
library(warc)
library(purrr)
library(rvest)

warc_dir <- file.path(tempdir(), "rfolks")
dir.create(warc_dir)

urls <- c("http://rud.is/",
          "http://hadley.nz/",
          "http://dirk.eddelbuettel.com/",
          "https://jeroenooms.github.io/",
          "https://ironholds.org/")

create_warc_wget(urls, warc_dir, warc_file="rfolks-warc")

cdx <- read_cdx(file.path(warc_dir, "rfolks-warc.cdx"))

sites <- map(1:nrow(cdx),
             ~read_warc_entry(file.path(cdx$warc_path[.],
                                        cdx$file_name[.]), 
                              cdx$compressed_arc_file_offset[.]))

map(sites, ~read_html(content(., as="text", encoding="UTF-8"))) %>% 
  map_chr(~html_text(html_nodes(., "title")))
```

    ## [1] "rud.is"              "Hadley Wickham"      "Dirk Eddelbuettel"   "About Jeroen..."     "Oliver Keyes - Home"

``` r
unlink(warc_dir)
```

### Test Results

``` r
library(warc)
library(testthat)

date()
```

    ## [1] "Sun Sep 11 14:35:59 2016"

``` r
test_dir("tests/")
```

    ## testthat results ========================================================================================================
    ## OK: 0 SKIPPED: 0 FAILED: 0
    ## 
    ## DONE ===================================================================================================================
