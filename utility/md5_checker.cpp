#include <algorithm>
#include <cstring>
#include <iostream>
#include <openssl/md5.h>
#include <random>
#include <string>

using md5_string = std::basic_string<unsigned char>;

template<typename T>
static void print_integral_string(std::basic_string<T> const& str, bool newline=true) {
    std::cout << "\"";
    for (auto elem : str) {
        std::cout << static_cast<unsigned long long>(elem) << ' ';
    }
    std::cout << "\"";
    if (newline) {
        std::cout << std::endl;
    }
}

static void generate_string(md5_string& result, int size) {
    static char charset[] = "0123456789abcdefghjklmnopqrstuvwxyz";
    result.resize(size);
    // srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        result[i] = charset[rand() % sizeof(charset) - 1];
    }
}

static void search_md5_containing(char const* substr, int str_size, size_t ulimit=100) {
    md5_string tmp{};
    md5_string pattern{};
    md5_string md5{};
    pattern.resize(strlen(substr));
    std::transform(substr, substr + pattern.capacity(), pattern.begin(), [](char ch) { return static_cast<unsigned char>(tolower(ch)); });
    md5.resize(130);

    int k = 0;
    for (size_t i = 1; i < ulimit; ++i) {
        if (i % 10'000'000ull == 0) {
            std::cout << "At " << ++k << "0M" << '\n';
        }
        generate_string(tmp, str_size);

        unsigned char buf[130];
        MD5(tmp.c_str(), tmp.size(), const_cast<unsigned char*>(md5.c_str()));

        if (md5.find(pattern) != md5.npos) {
            std::cout << tmp.c_str() << " --> " << md5.c_str() << '\n';
        }
    }
}

int main(int argc, char* argv[]) {
    int ulimit = 100;
    if (argc > 1) {
        ulimit = strtoul(argv[1], NULL, 10);
    }
    search_md5_containing("'='", 10, ulimit);
}