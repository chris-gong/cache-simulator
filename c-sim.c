#include "stdio.h"
#include "c-sim.h"
#include "stdlib.h"
#include "string.h"
#include <math.h>
typedef struct CacheHashTable_ CacheHashTable;

typedef struct Block_ Block;

typedef struct SetHashTable_ SetHashTable;

typedef struct Queue_ Queue;

typedef struct Node_ Node;

CacheHashTable* createCache(long long numOfSets, long long numOfLines, int isFIFO, int isWT){
	CacheHashTable* cache = (CacheHashTable *)malloc(sizeof(CacheHashTable));
	cache->sets = (SetHashTable *)malloc(sizeof(SetHashTable) * numOfSets);
	
	cache->numberOfSets = numOfSets;
	cache->totalCount = 0;
	cache->sets->numberOfLines = numOfLines;
	cache->sets->isFIFO = isFIFO;
	cache->sets->isWT = isWT;
	long long i;
	long long j;
	for(i = 0; i < cache->numberOfSets; i++){
		cache->sets[i].blocks = (Block *)malloc(sizeof(Block) * numOfLines);
		cache->sets[i].queues = (Queue *)malloc(sizeof(Queue));
		cache->sets[i].queues->head = NULL;
		cache->sets[i].numberOfFilledLines = 0;
		for(j = 0; j < cache->sets->numberOfLines; j++){
			cache->sets[i].blocks[j].blockTag = j;
			cache->sets[i].blocks[j].isNull = 1;//initializing null blocks
			addToEnd(cache->sets[i].queues, j);
		}
	}
	return cache;
}

void addToEnd(Queue* queue, long long tag){
	if(queue->head == NULL){
		Node *node = (Node *)malloc(sizeof(Node));
		node->blockTag = tag;
		node->next = NULL;
		queue->head = node;
	}
	else{
		Node *ptr = queue->head;
		while(ptr->next != NULL){
			ptr = ptr->next;
		}
		Node *node = (Node *)malloc(sizeof(Node));
		node->blockTag = tag;
		node->next = NULL;
		ptr->next = node;
	}
}

void removeHead(Queue* queue){
	queue->head = queue->head->next;
}

void removeNode(Queue* queue, long long tag){
	long long i;
	Node *node = queue->head;
	//printf("%llu\n", tag);
	//printQueue(queue);
	if(node->blockTag == tag){//remove head
		queue->head = queue->head->next;
		return;
	}
	while(node->next != NULL){//remove node in the middle of queue or at the end
		//printf("b\n");
		if(node->next->blockTag == tag){
			break;
		}
		node = node->next;
	}
	//printf("c\n");
	node->next = node->next->next;
}
char* hexToBinary(char* hex){
	char *binary = (char *)malloc(sizeof(char) * strlen(hex) * 4);
	int i;
	
	for(i = 0; i < strlen(hex); i++){
		switch(hex[i]){
			case '0':
				strcat(binary,"0000");
				break;
		
			case '1':
				strcat(binary,"0001");
				break;
			case '2':
				strcat(binary,"0010");
				break;
			case '3':
				strcat(binary,"0011");
				break;
			case '4':
				strcat(binary,"0100");
				break;
			case '5':
				strcat(binary,"0101");
				break;
			case '6':
				strcat(binary,"0110");
				break;
			case '7':
				strcat(binary,"0111");
				break;
			case '8':
				strcat(binary,"1000");
				break;
			case '9':
				strcat(binary,"1001");
				break;
			case 'a':
				strcat(binary,"1010");
				break;
			case 'b':
				strcat(binary,"1011");
				break;
			case 'c':
				strcat(binary,"1100");
				break;
			case 'd':
				strcat(binary,"1101");
				break;
			case 'e':
				strcat(binary,"1110");
				break;
			case 'f':
				strcat(binary,"1111");
				break;
			default:
				break;
		}
		/*
		if(hex[i] == '0'){
			strcat(binary,"0000");
		}
		else if(hex[i] == '1'){
			strcat(binary,"0001");
		}
		else if(hex[i] == '2'){
			strcat(binary,"0010");
		}
		else if(hex[i] == '3'){
			strcat(binary,"0011");
		}
		else if(hex[i] == '4'){
			strcat(binary,"0100");
		}
		else if(hex[i] == '5'){
			strcat(binary,"0101");
		}
		else if(hex[i] == '6'){
			strcat(binary,"0110");
		}
		else if(hex[i] == '7'){
			strcat(binary,"0111");
		}
		else if(hex[i] == '8'){
			strcat(binary,"1000");
		}
		else if(hex[i] == '9'){
			strcat(binary,"1001");
		}
		else if(hex[i] == 'a'){
			strcat(binary,"1010");
		}
		else if(hex[i] == 'b'){
			strcat(binary,"1011");
		}
		else if(hex[i] == 'c'){
			strcat(binary,"1100");
		}
		else if(hex[i] == 'd'){
			strcat(binary,"1101");
		}
		else if(hex[i] == 'e'){
			strcat(binary,"1110");
		}
		else if(hex[i] == 'f'){
			strcat(binary,"1111");
		}*/
	}
	return binary;
}

void printCache(CacheHashTable* cache){
	long long i;
	long long j;
	//printf("%llu\n", cache->numberOfSets);
	for(i = 0; i < cache->numberOfSets; i++){
		printf("Set %llu:\n", i); 
		//fflush(stdout);
		//printf("hi3");
		//printf("%llu\n", cache->sets[i].numberOfFilledLines);
		for(j = 0; j < cache->sets[i].numberOfFilledLines; j++){
			printf("Line %llu: tag: %llu\n", j, cache->sets[i].blocks[j].tag);
			//fflush(stdout);
			//printf("hi4");
		}
		//printf("%llu\n", i);
	}
	
	//printf("eeeeeeee");
	//printf("hi");
}
long log2n(long long n){
        return (n>1)? 1 + log2n(n/2): 0;
}
void printQueue(Queue* queue){
	Node* ptr = queue->head;
	while(ptr != NULL){
		printf("%llu\n", ptr->blockTag);
		ptr = ptr->next;
	}
}

int main(int argc, char **argv){
	char *cache_Size = argv[1];
	char *set_Associativity = argv[2];
	char *block_Size = argv[3];
	char *replacement_Policy = argv[4];
	char *write_Policy = argv[5];
	char *traceFile = argv[6];

	long long cacheSize = atoi(cache_Size); //C
	long long setAssociativity = 0; //E
	long long numberOfSets = 0; //S
	long long blockSize = atoi(block_Size); //B
	int isFIFO;
	int isWT;

	long long reads = 0;
	long long writes = 0;
	long long hits = 0;
	long long misses = 0;
	if(strcmp(set_Associativity, "direct") == 0){//checks if 2nd parameter is "direct"
		setAssociativity = 1;
		numberOfSets = cacheSize/(blockSize * setAssociativity);
	}
	else if(strcmp(set_Associativity, "assoc") == 0){
		numberOfSets = 1;
		setAssociativity = cacheSize/blockSize;

	}
	else{
		char *temp = set_Associativity+6;
		setAssociativity = atoi(temp);
		numberOfSets = cacheSize/(blockSize * setAssociativity);
	}
	if(strcmp(replacement_Policy, "FIFO") == 0){
		isFIFO = 1;
	}
	else{
		isFIFO = 0;
	}
	if(strcmp(write_Policy, "wt") == 0){
		isWT = 1;
	}
	else{
		isWT = 0;
	}

	//printf("%s", traceFile);
	FILE *file = fopen(traceFile, "r");
	if(!file){
		printf("error");
		return 0;
	}
	char buffer[1000];
	
	CacheHashTable* cache = createCache(numberOfSets, setAssociativity, isFIFO, isWT);
	if(fgets(buffer, 1000, file) == NULL){
		printf("error");
		return 0;
	}
	int count = 10;
	rewind(file);
	//printf("%llu %llu %llu\n", numberOfSets, setAssociativity, blockSize);
	while(fgets(buffer, 1000, file) != NULL){
		//printf("anyone");
		char line[1000];
		char *ip;
		char *readOrWrite;
		char *address;
		char *binary;
		
		strcpy(line, &buffer[0]);
		char check[5]; //one extra for the null terminating character
		
		strncpy(check, line, 4);
		check[4] = '\0';//check to see if we've reached end of text file, "#eof", by taking a substring
		
		if(strcmp(check, "#eof") == 0){
			break;
		}
		ip = strtok(line, " ");
		
		readOrWrite = strtok(NULL, " ");
		address = strtok(NULL, "\n");
		address = address + 2;
		if(strlen(address) < 8){
			int numOfZeros = 8 - strlen(address);
			char paddedZeros[9];
			int i;
			for(i = 0; i < numOfZeros; i++){
				paddedZeros[i] = '0';
			}
			strncpy(paddedZeros + i, address, strlen(address));
			paddedZeros[8] = '\0';
			//printf("%s\n", paddedZeros);
			address = paddedZeros;
		}
		binary = hexToBinary(address);
		double a = 1;
		long long s = (long long)log2n(numberOfSets);
		long long b = (long long)log2n(blockSize);
		long long t = strlen(binary) - (s + b); //number of tag bits in 32-bit architecture, m = 32
							 //but in this case the address is 32, so m = strlen(address), 4 * 8 = 32
		char tagBits[t + 1];//extra bit for null-terminating character
		char setBits[s + 1];
		char blockOffsetBits[b + 1];
		//set bits in the middle
		
		strncpy(tagBits, binary, t);
		strncpy(setBits, binary + t, s);
		strncpy(blockOffsetBits, binary + t + s, b);
		
		//set bits at the very beginning, tag bits in the middle
		/*
		strncpy(setBits, binary, s);
		strncpy(tagBits, binary + s, t);
		strncpy(blockOffsetBits, binary + t + s, b);*/
		tagBits[t] = '\0';
		setBits[s] = '\0';
		blockOffsetBits[b] = '\0';
		//printf("%llu %llu %llu\n", s, t, b);
		//printf("%s\n", address);
		if(strcmp(readOrWrite, "R") == 0){
			//printf("herro");
			//fflush(stdout);
			long long setNum = (long long)strtol(setBits, NULL, 2);
			long long blockOffsetNum = (long long)strtol(blockOffsetBits, NULL, 2);
			long long tagNum = (long long)strtol(tagBits, NULL, 2);
			long long i;
			//printf("%llu %s %s %llu %llu\n", tagNum, tagBits, address, s, b);
			int exists = 0;
			//printf("%s\n", tagBits);
			//printf("%s %llu", blockOffsetBits, b);
			for(i = 0; i < cache->sets->numberOfLines; i++){ //check through each line in the set
				//printf("%llu %llu\n", cache->sets[setNum].blocks[i].tag, tagNum);
				//printf("hi");
				if(cache->sets[setNum].blocks[i].isNull || cache->sets[setNum].blocks[i].tag != tagNum){
					continue;
				}
				else{
					//printf("%llu %llu\n", cache->sets[setNum].blocks[i].tag, tagNum);
					exists = 1;
					break;
				}
			}
			if(exists){
				hits++;
				if(!cache->sets->isFIFO){
					removeNode(cache->sets[setNum].queues, i);
					addToEnd(cache->sets[setNum].queues, i);
				}
				//printf("%s\n", tagBits);
			}
			else{
				//printf("hello");
				misses++;
				reads++;
				if(cache->sets->isFIFO){
					long long blockTag = cache->sets[setNum].queues->head->blockTag;
					if(cache->sets[setNum].numberOfFilledLines != cache->sets->numberOfLines){ //initializing empty block
						cache->sets[setNum].blocks[blockTag].isNull = 0;
						cache->sets[setNum].blocks[blockTag].tag = tagNum;
						cache->sets[setNum].blocks[blockTag].set = setNum;
						cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
						cache->sets[setNum].numberOfFilledLines++;
						if(!cache->sets->isWT){
							cache->sets[setNum].blocks[blockTag].dirtyBit = 0;
						}
						removeHead(cache->sets[setNum].queues);
						addToEnd(cache->sets[setNum].queues, blockTag);
					}
					else{
						if(cache->sets->isWT){
							cache->sets[setNum].blocks[blockTag].tag = tagNum;
							cache->sets[setNum].blocks[blockTag].set = setNum;
							cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
							
						}
						else{
							cache->sets[setNum].blocks[blockTag].tag = tagNum;
							cache->sets[setNum].blocks[blockTag].set = setNum;
							cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
							if(cache->sets[setNum].blocks[blockTag].dirtyBit == 1){
								writes++;
								cache->sets[setNum].blocks[blockTag].dirtyBit = 0;
							}
						}
						
						removeHead(cache->sets[setNum].queues);
						addToEnd(cache->sets[setNum].queues, blockTag);
					}
				}
				else{
					//printf("hi");
					long long blockTag = cache->sets[setNum].queues->head->blockTag;
					if(cache->sets[setNum].numberOfFilledLines != cache->sets->numberOfLines){ //initializing empty block
						cache->sets[setNum].blocks[blockTag].isNull = 0;
						cache->sets[setNum].blocks[blockTag].tag = tagNum;
						cache->sets[setNum].blocks[blockTag].set = setNum;
						cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
						cache->sets[setNum].numberOfFilledLines++;
						if(!cache->sets->isWT){
							cache->sets[setNum].blocks[blockTag].dirtyBit = 0;
						}
						//printf("hi1");
						removeHead(cache->sets[setNum].queues);
						addToEnd(cache->sets[setNum].queues, blockTag);
						//printf("hi2");
						//printQueue(cache->sets[setNum].queues);
					}
					else{
						if(cache->sets->isWT){
							cache->sets[setNum].blocks[blockTag].tag = tagNum;
							cache->sets[setNum].blocks[blockTag].set = setNum;
							cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
							
						}
						else{
							cache->sets[setNum].blocks[blockTag].tag = tagNum;
							cache->sets[setNum].blocks[blockTag].set = setNum;
							cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
							if(cache->sets[setNum].blocks[blockTag].dirtyBit == 1){
								writes++;
								cache->sets[setNum].blocks[blockTag].dirtyBit = 0;
							}
						}
						//printf("hi1");
						removeHead(cache->sets[setNum].queues);
						addToEnd(cache->sets[setNum].queues, blockTag);
						//printf("hi2");
					}
				}
			}
			
		}
		else{
			long long setNum = (long long)strtol(setBits, NULL, 2);
			long long blockOffsetNum = (long long)strtol(blockOffsetBits, NULL, 2);
			long long tagNum = (long long)strtol(tagBits, NULL, 2);
			long long i;
			int exists = 0;
			for(i = 0; i < cache->sets->numberOfLines; i++){ //check through each line in the set
				//printf("hi");
				if(cache->sets[setNum].blocks[i].isNull || cache->sets[setNum].blocks[i].tag != tagNum){
					continue;
				}
				else{
					exists = 1;
					//printf("%llu %llu\n", cache->sets[setNum].blocks[i].tag, tagNum);
					break;
				}
			}
			if(exists){
				hits++;
				//printf("b\n");
				if(cache->sets->isWT){
					writes++;
				}
				else{
					cache->sets[setNum].blocks[i].dirtyBit = 1;
				}
				if(!cache->sets->isFIFO){
					//printf("%llu\n", i);
					//printf("c\n");
					//printQueue(cache->sets[setNum].queues);
					removeNode(cache->sets[setNum].queues, i);
					addToEnd(cache->sets[setNum].queues, i);
				}
			}
			else{
				misses++;
				reads++;
				if(cache->sets->isFIFO){
					long long blockTag = cache->sets[setNum].queues->head->blockTag;
					if(cache->sets[setNum].numberOfFilledLines != cache->sets->numberOfLines){ //initializing empty block
						cache->sets[setNum].blocks[blockTag].isNull = 0;
						cache->sets[setNum].blocks[blockTag].tag = tagNum;
						cache->sets[setNum].blocks[blockTag].set = setNum;
						cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
						cache->sets[setNum].numberOfFilledLines++;
						if(!cache->sets->isWT){
							cache->sets[setNum].blocks[blockTag].dirtyBit = 1;
						}
						else{
							writes++;
						}
						removeHead(cache->sets[setNum].queues);
						addToEnd(cache->sets[setNum].queues, blockTag);
					}
					else{
						if(cache->sets->isWT){
							cache->sets[setNum].blocks[blockTag].tag = tagNum;
							cache->sets[setNum].blocks[blockTag].set = setNum;
							cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
							writes++;
						}
						else{
							cache->sets[setNum].blocks[blockTag].tag = tagNum;
							cache->sets[setNum].blocks[blockTag].set = setNum;
							cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
							if(cache->sets[setNum].blocks[blockTag].dirtyBit == 1){
								writes++;
							}
							else{
								cache->sets[setNum].blocks[blockTag].dirtyBit = 1;
							}
						}
						removeHead(cache->sets[setNum].queues);
						addToEnd(cache->sets[setNum].queues, blockTag);
					}
				}
				else{
					long long blockTag = cache->sets[setNum].queues->head->blockTag;
					//printf("%llu\n", blockTag);
					if(cache->sets[setNum].numberOfFilledLines != cache->sets->numberOfLines){ //initializing empty block
						cache->sets[setNum].blocks[blockTag].isNull = 0;
						cache->sets[setNum].blocks[blockTag].tag = tagNum;
						cache->sets[setNum].blocks[blockTag].set = setNum;
						cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
						cache->sets[setNum].numberOfFilledLines++;
						if(!cache->sets->isWT){
							cache->sets[setNum].blocks[blockTag].dirtyBit = 1;
						}
						else{
							writes++;
						}
						removeHead(cache->sets[setNum].queues);
						addToEnd(cache->sets[setNum].queues, blockTag);
					}
					else{
						if(cache->sets->isWT){
							cache->sets[setNum].blocks[blockTag].tag = tagNum;
							cache->sets[setNum].blocks[blockTag].set = setNum;
							cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
							writes++;
						}
						else{
							cache->sets[setNum].blocks[blockTag].tag = tagNum;
							cache->sets[setNum].blocks[blockTag].set = setNum;
							cache->sets[setNum].blocks[blockTag].blockOffset = blockOffsetNum;
							if(cache->sets[setNum].blocks[blockTag].dirtyBit == 1){
								writes++;
							}
							else{
								cache->sets[setNum].blocks[blockTag].dirtyBit = 1;
							}
						}
						removeHead(cache->sets[setNum].queues);
						addToEnd(cache->sets[setNum].queues, blockTag);
					}
				}
			}
		}
		/*
		if(count > 0){
			printCache(cache);
			
		}
		count--;*/
		//fflush(stdout);
		//printf("hi1");
		/*
		printf("Memory reads: %llu\n", reads);
		printf("Memory writes: %llu\n", writes);
		printf("Cache hits: %llu\n", hits);
		printf("Cache misses: %llu\n", misses);*/
	}	
	printf("Memory reads: %llu\n", reads);
	printf("Memory writes: %llu\n", writes);
	printf("Cache hits: %llu\n", hits);
	printf("Cache misses %llu\n", misses);
	fclose(file);
	return 1;
}
