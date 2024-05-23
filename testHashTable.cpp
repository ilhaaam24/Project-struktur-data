#include <iostream>
#include <string>

using namespace std;

struct Item
{
  int id;
  string name;
  int stock;
};

struct TreeNode
{
  Item item;
  TreeNode *left;
  TreeNode *right;
};

struct CategoryNode
{
  string name;
  TreeNode *itemsTree;
  CategoryNode *next;
};

struct Orderan
{
  int IDorder;
  int jmlhOrderProduk;
  string name;
  string orderProduk;
  long totalHarga;
  Orderan *next;
};

struct HashNode
{
  string key;
  Orderan *value;
  HashNode *next;
};

Orderan *head, *tail, *cur, *newNode, *del;
int maksimalOrder = 2;
const int hashTableSize = 10;
CategoryNode *categories = nullptr;

int generateID(string name, string category)
{
  int id = 0;
  int digit1 = toupper(name[0]) - 'A' + 1;
  int digit2 = toupper(name[name.length() - 1]) - 'A' + 1;
  id = (digit1 + digit2) % 10 * 1000;
  int digit3 = name.length();
  int digit4;
  if (category == "Men")
  {
    digit4 = 1;
  }
  else if (category == "Women")
  {
    digit4 = 2;
  }
  else if (category == "Kids")
  {
    digit4 = 3;
  }
  id = id + digit3 * 10 + digit4;
  return id;
}

int convertStrToInt(string str)
{
  int length = str.length();
  int num = 0;
  for (char ch : str)
  {
    num += ch;
  }
  return num % hashTableSize;
}

void insertHash(string key, Orderan *value, HashNode **hashTable)
{
  int index = convertStrToInt(key);
  HashNode *newNode = new HashNode();
  newNode->key = key;
  newNode->value = value;
  newNode->next = nullptr;

  if (hashTable[index] == nullptr)
  {
    hashTable[index] = newNode;
  }
  else
  {
    HashNode *temp = hashTable[index];
    while (temp->next != nullptr)
    {
      temp = temp->next;
    }
    temp->next = newNode;
  }
}

TreeNode *createNode(int id, string name, int stock)
{
  TreeNode *newNode = new TreeNode();
  newNode->item.id = id;
  newNode->item.name = name;
  newNode->item.stock = stock;
  newNode->left = newNode->right = nullptr;
  return newNode;
}

void insertTree(TreeNode **root, int id, string name, int stock)
{
  if (*root == nullptr)
  {
    *root = createNode(id, name, stock);
  }
  else if (id < (*root)->item.id)
  {
    insertTree(&((*root)->left), id, name, stock);
  }
  else
  {
    insertTree(&((*root)->right), id, name, stock);
  }
}

CategoryNode *findCategory(string category)
{
  CategoryNode *current = categories;
  while (current != nullptr)
  {
    if (current->name == category)
    {
      return current;
    }
    current = current->next;
  }
  return nullptr;
}

void insertItem(int id, string name, int stock, string category)
{
  CategoryNode *categoryNode = findCategory(category);
  if (categoryNode == nullptr)
  {
    categoryNode = new CategoryNode();
    categoryNode->name = category;
    categoryNode->itemsTree = nullptr;
    categoryNode->next = categories;
    categories = categoryNode;
  }
  insertTree(&(categoryNode->itemsTree), id, name, stock);
}

void showTree(TreeNode *root)
{
  if (root != nullptr)
  {
    showTree(root->left);
    cout << "ID: " << root->item.id << ", Name: " << root->item.name << ", Stock: " << root->item.stock << endl;
    showTree(root->right);
  }
}

void showCategoryItems(string category)
{
  CategoryNode *categoryNode = findCategory(category);
  if (categoryNode != nullptr)
  {
    cout << "Items in category '" << category << "':" << endl;
    showTree(categoryNode->itemsTree);
  }
  else
  {
    cout << "Category '" << category << "' not found." << endl;
  }
}

void listCategories()
{
  int i = 1;
  CategoryNode *current = categories;
  cout << "\nCategories:" << endl;
  while (current != nullptr)
  {
    cout << i++ << ". " << current->name << endl;
    current = current->next;
  }
  cout << endl;
}

TreeNode *findItem(TreeNode *root, int id)
{
  if (root == nullptr || root->item.id == id)
  {
    return root;
  }
  else if (id < root->item.id)
  {
    return findItem(root->left, id);
  }
  else
  {
    return findItem(root->right, id);
  }
}

void editDataBarang(int id)
{
  bool itemFound = false;
  CategoryNode *categoryNode = categories;

  while (categoryNode != nullptr && !itemFound)
  {
    TreeNode *itemNode = findItem(categoryNode->itemsTree, id);
    if (itemNode != nullptr)
    {
      itemFound = true;
      cout << "ID: " << itemNode->item.id << endl;
      cout << "Nama Barang saat ini: " ;
      cout << itemNode->item.name << endl;
      cout << "Stok saat ini: " ;
      cout << itemNode->item.stock << endl;
      cout << "Masukkan nama baru: ";
      cin >> itemNode->item.name;
      cout << "Masukkan stok baru: ";
      cin >> itemNode->item.stock;

      cout << "Data barang telah berhasil diupdate." << endl;

      itemNode->item.id = generateID(itemNode->item.name, categoryNode->name);
    }
    categoryNode = categoryNode->next;
  }

  if (!itemFound)
  {
    cout << "Barang dengan ID " << id << " tidak ditemukan." << endl;
  }
}

int countOrder()
{
  if (head == nullptr)
  {
    return 0;
  }
  else
  {
    int count = 0;
    cur = head;
    while (cur != nullptr)
    {
      cur = cur->next;
      count++;
    }
    return count;
  }
}

bool isEmptyOrder()
{
  if (countOrder() == 0)
  {
    return true;
  }
  return false;
}

void insertOrderan(string namaPemesan, string namaProduk, int jumlahOrder, HashNode **hashTable)
{
  newNode = new Orderan();
  newNode->name = namaPemesan;
  newNode->orderProduk = namaProduk;
  newNode->jmlhOrderProduk = jumlahOrder;
  newNode->next = nullptr;
  if (isEmptyOrder())
  {
    head = newNode;
    tail = newNode;
    cout << "berhasil membuat head" << endl;
  }
  else
  {
    tail->next = newNode;
    tail = newNode;
    cout << "berhasil menambah tail" << endl;
  }
  insertHash(namaPemesan, newNode, hashTable);
}

void showOrderan()
{
  cur = head;
  if (cur == nullptr)
  {
    cout << "Orderan masih kosong" << endl;
  }
  else
  {
    cout << "Daftar Orderan" << endl;
    while (cur != nullptr)
    {
      cout << cur->name << " " << cur->orderProduk << " " << cur->jmlhOrderProduk << endl;
      cur = cur->next;
    }
  }
}

void showhash(HashNode **hashTable)
{
  HashNode *temp = nullptr;
  for (int i = 0; i < hashTableSize; i++)
  {
    temp = hashTable[i];
    cout << "index: " << i << endl;
    while (temp != nullptr)
    {
      cout << temp->key << " " << temp->value->orderProduk << " " << temp->value->jmlhOrderProduk << endl;
      temp = temp->next;
    }
  }
}

int main()
{
  HashNode *hashTable[hashTableSize] = {nullptr}; // Initialize hash table

  string kategori;
  int pilihMenu;
  do
  {
    cout << "1. Input Barang" << endl;
    cout << "2. Edit Data Barang" << endl;
    cout << "3. Delete data" << endl;
    cout << "4. Display Data" << endl;
    cout << "5. Input Orderan" << endl;
    cout << "6. Show Orderan" << endl;
    cout << "7. Convert string to int" << endl;
    cout << "8. Show Hash Table" << endl;
    cout << "0. exit" << endl;
    cout << ">";
    cin >> pilihMenu;
    switch (pilihMenu)
    {
    case 1:
    {
      int ID, stok;
      string namaBarang, kategori;
      cout << "Masukan nama barang : ";
      cin >> namaBarang;
      cout << "Masukan stok : ";
      cin >> stok;
      cout << "Masukan kategory : ";
      cin >> kategori;
      ID = generateID(namaBarang, kategori);
      insertItem(ID, namaBarang, stok, kategori);
      break;
    }
    case 2:
      cout << "Masukan ID yang ingin di edit : ";
      int id;
      cin >> id;
      editDataBarang(id);
      break;
    case 3:
      cout << "Masukan ID yang ingin di hapus : ";
      int id2;
      cin >> id2;
      break;
    case 4:
    {
      int pilih;
      listCategories();
      cout << ">";
      cin >> pilih;
      if (pilih == 1)
      {
        showCategoryItems("Men");
      }
      else if (pilih == 2)
      {
        showCategoryItems("Women");
      }
      else if (pilih == 3)
      {
        showCategoryItems("Kids");
      }
      break;
    }
    case 5:
    {
      string namaPemesan;
      string orderProduk;
      int jumlah;
      cout << "Nama Pemesan: ";
      cin >> namaPemesan;
      cout << "Nama Produk: ";
      cin >> orderProduk;
      cout << "Jumlah: ";
      cin >> jumlah;
      insertOrderan(namaPemesan, orderProduk, jumlah, hashTable);
      break;
    }
    case 6:
      showOrderan();
      break;
    case 7:
    {
      int result = convertStrToInt("Arif");
      cout << result << endl;
      break;
    }
    case 8:
    {
      showhash(hashTable);
      break;
    }
    default:
      cout << "Pilih menu yang tersedia." << endl;
      break;
    }
  } while (pilihMenu != 0);
  return 0;
}
