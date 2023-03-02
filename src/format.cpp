#include "config.h"

#include <Rcpp/Lightest>
#include <toml11.hpp>

toml_value_type toTOML(const Rcpp::List& data) {
    toml_table_type tbl;
    Rcpp::CharacterVector dataNames = Rcpp::as<Rcpp::CharacterVector>(data.names());
    for (R_xlen_t i = 0; i < data.size(); ++i) {
        const auto key = dataNames[i];
        const Rcpp::RObject el = data[i];
        if (Rcpp::is<Rcpp::List>(el)) {
            tbl.emplace(key, toTOML(Rcpp::as<Rcpp::List>(el)));
        //} else if (Rcpp::is<Rcpp::S4>(el)) {
        //    tbl.emplace(key, toTOML(el));
        } else if (Rcpp::is<Rcpp::String>(el)) {
            tbl.emplace(key, Rcpp::as<std::string>(el));
        } else if (Rcpp::is<int>(el)) {
            tbl.emplace(key, Rcpp::as<int>(el));
        } else if (Rcpp::is<double>(el)) {
            tbl.emplace(key, Rcpp::as<double>(el));
        } else if (Rcpp::is<bool>(el)) {
            tbl.emplace(key, Rcpp::as<bool>(el));
        } else if (Rcpp::is<Rcpp::IntegerVector>(el)) {
            tbl.emplace(key, Rcpp::as< toml_array<int> >(el));
        } else if (Rcpp::is<Rcpp::NumericVector>(el)) {
            tbl.emplace(key, Rcpp::as< toml_array<double> >(el));
        } else if (Rcpp::is<Rcpp::LogicalVector>(el)) {
            tbl.emplace(key, Rcpp::as< toml_array<bool> >(el));
        } else if (Rcpp::is<Rcpp::CharacterVector>(el)) {
            tbl.emplace(key, Rcpp::as< toml_array<std::string> >(el));
        } else {
            Rcpp::Rcout << "unknown type: " << el.sexp_type() << "\n";
        }
    }
    return toml_value_type(tbl);
}

//' @noRd
// [[Rcpp::export()]]
Rcpp::String formatTOMLImpl(const Rcpp::List data) {
    return Rcpp::String(toml::format(toTOML(data)));
}
