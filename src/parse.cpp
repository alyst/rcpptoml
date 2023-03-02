//  RcppTomlPlusPlus -- Rcpp bindings to TOML via cpptomlplusplus
//                      (based on earlier work in RcppTOML using cpptoml)
//
//  Copyright (C) 2015 - 2023  Dirk Eddelbuettel
//
//  This file is part of RcppTomlPlusPlus
//
//  RcppTomlPlusPlus is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 2 of the License, or
//  (at your option) any later version.
//
//  RcppTomlPlusPlus is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with RcppTomlPlusPlus.  If not, see <http://www.gnu.org/licenses/>.

#include <sstream>
#include <Rcpp/Lightest>
#include "parse.h"

SEXP getValue(const toml_value_type& val, bool escape=true) {
    if (val.is_string()) {
        std::string v{val.as_string()};
        if (escape) {
            v = escapeString(v);
        }
        Rcpp::String se(v, CE_UTF8);
        return Rcpp::wrap(se);
    } else if (val.is_integer()) {
        int64_t v{val.as_integer()};
        int32_t iv = static_cast<int32_t>(v); // known lossy but R has really only int32
        return Rcpp::wrap(iv);
    } else if (val.is_floating()) {
        double v{val.as_floating()};
        return Rcpp::wrap(v);
    } else if (val.is_boolean()) {
        bool v{val.as_boolean()};
        return Rcpp::wrap(v);
    } else if (val.is_local_date()) {
        const toml::local_date v{val.as_local_date()};
        Rcpp::Date d(v.year, v.month, v.day);
        return Rcpp::wrap(d);
    } else if (val.is_local_datetime() || val.is_offset_datetime()) {
        const bool islocal = val.is_local_datetime();
        const toml::local_date date{
            islocal ? val.as_local_datetime().date : val.as_offset_datetime().date
        };
        const toml::local_time time{
            islocal ? val.as_local_datetime().time : val.as_offset_datetime().time
        };

        std::tm tm{};
        tm.tm_year = date.year - 1900;
        tm.tm_mon = date.month - 1;
        tm.tm_mday = date.day;
        tm.tm_hour = time.hour;
        tm.tm_min = time.minute;
        tm.tm_sec = time.second;
        //tm.tm_isdst = 1; // not filled
        time_t tt = local_timegm(&tm); // helper also used earlier in RcppTOML
        if (!islocal) {
            toml::time_offset offset{val.as_offset_datetime().offset};
            tt = tt - offset.minute*60 - offset.hour*3600;
        }
        Rcpp::DatetimeVector dt(1, "UTC"); // we always set UTC as RcppTOML did
        dt[0] =  static_cast<double>(tt) + time.nanosecond * 1.0e-9;
        return Rcpp::wrap(dt);
    } else if (val.is_local_time()) {
        const toml::local_time v{val.as_local_time()};
        std::stringstream ss;   		// print the time to string as there is no
        ss << val;               		// base R time type (we could pull in hms
        return Rcpp::wrap(ss.str());    // but rather not have the dependency
    }
    std::stringstream ss;   // because we have no Datetime ctor from components :-/
    ss << val.type();
    Rcpp::warning("Unknown type: %s", ss.str());
    return R_NilValue;
}

bool is_array_of_tables(const toml_value_type& arr) {
    return arr.is_array() && (arr.as_array().size() > 0) && arr.as_array().at(0).is_table();
}

SEXP getTable(const toml_table_type& tbl, bool escape = true) {
    Rcpp::StretchyList sl;
    for (const auto&[key, val] : tbl) {
        if (is_array_of_tables(val)) {
            Rcpp::StretchyList l;
            const toml_array_type& arr = val.as_array();
            for (const auto& el : arr) {
                l.push_back(getTable(el.as_table(), escape));
            }
            sl.push_back(Rcpp::Named(key.data()) = Rcpp::as<Rcpp::List>(l));
        } else if (val.is_table()) {
            sl.push_back(Rcpp::Named(key.data()) = getTable(val.as_table(), escape));
        } else if (val.is_array()) {
            sl.push_back(Rcpp::Named(key.data()) = getArray(val.as_array(), escape));
         } else if (!val.is_uninitialized()) {
            sl.push_back(Rcpp::Named(key.data()) = getValue(val, escape));
        } else {
            Rcpp::Rcout << "unknown type in table: " << val.type() << "\n";
        }
    }
    return Rcpp::as<Rcpp::List>(sl);
}

SEXP getArray(const toml_array_type& arr, bool escape) {
    Rcpp::StretchyList sl;
    bool nonested = true;       // ie no embedded array
    for (const auto& val: arr) {
        if (val.is_array()) {
            sl.push_back(getArray(val.as_array(), escape));
            nonested = false;
        } else if (!val.is_uninitialized()) {
            sl.push_back(getValue(val, escape));
        } else {
            Rcpp::Rcout << "unknown type in array: " << val.type() << "\n";
        }
    }
    if (nonested) {
        return collapsedList(Rcpp::as<Rcpp::List>(sl));
    } else {
        return Rcpp::as<Rcpp::List>(sl);
    }
}

//' @noRd
// [[Rcpp::export()]]
Rcpp::List tomlparseImpl(const std::string input, bool fromfile=true, bool escape=true) {
    if (fromfile) {
        const auto tbl = toml::parse<toml_comment_policy, toml_map, toml_array>(input);
        return Rcpp::as<Rcpp::List>(getTable(tbl.as_table(), escape));
    } else {
        std::istringstream istream(input);
        const auto tbl = toml::parse<toml_comment_policy, toml_map, toml_array>(istream);
        return Rcpp::as<Rcpp::List>(getTable(tbl.as_table(), escape));
    }
}
