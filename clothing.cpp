#include "clothing.h"
#include "product.h"
#include "util.h"
#include <set>
#include <sstream>
#include <string>

Clothing::Clothing(const std::string category, const std::string name,
                   double price, int qty, const std::string size,
                   const std::string brand)
    : Product(category, name, price, qty), size_{size}, brand_{brand} {};

std::set<std::string> Clothing::keywords() const {
    std::set<std::string> kw;
    std::set<std::string> name_kw = parseStringToWords(name_);
    for (std::string s : name_kw) {
        kw.insert(s);
    }
    std::set<std::string> brand_kw = parseStringToWords(brand_);
    for (std::string s : brand_kw) {
        kw.insert(s);
    }
    return kw;
}

std::string Clothing::displayString() const {
    std::ostringstream oss;
    oss << name_ << "\n"
        << "Size: " << size_ << " Brand: " << brand_ << "\n"
        << price_ << " " << qty_ << " left." << std::endl;
    return oss.str();
}

void Clothing::dump(std::ostream &os) const {
    Product::dump(os);
    os << size_ << "\n" << brand_ << std::endl;
}