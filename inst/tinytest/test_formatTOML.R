
library(RcppTOML)

toml <- formatTOML(list(y = "abc", x = 1,
                        z = list(s2 = 5, s1 = 12.5, s4=as.array(c("cd", "ab"))),
                        a = as.array(c(1, 2, 3))))

expect_equal(toml, "y = \"abc\"\nx = 1.0\na = [\n1.0,\n2.0,\n3.0,\n]\n\n[z]\ns2 = 5.0\ns1 = 12.5\ns4 = [\n\"cd\",\n\"ab\",\n]\n\n")