#include <iostream>
using namespace std;



struct Barang{
  
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