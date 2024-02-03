#include "mydatastore.h"
#include "util.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <deque>

MyDataStore::MyDataStore() {};

MyDataStore::~MyDataStore() {
    for (Product *p: prods_) {
        delete p;
    }
    for (std::map<User *, std::deque<Product *>>::iterator it = users_.begin(); it != users_.end(); ++it) {
        delete it->first;
    }
};

void MyDataStore::addProduct(Product *p) {
    prods_.push_back(p);
    std::set<std::string> kws = p->keywords();
    for (std::set<std::string>::iterator it = kws.begin(); it != kws.end(); ++it) {
        if (keyword_map_.find(*it) != keyword_map_.end()) {
            keyword_map_[*it].push_back(p);
        } else {
            keyword_map_[*it] = {p};
        }
    }
}

void MyDataStore::addUser(User *u) {
    std::deque<Product *> q;
    users_.insert({u, q});
}

std::vector<Product *> MyDataStore::search(std::vector<std::string> &terms, int type) {
    std::set<Product *> ps;
    std::set<std::string> set_terms(terms.begin(), terms.end());
    if (type == 0) {
        std::vector<Product *> initP = keyword_map_[terms[0]];
        for (Product *p: initP) {
            std::set<std::string> kws = p->keywords();
            if (setIntersection<std::string>(kws, set_terms) == set_terms) {
                ps.insert(p);
            }
        }
    } else if (type == 1) {
        for (std::string t: terms) {
            for (Product *p: keyword_map_[t]) {
                ps.insert(p);
            }
        }
    }
    return std::vector<Product *>(ps.begin(), ps.end());
}

void MyDataStore::viewcart(std::string username) {
    User *u = nullptr;
    for (std::map<User *, std::deque<Product *>>::iterator it = users_.begin(); it != users_.end(); ++it) {
        if (it->first->getName() == username) {
            u = it->first;
        }
    }
    if (u == nullptr) {
        std::cout << "Invalid username" << std::endl;
        return;
    }
    // int i = 1;
    // for (Product *p: users_[u]) {
        // std::cout << p->displayString() << std::endl;
    // }

    int resultNo = 1;
    if (users_[u].begin() == users_[u].end()) {
        std::cout << "No results found!" << std::endl;
        return;
    }
    for (std::deque<Product *>::iterator it = users_[u].begin(); it != users_[u].end();
         ++it) {
        std::cout << "Hit " << std::setw(3) << resultNo << std::endl;
        std::cout << (*it)->displayString() << std::endl;
        std::cout << std::endl;
        resultNo++;
    }

}

void MyDataStore::buycart(std::string username) {
    User *u = nullptr;
    for (std::map<User *, std::deque<Product *>>::iterator it = users_.begin(); it != users_.end(); ++it) {
        if (it->first->getName() == username) {
            u = it->first;
        }
    }
    if (u == nullptr) {
        std::cout << "Invalid username" << std::endl;
        return;
    }
    std::deque<Product *>::iterator it = users_[u].begin();
    while (it != users_[u].end()) {
        if ((*it)->getQty() > 0 && (*it)->getPrice() <= u->getBalance()) {
            u->deductAmount((*it)->getPrice());
            (*it)->subtractQty(1);
            it = users_[u].erase(it);
        } else {
            it++;
        }
    }
}

void MyDataStore::add_to_cart(std::string username, int search_hit_number) {
    User *u = nullptr;
    for (std::map<User *, std::deque<Product *>>::iterator it = users_.begin(); it != users_.end(); ++it) {
        if (it->first->getName() == username) {
            u = it->first;
        }
    }
    if (u == nullptr) {
        std::cout << "Invalid request" << std::endl;
        return;
    }
    if (search_hit_number <= 0 || search_hit_number > (int)last_hits_.size()) {
        std::cout << "Invalid request" << std::endl;
    }
    users_[u].push_back(last_hits_[search_hit_number - 1]);
}

void MyDataStore::dump(std::ostream &ofile) {
    ofile << "<products>" << std::endl;
    for (Product *p: prods_) {
        p->dump(ofile);
    }
    ofile << "</products>" << std::endl;
    ofile << "<users>" << std::endl;
    for (std::map<User *, std::deque<Product *>>::iterator it = users_.begin(); it != users_.end(); ++it) {
        it->first->dump(ofile);
    }
    ofile << "</users>" << std::endl;
}

void MyDataStore::store_hits(std::vector<Product *> hits) {
    last_hits_ = hits;
}