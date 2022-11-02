#include "headers.h"
#include "generate.cpp"

#define THREAD_NUM 5

int main() {
  int who;
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
    stop = 0;
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

  int limit = 50000, mined = 0;
  while(mined != 5) {
    stop = 0;
    #pragma omp parallel num_threads(THREAD_NUM) shared(mined, stop, bc)
    {
      int nonce = limit * omp_get_thread_num();
      string temp;
      bc.at(mined).version = "v" + to_string(mined+1) + ".0";
      bc.at(mined).diff = "000";
      bc.at(mined).merkelRoot = generateMerkleRoot(bc.at(mined).block.transactions);
      if (mined == 0) {
        for (int i=0; i<limit * omp_get_thread_num(); i++) {
          if (stop)
            break;
          temp = mineBlock(bc.at(mined), "", mined, nonce);
          if (temp.size() > 2) {
            bc.at(mined).block.hash = temp;
            stop = 1;
            bc.at(mined).nonce = nonce;
            who = omp_get_thread_num();
            break;
          }
          nonce++;
        }
      } else {
        for (int i=0; i<limit * omp_get_thread_num(); i++) {
          if (stop)
            break;
          temp = mineBlock(bc.at(mined), bc.at(mined-1).block.hash, mined, nonce);
          if (temp.size() > 2) {
            bc.at(mined).block.hash = temp;
            stop = 1;
            bc.at(mined).nonce = nonce;
            who = omp_get_thread_num();
            break;
          }
          nonce++;
        }
      }

      if (bc.at(mined).block.hash.length() > 1 && who == omp_get_thread_num()) {
        cout << "MINED BY " << who << " THREAD" << endl;
        for (auto tran : bc.at(mined).block.transactions) {
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
        printBlock(bc.at(mined), mined);
        mined++;
      }
    }
  }

  usersData(users);

  return 0;
}