##  RcppTomlPlusPlus -- Rcpp bindings to TOML via cpptomlplusplus
##                      (based on earlier work in RcppTOML using cpptoml)
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
##' The function \code{formatTOML} formats given data (a list) into TOML-formatted string.
##'
##' This implementation uses the \sQuote{tomlplusplus} library by Mark Gillar (see
##' \url{https://github.com/marzer/tomlplusplus}) which is TOML 1.0 compliant.
##'
##' @param data [list] list of values
##' @return A TOML-formatted string
formatTOML <- function(data) {
    formatTOMLImpl(data)
}
