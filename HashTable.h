#ifndef HASH_TABLE
#define HASH_TABLE

/*
 *  HashTable.h
 *  Jonathan Mathew (jmathe12) and Cooper Bailey (cbaile07)
 *  12/3/24
 *
 *  CS 15 Proj 4: gerp
 *
 *  
 *
 */

#include <string>
#include <iostream>
#include <functional>

using namespace std;

template <typename keyType, typename valueType>
class HashTable{
public: 
    struct entry {
        keyType key;                //Insensitive word
        valueType value;            //Array of senesitive words
        bool empty;                 //To check if hash index is empty

        //default constructor to make instance empty
        entry() { empty = true; }

        //constructor with information all inside the struct
        entry(const keyType &keyInput, const valueType &valueInput){
            key = keyInput;
            value = valueInput;
            empty = false;
        }
    }; 
    //default hashTable constructor
    HashTable() {
        capacity = init_capacity;
        num_elements = 0;
        table = new entry[capacity];
    }

    //constructor with key, value
    HashTable(const keyType &key, const valueType &value) {
        capacity = init_capacity;
        num_elements = 0;
        table = new entry[capacity];
        insert(key, value);
    }

    ~HashTable() { delete [] table; }

    //insert into hash table
    void insert(const keyType &key, const valueType &value) {
        //create item
        rehash();
        entry item(key, value);
        int index = hashFunction(key) % capacity;

        while (not table[index].empty)
            index = (index + 1) % capacity;

        table[index] = item; 
        num_elements++;
    }


    entry *find(const keyType &key) {
        if(findHelper(key) == nullptr)
            return nullptr;
        else return findHelper(key);
    }
private: 
    entry *findHelper(const keyType &key){
        int index = hashFunction(key) % capacity;
        while (table[index].key != key) {
            index = (index + 1) % capacity; //Add one to index

            if (index == hashFunction(key) % capacity) 
                return nullptr;  //If at the end of the table
        }
        return &table[index];   //Else return the entry at key
    }
    void rehash() {
        float load_factor = (float)(num_elements/capacity);
        if (load_factor > .8) {
            int old_capacity = capacity;

            //Update capacity and create new table
            capacity = capacity * 2 + 2;
            entry *oldTable = table;
            table = new entry[capacity];

            for (int i = 0; i < old_capacity; i++) {
                while (not oldTable[i].empty)
                    insert(oldTable[i].key, oldTable[i].value);
            }
            delete [] oldTable;
        }
    }
    //Variables
    const int init_capacity = 30; 
    hash<keyType> hashFunction; 
    int capacity, num_elements;
    entry *table; 
};

#endif