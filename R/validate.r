# gzf <- file(wf, open="rb", raw=TRUE)
# for (i in seq_along(gzpos)) {
#   seek(gzf, gzpos[i], "start")
#   d <- readBin(gzf, "raw", 2, 1)
#   if (d[1] != as.raw(0x1f)) {
#     cat(sprintf("%d: ", i))
#     print(d)
#   }
# }
# close(gzf)