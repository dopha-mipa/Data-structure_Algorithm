/* Hash Table */
#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 30

typedef struct _node {
    char* name;
    int studentID;
    char* major;
} Node;

static Node* hash_table[HASH_SIZE];

unsigned hash(char* key);
Node* find(char* key);
void put(char* name, int studentID, char* major);
int probe(char* key);
void print_table();

int main() {
    char* n1 = "MM";
    char* m1 = "CS";
    char* n2 = "AA";
    char* m2 = "CS";
    char* n3 = "BB";
    char* m3 = "CS";
    char* n4 = "VV";
    char* m4 = "CS";
    char* n5 = "EE";
    char* m5 = "CS";
    put(n1, 120205, m1);
    put(n2, 120205, m2);
    put(n3, 120205, m3);
    put(n4, 120205, m4);
    put(n5, 120205, m5);

    find(n1);
    find(n3);
    find(n5);

    print_table();

    return 0;
}

unsigned hash(char* key) {
    unsigned hashed_result;

    for (hashed_result = 0; *key != '\0'; key++) {
        hashed_result = *key + 31 * hashed_result;
        return hashed_result % HASH_SIZE;
    }
}

void put(char* name, int studentID, char* major) {
    unsigned result = hash(name);
    Node* student = (Node*) malloc(sizeof(Node));
    student->name = (char*) malloc(sizeof(char) * 20);
    student->major = (char*) malloc(sizeof(char) * 20);
    student->name = name;
    student->studentID = studentID;
    student->major = major;

    if (hash_table[result] == NULL) {  //비어있었다면
        hash_table[result] = student;
    } else {  //이미 있었다면
        result = probe(name);
        hash_table[result] = student;
    }
    printf("%d\t %s\n", result, name);
    return;  //그런데 왜 굳이 Node* 반환인지 아직 잘 모르겠어ㅇㅅㅇ
}

Node* find(char* key) {
    unsigned result = hash(key);
    if (key == NULL) {
        return NULL;
    }

    if (hash_table[result]->name == key) {
        printf("%d\t %s\t %d\t %s\n",
                    result, hash_table[result]->name, 
                    hash_table[result]->studentID,hash_table[result]->major);
        return hash_table[result];
    } else {
        //TODO : some set collisionㅜㅠ
        printf(".........");
        return NULL;
    }
}

int probe(char* key) {  // open addressing
    //1: linear probing
    unsigned result = hash(key);
    int linear = 1;
    int i;
    for(i = 0; i < HASH_SIZE; i++) {
        result = (result + linear) % HASH_SIZE;
        if (hash_table[result] == NULL) {
            return result;
        }
    }
    printf("Can't find room!");

    /*
    //2: quadratic probing
    unsigned result = hash(key);
    int quadratic = 2;
    int linear = 1;
    for(int i = 0; i < HASH_SIZE; i++) {
        result = (quadratic * result + linear) % HASH_SIZE;
        if (hash_table[result] == NULL) {
            return result;
        }
    }
    printf("Can't find room!");
    */
    /*
    //3: double hashing
    unsigned result = hash(key);
    for (int i = 0; i < HASH_SIZE; i++) {
        result = hash(&hash_table[result]);
        if (hash_table[result] == NULL) {
            return result;
        }
    }
    printf("Can't find room!");
    */
}

void print_table() {
    int i;
    for (i = 0; i < HASH_SIZE; i++) {
        if (hash_table[i] != NULL) {
            printf("%d\t %s\t %d\t %s\n",
                    i, hash_table[i]->name, 
                    hash_table[i]->studentID,hash_table[i]->major);
        }
    }
    return;
}
