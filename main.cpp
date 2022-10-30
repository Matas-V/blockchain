#include "headers.h"
#include "generate.cpp"

#define THREAD_NUM 5

int main() {
  int r = 10000;
  bool stop = 0;
  string temp;
  vector<user> users;
  vector<transaction> trans;
  vector<blockChain> bc;
  bc.reserve(100);
  random_device rd;
  mt19937 gen(rd());

  omp_set_num_threads(THREAD_NUM);

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
  }

  int limit = 20000, mined = 0;

  #pragma omp parallel
  {
    bool stop=0;
    while(!stop) {
      bc.at(omp_get_thread_num()).block.hash = mineBlock(bc.at(omp_get_thread_num()), "", 0, limit);

      if (bc.at(omp_get_thread_num()).block.hash.length() > 1) {
        mined++;
        cout << "new hash : " << bc.at(omp_get_thread_num()).block.hash << endl;
        cout << "prev hash: " << bc.at(omp_get_thread_num()).prevHash << endl << endl; 

        for (auto tran : bc.at(omp_get_thread_num()).block.transactions) {
          int send = 0, get = 0;
          for (int i = 0; i < 1000; i++) {
            if (users.at(i).public_key == tran.sender)
              send = i;
            else if (users.at(i).public_key == tran.receiver)
              get = i;
            if (send != 0 && get != 0)
              break;
          }

          if (users.at(send).balance >= tran.sum) {
            users.at(send).balance -= tran.sum;
            users.at(get).balance += tran.sum;
          }
        }
        stop = 1;
      } else 
        limit *= 2;
    }
    printBlock(bc.at(omp_get_thread_num()));
  }

  usersData(users);
  cout << "MINED " << mined << endl;
  cout << "LIMIT " << limit << endl;

  return 0;
}