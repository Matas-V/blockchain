#include "headers.h"
#include "generate.cpp"

int main() {
  vector<user> users;
  vector<transaction> trans;
  block bs;
  bs.transactions.reserve(100);

  generateUsers(users);
  generateTransactions(trans, users);

  random_device rd;
  mt19937 gen(rd());
  for (int i = 0; i < 100; i++) {
    uniform_int_distribution<> distr(0, trans.size());
    int random = distr(gen);
    bs.transactions.push_back(trans.at(random).transactionId);
    trans.erase(trans.begin()+random-1);
  }

  return 0;
}