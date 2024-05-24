#include <iostream>
#include <string>
using namespace std;

struct Item
{
  int id;
  string name;
  int stock;
  double price;
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
  double totalHarga;
  Orderan *next;
};

struct HashNode
{
  string key;
  Orderan *value;
  HashNode *next;
};

Orderan *head = nullptr, *tail = nullptr, *cur = nullptr, *newNode = nullptr, *del = nullptr;
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

void searchOrderan(string key, HashNode **hashTable)
{
  int index = convertStrToInt(key);
  HashNode *temp = hashTable[index];
  while (temp != nullptr)
  {
    if (temp->key == key)
    {
      cout << temp->key << " " << temp->value->orderProduk << " " << temp->value->jmlhOrderProduk << " " << temp->value->totalHarga << endl;
      return;
    }
    temp = temp->next;
  }
  cout << "Produk tidak ditemukan" << endl;
}

TreeNode *createNode(int id, string name, int stock, double price)
{
  TreeNode *newNode = new TreeNode();
  newNode->item.id = id;
  newNode->item.name = name;
  newNode->item.stock = stock;
  newNode->item.price = price;
  newNode->left = newNode->right = nullptr;
  return newNode;
}

void insertTree(TreeNode **root, int id, string name, int stock, double price)
{
  if (*root == nullptr)
  {
    *root = createNode(id, name, stock, price);
  }
  else if (id < (*root)->item.id)
  {
    insertTree(&((*root)->left), id, name, stock, price);
  }
  else
  {
    insertTree(&((*root)->right), id, name, stock, price);
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

void insertItem(int id, string name, int stock, double price, string category)
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
  insertTree(&(categoryNode->itemsTree), id, name, stock, price);
}

void showTree(TreeNode *root)
{
  if (root != nullptr)
  {
    showTree(root->left);
    cout << "ID: " << root->item.id << ", Name: " << root->item.name << ", Stock: " << root->item.stock << ", Price: " << root->item.price << endl;
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

TreeNode *findItemByName(TreeNode *root, string name)
{
  if (root == nullptr)
  {
    return nullptr;
  }
  if (root->item.name == name)
  {
    return root;
  }
  TreeNode *leftResult = findItemByName(root->left, name);
  if (leftResult != nullptr)
  {
    return leftResult;
  }
  return findItemByName(root->right, name);
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
      cout << "Nama Barang saat ini: " << itemNode->item.name << endl;
      cout << "Stok saat ini: " << itemNode->item.stock << endl;
      cout << "Harga saat ini: " << itemNode->item.price << endl;
      cout << "Masukkan nama baru: ";
      cin >> itemNode->item.name;
      cout << "Masukkan stok baru: ";
      cin >> itemNode->item.stock;
      cout << "Masukkan harga baru: ";
      cin >> itemNode->item.price;

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

void insertOrderan(string namaPemesan, string namaProduk, int jumlahOrder, double harga, HashNode **hashTable)
{
  newNode = new Orderan();
  newNode->name = namaPemesan;
  newNode->orderProduk = namaProduk;
  newNode->jmlhOrderProduk = jumlahOrder;
  newNode->totalHarga = jumlahOrder * harga;
  newNode->next = nullptr;
  if (head == nullptr)
  {
    head = newNode;
    tail = newNode;
    cout << "Berhasil membuat head" << endl;
  }
  else
  {
    tail->next = newNode;
    tail = newNode;
    cout << "Berhasil menambah tail" << endl;
  }
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
      cout << cur->name << " " << cur->orderProduk << " " << cur->jmlhOrderProduk << " " << cur->totalHarga << endl;
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
      cout << temp->key << " " << temp->value->orderProduk << " " << temp->value->jmlhOrderProduk << " " << temp->value->totalHarga << endl;
      temp = temp->next;
    }
  }
}

void populateDatabase()
{
  // Jaket
  insertItem(generateID("Jaket Kulit", "Jaket"), "Jaket Kulit", 5, 150000, "Jaket");
  insertItem(generateID("Jaket Jeans", "Jaket"), "Jaket Jeans", 5, 200000, "Jaket");

  // Celana
  insertItem(generateID("Celana Jeans", "Celana"), "Celana Jeans", 5, 100000, "Celana");
  insertItem(generateID("Celana Chino", "Celana"), "Celana Chino", 5, 120000, "Celana");

  // Kemeja
  insertItem(generateID("Kemeja Flanel", "Kemeja"), "Kemeja Flanel", 5, 130000, "Kemeja");
  insertItem(generateID("Kemeja Batik", "Kemeja"), "Kemeja Batik", 5, 170000, "Kemeja");

  // Sepatu
  insertItem(generateID("Sepatu Sneakers", "Sepatu"), "Sepatu Sneakers", 5, 250000, "Sepatu");
  insertItem(generateID("Sepatu Formal", "Sepatu"), "Sepatu Formal", 5, 300000, "Sepatu");

  // Dress
  insertItem(generateID("Dress Pesta", "Dress"), "Dress Pesta", 5, 350000, "Dress");
  insertItem(generateID("Dress Casual", "Dress"), "Dress Casual", 5, 200000, "Dress");
}

void processOrderan(HashNode **hashTable)
{
  if (head == nullptr)
  {
    cout << "Tidak ada orderan yang perlu diproses." << endl;
    return;
  }

  Orderan *currentOrder = head;
  cout << "Orderan Teratas:" << endl;
  cout << "Nama Pemesan: " << currentOrder->name << endl;
  cout << "Produk: " << currentOrder->orderProduk << endl;
  cout << "Jumlah: " << currentOrder->jmlhOrderProduk << endl;
  cout << "Total Harga: " << currentOrder->totalHarga << endl;

  int choice;
  cout << "1. Accept Orderan" << endl;
  cout << "2. Reject Orderan" << endl;
  cout << "Pilih opsi: ";
  cin >> choice;

  switch (choice)
  {
  case 1:
    // Accept order
    insertHash(currentOrder->name, currentOrder, hashTable);
    cout << "Orderan diterima dan dimasukkan ke dalam hash table." << endl;

    // Remove from queue
    head = head->next;
    if (head == nullptr)
    {
      tail = nullptr;
    }
    break;
  case 2:
    // Reject order
    cout << "Orderan ditolak." << endl;

    // Remove from queue
    head = head->next;
    if (head == nullptr)
    {
      tail = nullptr;
    }
    break;
  default:
    cout << "Opsi tidak valid." << endl;
    break;
  }
}

TreeNode *searchItemByName(string name)
{
  CategoryNode *currentCategory = categories;
  while (currentCategory != nullptr)
  {
    TreeNode *foundItem = findItemByName(currentCategory->itemsTree, name);
    if (foundItem != nullptr)
    {
      return foundItem;
    }
    currentCategory = currentCategory->next;
  }
  return nullptr;
}

int main()
{
  HashNode *hashTable[hashTableSize] = {nullptr}; // Initialize hash table

  // Populate database with initial items
  populateDatabase();

  int pilihMenuUtama;
  do
  {
    cout << "-------------------------" << endl;
    cout << "Selamat datang di Aplikasi" << endl;
    cout << "-------------------------" << endl;
    cout << "1. Masuk Sebagai Admin" << endl;
    cout << "2. Masuk Sebagai User" << endl;
    cout << "0. Exit" << endl;
    cout << ">";
    cin >> pilihMenuUtama;
    switch (pilihMenuUtama)
    {
    case 1:
    {
      int pilihMenuAdmin;
      do
      {
        cout << "-------------------" << endl;
        cout << "    MENU ADMIN     " << endl;
        cout << "-------------------" << endl;
        cout << "1. Tambah Kategori" << endl;
        cout << "2. Tambah Item" << endl;
        cout << "3. Edit Item" << endl;
        cout << "4. Hapus Item" << endl;
        cout << "5. Tampilkan Data Item" << endl;
        cout << "6. Proses Orderan" << endl;
        cout << "7. Tampilkan Semua Orderan" << endl;
        cout << "0. Exit" << endl;
        cout << ">";
        cin >> pilihMenuAdmin;
        switch (pilihMenuAdmin)
        {
        case 1:
        {
          string namaKategori;
          cout << "Masukkan nama kategori: ";
          cin >> namaKategori;
          insertItem(-1, "", 0, 0, namaKategori); // Add empty item to create the category
          cout << "Kategori berhasil ditambahkan." << endl;
          break;
        }
        case 2:
        {
          string namaItem, kategoriItem;
          int stokItem;
          double hargaItem;
          cout << "Masukkan nama item: ";
          cin >> namaItem;
          cout << "Masukkan stok item: ";
          cin >> stokItem;
          cout << "Masukkan harga item: ";
          cin >> hargaItem;
          cout << "Masukkan kategori item: ";
          cin >> kategoriItem;
          int idItem = generateID(namaItem, kategoriItem);
          insertItem(idItem, namaItem, stokItem, hargaItem, kategoriItem);
          cout << "Item berhasil ditambahkan." << endl;
          break;
        }
        case 3:
        {
          int idItem;
           cout << "Masukkan ID item yang ingin diedit: ";
          cin >> idItem;
          editDataBarang(idItem);
          break;
        }
        case 4:
          // Implementasi Hapus Item
          break;
        case 5:
        {
          string kategori;
          cout << "Masukkan nama kategori: ";
          cin >> kategori;
          showCategoryItems(kategori);
          break;
        }
        case 6:
          processOrderan(hashTable);
          break;
        case 7:
          showOrderan();
          break;
        default:
          break;
        }
      } while (pilihMenuAdmin != 0);

      break;
    }
    case 2:
    {
      int pilihMenuUser;
      do
      {
        cout << "-------------------" << endl;
        cout << "    MENU USER     " << endl;
        cout << "-------------------" << endl;
        cout << "1. Tampilkan Item" << endl;
        cout << "2. Cari Item Berdasarkan Nama" << endl;
        cout << "3. Buat Orderan" << endl;
        cout << "4. Tampilkan Orderan Saya" << endl;
        cout << "0. Exit" << endl;
        cout << ">";
        cin >> pilihMenuUser;
        switch (pilihMenuUser)
        {
        case 1:
        {
          listCategories();
          break;
        }
        case 2:
        {
          string namaItem;
          cout << "Masukkan nama item: ";
          cin >> namaItem;
          TreeNode *foundItem = searchItemByName(namaItem);
          if (foundItem != nullptr)
          {
            cout << "ID: " << foundItem->item.id << ", Name: " << foundItem->item.name << ", Stock: " << foundItem->item.stock << ", Price: " << foundItem->item.price << endl;
          }
          else
          {
            cout << "Item not found." << endl;
          }
          break;
        }
        case 3:
        {
          string namaPemesan, namaProduk;
          int jumlahOrder;
          cout << "Masukkan nama pemesan: ";
          cin >> namaPemesan;
          cout << "Masukkan nama produk: ";
          cin >> namaProduk;
          cout << "Masukkan jumlah order: ";
          cin >> jumlahOrder;

          // Search for the item by name
          TreeNode *foundItem = searchItemByName(namaProduk);
          if (foundItem != nullptr)
          {
            // Calculate total price
            double totalPrice = foundItem->item.price * jumlahOrder;
            // Insert order
            insertOrderan(namaPemesan, namaProduk, jumlahOrder, totalPrice, hashTable);
          }
          else
          {
            cout << "Produk tidak ditemukan." << endl;
          }
          break;
        }
        case 4:
        {
          string namaPemesan;
          cout << "Masukkan nama pemesan: ";
          cin >> namaPemesan;
          searchOrderan(namaPemesan, hashTable);
          break;
        }
        default:
          break;
        }
      } while (pilihMenuUser != 0);
      break;
    }
    }
  } while (pilihMenuUtama != 0);

  return 0;
}