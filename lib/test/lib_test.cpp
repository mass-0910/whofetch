#include <gtest/gtest.h>
#include <util.hpp>
#include <parsearg.hpp>
#include <resource.hpp>
#include <vector>

struct has_name_t {
    int id;
    std::string name;
};

// Test for find_by_name
TEST(UtilTest, find_by_name_test) {
    std::vector<has_name_t> list;
    list.push_back(has_name_t{1, "a"});
    list.push_back(has_name_t{2, "b"});
    list.push_back(has_name_t{3, "c"});
    auto ptr = find_by_name(list, std::string("b"));
    EXPECT_EQ(ptr->id, 2);
}

// Test of argument parsing
TEST(parsearg_test, argument_test) {
    parsearg pa;
    int argc = 3;
    char *argv[] = {"program_name", "arg1", "arg2"};
    pa.argument("foo", "foo is mock");
    pa.argument("bar", "bar is mock");
    pa.parse_args(argc, argv);
    EXPECT_EQ(pa.parsed_value("foo", false), "arg1");
    EXPECT_EQ(pa.parsed_value("bar", false), "arg2");
}

// Test of option parsing
TEST(parsearg_test, option_test) {
    parsearg pa;
    int argc = 2;
    char *argv[] = {"program_name", "--opt2"};
    pa.option("opt1", "opt1 is mock", false);
    pa.option("opt2", "opt2 is mock", false);
    pa.parse_args(argc, argv);
    EXPECT_FALSE(pa.contains_option("opt1"));
    EXPECT_TRUE(pa.contains_option("opt2"));
}

// Test of parsing options with arguments of its own
TEST(parsearg_test, option_with_argument_test) {
    parsearg pa;
    int argc = 5;
    char *argv[] = {"program_name", "--opt1", "arg1", "--opt2", "arg2"};
    pa.option("opt1", "opt1 is mock", true);
    pa.option("opt2", "opt2 is mock", true);
    pa.parse_args(argc, argv);
    EXPECT_TRUE(pa.contains_option("opt1"));
    EXPECT_TRUE(pa.contains_option("opt2"));
    EXPECT_EQ(pa.parsed_value("opt1", true), "arg1");
    EXPECT_EQ(pa.parsed_value("opt2", true), "arg2");
}

// Test for parsing option abbreviated to one character
TEST(parsearg_test, option_shortname_test) {
    parsearg pa;
    int argc = 7;
    char *argv[] = {"program_name", "-A", "-B", "arg1", "-CD", "arg2", "-Earg3"};
    pa.option("optA", "optA is mock", false, 'A');
    pa.option("optB", "optB is mock", true, 'B');
    pa.option("optC", "optC is mock", false, 'C');
    pa.option("optD", "optD is mock", true, 'D');
    pa.option("optE", "optE is mock", true, 'E');
    pa.parse_args(argc, argv);
    EXPECT_TRUE(pa.contains_option("optA"));
    EXPECT_TRUE(pa.contains_option("optB"));
    EXPECT_TRUE(pa.contains_option("optC"));
    EXPECT_TRUE(pa.contains_option("optD"));
    EXPECT_TRUE(pa.contains_option("optE"));
    EXPECT_EQ(pa.parsed_value("optB", true), "arg1");
    EXPECT_EQ(pa.parsed_value("optD", true), "arg2");
    EXPECT_EQ(pa.parsed_value("optE", true), "arg3");
}

// Test if a resource exists after creating it
TEST(resource_test, exist_test) {
    resource res("exist_test");
    EXPECT_FALSE(res);
    res.make_resource(std::string("exist_test"));
    EXPECT_TRUE(res);
    res.delete_resource();
    EXPECT_FALSE(res);
}

// Test for reproducibility when reloading resources
TEST(resource_test, data_test) {
    {
        resource res("data_test");
        res.make_resource(std::string("data_test"));
    }
    resource res_reloaded("data_test");
    std::vector<std::byte> buffer;
    res_reloaded.copy_resource_to_buffer(buffer);
    EXPECT_EQ(std::memcmp(buffer.data(), "data_test", strlen("data_test") + 1), 0);
    res_reloaded.delete_resource();
}