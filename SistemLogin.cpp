#include <iostream>
#include <string>
using namespace std;

struct Node
{
  string username;
  string password;
  Node *next;
};

struct HashTable
{
  Node *head;
} userHashTable[26], adminHashTable[26];

int hashFunction(const string &key)
{
  int sum = 0;
  for (char ch : key)
  {
    sum += ch;
  }
  return sum % 26;
}

bool cekdaftar(HashTable hashTable[], const string &username, const string &password)
{
  // Gabungkan username dan password untuk membuat kunci unik
  string key = username + password;
  int index = hashFunction(key);

  Node *current = hashTable[index].head;
  while (current != nullptr)
  {
    if (current->username == username && current->password == password)
    {
      cout << "User sudah ada!!" << endl;
      return false;
    }
    current = current->next;
  }

  Node *newNode = new Node();
  newNode->username = username;
  newNode->password = password;
  newNode->next = hashTable[index].head;
  hashTable[index].head = newNode;
  cout << "User berhasil didaftarkan!!" << endl;
  return true;
}

void viewDaftar(HashTable hashTable[])
{
  string username, password;
  cout << "----------------------------------" << endl;
  cout << "|           MENU DAFTAR          |" << endl;
  cout << "----------------------------------" << endl;
  cout << " Masukkan username dan password" << endl;
  cout << "Username: ";
  cin >> username;
  cout << "\nPassword: ";
  cin >> password;

  if (cekdaftar(hashTable, username, password))
  {
    cout << "Akun berhasil terdaftar!!" << endl;
  }
  else
  {
    cout << "Akun gagal terdaftar!!" << endl;
  }
}

bool cekLogin(HashTable hashTable[], const string &username, const string &password)
{
  // Gabungkan username dan password untuk membuat kunci unik
  string key = username + password;
  int index = hashFunction(key);

  Node *current = hashTable[index].head;
  while (current != nullptr)
  {
    if (current->username == username && current->password == password)
    {
      return true;
    }
    current = current->next;
  }
  return false;
}

void viewLogin(HashTable hashTable[])
{
  system("cls");
  string username, password;
  cout << "----------------------------------" << endl;
  cout << "|           MENU LOGIN           |" << endl;
  cout << "----------------------------------" << endl;
  cout << "  Masukkan username dan password  " << endl;
  cout << "Username: ";
  cin >> username;
  cout << "\nPassword: ";
  cin >> password;
  cout << "----------------------------------" << endl;

  if (cekLogin(hashTable, username, password))
  {
    cout << "Login berhasil!" << endl;
  }
  else
  {
    cout << "Username atau password salah!" << endl;
  }
}

void viewUser()
{
  int pilih;
  do
  {
    cout << "\n-------------------------" << endl;
    cout << "|   MASUK SEBAGAI USER  |" << endl;
    cout << "-------------------------" << endl;
    cout << "| 1. Daftar Akun        |" << endl;
    cout << "| 2. Login              |" << endl;
    cout << "| 3. Exit               |" << endl;
    cout << "-------------------------" << endl;
    cout << "Pilih: ";
    cin >> pilih;
    cout << "-------------------------" << endl;
    switch (pilih)
    {
    case 1:
      viewDaftar(userHashTable);
      break;
    case 2:
      viewLogin(userHashTable);
      break;
    case 3:
      cout << "Exit" << endl;
      break;
    }
  } while (pilih != 3);
}

void viewAdmin()
{
  int pilih;
  do
  {
    cout << "\n-------------------------" << endl;
    cout << "|   MASUK SEBAGAI ADMIN |" << endl;
    cout << "-------------------------" << endl;
    cout << "| 1. Daftar Akun        |" << endl;
    cout << "| 2. Login              |" << endl;
    cout << "| 3. Exit               |" << endl;
    cout << "-------------------------" << endl;
    cout << "Pilih: ";
    cin >> pilih;
    cout << "-------------------------" << endl;
    switch (pilih)
    {
    case 1:
      viewDaftar(adminHashTable);
      break;
    case 2:
      viewLogin(adminHashTable);
      break;
    case 3:
      cout << "Exit" << endl;
      break;
    }
  } while (pilih != 3);
}

int main()
{
  int pilih;
  do
  {
    cout << "\n-------------------------" << endl;
    cout << "|   PILIH JENIS AKUN    |" << endl;
    cout << "-------------------------" << endl;
    cout << "| 1. Admin               |" << endl;
    cout << "| 2. User              |" << endl;
    cout << "| 3. Exit               |" << endl;
    cout << "-------------------------" << endl;
    cout << "Pilih: ";
    cin >> pilih;
    cout << "-------------------------" << endl;
    switch (pilih)
    {
    case 1:
      viewAdmin();
      break;
    case 2:
      viewUser();
      break;
    case 3:
      cout << "Exit" << endl;
      break;
    }
  } while (pilih != 3);

  return 0;
}
