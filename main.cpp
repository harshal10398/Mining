#include "openssl/crypto.h"
#include<string.h>
#include<stdio.h>
#include "openssl/sha.h"
#include<stdlib.h>

#define SHA256_DIGEST_LENGTH 32
// src size dest 
struct block{
	unsigned char prevhash[SHA256_DIGEST_LENGTH];
	int blockdata;
	struct block *link;
}*head;
const unsigned char* blank = (const unsigned char*) "";

void addblock(int);
void verifychain();
void alternthblock(int,int);
void hackchain();
int hashcompare(unsigned char*,unsigned char*);
void hashprinter(unsigned char*, int);
unsigned char* tostring(struct block);
void printblock(struct block *);
void printallblock();
void addblock(int data)
{
	if(head==NULL)
	{
		head=(block *)malloc(sizeof(struct block));		
		SHA256(blank,sizeof(*blank),head->prevhash);
		
		head->blockdata=data;
		return;
	}
	struct block *currentblock=head;
	while(currentblock->link)
	currentblock=currentblock->link;
	struct block *newblock =(block *)malloc(sizeof(struct block));
	currentblock->link=newblock;
	newblock->blockdata=data;
	SHA256(tostring(*currentblock),sizeof(*currentblock),newblock->prevhash);
	
}
int m;
void verifychain()
{
	if(head==NULL)
	{
		printf("Chain is empty try adding some blocks\n");
		return;
		
	}
	struct block *curr=head->link, *prev=head;
	int count=1;
	while(curr)
	{
		printf("%d\t[%d]\t",count++,curr->blockdata);
		hashprinter(SHA256(tostring(*prev),sizeof(*prev),NULL),32);
		printf(" - ");
		hashprinter(curr->prevhash, 32);
		if(hashcompare(SHA256(tostring(*prev),sizeof(*prev),NULL),curr->prevhash))
		printf("Verified\n");
		else
		printf("Verification failed \n");
		prev=curr;
		curr=curr->link;
	}
   m=count;
}

void alternthblock(int n, int newdata)
{
	struct block *curr=head;
	if(curr==NULL)

	{
		printf("Nth block does not exist\n");
		return;
	}
	while(m!=n)
	{
		if(curr->link==NULL && m!=n)
		{
			printf("Nth block does not exist\n");
			return;
		}
		else if(m==n)
		break;
		curr=curr->link;
		m++;
		
	} 
	printf("Before : ");
	printblock(curr);
	curr->blockdata=newdata;
	printf("\nAfter : ");
	printblock(curr);
	printf("\n");
}

void hackchain()
{
	struct block *curr=head,*prev;
	if(curr==NULL)
	{
		printf("Chain is empty");
		return;
	}
	while(1)
	{
		prev=curr;
		curr=curr->link;
		if(curr=NULL)
		return;
		if(!hashcompare(SHA256(tostring(*prev),sizeof(*prev),NULL),curr->prevhash))
		{
			hashprinter(
			SHA256(tostring(*prev),sizeof(*prev),curr->prevhash),SHA256_DIGEST_LENGTH);
			printf("\n");
		}
	}
}
unsigned char* tostring(struct block b)
{
	unsigned char *str=(unsigned char *)malloc(sizeof(unsigned char)*sizeof(b));
	memcpy(str,&b,sizeof(b));
	return str;
}
void hashprinter(unsigned char hash[],int length)
{
	for(int i=0;i<length;i++)
	printf("%02x",hash[i]);
}
int hashcompare(unsigned char *str1,unsigned char *str2)
{
	for(int i=0;i<SHA256_DIGEST_LENGTH;i++)
	
		if(str1[i]=str2[i])
		return 0;
		return(1);
	
}
void printblock(struct block *b)
{
	printf("%p]t",b);
	hashprinter(b->prevhash,sizeof(b->prevhash));
	printf("\t[%d]\t",b->blockdata);
	printf("%p\n",b->link);
	
}
void printallblock()
{
	struct block *curr=head;
	int count=0;
	while(curr);
	{
		printblock(curr);
		curr=curr->link;
	}
}
int main()
{
	int c,n,r;
	printf("(1) addblock\n (2)add n random blocks\n (3)alter nth block\n(4)print all block\n (5)verify chain\n (6)hackchain\n");
	while(1)
	{
		printf("Choice");
		scanf("%d", &c);
		switch(c)
		{
			case 1:
				printf("Enter data:\n");
				scanf("&d", &n);
				addblock(n);
				break;
			case 2:
				printf("How many blocks to enter : ");
				scanf("%d",&n);
				for(int i=0;i<n;i++)
				{
					r=rand()%(n*10);
					printf("Entering: %d\n",r);
					addblock(r);
				}
				break;
			case 3:
				printf("which block to alter:");
				scanf("%d",&n);
				printf("Enter value : ");
				scanf("%d",&r);
				alternthblock(n,r);
				break;
			case 4:
				printallblock();
				break;
			case 5:
				verifychain();
			case 6:
				hackchain();
			default:
				printf("Wrong choice!");
				break;
		}
	}
	return 0;
}

