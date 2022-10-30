#include "headers.h"
#include "hash.cpp"
#include "dateConverter.cpp"

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
    wf << users.at(i).balance << fixed << setprecision(2) << endl;
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
    transaction.sum = round(uni(rng));
    a = uni1(rng);
    b = uni1(rng);

    while(a == b) {
      b = uni1(rng);
    }

    transaction.sender = users.at(a).public_key;
    transaction.receiver = users.at(b).public_key;
    transaction.transactionId = hashing(transaction.sender + transaction.receiver + to_string(transaction.sum));
    trans.push_back(transaction);
  }

  ofstream wf("transactions.txt");

  for (unsigned int i = 0; i < trans.size(); i++) {
    wf << trans.at(i).sum << fixed << setprecision(2) << endl;
    wf << trans.at(i).sender << endl;
    wf << trans.at(i).receiver << endl;
    wf << trans.at(i).transactionId << endl;
    wf << endl;
  }
  wf.close();
}

string mineBlock(blockChain &bc, string prevHash, int b, int n) {
  int x;
	string newhash;
	if (b == 0)
		bc.prevHash = hashing("");
	else
		bc.prevHash = prevHash;

  bc.timestamp = time(nullptr);
	bc.version = "v" + to_string(b+1) + ".0";
	bc.diff = "000";
  
  bc.merkelRoot = generateMerkleRoot(bc.block.transactions);

	x = bc.diff.size();
  int nonce=0;
  for (int i=0; i<n; i++) {
    newhash = hashing(bc.diff + bc.merkelRoot + bc.prevHash + to_string(bc.timestamp) + bc.version + to_string(nonce));
		if (newhash.substr(0, x) == bc.diff) {
      bc.nonce = nonce;
			return newhash;
    }
    nonce++;
  }

  return "0";
}

void readUsers(vector<user> &con) {
  ifstream df("Users.txt");
  stringstream my_buffer;
  my_buffer.clear();
  if (df.is_open()) {
    my_buffer << df.rdbuf();
    df.close();
  }
  user u;
  while(!my_buffer.eof()) {
    my_buffer >> u.name;
    my_buffer >> u.public_key;
    my_buffer >> u.balance;
    my_buffer >> ws;
    con.push_back(u);
  }
}

void readTrans(vector<transaction> &con) {
  ifstream df("transactions.txt");
  stringstream my_buffer;
  my_buffer.clear();
  if (df.is_open()) {
    my_buffer << df.rdbuf();
    df.close();
  }
  transaction t;
  while(!my_buffer.eof()) {
    my_buffer >> t.sum;
    my_buffer >> t.sender;
    my_buffer >> t.receiver;
    my_buffer >> t.transactionId;
    my_buffer >> ws;
    con.push_back(t);
  }
}

void usersData(vector<user> users) {
  ofstream wf("UsersRez.txt");
  for(unsigned int i = 0; i < users.size(); i++) {
    wf << users.at(i).name << endl;
    wf << users.at(i).public_key << endl;
    wf << users.at(i).balance << fixed << setprecision(2) << endl;
    wf << endl;
  }
  wf.close();
}

void printTrans(transaction t) {
  cout << "---------------------- TRANSACTION ----------------------" << endl;
  cout << "ID: " << t.transactionId << endl;
  cout << "Sender: " << t.sender << endl;
  cout << "Receiver: " << t.receiver << endl;
  cout << "Amount: " << t.sum << endl;
  cout << "---------------------------------------------------------" << endl;
}

void printBlock(blockChain bc) {
  cout << "------------------------- BLOCK -------------------------" << endl;
  cout << "Block hash: " << bc.block.hash << endl;
  cout << "Previous block hash: " << bc.prevHash << endl;
  cout << "Version: " << bc.version << endl;
  cout << "Merkle Root: " << bc.merkelRoot.substr(0, 64) << endl;
  cout << "Nonce: " << bc.nonce << endl;
  cout << "Mined on: " << unixTimeToHumanReadable(bc.timestamp) << " (GMT+2)" << endl;
  cout << "Difficulty: " << bc.diff << endl;
  cout << "---------------------------------------------------------" << endl;
}

string generateMerkleRoot(vector<transaction> trans) {
  vector<string> merkel, merkel2;
  string word, hash;

  for(unsigned int i = 0; i < trans.size(); i++) {
    merkel.push_back(trans.at(i).transactionId);
  }

  while (merkel.size() > 1) {
    if (merkel.size() % 2 == 0) {
      for (unsigned i = 0; i < merkel.size(); i+=2) {
        word = merkel.at(i) + merkel.at(i+1);
        hash = hashing(word);
        merkel2.push_back(hash);
      }
    } else {
      for (unsigned i = 0; i < merkel.size() - 1; i+=2) {
        word = merkel.at(i) + merkel.at(i+1);
        hash = hashing(word);
        merkel2.push_back(hash);
      } 

      word = merkel.at(merkel.size()-1);
      hash = hashing(word);
      merkel2.push_back(hash);
    }

    merkel.clear();
    merkel = merkel2;
    merkel2.clear();
  }

  return merkel.at(0);
}