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
	block blocks;
};


void generateUsers(vector<user> &users);
void generateTransactions(vector<transaction> &trans, vector<user> &users);
string mineBlock(vector<blockChain> bc, int b, int &n);