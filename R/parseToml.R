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


##' Parse a TOML file
##'
##' The function \code{parseTOML}, also aliased to \code{tomlparse} and \code{parseToml},
##' parses TOML content input. This is usually a file given by its name, or a character variable
##' with a TOML expression.
##'
##' This implementation uses the \sQuote{toml11} library by Toru Niina (see
##' \url{https://github.com/ToruNiina/toml11}), which is TOML 1.0 compliant.
##'
##' @param input [character] TOML input, either as chracter value or path to TOML file
##' @param verbose [logical] Optional verbosity flag, no current effect
##' @param fromFile [logical] Optional with default value \sQuote{TRUE} indicating parsing from file
##' @param includize [logical] Optional legacy option, no current effect
##' @param escape [logical] Optional boolean whether strings should be escaped, defaults to
##' \sQuote{TRUE}
##' @param ... Used for aliased function and S3 methods
##' @param x Used for S3 method
##' @param object Used for S3 method
##' @return A list object with the parsed content as an S3 object of class \sQuote{toml}
parseTOML <- function(input, verbose=FALSE, fromFile=TRUE, includize=FALSE, escape=TRUE) {
    if (verbose || includize) {
        warning("Options 'verbose' and 'includize' are available for legacy ",
                "support, have no current effect, and may be removed.")
    }
    if (fromFile) {
        toml <- tomlparseImpl(path.expand(input), fromFile, escape)
    } else {
        toml <- tomlparseImpl(enc2utf8(input), fromFile, escape)
    }
    class(toml) <- c("toml", "list")
    attr(toml, "file") <- input
    toml
}

## alias for now, to be renamed
##' @rdname parseTOML
tomlparse <- function(...) parseTOML(...)

## alias for now, to be renamed
##' @rdname parseTOML
parseToml <- function(...) parseTOML(...)

##' @rdname parseTOML
print.toml <- function(x, ...) {
    cat(utils::str(x, give.attr=FALSE))		           # convenient shortcut
    invisible(x)
}

##' @rdname parseTOML
summary.toml <- function(object, ...) {
    cat("toml object with top-level slots:\n")
    cat("  ", paste(names(object), collapse=", "), "\n")
    cat("read from '", attr(object, "file"), "'\n", sep="")
    invisible(NULL)
}
