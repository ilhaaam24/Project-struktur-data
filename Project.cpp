// input Barang
// edit data Barang
// delete data Barang
// kategory  barang
// show data Barang berdasarkan kategory

#include <iostream>
using namespace std;


struct Kategory{
  string namaKategory = "Pria";
};

struct Tree
{
  int id;
  string namaBarang;
  int stok;
  Tree *right;
  Tree *left;
}barang;

void insertToKategori(Tree **root, string kategory){  

}

void insert(Tree **root, int id, string namaBarang, int stok, string kategory)
{
  Kategory k;
  if (*root == NULL)
  {
    Tree *baru;
    baru = new Tree;
    baru->id = id;
    baru->namaBarang = namaBarang;
    baru->stok = stok;
    baru->right = NULL;
    baru->left = NULL;
    *root = baru;
  }
  else if (id < (*root)->id)
  {
    insert(&((*root)->left), id, namaBarang, stok, kategory);
  }
  else if (id > (*root)->id)
  {
    insert(&((*root)->right), id, namaBarang, stok, kategory);
  }

  if(kategory == k.namaKategory){
    cout << "halooo" << endl;
    insertToKategori(namaBarang,stok,id)
  }
}
void generateID(Tree **root, string namaBarang,string kategory){

  
}


void show(Tree *root)
{
  if (root != NULL)
  {
    show(root->left);
    cout << root->id << " " << root->namaBarang << " " << root->stok << endl;
    show(root->right);
  }
}

void pop(Tree **root, int id)
{
  if (*root == NULL)
  {
    return;
  }
  else if (id < (*root)->id)
  {
    pop(&((*root)->left), id);
  }
  else if (id > (*root)->id)
  {
    pop(&((*root)->right), id);
  }
  else if (id == (*root)->id)
  {
    Tree *temp;
    temp = (*root);
    if ((*root)->left == NULL && (*root)->right == NULL)
    {
      (*root) = NULL;
      delete temp;
    }
    else if ((*root)->left == NULL)
    {
      (*root) = (*root)->right;
      delete temp;
    }
  }
}
void editStok(Tree **root, int id, int stok = 0)
{
  if (*root == NULL)
  {
    return;
  }
  else if (id < (*root)->id)
  {
    editStok(&((*root)->left), id, stok);
  }
  else if (id > (*root)->id)
  {
    editStok(&((*root)->right), id, stok);
  }
  else if (id == (*root)->id)
  {
    cout << "Masukan stok yang ingin di edit : ";
    cin >> stok;
    (*root)->stok = stok;

  }
}






int main()
{
  Tree *root = NULL;
  int ID, stok;
  string namaBarang, kategori;
  cout << "Masukan id barang : ";
  cin >> ID;
  cout << "Masukan nama barang : ";
  cin >> namaBarang;
  cout << "Masukan stok : ";  
  cin >> stok;
  cout << "Masukan kategory : ";
  cin >> kategori;
  insert(&root, ID, namaBarang, stok, kategori);  


  

  show(root);


  cout << "Masukan barang yang ingin dihapus : ";
  int id;
  cin >> id;
  pop(&root, id);
  show(root);
  cout << "Masukan id barang yang ingin di edit : ";
  int id2;
  cin >> id2;
  editStok(&root, id2, 50);
  show(root);

  return 0;
}