#include "movie.h"
#include "product.h"
#include "util.h"
#include <sstream>
#include <string>

Movie::Movie(const std::string category, const std::string name, double price,
             int qty, const std::string genre, std::string rating)
    : Product(category, name, price, qty), genre_{genre}, rating_{rating} {};

std::set<std::string> Movie::keywords() const {
    std::set<std::string> kw;
    std::set<std::string> name_kw = parseStringToWords(name_);
    for (std::string s : name_kw) {
        kw.insert(s);
    }
    std::set<std::string> genre_kw = parseStringToWords(genre_);
    for (std::string s : genre_kw) {
        kw.insert(s);
    }
    return kw;
}

std::string Movie::displayString() const {
    std::ostringstream oss;
    oss << name_ << "\n"
        << "Genre: " << genre_ << " Rating: " << rating_ << "\n"
        << price_ << " " << qty_ << " left." << std::endl;
    return oss.str();
}

void Movie::dump(std::ostream &os) const {
    Product::dump(os);
    os << genre_ << "\n" << rating_ << std::endl;
}