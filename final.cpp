// MENU ADMIN
//     -Tambah kategori ✅
//     -Tambah Item ✅
//     -Edit Item by ID > searching ✅
//     -Delete Item by ID ✅
//     -Tampilkan Data per kategori ✅
//     - Urutkan Data dari termahal - termurah > sorting ✅
//     -Proses Orderan > queue
//     -Sinkronkan stok ketika orderan masuk dan update stok

// MENU USER
//      -Tampilkan Data Item per kategori > sorting
//      -Seach barang pakai nama barang ✅
//      - Order berdasarkan nama barang  > queue
//      - Dapat menampilkan riwayat orderan

// SISTEM LOGIN USER

//     -daftar✅
//     -Login✅
//     -Username & password > Hastable✅

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

  TreeNode(Item newItem) : item(newItem), left(nullptr), right(nullptr) {}
};

struct CategoryNode
{
  string name;
  TreeNode *itemsTree;
  CategoryNode *next;

  CategoryNode(string category) : name(category), itemsTree(nullptr), next(nullptr) {}
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

struct Order
{
  string id;
  string namaPemesan;
  string namaProduk;
  int jumlahOrder;
  double totalPrice;
  bool status;
};

struct QueueNode
{
  Order order;
  QueueNode *next;

  QueueNode(Order newOrder) : order(newOrder), next(nullptr) {}
};

struct Queue
{
  QueueNode *front;
  QueueNode *rear;

  Queue() : front(nullptr), rear(nullptr) {}
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
string generateIDOrder(string kategori, string name, int idItem)
{
  string id;
  char digit1 = kategori[0];
  int digit2 = tolower(name[0] - 'a' + 1);
  int digit3 = tolower(name[name.length() - 1] - 'a' + 1);
  int digit4 = idItem;
  id = digit1 + to_string(digit2) + to_string(digit3) + to_string(digit4);
  return id;
}

TreeNode *createNode(int id, string name, int stock, double price)
{
  Item newItem;
  TreeNode *newNode = new TreeNode(newItem);
  newNode->item.id = id;
  newNode->item.name = name;
  newNode->item.stock = stock;
  newNode->item.price = price;
  newNode->left = newNode->right = nullptr;

  return newNode;
}

TreeNode *insertItem(TreeNode *root, Item newItem)
{
  if (root == nullptr)
  {
    return new TreeNode(newItem);
  }

  if (newItem.price < root->item.price)
  {
    root->left = insertItem(root->left, newItem);
  }
  else
  {
    root->right = insertItem(root->right, newItem);
  }

  return root;
}

TreeNode *findCheapestItem(TreeNode *root)
{
  TreeNode *current = root;
  while (current != nullptr && current->left != nullptr)
  {
    current = current->left;
  }
  return current;
}

void displayCheapestItems(CategoryNode *categories)
{
  CategoryNode *currentCategory = categories;

  cout << "~~~~~~~~~~~~~~ Barang Termurah ~~~~~~~~~~~~~\n";
  cout << "===========================================\n";
  cout << "Barang\tKategori\tHarga\n";
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

  while (currentCategory != nullptr)
  {
    TreeNode *cheapestItem = findCheapestItem(currentCategory->itemsTree);

    if (cheapestItem != nullptr)
    {
      cout << currentCategory->name << "\t" << cheapestItem->item.name << "\t" << cheapestItem->item.price << "\n";
    }
    else
    {
      cout << currentCategory->name << "\tTidak ada barang\tN/A\n";
    }

    currentCategory = currentCategory->next;
  }
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
    CategoryNode *newCategory = new CategoryNode(category);
    newCategory->name = category;
    newCategory->itemsTree = nullptr;
    newCategory->next = categories;
    categories = newCategory;

    cout << "Kategori " << category << " berhasil ditambahkan :)" << endl;

    return newCategory;
  }
}

void printItemTableHeader()
{
  cout << "ID      | Nama Barang           | Stok  |    Harga    |" << endl;
  cout << "--------|-----------------------|-------|-------------|" << endl;
}

void printItemTableRow(int id, const string &name, int stock, double price)
{
  int namePadding = max(0, 22 - static_cast<int>(name.length()));
  int stockPadding = max(0, 6 - static_cast<int>(to_string(stock).length()));
  int pricePadding = max(0, 8 - static_cast<int>(to_string(price).length()));

  cout << id << "    | "
       << name << string(namePadding, ' ') << "| "
       << stock << string(stockPadding, ' ') << "|   "
       << price << string(pricePadding, ' ') << "\t|" << endl;
}

void showTree(TreeNode *node)
{
  if (node != nullptr)
  {
    showTree(node->left);
    printItemTableRow(node->item.id, node->item.name, node->item.stock, node->item.price);
    showTree(node->right);
  }
}

void showCategoryItems(string category)
{
  CategoryNode *categoryNode = findCategory(category);

  if (categoryNode != nullptr)
  {
    cout << "=======================================================" << endl;
    cout << "\t\t\t" << category << endl;
    cout << "-------------------------------------------------------" << endl;
    printItemTableHeader();
    showTree(categoryNode->itemsTree);
    cout << "=======================================================" << endl;
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
    cout << "-----------------" << endl;
    cout << "Daftar kategori:" << endl;
    cout << "-----------------" << endl;
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
    categoryNode = new CategoryNode(category);
    categoryNode->name = category;
    categoryNode->itemsTree = nullptr;
    categoryNode->next = categories;
    categories = categoryNode;
  }

  Item newItem = {id, name, stock, price};
  categoryNode->itemsTree = insertItem(categoryNode->itemsTree, newItem);
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

          system("cls");
          cout << "Data berhasil diubah!" << endl;
          currentItem->item.id = generateIDItem(currentItem->item.name, currentCategory->name);
          cout << "ID: " << currentItem->item.id << ", Name: " << currentItem->item.name << ", Stock: " << currentItem->item.stock << ", Price: " << currentItem->item.price << endl;

          if (keepEditing)
          {
            cout << "Apakah ingin mengubah data lagi? (y/n): ";
            char choice;
            cin >> choice;
            system("cls");
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
    cout << "Item dengan ID " << id << " tidak ditemukan.\n"
         << endl;
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
        system("cls");
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

TreeNode *searchItemByName(TreeNode *root, const string &namaItem)
{
  if (root == nullptr || root->item.name == namaItem)
  {
    return root;
  }

  if (namaItem < root->item.name)
  {
    return searchItemByName(root->left, namaItem);
  }
  else
  {
    return searchItemByName(root->right, namaItem);
  }
}
bool updateStock(int stock, int jmlhOrder)
{
  if (stock >= jmlhOrder)
  {
    return true;
  }
  else
  {
    cout << "Stock tidak mencukupi." << endl;
    return false;
  }
}
void enqueue(Queue &q, Order newOrder)
{
  QueueNode *newNode = new QueueNode(newOrder);
  if (q.rear == nullptr)
  {
    q.front = q.rear = newNode;
    return;
  }
  q.rear->next = newNode;
  q.rear = newNode;
}

Order dequeue(Queue &q)
{
  if (q.front == nullptr)
  {
    cout << "Orderan masih kosong" << endl;
  }
  QueueNode *temp = q.front;
  Order order = temp->order;
  q.front = q.front->next;

  if (q.front == nullptr)
  {
    q.rear = nullptr;
  }
  delete temp;
  return order;
}

bool isQueueEmpty(Queue &q)
{
  return q.front == nullptr;
}

void printOrderTableHeader()
{
  cout << "      ID      | Nama Pemesan          | Nama Produk           | Jumlah Order | Total Price | Status " << endl;
  cout << "--------------|-----------------------|-----------------------|--------------|-------------|--------" << endl;
}

void printOrderTableRow(string id, const string &namaPemesan, const string &namaProduk, int jumlahOrder, double totalPrice, bool status)
{
  cout << id << "      | "
       << namaPemesan << string(22 - namaPemesan.length(), ' ') << "| "
       << namaProduk << string(22 - namaProduk.length(), ' ') << "| "
       << jumlahOrder << string(12 - to_string(jumlahOrder).length(), ' ') << " | "
       << totalPrice << string(8 - to_string(jumlahOrder).length(), ' ') << " | "
       << (status ? "Diproses" : "Remaining") << endl;
}

void displayQueue(Queue &q)
{
  if (q.front == nullptr)
  {
    cout << "Orderan masih kosong !" << endl;
    return;
  }
  printOrderTableHeader();

  QueueNode *current = q.front;
  while (current != nullptr)
  {
    printOrderTableRow(current->order.id, current->order.namaPemesan, current->order.namaProduk, current->order.jumlahOrder, current->order.totalPrice, current->order.status);
    current = current->next;
  }
}
void displayAllQueue(Queue &q)
{

  if (q.front == nullptr)
  {
    cout << "Orderan masih kosong !" << endl;
  }
  else
  {

    QueueNode *current = q.front;
    printOrderTableHeader();
    while (current != nullptr)
    {
      printOrderTableRow(current->order.id, current->order.namaPemesan, current->order.namaProduk, current->order.jumlahOrder, current->order.totalPrice, current->order.status);
      current = current->next;
    }
  }
}

void pushCart(Item item)
{
  CartNode *newCartNode = new CartNode;
  newCartNode->item = item;
  newCartNode->next = top;
  top = newCartNode;
  cout << "Item '" << item.name << "' dengan ID " << item.id << " berhasil dimasukkan ke keranjang." << endl;
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

void displayNota(Queue &order)
{
  if (order.front == nullptr)
  {
    cout << "Anda belum membuat pesanan !!" << endl;
  }
  else
  {
    cout << "----------------------------------------" << endl;
    cout << "            NOTA PEMESANAN              " << endl;
    cout << "----------------------------------------" << endl;
    cout << "ID Pesanan     : " << order.front->order.id << endl;
    cout << "Nama Pemesan   : " << order.front->order.namaPemesan << endl;
    cout << "Nama Produk    : " << order.front->order.namaProduk << endl;
    cout << "Jumlah Order   : " << order.front->order.jumlahOrder << endl;
    cout << "Total Harga    : Rp " << order.front->order.totalPrice << endl;
    cout << "----------------------------------------" << endl;
  }
}

int main()
{
  Queue orderQueue;
  int orderIdCounter = 1;
  insertItem(generateIDItem("Kemeja Flannel", "Kemeja"), "Kemeja Flannel", 2, 100000, "Kemeja");
  insertItem(generateIDItem("Kemeja Denim", "Kemeja"), "Kemeja Denim", 2, 100000, "Kemeja");
  insertItem(generateIDItem("Kemeja Oxford", "Kemeja"), "Kemeja Oxford", 2, 100000, "Kemeja");
  insertItem(generateIDItem("Celana Jeans", "Celana"), "Celana Jeans", 2, 100000, "Celana");
  insertItem(generateIDItem("Celana Kain", "Celana"), "Celana Kain", 2, 100000, "Celana");
  insertItem(generateIDItem("Celana Denim", "Celana"), "Celana Denim", 2, 100000, "Celana");

  int pilihMenuUtama;
  do
  {
    cout << "---------------------------------" << endl;
    cout << "|  Selamat datang di Aplikasi   |" << endl;
    cout << "---------------------------------" << endl;
    cout << "|1. Masuk Sebagai Admin         |" << endl;
    cout << "|2. Masuk Sebagai User          |" << endl;
    cout << "|0. Exit                        |" << endl;
    cout << "---------------------------------" << endl;
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
        cout << "------------------------------" << endl;
        cout << "|         MENU ADMIN         |" << endl;
        cout << "------------------------------" << endl;
        cout << "|1. Tambah Kategori          |" << endl;
        cout << "|2. Tambah Item              |" << endl;
        cout << "|3. Edit Item                |" << endl;
        cout << "|4. Hapus Item               |" << endl;
        cout << "|5. Tampilkan Data Item      |" << endl;
        cout << "|6. Proses Orderan           |" << endl;
        cout << "|7. Tampilkan Semua Orderan  |" << endl;
        cout << "|0. Exit                     |" << endl;
        cout << "------------------------------" << endl;
        cout << ">";
        cin >> pilihMenuAdmin;
        system("cls");
        switch (pilihMenuAdmin)
        {
        case 1:
        {
          string namaKategori;
          cout << "\nMasukkan nama kategori: ";
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
          cout << "Masukkan nama barang: ";
          cin.ignore();
          getline(cin, namaItem);
          cout << "Masukkan stok item: ";
          cout << "Masukkan stok barang: ";
          cin >> stokItem;
          cout << "Masukkan harga item: ";
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
            cout << "Masukkan kategori item: ";
            cin >> kategoriItem;
            system("cls");
          }
          int idItem = generateIDItem(namaItem, kategoriItem);
          insertItem(idItem, namaItem, stokItem, hargaItem, kategoriItem);
          cout << "Item berhasil ditambahkan." << endl;
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
        {
          int idItem;
          cout << "Masukkan ID barang yang ingin dihapus: ";
          cin >> idItem;
          deleteItem(idItem);
          break;
        }
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
            cout << "\nPilih kategori: ";
            cin >> category;
            showCategoryItems(category);
          }
          break;
        }
        case 6:
        {
          if (orderQueue.front == nullptr)
          {
            cout << "Tidak ada orderan yang tersedia" << endl;
          }
          else
          {
            while (orderQueue.front != nullptr)
            {
              cout << "----------------------------------------" << endl;
              cout << "            NOTA PEMESANAN              " << endl;
              cout << "----------------------------------------" << endl;
              cout << "ID Pesanan     : " << orderQueue.front->order.id << endl;
              cout << "Pemesan        : " << orderQueue.front->order.namaPemesan << endl;
              cout << "Produk         : " << orderQueue.front->order.namaProduk << endl;
              cout << "Jumlah Order   : " << orderQueue.front->order.jumlahOrder << endl;
              cout << "Total Harga    : Rp " << orderQueue.front->order.totalPrice << endl;
              cout << "----------------------------------------" << endl;
              cout << "Proses orderan?" << endl;
              cout << "1. Ya" << endl;
              cout << "2. Tidak" << endl;
              cout << "3. Exit" << endl;
              cout << ">";
              string confirm;
              cin >> confirm;
              if (confirm == "1")
              {
                orderQueue.front->order.status = true;
              }
              else if (confirm == "2")
              {
                dequeue(orderQueue);
              }
              else if (confirm == "3")
              {
                break;
              }
            }
          }
          break;
        }
        case 7:
        {
          displayAllQueue(orderQueue);
          break;
        }
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
        cout << "---------------------------------" << endl;
        cout << "|           MENU USER           |" << endl;
        cout << "---------------------------------" << endl;
        cout << "|1. Tampilkan Item              |" << endl;
        cout << "|2. Cari Item Berdasarkan Nama  |" << endl;
        cout << "|3. Tampilkan Orderan Saya      |" << endl;
        cout << "|4. Tampilkan Keranjang         |" << endl;
        cout << "|5. GRAPH                       |" << endl;
        cout << "|6. Buat Pesanan                |" << endl;
        cout << "|0. Exit                        |" << endl;
        cout << "---------------------------------" << endl;
        cout << ">";
        cin >> pilihMenuUser;
        system("cls");
        switch (pilihMenuUser)
        {
        case 1:
        {
          string category;
          if (categories == nullptr)
          {
            cout << "Tidak ada kategori yang tersedia" << endl;
            break;
          }
          else
          {
            showCategories();
            cout << "\nMasukkan kategori: ";
            cin >> category;
            showCategoryItems(category);
            int input;
            do
            {

              cout << "1. Ganti kategori" << endl;
              cout << "2. Buat pesanan " << endl;
              cout << "3. Masukan keranjang" << endl;
              cout << "4. Exit" << endl;
              cout << ">";
              cin >> input;

              switch (input)
              {
              case 1:
                showCategories();
                cout << "\nMasukkan kategori: ";
                cin >> category;
                showCategoryItems(category);
                break;
              case 2:
              {
                string namaPemesan, namaProduk, category;
                int jumlahOrder;

                cout << "Masukkan nama pemesan: ";
                cin >> namaPemesan;
                cout << "Masukkan kategori: ";
                cin >> category;
                cout << "Masukkan nama produk: ";
                cin.ignore();
                getline(cin, namaProduk);
                cout << "Masukkan jumlah order: ";
                cin >> jumlahOrder;

                CategoryNode *categories = findCategory(category);

                TreeNode *foundItem = searchItemByName(categories->itemsTree, namaProduk);
                if (foundItem == nullptr)
                {
                  cout << "Produk tidak ditemukan." << endl;
                }
                else
                {
                  if (updateStock(foundItem->item.stock, jumlahOrder))
                  {
                    foundItem->item.stock -= jumlahOrder;

                    int id = generateIDItem(namaProduk, category);
                    double totalPrice = foundItem->item.price * jumlahOrder;
                    Order newOrder = {generateIDOrder(category, namaPemesan, foundItem->item.id), namaPemesan, namaProduk, jumlahOrder, totalPrice, false};
                    enqueue(orderQueue, newOrder);

                    cout << "Order berhasil dibuat!" << endl;
                  }
                }
                break;
              }
              case 3:
              {

                cout << "Masukkan nama item yang ingin dimasukkan ke keranjang: ";
                string namaItem, category;
                cin.ignore();
                getline(cin, namaItem);
                cout << "Masukan kategori: ";
                cin >> category;

                CategoryNode *categories = findCategory(category);
                TreeNode *foundItem = searchItemByName(categories->itemsTree, namaItem);
                if (foundItem == nullptr)
                {
                  cout << "Produk tidak ditemukan." << endl;
                  break;
                }
                else
                {

                  pushCart(foundItem->item);
                }
                break;
              }
              default:
                break;
              }
            } while (input != 4);
            break;
          }
        }
        case 2:
        {
          string namaItem;
          cout << "Masukkan nama item: ";
          cin.ignore();
          getline(cin, namaItem);

          TreeNode *foundItem = searchItemByName(categories->itemsTree, namaItem);
          if (foundItem != nullptr)
          {
            cout << "ID: " << foundItem->item.id << ", Name: " << foundItem->item.name << ", Stock: " << foundItem->item.stock << ", Price: " << foundItem->item.price << endl;
          }
          else
          {
            cout << "Produk tidak ditemukan." << endl;
          }
          break;
        }
        case 3:
        {
          // Order order = dequeue(orderQueue);
          displayNota(orderQueue);
          break;
        }
        case 4:
        {
          if (top == nullptr)
          {
            cout << "\nKeranjang kosong !!\n"
                 << endl;
            break;
          }
          else
          {
            system("cls");
            CartNode *current;
            while (current != nullptr)
            {
              current = top;
              printItemTableHeader();
              printItemTableRow(current->item.id, current->item.name, current->item.stock, current->item.price);
              cout << "\n1.Buat Pesanan" << endl;
              cout << "2.Hapus dari keranjang" << endl;
              cout << "3.Exit" << endl;
              cout << ">";
              int choice;
              cin >> choice;
              if (choice == 1)
              {
                string namaPemesan, namaProduk, category;
                int jumlahOrder;

                cout << "Masukkan nama pemesan: ";
                cin >> namaPemesan;
                cout << "Masukkan kategori: ";
                cin >> category;
                cout << "Masukkan nama produk: ";
                cin.ignore();
                getline(cin, namaProduk);
                cout << "Masukkan jumlah order: ";
                cin >> jumlahOrder;

                CategoryNode *categories = findCategory(category);

                TreeNode *foundItem = searchItemByName(categories->itemsTree, namaProduk);
                if (foundItem == nullptr)
                {
                  cout << "Produk tidak ditemukan." << endl;
                }
                else
                {
                  if (updateStock(foundItem->item.stock, jumlahOrder))
                  {
                    foundItem->item.stock -= jumlahOrder;

                    int id = generateIDItem(namaProduk, category);
                    double totalPrice = foundItem->item.price * jumlahOrder;
                    Order newOrder = {generateIDOrder(category, namaPemesan, foundItem->item.id), namaPemesan, namaProduk, jumlahOrder, totalPrice, false};
                    enqueue(orderQueue, newOrder);

                    cout << "Order berhasil dibuat!" << endl;
                  }
                }

                popCart();
              }
              else if (choice == 2)
              {
                popCart();
                break;
              }else if(choice == 3){
                break;
              }
              current = current->next;
            }
          }
          break;
        }
        case 5:
        {
          displayCheapestItems(categories);
          break;
        }
        case 6:
        {
          string namaPemesan, namaProduk;
          int jumlahOrder;
          cout << "Masukkan nama pemesan: ";
          cin >> namaPemesan;
          cout << "Masukkan nama produk: ";
          cin >> namaProduk;
          cout << "Masukkan jumlah order: ";
          cin >> jumlahOrder;

          TreeNode *foundItem = searchItemByName(categories->itemsTree, namaProduk);
          updateStock(foundItem->item.stock, jumlahOrder);
          if (foundItem != nullptr)
          {
            double totalPrice = foundItem->item.price * jumlahOrder;
            Order newOrder = {generateIDOrder(categories->name, namaPemesan, foundItem->item.id), namaPemesan, namaProduk, jumlahOrder, totalPrice, false};
            enqueue(orderQueue, newOrder);
            cout << "Order berhasil dibuat!" << endl;
          }
          else
          {
            cout << "Produk tidak ditemukan." << endl;
          }
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