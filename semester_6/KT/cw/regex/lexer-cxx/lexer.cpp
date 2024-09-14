#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: lexer REGEXP FILE\n";
        std::exit(1);
    }
    std::ifstream ifs;
    ifs.open(argv[2], std::ifstream::in);
    std::regex regexp(argv[1]);
    std::string str;
    size_t line = 1;
    while(std::getline(ifs, str)) {
        auto lexeme_begin = std::sregex_iterator(str.begin(), str.end(), regexp);
        auto lexeme_end = std::sregex_iterator();
        int nlexemes = std::distance(lexeme_begin, lexeme_end);
        if (nlexemes > 0) {
            //std::cout << std::distance(lexeme_begin, lexeme_end) << " lexemes in line " << line << ":\n";
            for (std::sregex_iterator i = lexeme_begin; i != lexeme_end; ++i) {
                std::smatch match = *i;
                std::cout << match.str() << '\n';
            }
        }
        line++;
    }
    ifs.close();
    return 0;
}