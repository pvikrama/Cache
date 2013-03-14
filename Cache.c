/*Name: Pradeep Kumar Vikraman
 *ID  : pvikrama@andrew.cmu.edu
 */
#include "cachelab.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char inst;
long max,tag;
int S,E,B,hit=0,miss=0,eviction=0,size,set_mask=0,block_mask=0,set_bits;
unsigned int tag_mask;
struct cache_line
{
	int valid;
	long LRU;
	unsigned long tag;
};
struct cache_line cache[32][32];

int load_cache(long addr);
void initialize_cache();
void set_masks();

int main(int argc, char *argv[])
{
	int i=1;
	char *filename;
	filename = NULL;
	//The while loop is used to parse command line arguments
	while (i < argc && argv[i][0] == '-')
	{
		switch (argv[i][1])
		{
			case 's':
				/*atoi function is used because the value read from command 
				 *line is treated as a string even though its an int
				 */
				S=atoi(argv[i+1]);                 
				break;

			case 'E':
				E=atoi(argv[i+1]);
				break;

			case 'b':
				B=atoi(argv[i+1]);
				break;

			case 't':
				filename= argv[i+1];
				break;
			default:
				printf("Wrong Argument: %s\n", argv[i]);
				break;
		}
		i=i+2;
	}

	initialize_cache();
	set_masks();
    //Used to read file line by line 
	if(filename)
	{
		FILE *file = fopen (filename, "r" );
		if ( file != NULL )
		{
			long addr;
			/*fscanf was used because it reads acording to specified
			 *format load these values directly in variables
			 */
			while((fscanf(file," %c %lx,%d\n",&inst,&addr,&size))!=EOF)
			{
				if(inst=='L' || inst=='S')
				{
					load_cache(addr);
				}
				if(inst=='M')
				{
					load_cache(addr);
					load_cache(addr);
				}
				if(inst=='I')
				{
					continue;
				}		
			}
			printSummary(hit,miss,eviction);
			fclose ( file );
		}
		else
		{
			perror (filename); 
		}
	}
	return 0;
}

//sets the masks required to extract the set number and tag out of the address
void set_masks()
{
	int maskbit=1,s=S,b=B,maskbit1=1;
	while(s!=0)
	{
		set_mask=set_mask+maskbit;
		maskbit=maskbit<<1;
		s=s-1;
	}
	while(b!=0)
	{
		block_mask=block_mask+maskbit1;
		maskbit1=maskbit1<<1;
		b=b-1;
	}
	tag_mask=~(set_mask+block_mask);
	set_mask=set_mask<<B;
}

//initializes all cache elements to zero
void initialize_cache()
{
	int k,j;
	for(k=0;k<32;k++)
		for(j=0;j<32;j++)
			cache[k][j].valid=0;
			cache[k][j].tag=0;
			cache[k][j].LRU=0;
}

int load_cache(long addr1)
{
	int i,boolean=0,save;
	long min=1<<30;
	tag=addr1&tag_mask;
	set_bits=addr1 & set_mask;                   
	set_bits=set_bits>>B;
	tag=tag>>(S+B);			       
	//Checks each line in a set iteratively
	for(i=0;i<E;i++)
		/*boolean variable is used to decide when to do eviction
		 *max variable is used to keep track of most recently used value
		 *If tag bits match and valid is set,element found,increment hit counter
		 *Make it most recently used by putting max+1 value in LRU,set boolean
		 */
		if((cache[set_bits][i].tag==tag) && (cache[set_bits][i].valid ==1))
		{	
			hit++;	
			cache[set_bits][i].LRU=max+1;
			max=max+1;
			boolean=1;
			break;
		}
	    /*If cache line empty set valid bit and fill in tag bits
		 *Increment miss counter and make it most recently used,set boolean
		 */
		else if(cache[set_bits][i].valid==0) 
		{
			miss++;
			cache[set_bits][i].valid=1;
			cache[set_bits][i].tag=tag;
			cache[set_bits][i].LRU=max+1;
			max=max+1;  
			boolean=1;
			break;
		}
		else
		{   //else find the element with lease LRU value and save it
			if(cache[set_bits][i].LRU < min)
			{
				min=cache[set_bits][i].LRU;
				save=i;
			}
		}
	/*Evict the element with least LRU, modify tag and increment LRU value 
	 *Increment both the eviction and miss counter
	 */
	if (boolean == 0)
	{
		miss++;
		eviction++;
		cache[set_bits][save].tag=tag;
		cache[set_bits][save].LRU=max+1;
		max=max+1;
	}
	return 0;
}
