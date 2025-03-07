## ----setup, echo=FALSE--------------------------------------------------------
knitr::knit_engines$set(cpp = function(options) {
  options$engine <- "Rcpp"
  knitr::engine_output(options, options$code, out = "")
})
IS_GITHUB <- Sys.getenv("IS_GITHUB") != ""

## ----results='asis', echo=FALSE, eval=IS_GITHUB-------------------------------
#  cat('
#  [![R-CMD-check](https://github.com/qsbase/qs2/workflows/R-CMD-check/badge.svg)](https://github.com/qsbase/qs2/actions)
#  [![CRAN-Status-Badge](https://www.r-pkg.org/badges/version/qs2)](https://cran.r-project.org/package=qs2)
#  [![CRAN-Downloads-Badge](https://cranlogs.r-pkg.org/badges/qs2)](https://cran.r-project.org/package=qs2)
#  [![CRAN-Downloads-Total-Badge](https://cranlogs.r-pkg.org/badges/grand-total/qs2)](https://cran.r-project.org/package=qs2)
#  ')

## ----eval=FALSE---------------------------------------------------------------
#  qs_save(data, "myfile.qs2")
#  data <- qs_read("myfile.qs2")

## ----eval=FALSE---------------------------------------------------------------
#  install.packages("qs2")

## ----eval=FALSE---------------------------------------------------------------
#  remotes::install_cran("qs2", type = "source", configure.args = "--with-TBB --with-simd=AVX2")

## ----eval=FALSE---------------------------------------------------------------
#  remotes::install_cran("qs2", type = "source", configure.args = "--with-TBB")

## ----eval=FALSE---------------------------------------------------------------
#  file_qs2 <- tempfile(fileext = ".qs2")
#  file_rds <- tempfile(fileext = ".RDS")
#  x <- runif(1e6)
#  
#  # save `x` with qs_save
#  qs_save(x, file_qs2)
#  
#  # convert the file to RDS
#  qs_to_rds(input_file = file_qs2, output_file = file_rds)
#  
#  # read `x` back in with `readRDS`
#  xrds <- readRDS(file_rds)
#  stopifnot(identical(x, xrds))

## ----eval=FALSE---------------------------------------------------------------
#  qs_save(data, "myfile.qs2")
#  data <- qs_read("myfile.qs2", validate_checksum = TRUE)

## ----eval=FALSE---------------------------------------------------------------
#  qd_save(data, "myfile.qs2")
#  data <- qd_read("myfile.qs2")

