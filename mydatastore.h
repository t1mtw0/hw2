#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include "datastore.h"
#include <string>
#include <map>
#include <deque>

class MyDataStore : public DataStore {
  public:
    MyDataStore();

    ~MyDataStore();

    void addProduct(Product *p);

    void addUser(User *u);

    std::vector<Product *> search(std::vector<std::string> &terms, int type);

    void viewcart(std::string username);

    void buycart(std::string username);

    void add_to_cart(std::string username, int search_hit_number);

    void dump(std::ostream &ofile);

    void store_hits(std::vector<Product *> hits);

  private:
    std::vector<Product *> prods_;
    std::map<std::string, std::vector<Product *>> keyword_map_;
    std::map<User *, std::deque<Product *>> users_;
    std::vector<Product *> last_hits_;
};

#endif