# Blokų grandinių 2 užduotis
Centralizuota blokų grandinė
## Įdiegimo ir naudojimosi instrukcija (naudota ubuntu diegiant Libbitcoin biblioteką)
- git clone https://github.com/Matas-V/blockchain/tree/v0.3.1
- Įsirašyti Libbitcoin biblioteką
- Atsidaryti projektą VS code aplinkoje
- Konsolėje (WSL) parašyti komandą *g++ -std=c++11 -o main main.cpp $(pkg-config --cflags --libs libbitcoin-system)*
- Konsolėje (WSL) paleisti sukompiliuotą main tipo programą su komanda *./main*
## Kaip veikia programa?
- Sugeneruojama 1000 vartotojų
- Sugeneruojama 10000 transakcijų
- Išrenkama random 100 transakcijų bei sukuriamas merkle tree paprastuoju būdu (sudedant visų bloko transakcijų id ir gaunamas hash'as).
- Vyksta bloko kasimas, kol gaunamas reikalavimus atitinkantis hash'as
- Naujame bloke įvykdomos transakcijos
- Eliminuojamos įvykdytos transakcijos
- Naujai iškastas blokas pridedas prie pagrindinės grandinės (blockchain)
## Ką reikėjo pakeisti kode, kad veiktų Libbitcoin create-merkle funkcija?
- Include'inti <bitcoin/system.hpp> naują biblioteką bei tinkamai ją susikonfiguruoti.
- Susikurti naujus kintamuosius pritaikytus create-merkle funkcijai.
- Konvertuoti string tipo tranksakcijos ID į char, kad šis galėtų būti pridėtas į hash_list tipo kintamąjį.

## Naudingos nuorodos diegiant Libbitcoin biblioteką:
- https://github.com/libbitcoin/libbitcoin-system/tree/version3#debianubuntu
- https://bitcoin.stackexchange.com/questions/90555/was-libbitcoin-a-part-of-bitcoin-core-in-the-past
