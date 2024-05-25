// MENU ADMIN
//     -Tambah kategori ✅
//     -Tambah Item ✅
//     -Edit Item by ID > searching ✅
//     -Delete Item by ID ✅
//     -Tampilkan Data per kategori ✅
//     - Urutkan Data dari termurah or termahal > sorting
//     -Proses Orderan > queue
//     -Sinkronkan stok ketika orderan masuk dan update stok
//     -Riwayat ubah data or stok masuk ke dalam stack > stack

// MENU USER
//      -Tampilkan Data Item per kategori > sorting
//      - Order berdasarkan nama barang  > queue
//      - Dapat menampilkan riwayat orderan

// SISTEM LOGIN USER

//     -daftar
//     -Login
//     -Username & password > Hastable

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

struct HashNode
{
  string key;
  Item *value;
  HashNode *next;
};

struct CartNode
{
  Item item;
  int quantity;
  CartNode *next;
};

const int hashTableSize = 10;
CategoryNode *categories = nullptr;
CartNode *top = nullptr;

int generateIDItem(string name, string category)
{
  int id = 0;
  int digit1 = toupper(name[0]) - 'A' + 1;
  int digit2 = toupper(name[name.length() - 1]) - 'A' + 1;
  int digit3 = category.length();
  digit1 *= 1000;
  digit2 *= 100;
  digit3 *= 10;
  id = digit1 + digit2 + digit3;
  return id;
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

TreeNode *deleteMin(TreeNode *node)
{
  if (node->left == nullptr)
    return node->right;
  node->left = deleteMin(node->left);
  return node;
}

TreeNode *deleteNode(TreeNode *root, int id)
{
  if (root == nullptr)
    return nullptr;
  if (id < root->item.id)
  {
    root->left = deleteNode(root->left, id);
  }
  else if (id > root->item.id)
  {
    root->right = deleteNode(root->right, id);
  }
  else
  {
    if (root->left == nullptr)
      return root->right;
    if (root->right == nullptr)
      return root->left;
    TreeNode *temp = root;
    root = temp->right;
    while (root->left != nullptr)
      root = root->left;
    root->right = deleteMin(temp->right);
    root->left = temp->left;
    delete temp;
  }
  return root;
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
void showTree(TreeNode *root)
{
  if (root != nullptr)
  {
    showTree(root->left);
    cout << "ID: " << root->item.id << ", Barang: " << root->item.name << ", Stock: " << root->item.stock << ", Harga: " << root->item.price << endl;
    showTree(root->right);
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
CategoryNode *addCategory(string category)
{

  if (findCategory(category) != nullptr)
  {
    cout << "Kategori '" << category << " sudah ada." << endl;
    return nullptr;
  }
  else
  {
    CategoryNode *newCategory = new CategoryNode();
    newCategory->name = category;
    newCategory->itemsTree = nullptr;
    newCategory->next = categories;
    categories = newCategory;

    cout << "Kategori " << category << " berhasil ditambahkan :)" << endl;

    return newCategory;
  }
}

void showCategoryItems(string category)
{

  CategoryNode *categoryNode = findCategory(category);

  if (categoryNode != nullptr)
  {
    cout << "-------------------------------" << endl;
    cout << "\t" << category << endl;
    cout << "-------------------------------" << endl;
    showTree(categoryNode->itemsTree);
  }
  else
  {
    cout << "Kategori '" << category << "' tidak ditemukan." << endl;
  }
}
void showCategories()
{
  CategoryNode *current = categories;
  if (current == nullptr)
  {
    cout << "Tidak ada kategori yang ditemukan." << endl;
  }
  else
  {
    int i = 1;
    while (current != nullptr)
    {
      cout << i++ << ". " << current->name << endl;
      current = current->next;
    }
  }
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

void editItem(int id)
{
  CategoryNode *currentCategory = categories;
  bool itemFound = false;

  while (currentCategory != nullptr && !itemFound)
  {
    TreeNode *currentItem = currentCategory->itemsTree;
    while (currentItem != nullptr)
    {
      if (currentItem->item.id == id)
      {
        itemFound = true;
        bool keepEditing = true;

        while (keepEditing)
        {
          cout << "Pilih data yang ingin diubah : " << endl;
          cout << "1. Nama" << endl;
          cout << "2. Stock" << endl;
          cout << "3. Harga" << endl;
          cout << "0. Selesai" << endl;
          cout << ">";
          int pilih;
          cin >> pilih;

          if (pilih == 0)
          {
            keepEditing = false;
          }
          else if (pilih == 1)
          {
            string name;
            cout << "Masukkan Nama : ";
            cin.ignore();
            getline(cin, name);
            currentItem->item.name = name;
          }
          else if (pilih == 2)
          {
            int stock;
            cout << "Masukkan Stock : ";
            cin >> stock;
            currentItem->item.stock = stock;
          }
          else if (pilih == 3)
          {
            double price;
            cout << "Masukkan Harga : ";
            cin >> price;
            currentItem->item.price = price;
          }
          else
          {
            cout << "Pilihan tidak valid!" << endl;
            continue;
          }

          cout << "Data berhasil diubah!" << endl;
          currentItem->item.id = generateIDItem(currentItem->item.name, currentCategory->name);
          cout << "ID: " << currentItem->item.id << ", Name: " << currentItem->item.name << ", Stock: " << currentItem->item.stock << ", Price: " << currentItem->item.price << endl;

          if (keepEditing)
          {
            cout << "Apakah ingin mengubah data lagi? (y/n): ";
            char choice;
            cin >> choice;
            if (choice != 'y' && choice != 'Y')
            {
              keepEditing = false;
            }
          }
        }
        break;
      }
      currentItem = currentItem->right;
    }
    currentCategory = currentCategory->next;
  }

  if (!itemFound)
  {
    cout << "Item dengan ID " << id << " tidak ditemukan." << endl;
  }
}

void deleteItem(int id)
{
  CategoryNode *currentCategory = categories;
  bool itemFound = false;

  while (currentCategory != nullptr && !itemFound)
  {
    TreeNode *currentItem = currentCategory->itemsTree;
    while (currentItem != nullptr)
    {
      if (currentItem->item.id == id)
      {
        itemFound = true;
        currentCategory->itemsTree = deleteNode(currentCategory->itemsTree, id);
        cout << "Item dengan ID " << id << " berhasil dihapus." << endl;
        break;
      }
      currentItem = currentItem->right;
    }
    currentCategory = currentCategory->next;
  }

  if (!itemFound)
  {
    cout << "Item dengan ID " << id << " tidak ditemukan." << endl;
  }
}
TreeNode *findItemById(TreeNode *root, int id)
{
  if (root == nullptr)
    return nullptr;
  if (root->item.id == id)
    return root;
  if (id < root->item.id)
    return findItemById(root->left, id);
  else
    return findItemById(root->right, id);
}
TreeNode *findItemByIdInCategories(int id)
{
  CategoryNode *currentCategory = categories;
  TreeNode *currentItem = nullptr;
  while (currentCategory != nullptr)
  {
    currentItem = findItemById(currentCategory->itemsTree, id);
    if (currentItem != nullptr)
    {
      break;
    }
    currentCategory = currentCategory->next;
  }
  return currentItem;
}
void pushCart(int id)
{
  TreeNode *itemNode = findItemByIdInCategories(id);
  if (itemNode == nullptr)
  {
    CartNode *newCartNode = new CartNode();
    newCartNode->item = itemNode->item;
    newCartNode->next = top;
    top = newCartNode;
    cout << "Item '" << newCartNode->item.name << "' dengan ID " << id << " berhasil dimasukkan ke keranjang." << endl;
  }
  else
  { 
    
  }
    cout << "Item dengan ID " << id << " tidak ditemukan." << endl;
}

void popCart()
{
  if (top == nullptr)
  {
    cout << "Keranjang kosong" << endl;
  }
  else
  {
    CartNode *temp = top;
    cout << "Item '" << temp->item.name << "' dengan ID " << temp->item.id << " berhasil dihapus dari keranjang." << endl;
    top = temp->next;
    delete temp;
  }
}

void displayCart()
{
  if (top == nullptr)
  {
    cout << "Keranjang kosong" << endl;
  }
  else
  {
    CartNode *current;
    while (current != nullptr)
    {
      current = current->next;
    }
    current = top;
    cout << "ID: " << current->item.id << ", Nama: " << current->item.name << ", Stock: " << current->item.stock << ", Harga: " << current->item.price << endl;
    cout << "Apakah ingin menghapus item? (y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'y' || choice == 'Y')
    {
      popCart();
    }
  }
}

int main()
{
  HashNode *hashTable[hashTableSize] = {nullptr}; // Initialize hash table
  // OrderQueue orderQueue;
  insertItem(generateIDItem("Tas Slempang", "Tas"), "Tas", 10, 100000, "Tas");
  insertItem(generateIDItem("Celana Jeans", "Celana"), "Tas", 10, 200000, "Celana");

  int pilihMenuUtama;
  do
  {
    cout << "--------------------------------" << endl;
    cout << "   Selamat datang di Aplikasi   " << endl;
    cout << "--------------------------------" << endl;
    cout << "1. Masuk Sebagai Admin" << endl;
    cout << "2. Masuk Sebagai User" << endl;
    cout << "0. Exit" << endl;
    cout << ">";
    cin >> pilihMenuUtama;
    system("cls");
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
        system("cls");
        switch (pilihMenuAdmin)
        {
        case 1:
        {
          string namaKategori;
          cout << "Masukkan nama kategori: ";
          cin >> namaKategori;
          addCategory(namaKategori);
          break;
        }
        case 2:
        {
          string namaItem, kategoriItem;
          int stokItem;
          double hargaItem;
          cout << "Masukkan nama barang: ";
          cin.ignore();
          getline(cin, namaItem);
          cout << "Masukkan stok barang: ";
          cin >> stokItem;
          cout << "Masukkan harga barang: ";
          cin >> hargaItem;

          if (categories == nullptr)
          {
            cout << "Tidak ada kategori yang tersedia" << endl;
            cout << "Mohon tambahkan kategori baru : ";
            cin >> kategoriItem;
          }
          else
          {
            showCategories();
            cout << "Masukkan kategori barang: ";
            cin >> kategoriItem;
            system("cls");
          }
          int idItem = generateIDItem(namaItem, kategoriItem);
          insertItem(idItem, namaItem, stokItem, hargaItem, kategoriItem);
          cout << "Barang berhasil ditambahkan." << endl;
          break;
        }
        case 3:
        {
          int idItem;
          cout << "Masukkan ID barang yang ingin diedit: ";
          cin >> idItem;
          editItem(idItem);
          break;
        }
        case 4:
          int idItem;
          cout << "Masukkan ID barang yang ingin dihapus: ";
          cin >> idItem;
          deleteItem(idItem);
          break;
        case 5:
        {
          if (categories == nullptr)
          {
            cout << "Tidak ada kategori yang tersedia" << endl;
            break;
          }
          else
          {
            showCategories();
            string category;
            cout << "Masukkan kategori: ";
            cin >> category;
            showCategoryItems(category);
          }
          break;
        }
        case 6:
          // orderQueue.dequeue();
          break;
        case 7:
          // orderQueue.displayQueue();
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
        cout << "5. Tampilkan Keranjang" << endl;
        cout << "0. Exit" << endl;
        cout << ">";
        cin >> pilihMenuUser;
        system("cls");
        switch (pilihMenuUser)
        {
        case 1:
        {
          if (categories == nullptr)
          {
            cout << "Tidak ada kategori yang tersedia" << endl;
            break;
          }
          else
          {
            showCategories();
            string category;
            cout << "Masukkan kategori: ";
            cin >> category;
            showCategoryItems(category);

            cout << "\nApakah anda ingin menambahkan item ke keranjang? (y/n): ";
            char input;
            cin >> input;
            if (input == 'y')
            {
              int idItem;
              cout << "Masukkan ID item: ";
              cin >> idItem;
              pushCart(idItem);
            }
            else if (input == 'n')
            {
              break;
            }
          }
          break;
        }
        case 2:
        {
          // string namaItem;
          // cout << "Masukkan nama item: ";
          // cin >> namaItem;
          // // TreeNode *foundItem = searchItemByName(namaItem);
          // if (foundItem != nullptr)
          // {
          //   cout << "ID: " << foundItem->item.id << ", Name: " << foundItem->item.name << ", Stock: " << foundItem->item.stock << ", Price: " << foundItem->item.price << endl;
          // }
          // else
          // {
          //   cout << "Item not found." << endl;
          // }
          // break;
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

          // // Search for the item by name
          // TreeNode *foundItem = searchItemByName(namaProduk);
          // if (foundItem != nullptr)
          // {
          //   // Calculate total price
          //   double totalPrice = foundItem->item.price * jumlahOrder;
          //   // Insert order
          //   insertOrderan(namaPemesan, namaProduk, jumlahOrder, totalPrice, hashTable);
          // }
          // else
          // {
          //   cout << "Produk tidak ditemukan." << endl;
          // }
          break;
        }
        case 4:
        {
          string namaPemesan;
          cout << "Masukkan nama pemesan: ";
          cin >> namaPemesan;
          // searchOrderan(namaPemesan, hashTable);
          break;
        }

        case 5:
        {
          displayCart();
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

struct Order
{
  int orderId;
  string namaPemesan;
  string namaProduk;
  int jumlahOrder;
  double totalPrice;
};

struct OrderNode
{
  Order order;
  OrderNode *next;
};