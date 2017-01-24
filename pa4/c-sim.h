#ifndef C-SIM_H
#define C-SIM_H

struct Block_{
	long long blockTag; //not the tag bits, a custom marker for the blocks
	long long tag; //value used to compare two blocks aka to find a block
	long long set; //key
	long long blockOffset;
	int dirtyBit;
	int isNull;
	
};

struct CacheHashTable_{
	long long totalCount; //number of elements in the cache
	long long numberOfSets; //number of indices in sets hash table
	struct SetHashTable_ *sets;
};

struct SetHashTable_{
	long long numberOfFilledLines; //number of blocks that have something in them already
	long long numberOfLines; //number of indices in each blocks array
	int isFIFO;
	int isWT;
	struct Block_ *blocks;
	struct Queue_ *queues;
};

struct Queue_{
	struct Node_ *head;	
};

struct Node_{
	long long blockTag;
	struct Node_ *next;
};

struct CacheHashTable_* createCache(long long numOfSets, long long numOfLines, int isFIFO, int isWT);

void addToEnd(struct Queue_*, long long tag); //enqueue

void removeHead(struct Queue_*); //dequeue

void removeNode(struct Queue_*, long long tag); //dequeue a node that isn't the head

char* hexToBinary(char* hex);

void printCache(struct CacheHashTable_*);

void printQueue(struct Queue_*);
#endif
