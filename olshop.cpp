// MENU ADMIN
//     -Tambah kategori✅
//     -Tambah Item✅
//     -Edit Item by ID > searching
//     -Delete Item by ID
//     -Tampilkan Data per kategori 
//     - Urutkan Data dari termurah or termahal > sorting
//     -Proses Orderan > queue
//     -Sinkronkan stok ketika orderan masuk dan update stok 
//             -Riwayat ubah data or stok masuk ke dalam stack > stack

// MENU USER
//     -Tampilkan Data Item per kategori > sorting
//           - Order berdasarkan nama barang  > queue
//           - Dapat menampilkan riwayat orderan

// SISTEM LOGIN USER

//     -daftar
//     -Login
//     - Username & password > Hastable
    







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

const int hashTableSize = 10;
CategoryNode *categories = nullptr;

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
    cout << "ID: " << root->item.id << ", Name: " << root->item.name << ", Stock: " << root->item.stock << ", Price: " << root->item.price << endl;
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
    cout << "Category '" << category << "' not found." << endl;
  }
}
void showCategories()
{
  CategoryNode *current = categories;
  if (current == nullptr)
  {
    cout << "No categories found." << endl;
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

void deleteItem(int id){
  TreeNode *current = categories->itemsTree;
  while (current != nullptr){
    if (current->item.id == id){
      cout << "ID: " << current->item.id << ", Name: " << current->item.name << ", Stock: " << current->item.stock << ", Price: " << current->item.price << endl;
    }
    current = current->right;
  }
}

int main()
{
  HashNode *hashTable[hashTableSize] = {nullptr}; // Initialize hash table

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
          addCategory(namaKategori);
          break;
        }
        case 2:
        {
          string namaItem, kategoriItem;
          int stokItem;
          double hargaItem;
          cout << "Masukkan nama item: ";
          cin.ignore();
          getline(cin, namaItem);
          cout << "Masukkan stok item: ";
          cin >> stokItem;
          cout << "Masukkan harga item: ";
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
            cout << "Masukkan kategori item: ";
            cin >> kategoriItem;
          }
          int idItem = generateIDItem(namaItem, kategoriItem);
          insertItem(idItem, namaItem, stokItem, hargaItem, kategoriItem);
          cout << "Item berhasil ditambahkan." << endl;
          break;
        }
        case 3:
        {
          int idItem;
          cout << "Masukkan ID item yang ingin diedit: ";
          cin >> idItem;
          editItem(idItem);
          break;
        }
        case 4:
          // Implementasi Hapus Item
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
          // processOrderan(hashTable);
          break;
        case 7:
          // showOrderan();
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
          // listCategories();
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