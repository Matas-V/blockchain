# Blokų grandinių 2 užduotis
Centralizuota blokų grandinė. Programoje blokų kasimas vykdomas lygiagrečiai, t.y. vienu metu 5 skirtingi thread'ai ieško greičiau atitinkamo hash'o. Kiekvienam thread'as turi skirtingą nonce reikšmę nuo kurios pradeda skaičiavimus. Suradus naują bloką, visi thread'ai baigia darbą bei pradeda kasti naują bloką.
## Įdiegimo ir naudojimosi instrukcija
- git clone https://github.com/Matas-V/blockchain.git
- Atsidaryti projektą VS code aplinkoje
- Konsolėje (CMD) parašyti komandą *make main*
- Konsolėje (CMD) paleisti sukompiliuotą main.exe tipo programą su komanda *main*
## Kaip veikia programa?
- Sugeneruojama 1000 vartotojų
- Sugeneruojama 10000 transakcijų
- Išrenkama random 100 transakcijų bei sukuriamas merkle tree
- Vyksta bloko kasimas, kol gaunamas reikalavimus atitinkantis hash'as
- Naujame bloke esančios transakcijos įvykdomos
- Eliminuojamos įvykdytos transakcijos
- Naujai iškastas blokas pridedamas prie pagrindinės grandinės (blockchain)
## Programos rezultato pavyzdys:
![mining2](https://user-images.githubusercontent.com/80633152/199516508-2fa3173b-6d86-407f-b1ae-4c1a620a21a5.png)
