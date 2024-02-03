#include "book.h"
#include "product.h"
#include "util.h"
#include <set>
#include <map>
#include <sstream>
#include <string>

Book::Book(const std::string category, const std::string name, double price,
           int qty, const std::string isbn, const std::string author)
    : Product(category, name, price, qty), isbn_{isbn}, author_{author} {};

std::set<std::string> Book::keywords() const {
    std::set<std::string> kw;
    std::set<std::string> name_kw = parseStringToWords(name_);
    for (std::string s : name_kw) {
        kw.insert(s);
    }
    std::set<std::string> author_kw = parseStringToWords(author_);
    for (std::string s : author_kw) {
        kw.insert(s);
    }
    kw.insert(isbn_);
    return kw;
}

std::string Book::displayString() const {
    std::ostringstream oss;
    oss << name_ << "\n"
        << "Author: " << author_ << " ISBN: " << isbn_ << "\n"
        << price_ << " " << qty_ << " left." << std::endl;
    return oss.str();
}

void Book::dump(std::ostream &os) const {
    Product::dump(os);
    os << isbn_ << "\n" << author_ << std::endl;
}