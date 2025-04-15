#ifndef TABLE
#define TABLE

/*
 *  table.h
 *  Jonathan Mathew (jmathe12) and Cooper Bailey (cbaile07)
 *  12/3/24
 *
 *  CS 15 Proj 4: gerp
 * This file creates a hash table that takes in a keyType and valueType and
 * indexes them based on the key
 *
 *  
 *
 */

#include <string>
#include <iostream>
#include <functional>

using namespace std;

template <typename keyType, typename valueType>
class table{
public: 
    struct entry {
        keyType key;                //Insensitive word
        valueType value;            //Array of senesitive words
        bool empty;                 //To check if hash index is empty

        //default constructor to make instance empty
        entry() { empty = true; }

        //constructor with information all inside the struct
        // entry(const keyType &key, const valueType &value){
        //     key = key;
        //     value = value;
        //     empty = false;
        // }

        entry(const keyType &key, const valueType &value)
            : key(key), value(value), empty(false) {}

    }; 

    //default hashTable constructor
    table(){
        capacity = init_capacity;
        num_elements = 0;
        hash_table = new entry[capacity];
    }

    //constructor with key, value
    table(const keyType &key, const valueType &value) {
        capacity = init_capacity;
        num_elements = 0;
        hash_table = new entry[capacity];
        insert(key, value);
    }

    ~table() { delete [] hash_table; }

    //insert into hash table
    void insert(const keyType &key, const valueType &value) {
        //create item
        rehash();
        entry item(key, value);
        int index = hashFunction(key) % capacity;

        while (not hash_table[index].empty)
            index = (index + 1) % capacity;

        hash_table[index] = item; 
        num_elements++;
    }


    entry *find(const keyType &key) {
        int index = hashFunction(key) % capacity;

        if (hash_table[index].empty) 
            return nullptr;
        
        else {
            while (hash_table[index].key != key) {
                if (hash_table[index].key.empty()) //If key is null
                    return nullptr;

                index = (index + 1) % capacity; //Add one to index

                if (index == hashFunction(key) % capacity) //If at the end of the
                    return nullptr;                       //table
            }
            return &hash_table[index];               //Else return the entry at key
        }
    }

private: 
    void rehash() {
        float load_factor = static_cast<float>(num_elements) / capacity;
        if (load_factor > .8) {
            int old_capacity = capacity;

            //Update capacity and create new table
            capacity = capacity * 2 + 2;
            entry *oldTable = hash_table;
            hash_table = new entry[capacity];
            num_elements = 0;
            for (int i = 0; i < old_capacity; i++) {
                if (not oldTable[i].empty)
                    insert(oldTable[i].key, oldTable[i].value);
            }
            delete [] oldTable;
        }
    }
    //Variables
    const int init_capacity = 20; 
    hash<keyType> hashFunction; 
    int capacity, num_elements;
    entry *hash_table; 
};

#endif