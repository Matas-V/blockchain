#include "headers.h"
#include "generate.cpp"

int main() {
  int b = 0, r = 10000;
  string temp;
  vector<user> users;
  vector<transaction> trans;
  vector<blockChain> bc;

  generateUsers(users);
  generateTransactions(trans, users);

  while (trans.size()) {
    random_device rd;
    mt19937 gen(rd());
    while (bc.at(b).blocks.transactions.size() != 100 && trans.size()) {
      uniform_int_distribution<> distr(0, trans.size());
      int random = distr(gen), j;

      for (j = 0; j < 1000; j++)
        if (trans.at(random).sender == users.at(j).public_key)
          break;

      if (trans.at(random).sum <= users.at(j).balance) {
        bc.at(b).blocks.transactions.push_back(trans.at(random));
        trans.erase(trans.begin()+random-1);
      } else {
        trans.erase(trans.begin()+random-1);
      }
    }

    if (bc.at(b).blocks.transactions.size() == 100)
      b++;

    temp = "0";
    while (temp == "0") {
			for (int i = 0; i < 5; i++) {
				temp = mineBlock(bc, b, r);
			}
      r *= 2;
		}
    cout << temp << endl;

    for (auto tran : bc.at(b).blocks.transactions) {
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

  return 0;
}