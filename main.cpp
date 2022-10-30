#include "headers.h"
#include "generate.cpp"

int main() {
  int b = 0, r = 10000;
  bool stop = 0;
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

  for (int i=0; i<5; i++) {
    blockChain newBC;
    while (newBC.block.transactions.size() != 100 && trans.size()) {
      uniform_int_distribution<> distr(0, trans.size()-1);
      int random = distr(gen), j;

      for (j = 0; j < 1000; j++)
        if (trans.at(random).sender == users.at(j).public_key)
          break;

      if (trans.at(random).sum <= users.at(j).balance &&
        trans.at(random).transactionId == hashing(trans.at(random).sender + trans.at(random).receiver + to_string(trans.at(random).sum))
      ) {
        newBC.block.transactions.push_back(trans.at(random));
        trans.erase(trans.begin()+random);
      } else {
        trans.erase(trans.begin()+random);
      }
    }
    bc.push_back(newBC);
    b++;
  }

  vector<int> set{0,1,2,3,4};
  int next = 0, limit = 20000, mined = 0;
  while (!stop) {
    uniform_int_distribution<> distr(0, set.size()-1);
    int rand = distr(gen), r;
    r = set.at(rand);
    set.erase(set.begin()+rand);

    if (next == 0) bc.at(next).block.hash = mineBlock(bc.at(next), "", next, limit);
    else bc.at(next).block.hash = mineBlock(bc.at(next), bc.at(next-1).block.hash, next, limit);

    if (bc.at(next).block.hash.length() > 1) {
      mined++;
      cout << "new hash : " << bc.at(next).block.hash << endl;
      cout << "prev hash: " << bc.at(next).prevHash << endl << endl; 

      for (auto tran : bc.at(next).block.transactions) {
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

      next++;
      if (mined == 5)
        stop = 1;
    } else {
      if (mined > 0)
        stop = 1;
      else {
        limit *= 2;
        mined = 0;
        next = 0;
      }
    }
  }

  usersData(users);

  for (int i=0; i<5; i++)
    if (bc.at(i).block.hash != "0")
      printBlock(bc.at(i));

  cout << "MINED " << mined << endl;
  cout << "LIMIT " << limit << endl;

  return 0;
}