/*
 * @Author: rubo
 * @Date: 2024-04-30 19:28:13
 * @LastEditors: HUAWEI-Ubuntu ruluy0205@163.com
 * @LastEditTime: 2024-04-30 19:54:15
 * @FilePath: /Skiplist-CPP/skiplist-Node.h
 * @Description: 
 */
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <mutex>
#include <fstream>


template <typename K, typename V>
class Node
{
public:
    Node() {}

    Node(K k, V v, int);

    ~Node();

    K get_key() const;

    V get_value() const;

    void set_value(V);

    Node<K, V> **forword;

    int node_level;

private:
    K key;
    V value;
};

template <typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level)
{
    this->key = k;
    this->value = v;
    this->node_level = level;

    this->forward = new Node<K, V> *[level + 1];
};

template <typename K, typename V>
Node<K, V>::~Node()
{
    delete[] forward;
};

template <typename K, typename V>
K Node<K, V>::get_key() const
{
    return key;
};

template <typename K, typename V>
V Node<K, V>::get_value() const
{
    return value;
};

template <typename K, typename V>
void Node<K, V>::set_value(V value){
    this->value = value;
}
