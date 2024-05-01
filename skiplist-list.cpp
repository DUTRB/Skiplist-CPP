#include <iostream>
#include <cstdlib>
#include <cmath>
#include <mutex>
#include <fstream>
#include "skiplist-Node.h"

#define STORE_FILE "store/dumpFile"

std::mutex mtx;
std::string delimiter = ":";

// 跳表类实现
template <typename K, typename V>
class SkipList
{
public:
    SkipList(int);
    ~SkipList();
    int get_random_level();
    Node<K, V> *create_node(K, V, int);
    int insert_element(K, V);
    void display_list();
    bool search_element(K);
    void delete_element(K);
    void dump_file();
    void load_file();
    // 递归删除节点
    void clear(Node<K, V> *);
    int size();

private:
    int max_level;
    int skip_list_level;
    Node<K, V> *header;
    std::ofstream file_writer;
    std::ifstream file_reader;

    int element_count;
};

template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level)
{
    this->max_level = max_level;
    this->skip_list_level = 0;
    this->element_count = 0;

    K k,
        V v;
    this->header = new Node<K, V>(k, v, max_level);
};

template <typename K, typename V>
SkipList<K, V>::~SkipList()
{
    if (file_writer.is_open())
    {
        file_writer.close();
    }
    if (file_reader.is_open())
    {
        file_reader.close();
    }
    // 递归删除跳表
    if (header->forward[0] != nullptr)
    {
        clear(header->forward[0]);
    }
    delete (header);
};

template <typename K, typename V>
void SkipList<K, V>::clear(Node<K, V> *cur)
{
    if (cur->forword[0] != nullptr)
    {
        clear(cur->forword[0]);
    }
    delete cur;
}

template <typename K, typename V>
int SkipList<K, V>::get_random_level()
{
    int k = 1;
    while (rand() % 2)
    {
        k++;
    }
    k = (k < max_level) ? k : max_level;
    return k;
}

/* ---------------------------------- 跳表操作 ---------------------------------- */
template <typename K, typename V>
Node<K, V> *SkipList<K, V>::create_node(const K k, const V v, int level)
{
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n;
}

template <typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value)
{
    mtx.lock();
    Node<K, V> *current = this->header;

    // 创建 update 数组并初始化
    Node<K, V> *update[max_level + 1];
    memset(update, 0, sizeof(Node<K, V> *) * (max_level + 1));

    for (int i = skip_list_level; i >= 0; --i)
    {
        while (current->forword[i] != nullptr && current->forword[i]->get_value() < k)
        {
            current = current->forword[i];
        }
        update[i] = current;
    }
    current = current->forword[0];

    if (current != nullptr && current->get_key() == key)
    {
        std::cout << "key: "
                  << ", exist" << set::endl;
        mtx.unlock();
        return 1;
    }

    if (current == nullptr || current->get_key() != key)
    {
        int random_level = get_random_level();

        if (random_level > skip_list_level)
        {
            for (int i = skip_list_level; i < random_level + 1; i++)
            {
                update[i] = header;
            }
            skip_list_level = random_level;
        }

        Node<K, V> *insert_node = create_node(key, value, random_level);
        for (int i = 0; i <= random_level; ++i)
        {
            insert_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = insert_node;
        }
        std::cout << "Successfully inserted key:" << key << ", value"
                  << value << std::endl;
        element_count++;
    }
    mtx.unlock();
    return 0;
}

template <typename K, typename V>
void SkipList<K, V>::delete_element(K key)
{
    mtx.lock();
    Node<K, V> *current = this->header;
    NOde<K, V> *update[max_level + 1];
    memset(update, 0, sizeof(Node<K, V> *) * (max_level + 1));

    for (int i = skip_list_level; i >= 0; i--)
    {
        while (current->forward[i] != nullptr && current->forword[i]->get_value() < key)
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != nullptr && current->get_key() == key)
    {
        for (int i = 0; i <= skip_list_level; i++)
        {
            if (update[i]->forward[i] != current)
                break;
            update[i]->forward[i] = current->forward[i];
        }

        // 若删除节点后该层无元素 则调整当前层数
        while (skip_list_level > 0 && header->forward[skip_list_level] == 0)
        {
            skip_list_level--;
        }

        std::cout << "Successfully delete key " << key << std::endl;
        delete current;
        element_count--;
    }
    mtx.unlock();
    return;
}

template <typename K, typename V>
bool SkipList<K, V>::search_element(K key)
{
    std::cout << "Search_element---------" << std::endl;
    Node<K, v> *current = header;

    for (int i = skip_list_level; i >= 0; i--)
    {
        while ((current->forward[i] && current->forword[i]->get_key() < key))
        {
            current = current->forward[i];
        }
        
    }

    current = current->forward[0];
    if(current && current->get_key() == key){
        std::cout << "Found key: " << key << ", value" << current->get_value() << std::endl;
        return true;
    }

    std::cout << " Not found key: " << key <<std::endl;
    return false;
}

template<typename K, typename V>
void SkipList<K, V>::display_list(){
    std::cout << "\n*******SKip List*********" << "\n";
    for(int i = 0; i <=skip_list_level; ++i){
        Node<K, V>* node = this->header->forward[i];
        std::cout << "Level " << i << ":";
        while(node != nullptr){
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            node = node->forword[i];
        }
        std::cout << std::endl;
    }
}

template<typename K, typename V>
int SkipList<K, V>::size(){
    return element_count;
}