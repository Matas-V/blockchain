#include "headers.h"
#include "hash.cpp"

void generateUsers(vector<user> &users) {
  random_device rd;
  mt19937 rng(rd());
  uniform_real_distribution<double> uni(MIN_VALUE, MAX_VALUE);
  users.reserve(1000);

  for(int i = 0; i < 1000; i++) {
    user user;
    user.name = "Vardas" + to_string(i);
    user.public_key = hashing(user.name);
    user.balance = uni(rng);
    users.push_back(user);
  }

  ofstream wf("Users.txt");

  for(unsigned int i = 0; i < users.size(); i++) {
    wf << users.at(i).name << endl;
    wf << users.at(i).public_key << endl;
    wf << users.at(i).balance << setprecision(7) << endl;
    wf << endl;
  }
  wf.close();
}

void generateTransactions(vector<transaction> &trans, vector<user> &users) {
  random_device rd;
  mt19937 rng(rd());
  uniform_real_distribution<double> uni(10.0,100000.0);
  uniform_int_distribution<int> uni1(0, 999);
  transaction transaction;
  trans.reserve(10000);
  
  double amount;
  int a, b;
  string sender, receiver;

  for(int i = 0; i < 10000; i++) {
    transaction.sum = uni(rng);
    a = uni1(rng);
    b = uni1(rng);

    while(a == b) {
      b = uni1(rng);
    }

    transaction.sender = users.at(a).public_key;
    transaction.receiver = users.at(b).public_key;
    transaction.transactionId = transaction.sender + transaction.receiver + to_string(transaction.sum);
    trans.push_back(transaction);
  }

  ofstream wf("transactions.txt");

  for (unsigned int i = 0; i < trans.size(); i++) {
    wf << trans.at(i).sum << setprecision(7) << endl;
    wf << trans.at(i).sender << endl;
    wf << trans.at(i).receiver << endl;
    wf << endl;
  }
  wf.close();
}