/**
@file asd.cc

Plik do modyfikacji w w ramach cwiczenia 4 z AISDI.
Zawiera niekompletne implementacje metod klasy TreeMap.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY TreeMap.

@author
Pawel Cichocki, Michal Nowacki

@date
last revision
- 2005.12.01 Pawel Cichocki: TreeNodeDetail class
- 2005.12.01 Michal Nowacki: lab #4
- 2005.11.17 Michal Nowacki: lab #3, copy operator and constructor
- 2005.11.04 Pawel Cichocki: copied comments from the header
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.26 Michal Nowacki: removed some method bodies
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

/**
 * Tomasz Jakubczyk
 * grupa 3I3
 * data: 5.12.2014
 *
 *
 */

#include <assert.h>
#include <algorithm>

#include <iostream>

#include<climits>

#ifdef _SUNOS
#include "/materialy/AISDI/tree/TreeMap.h"
#else
#include "TreeMap.h"
#endif

#define DEBUG if(false)
#define DEBUG2 if(false)

void print(const std::pair<int, std::string>& p);

/// A helper class.
class TreeMapDetail //Helper
{
protected:
    friend class TreeMap;
    //friend class const_iterator;
    TreeNode* first;
    TreeNode* last;
    TreeNode* sentinel;
   /// Stupid example of a method that modifies a protected field in
   /// the TreeMap class. Feel free to remove this method or add new
   /// ones here.
   /*static void erase(TreeMap* tm, const TreeMap::Key& k)
   {
      tm->root=NULL; // we just modified a protected field in tree map
   }*/
public:
    unsigned map_size;
    TreeMapDetail()
    {
DEBUG std::cout<<"TreeMapDetail()"<<std::endl;
        map_size=0;
    }
};

//////////////////////////////////////////////////////////////////////////////
// TreeMap and TreeMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

TreeMap::TreeMap()
{
    root = NULL;
    detail=new TreeMapDetail();
};

TreeNode* DFScopy(TreeNode* root, TreeNode* root2=NULL)
{
    TreeNode* tmp;
    TreeNode* tmp2;
    TreeNode* tmp3;
    if(root!=NULL)
    {
        tmp=new TreeNode(root->data, root2);//data, parent
        tmp2=DFScopy(root->left, tmp);
        tmp3=DFScopy(root->right, tmp);
        tmp->left=tmp2;
        tmp->right=tmp3;
        return tmp;
    }
    return NULL;
}

/// Content of existing TreeMap object is copied into the new object.
TreeMap::TreeMap( const TreeMap& m )
{
    detail=new TreeMapDetail();
    detail->map_size=m.detail->map_size;
    root=DFScopy(m.root);
    /*iterator i=m.begin();
    while(i!=m.end())
    {
        unsafe_insert(i.node->data);
        i++;
    }*/
};

TreeMap::~TreeMap()
{
DEBUG std::cout <<"TreeMap::~TreeMap()"<< std::endl;
    clear();
DEBUG std::cout <<detail->map_size<< std::endl;
    delete detail;
};

// Inserts an element into the map.
// @returns A pair whose bool component is true if an insertion was
//          made and false if the map already contained an element
//          associated with that key, and whose iterator component coresponds to
//          the address where a new element was inserted or where the element
//          was already located.
std::pair<TreeMap::iterator, bool> TreeMap::insert(const std::pair<Key, Val>& entry)
{
DEBUG std::cout<<"TreeMap::insert(const std::pair<Key, Val>& entry)"<<std::endl;
    iterator i=find(entry.first);//find moze byc szybszy od ++
    if(i==end())
    {
        i=unsafe_insert(entry);
        return std::make_pair(i, (bool)true);
    }
    return std::make_pair(i, (bool)false);

}

// Inserts an element into the map.
// This method assumes there is no value asociated with
// such a key in the map.

TreeMap::iterator TreeMap::unsafe_insert(const std::pair<Key, Val>& entry)
{
DEBUG std::cout<<"TreeMap::unsafe_insert(const std::pair<Key, Val>& entry)"<<std::endl;
    TreeNode* nowy=new TreeNode(entry);
    if(root==NULL)
    {
        root=new TreeNode(std::make_pair(INT_MAX,"sentinel"));
    }
    TreeNode* tmp=root;
    //TreeMap m;
    //iterator i;
    while(tmp!=NULL)
    {
DEBUG std::cout<<"tmp: "<<tmp<<std::endl;
DEBUG print(tmp->data);
        if(entry.first<tmp->data.first)//mniejsze na lewo
        {
            if(tmp->left==NULL)//koniec
            {
                nowy->parent=tmp;
                tmp->left=nowy;
                detail->map_size++;
                /*m=*this;
                clear();
                root = NULL;
                detail=new TreeMapDetail();
                i=m.begin();
                while(i!=m.end())
                {
                    unsafe_insert(i.node->data);
                    i++;
                }*/
                return iterator(nowy);
            }
            else//zaglebiaj dalej
            {
                tmp=tmp->left;
            }
        }
        else//wieksze na prawo
        {
            if(tmp->right==NULL)//koniec
            {
                nowy->parent=tmp;
                tmp->right=nowy;
                detail->map_size++;
                /*m=*this;
                clear();
                root = NULL;
                detail=new TreeMapDetail();
                i=m.begin();
                while(i!=m.end())
                {
                    unsafe_insert(i.node->data);
                    i++;
                }*/
                return iterator(nowy);
            }
            else//zaglebiaj dalej
            {
                tmp=tmp->right;
            }
        }
    }
    root=nowy;//root=nowy
    detail->map_size++;
DEBUG std::cout<<"root: "<<root<<std::endl;
DEBUG print(root->data);
    /*m=*this;
    clear();
    root = NULL;
    detail=new TreeMapDetail();
    i=m.begin();
    while(i!=m.end())
    {
        unsafe_insert(i.node->data);
        i++;
    }*/
    return iterator(nowy);
}

// Returns an iterator addressing the location of the entry in the map
// that has a key equivalent to the specified one or the location succeeding the
// last element in the map if there is no match for the key.
TreeMap::iterator TreeMap::find(const Key& k)
{
DEBUG std::cout<<"TreeMap::find(const Key& k)"<<std::endl;
    TreeNode* tmp=root;
    while(tmp!=NULL)
    {
        if(tmp->data.first==k)
        {
            return iterator(tmp);
        }
        if(k<tmp->data.first)//w lewo
        {
            tmp=tmp->left;
        }
        else
        {
            tmp=tmp->right;
        }
    }
    return end();
}

TreeNode* DFSfind(TreeNode* tmp,const int& k)
{
    TreeNode* tmp2;
    if(tmp==NULL)
    {
        return NULL;
    }
    if(tmp->data.first==k)
    {
        return tmp;
    }
    tmp2=DFSfind(tmp->left,k);
    if(tmp2!=NULL)
    {
        return tmp2;
    }
    return DFSfind(tmp->right,k);
}

TreeMap::const_iterator TreeMap::find(const Key& k) const
{
DEBUG std::cout<<"TreeMap::find(const Key& k) const"<<std::endl;
    /*TreeNode* tmp=root;
    while(tmp!=NULL)
    {
        if(tmp->data.first==k)
        {
            return const_iterator(tmp);
        }
        if(k<tmp->data.first)//w lewo
        {
            tmp=tmp->left;
        }
        else
        {
            tmp=tmp->right;
        }
    }
    return end();*/
    TreeNode* tmp=DFSfind(root,k);
    if(tmp==NULL)
        return end();
    return const_iterator(tmp);
}

// Inserts an element into a map with a specified key value
// if one with such a key value does not exist.
// @returns Reference to the value component of the element defined by the key.
TreeMap::Val& TreeMap::operator[](const Key& k)
{
DEBUG std::cout<<"TreeMap::operator[](const Key& k)"<<std::endl;
    iterator i=find(k);
    if(i==end())
    {
        i=unsafe_insert(P(k,"domyslna"));
    }
    return i->second;
}

// Tests if a map is empty.
bool TreeMap::empty( ) const
{
DEBUG std::cout <<"TreeMap::empty( ) const"<< std::endl;
    return detail->map_size==0;
}

// Returns the number of elements in the map.
TreeMap::size_type TreeMap::size( ) const
{
DEBUG std::cout <<"TreeMap::size( ) const"<< std::endl;
    return detail->map_size;
}

// Returns the number of elements in a map whose key matches a parameter-specified key.
TreeMap::size_type TreeMap::count(const Key& _Key) const
{
DEBUG std::cout <<"TreeMap::count(const Key& _Key) const"<< std::endl;
    if(find(_Key)!=end())
        return 1;  // this is not a multimap
    return 0;
}

// Removes an element from the map.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator i)
{
DEBUG std::cout<<"TreeMap::erase(TreeMap::iterator i)"<<std::endl;
    iterator j=i;
    j++;
    //usuwamy wezel i wstawiamy od nowa jego galezie
    TreeNode* leftb;//lewa galaz
    TreeNode* rightb;//prawa galaz
    TreeNode* tmp;
    if(i.node==NULL)
    {
        return end();
    }
    detail->map_size--;//istnieje i da sie usunac
    leftb=i.node->left;
    rightb=i.node->right;
    if(i.node->parent==NULL)
    {
DEBUG std::cout<<"usuniecie korzenia"<<std::endl;
        root=NULL;
    }
    else if(i.node->parent->left==i.node)
    {
DEBUG std::cout<<"usuniecie lewego"<<std::endl;
        i.node->parent->left=NULL;
    }
    else if(i.node->parent->right==i.node)
    {
DEBUG std::cout<<"usuniecie prawego"<<std::endl;
        i.node->parent->right=NULL;
    }
    else
    {
DEBUG std::cout<<"TreeMap::erase(TreeMap::iterator i) cos jest bardzo zle"<<std::endl;
        throw std::string("TreeMap::erase(TreeMap::iterator i) cos jest bardzo zle");
    }
    delete i.node;//

DEBUG std::cout<<"wstawienie lewej galezi"<<std::endl;
    tmp=root;
    while(tmp!=NULL && leftb!=NULL)
    {
DEBUG std::cout<<"tmp: "<<tmp<<std::endl;
DEBUG print(tmp->data);
        if(leftb->data.first<tmp->data.first)//mniejsze na lewo
        {
            if(tmp->left==NULL)//koniec
            {
DEBUG std::cout<<"if(tmp->left==NULL)//koniec"<<std::endl;
                tmp->left=leftb;
                leftb->parent=tmp;
                //detail->map_size++;
                //return iterator(nowy);
                break;
            }
            else//zaglebiaj dalej
            {
                tmp=tmp->left;
            }
        }
        else//wieksze na prawo
        {
            if(tmp->right==NULL)//koniec
            {
DEBUG std::cout<<"if(tmp->right==NULL)//koniec"<<std::endl;
                tmp->right=leftb;
                leftb->parent=tmp;
                //detail->map_size++;
                //return iterator(nowy);
                break;
            }
            else//zaglebiaj dalej
            {
                tmp=tmp->right;
            }
        }
    }
    if(tmp==NULL && leftb!=NULL)
    {
DEBUG std::cout<<"if(tmp==NULL)"<<std::endl;
        root=leftb;//root=nowy
        leftb->parent=NULL;
    }
DEBUG std::cout<<"wstawienie prawej galezi"<<std::endl;
    tmp=root;
    while(tmp!=NULL && rightb!=NULL)
    {
        if(rightb->data.first<tmp->data.first)//mniejsze na lewo
        {
            if(tmp->left==NULL)//koniec
            {
DEBUG std::cout<<"if(tmp->left==NULL)//koniec"<<std::endl;
                tmp->left=rightb;
                rightb->parent=tmp;
                //detail->map_size++;
                //return iterator(nowy);
                break;
            }
            else//zaglebiaj dalej
            {
                tmp=tmp->left;
            }
        }
        else//wieksze na prawo
        {
            if(tmp->right==NULL)//koniec
            {
DEBUG std::cout<<"if(tmp->right==NULL)//koniec"<<std::endl;
                tmp->right=rightb;
                rightb->parent=tmp;
                //detail->map_size++;
                //return iterator(nowy);
                break;
            }
            else//zaglebiaj dalej
            {
                tmp=tmp->right;
            }
        }
    }
    if(tmp==NULL && rightb!=NULL)
    {
DEBUG std::cout<<"root=rightb tak nie piowinno byc"<<std::endl;
        root=rightb;//root=nowy
        rightb->parent=NULL;
    }

    return j;
}

// Removes a range of elements from the map.
// The range is defined by the key values of the first and last iterators
// first is the first element removed and last is the element just beyond the last elemnt removed.
// @returns The iterator that designates the first element remaining beyond any elements removed.
TreeMap::iterator TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l)
{
DEBUG std::cout <<"TreeMap::erase(TreeMap::iterator f, TreeMap::iterator l)"<< std::endl;
    iterator i=f;
    while(i!=f || i!=end())
    {
        i=erase(i);
    }
    return l;
}

// Removes an element from the map.
// @returns The number of elements that have been removed from the map.
//          Since this is not a multimap itshould be 1 or 0.
TreeMap::size_type TreeMap::erase(const Key& key)
{
DEBUG std::cout<<"TreeMap::erase(const Key& key)"<<std::endl;
    iterator i=find(key);
    if(i!=end())
    {
        erase(i);
        return 1;
    }
    return 0;
}

// Erases all the elements of a map.
void TreeMap::clear( )
{
DEBUG std::cout <<"TreeMap::clear( )"<< std::endl;
    //erase(begin(),end());
    TreeNode* tmp=root;
    TreeNode* tmp2;
    while(tmp!=NULL)
    {
        if(tmp->left!=NULL)
        {
            tmp=tmp->left;
        }
        else if(tmp->right!=NULL)
        {
            tmp=tmp->right;
        }
        else if(tmp->left==NULL && tmp->right==NULL)
        {
            if(tmp->parent!=NULL)
            {
                tmp2=tmp;
                tmp=tmp->parent;
                if(tmp->left==tmp2)
                {
                    tmp->left=NULL;
                }
                else if(tmp->right==tmp2)
                {
                    tmp->right=NULL;
                }
                delete tmp2;
                detail->map_size--;
            }
            else
            {
                delete tmp;
                tmp=NULL;
                detail->map_size--;
            }
        }
    }
    root=NULL;
}

bool DFScmp(TreeNode* tmp1, TreeNode* tmp2)
{
    if(tmp1==NULL && tmp2==NULL)
        return true;
    if(tmp1->data.first!=tmp2->data.first || tmp1->data.second!=tmp2->data.second)
        return false;
    return DFScmp(tmp1->left,tmp2->left) && DFScmp(tmp1->right,tmp2->right);
}

bool TreeMap::struct_eq(const TreeMap& another) const
{
DEBUG2 std::cout <<"TreeMap::struct_eq(const TreeMap& another) const"<< std::endl;
    //iterator i=begin();
    //iterator j=another.begin();
    if(size()!=another.size())
    {
DEBUG std::cout<<"if(size()!=another.size())"<<std::endl;
        return false;
    }
//return true;
    /*while( i!=end() && j!=another.end() )
    {
DEBUG std::cout<<i->first<<" "<<j->first<<" "<<i->second<<" "<<j->second<<std::endl;
        if(i->first!=j->first || i->second!=j->second)//porownanie na tych samych pozycjach
        {
DEBUG std::cout<<"if(i->first!=j->first || i->second!=j->second)"<<std::endl;

            return false;
        }
        i++;
        j++;
    }
    return true;*/
    return DFScmp(root, another.root);
}

bool TreeMap::info_eq(const TreeMap& another) const
{
DEBUG2 std::cout <<"TreeMap::info_eq(const TreeMap& another) const"<< std::endl;
    iterator i;
    iterator j;
    if(size()!=another.size())//sa to mapy wiec jesli maja rozne rozmiary to musza roznic sie informacyjnie
    {
DEBUG std::cout<<"if(size()!=another.size())"<<std::endl;
        return false;
    }
//return true;
    for(i=begin(); i!=end(); ++i)
    {
        j=another.find(i->first);//znajdz klucz
        if(j==another.end())
        {
DEBUG std::cout<<"if(j==another.end())"<<std::endl;
            return false;
        }
        if(i->second!=j->second)//porownaj wartosci
        {
DEBUG std::cout<<"if(i->second!=j->second)"<<std::endl;
            return false;
        }
    }
    return true;
}

// preincrement
TreeMap::const_iterator& TreeMap::const_iterator::operator ++()
{
DEBUG std::cout<<"TreeMap::const_iterator::operator ++()"<<std::endl;
    TreeNode* tmp1;
    TreeNode* tmp2;
DEBUG std::cout<<"node: "<<node<<std::endl;
    if(node!=NULL)
    {
DEBUG print(node->data);
        if(node->right!=NULL)//jesli jest prawe dziecko
        {
DEBUG std::cout<<"if(node->right!=NULL)//jesli jest prawe dziecko"<<std::endl;
            tmp1=node->right;//jeden w prawo i w lewo az do konca
            while(tmp1!=NULL && tmp1->left!=NULL)
            {
                tmp1=tmp1->left;
            }
            node=tmp1;
DEBUG print(node->data);
//DEBUG system("pause");
            return *this;
        }
        else
        {
            if(node->parent!=NULL)//jesli jest rodzic
            {
DEBUG std::cout<<"if(node->parent!=NULL)//jesli jest rodzic"<<std::endl;
DEBUG std::cout<<"node->parent: "<<node->parent<<std::endl;
DEBUG print(node->parent->data);
                if(node==node->parent)//blad
                {
DEBUG std::cout<<"if(node==node->parent)//blad"<<std::endl;
                }
                if(node==node->parent->left)//jesli jestesmy lewym dzieckiem
                {
DEBUG std::cout<<"if(node==node->parent->left)//jesli jestesmy lewym dzieckiem"<<std::endl;
                    node=node->parent;//rodzic jest nastepny
DEBUG print(node->data);
//DEBUG system("pause");
                    return *this;
                }
                else if(node==node->parent->right)
                    //jesli jestesmy lisciem i prawym dzieckiem
                {
DEBUG std::cout<<"else if(node==node->parent->right)//jesli jestesmy lisciem i prawym dzieckiem"<<std::endl;
                    tmp1=node;
                    tmp2=node->parent;
                    while(tmp2!=NULL && tmp2->right==tmp1)
                    {
                        tmp1=tmp2;//idzemy w gore
                        tmp2=tmp2->parent;
                    }
                    node=tmp2;//albo zanalezlismy wyzsze rozgalezieni9e w prawo, albo jest end()
                    if(tmp2==NULL)
                    {
                        node=tmp1;//sentinel w root
                    }
DEBUG if(node!=NULL) print(node->data);
//DEBUG system("pause");
                    return *this;
                }
            }
            else//koniec w korzeniu
            {
                node=NULL;//end
DEBUG std::cout<<"return end"<<std::endl;
//DEBUG system("pause");
                return *this;
            }
        }
    }
DEBUG print(node->data);
DEBUG std::cout<<"return NULL ?"<<std::endl;
DEBUG system("pause");
    return *this;
}

// postincrement
TreeMap::const_iterator TreeMap::const_iterator::operator++(int)
{
DEBUG std::cout<<"TreeMap::const_iterator::operator++(int)"<<std::endl;
    const_iterator i=*this;
    ++(*this);
    return i;
}

// predecrement
TreeMap::const_iterator& TreeMap::const_iterator::operator--()
{
DEBUG std::cout<<"TreeMap::const_iterator::operator ++()"<<std::endl;
    TreeNode* tmp1;
    TreeNode* tmp2;
DEBUG std::cout<<"node: "<<node<<std::endl;
    if(node!=NULL)
    {
DEBUG print(node->data);
        if(node->left!=NULL)//jesli jest lewe dziecko
        {
DEBUG std::cout<<"if(node->left!=NULL)//jesli jest prawe dziecko"<<std::endl;
            tmp1=node->left;//jeden w lewo i w prawo az do konca
            while(tmp1!=NULL && tmp1->right!=NULL)
            {
                tmp1=tmp1->right;
            }
            if(tmp1!=NULL)
            node=tmp1;
DEBUG print(node->data);
//DEBUG system("pause");
            return *this;
        }
        else
        {
            if(node->parent!=NULL)//jesli jest rodzic
            {
DEBUG std::cout<<"if(node->parent!=NULL)//jesli jest rodzic"<<std::endl;
DEBUG std::cout<<"node->parent: "<<node->parent<<std::endl;
DEBUG print(node->parent->data);
                if(node==node->parent)//blad
                {
DEBUG std::cout<<"if(node==node->parent)//blad"<<std::endl;
                }
                if(node==node->parent->right)//jesli jestesmy prawym dzieckiem
                {
DEBUG std::cout<<"if(node==node->parent->right)//jesli jestesmy prawym dzieckiem"<<std::endl;
                    if(node->parent!=NULL)
                    node=node->parent;//rodzic jest nastepny
DEBUG print(node->data);
//DEBUG system("pause");
                    return *this;
                }
                else if(node==node->parent->left)
                    //jesli jestesmy lisciem i lewym dzieckiem
                {
DEBUG std::cout<<"else if(node==node->parent->left)//jesli jestesmy lisciem i lewym dzieckiem"<<std::endl;
                    tmp1=node;
                    tmp2=node->parent;
                    while(tmp2!=NULL && tmp2->left==tmp1)
                    {
                        tmp1=tmp2;//idzemy w gore
                        tmp2=tmp2->parent;
                    }
                    if(tmp2!=NULL)
                    node=tmp2;//albo zanalezlismy wyzsze rozgalezieni9e w lewo, albo jest begin()
DEBUG if(node!=NULL) print(node->data);
//DEBUG system("pause");
                    return *this;
                }
            }
            else//koniec w korzeniu
            {
                //node=NULL;//end
DEBUG std::cout<<"return begin"<<std::endl;
//DEBUG system("pause");
                return *this;
            }
        }
    }
    /*tmp1=TreeMap::root;
    tmp2=tmp1;
    while(tmp1!=NULL)
    {
        tmp2=tmp1;
        tmp1=tmp1->right;
    }
    node=tmp2;*/
DEBUG print(node->data);
DEBUG std::cout<<"return NULL ?"<<std::endl;
DEBUG system("pause");
    return *this;
}

// postdecrement
TreeMap::const_iterator TreeMap::const_iterator::operator--(int)
{
   ///@todo Implement this
    const_iterator i=*this;
    --(*this);
    return i;
}


/// Assignment operator copy the source elements into this object.
TreeMap& TreeMap::operator=(const TreeMap& other)
{
DEBUG std::cout<<"TreeMap::operator=(const TreeMap& other)"<<std::endl;
    if(this==&other)//w tym wypadku nic nie definjuje poprawnego zachowania operatora
        return *this;
    clear();
    //root = NULL;
    detail=new TreeMapDetail();
    detail->map_size=other.detail->map_size;
    root=DFScopy(other.root);
    /*iterator i=other.begin();
    while(i!=other.end())
    {
        unsafe_insert(i.node->data);
        i++;
    }*/
    return *this;
}

/// Returns an iterator addressing the first element in the map
/**< @todo jak wszystko bedzie dzialac to poprawwic przez dodanie first last */
TreeMap::iterator TreeMap::begin()
{
   ///@todo Implement this
DEBUG std::cout<<"TreeMap::begin()"<<std::endl;
    TreeNode* tmp=root;
    while(tmp!=NULL && tmp->left!=NULL)
    {
        tmp=tmp->left;
    }
    return iterator(tmp);
}

TreeMap::const_iterator TreeMap::begin() const
{
   ///@todo Implement this
DEBUG std::cout<<"TreeMap::begin() const"<<std::endl;
    TreeNode* tmp=root;
    while(tmp!=NULL && tmp->left!=NULL)
    {
        tmp=tmp->left;
    }
    return const_iterator(tmp);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::iterator TreeMap::end()
{
   ///@todo Implement this
DEBUG std::cout<<"TreeMap::end()"<<std::endl;
    return iterator(root);
}

/// Returns an iterator that addresses the location succeeding the last element in a map
TreeMap::const_iterator TreeMap::end() const
{
   ///@todo Implement this
DEBUG std::cout<<"TreeMap::end() const"<<std::endl;
    return iterator(root);
}

//////////////////////////////////////////////////////////////////////////////
// Tests
//////////////////////////////////////////////////////////////////////////////

/// A helper function that outputs a key-value pair.
void print(const std::pair<int, std::string>& p)
{
   std::cout<<p.first<<", "<<p.second<<std::endl;
}

#include <map>

/// The big mean test function ;)
void test()
{
   // A typedef used by the test.
   //typedef std::map<int, std::string> TEST_MAP;
   //typedef SmallMap<int, std::string> TEST_MAP;
   typedef TreeMap TEST_MAP;

   std::cout << "Testy uzytkownika" << std::endl;

        TEST_MAP m;

   m[2] = "Merry";
   m[4] = "Jane";
   m[8] = "Korwin";
   m[4] = "Magdalena";
   //system("PAUSE");
   std::cout << m.size() << std::endl;
   //system("PAUSE");
   std::cout << m.find(4)->second << std::endl;
   //system("PAUSE");


    for(TreeMap::iterator i=m.begin(); i!=m.end(); i++ )
    print(std::make_pair(i->first, i->second));
    system("PAUSE");
    //m.erase(4);
    //system("PAUSE");
    for(TreeMap::iterator i=m.begin(); i!=m.end(); i++ )
    print(std::make_pair(i->first, i->second));

    TEST_MAP m2;
    m2=m;

    std::cout << m.info_eq(m2) << std::endl;
    std::cout << m.struct_eq(m2) << std::endl;

    system("PAUSE");
    for(TreeMap::iterator i=m2.begin(); i!=m2.end(); i++ )
    print(std::make_pair(i->first, i->second));

    std::cout << "koniec" << std::endl;
   system("PAUSE");

}

//////////////////////////////////////////////////////////////////////////////
// main - jest w pliku /materialy/AISDI/z2/main.cc
//////////////////////////////////////////////////////////////////////////////

//int main()
//{
//   std::cout << "AISDI cwiczenie 4: wchodze do funkcji main." << std::endl;

//   test();
//   // Biblioteka z bardziej rygorystyczna wersja tych testow bedzie udostepniona na nastepnych zajeciach.
//   Test2();
//   //system("PAUSE");
//   return EXIT_SUCCESS;
//}
