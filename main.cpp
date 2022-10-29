#include "headers.h"
#include "generate.cpp"

int main() {
  int b = 0, r = 10000;
  string temp;
  vector<user> users;
  vector<transaction> trans;
  vector<blockChain> bc;
  bc.reserve(100);
  random_device rd;
  mt19937 gen(rd());

  // generateUsers(users);
  // generateTransactions(trans, users);
  // printTrans(trans.at(99));
  readUsers(users);
  readTrans(trans);

  while (trans.size() >= 9500) {
    blockChain newBC;
    while (newBC.block.transactions.size() != 100 && trans.size()) {
      uniform_int_distribution<> distr(0, trans.size()-1);
      int random = distr(gen), j;

      for (j = 0; j < 1000; j++)
        if (trans.at(random).sender == users.at(j).public_key)
          break;

      if (trans.at(random).sum <= users.at(j).balance) {
        newBC.block.transactions.push_back(trans.at(random));
        trans.erase(trans.begin()+random);
      } else {
        trans.erase(trans.begin()+random);
      }
    }
    b++;
    if (b==1) newBC.block.hash = mineBlock(newBC, "", b, r);
    else newBC.block.hash = mineBlock(newBC, bc.at(b-2).block.hash, b, r);

    bc.push_back(newBC);
    cout << "new hash : " << bc.at(b-1).block.hash << endl;
    cout << "prev hash:  " << bc.at(b-1).prevHash << endl; 

    for (auto tran : bc.at(b-1).block.transactions) {
      int send = 0, get = 0;
      for (int i = 0; i < 1000; i++) {
				if (users.at(i).public_key == tran.sender)
					send = i;
				else if (users.at(i).public_key == tran.receiver)
					get = i;
				if (send != 0 && get != 0)
					break;
			}

      users.at(send).balance -= tran.sum;
      users.at(get).balance += tran.sum;
    }
  }

  usersData(users);
  printBlock(bc.at(3));

  return 0;
}