#include "db_parser.h"
#include "product.h"
#include "product_parser.h"
#include "mydatastore.h"
#include "util.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

struct ProdNameSorter {
    bool operator()(Product *p1, Product *p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(std::vector<Product *> &hits);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Please specify a database file" << std::endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;

    // Instantiate the individual section and product parsers we want
    ProductSectionParser *productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser *userSectionParser = new UserSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if (parser.parse(argv[1], ds)) {
        std::cerr << "Error parsing!" << std::endl;
        return 1;
    }

    std::cout << "=====================================" << std::endl;
    std::cout << "Menu: " << std::endl;
    std::cout << "  AND term term ...                  " << std::endl;
    std::cout << "  OR term term ...                   " << std::endl;
    std::cout << "  ADD username search_hit_number     " << std::endl;
    std::cout << "  VIEWCART username                  " << std::endl;
    std::cout << "  BUYCART username                   " << std::endl;
    std::cout << "  QUIT new_db_filename               " << std::endl;
    std::cout << "====================================" << std::endl;

    std::vector<Product *> hits;
    bool done = false;
    while (!done) {
        std::cout << "\nEnter command: " << std::endl;
        std::string line;
        std::getline(std::cin, line);
        std::stringstream ss(line);
        std::string cmd;
        if (ss >> cmd) {
            if (cmd == "AND") {
                std::string term;
                std::vector<std::string> terms;
                while (ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                std::vector<Product *> sorted_hits(hits.begin(), hits.end());
                std::sort(sorted_hits.begin(), sorted_hits.end(), ProdNameSorter());
                ds.store_hits(hits);
                displayProducts(hits);
            } else if (cmd == "OR") {
                std::string term;
                std::vector<std::string> terms;
                while (ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                std::vector<Product *> sorted_hits(hits.begin(), hits.end());
                std::sort(sorted_hits.begin(), sorted_hits.end(), ProdNameSorter());
                ds.store_hits(hits);
                displayProducts(hits);
            } else if (cmd == "QUIT") {
                std::string filename;
                if (ss >> filename) {
                    std::ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            } else if (cmd == "ADD") {
                std::string username;
                int search_hit_number;
                ss >> username;
                username = convToLower(username);
                ss >> search_hit_number;
                ds.add_to_cart(username, search_hit_number);
            } else if (cmd == "VIEWCART") {
                std::string username;
                ss >> username;
                username = convToLower(username);
                ds.viewcart(username);
            } else if (cmd == "BUYCART") {
                std::string username;
                ss >> username;
                ds.buycart(username);
            }
            else {
                std::cout << "Unknown command" << std::endl;
            }
        }
    }
    return 0;
}

void displayProducts(std::vector<Product *> &hits) {
    int resultNo = 1;
    if (hits.begin() == hits.end()) {
        std::cout << "No results found!" << std::endl;
        return;
    }
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for (std::vector<Product *>::iterator it = hits.begin(); it != hits.end();
         ++it) {
        std::cout << "Hit " << std::setw(3) << resultNo << std::endl;
        std::cout << (*it)->displayString() << std::endl;
        std::cout << std::endl;
        resultNo++;
    }
}
