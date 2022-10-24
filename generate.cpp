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

string mineBlock(vector<blockChain> bc, int b, int &n) {
  int x;
	string newhash;
	if (b == 1)
		bc.at(b).prevHash = hashing("");
	else
		bc.at(b).prevHash = bc.at(b-1).blocks.hash;

  bc.at(b).timestamp = time(nullptr);
	bc.at(b).version = "v" + to_string(b) + ".0";
	bc.at(b).diff = "0";
  
  for (int i=0; i<100; i++)
    bc.at(b).merkelRoot += bc.at(b).blocks.transactions.at(i).transactionId;

	x = bc.at(b).diff.size();
  srand((unsigned)time(0));
  for (int i = 0; i < n; i++) {
		bc.at(b).nonce = rand() % 1000000;
		newhash = hashing(bc.at(b).diff + bc.at(b).merkelRoot + bc.at(b).prevHash + to_string(bc.at(b).timestamp) + bc.at(b).version + to_string(bc.at(b).nonce));
		if (newhash.substr(1, x) == bc.at(b).diff)
			return newhash;
	}

	return "0";
}