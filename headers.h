#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <algorithm>
#include <iomanip>
#include <bitset>
#include <sstream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
using namespace std;

const int MIN_VALUE = 1000;
const int MAX_VALUE = 1000000;

struct user {
  string name;
  string public_key;
  double balance;
};

struct transaction {
  string transactionId = "";
  string sender;
  string receiver;
  double sum;
};

struct block {
  string hash;
  vector<transaction> transactions;
};

struct blockChain {
	string prevHash;
	int timestamp;
	string version;
	string merkelRoot;
	int nonce;
	string diff;
	block block;
};


void generateUsers(vector<user> &users);
void generateTransactions(vector<transaction> &trans, vector<user> &users);
string mineBlock(blockChain &bc, string prevHash, int b, int n);
void readUsers(vector<user> &con);
void readTrans(vector<transaction> &con);
void usersData(vector<user> users);
void printBlock(blockChain bc);
void printTrans(transaction t);
string generateMerkleRoot(vector<transaction> trans);