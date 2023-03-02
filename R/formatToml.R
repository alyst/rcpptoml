##  RcppTomlPlusPlus -- Rcpp bindings to TOML via toml11
##                      (based on earlier work in RcppTOML using cpptoml/toml++)
##
## Copyright (C) 2015 - 2023  Dirk Eddelbuettel
##
## This file is part of RcppTomlPlusPlus.
##
## RcppTomlPlusPlus is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 2 of the License, or
## (at your option) any later version.
##
## RcppTomlPlusPlus is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with RcppTomlPlusPlus.  If not, see <http://www.gnu.org/licenses/>.


##' Format data into a TOML string
##'
##' The function \code{formatTOML} formats given data (a list) into
##' TOML-formatted string.
##'
##' This implementation uses the \sQuote{toml11} library by Toru Niina
##' (see \url{https://github.com/ToruNiina/toml11}), which is
##' TOML 1.0 compliant.
##'
##' @param data [list] list of values to encode as TOML
##' @return A TOML-formatted string
formatTOML <- function(data) {
    formatTOMLImpl(data)
}
