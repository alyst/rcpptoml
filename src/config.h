#include <toml11.hpp>
#include <tsl/ordered_map.h>

template<typename K, typename V>
using toml_map = tsl::ordered_map<K,V>;
template<typename T>
using toml_array = std::vector<T>;

using toml_comment_policy = toml::discard_comments;
using toml_value_type = toml::basic_value<toml_comment_policy, toml_map, toml_array>;
using toml_table_type = toml_value_type::table_type;
using toml_array_type = toml_value_type::array_type;
