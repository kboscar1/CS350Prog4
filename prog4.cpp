#include <algorithm>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int * no_locality();
int * eighty_twenty();
int * looping();
float fifo(int cache_size, int * pages);
float random(int cache_size, int * pages);
float lru(int cache_size, int * pages);
float clock(int cache_size, int * pages);
float optimal(int cache_size, int * pages);

int main(int argc, char ** argv){
  if(argc != 7){
    cout << "Usage: ./prog4swap [-m size-of-memory] [-r replacement-policy] [-w workload]" << ndl;
    exit(1);
  }

  int cache_size = atoi(argv[2]);
  string replacement_policy = argv[4];
  string workload = argv[6];
  
  if(replacement_policy == "OPT" && workload  == "No-locality"){
    optimal(cache_size, no_locality());
  } else if(replacement_policy == "OPT" && workload  == "80-20"){
    optimal(cache_size, eighty_twenty());
  } else if (replacement_policy == "OPT" && workload  == "Looping"){
    optimal(cache_size, looping());
  }

  if(replacement_policy == "LRU" && workload  == "No-locality"){
    lru(cache_size, no_locality());
  } else if(replacement_policy == "LRU" && workload  == "80-20"){
    lru(cache_size, eighty_twenty());
  } else if (replacement_policy == "LRU" && workload  == "Looping"){
    lru(cache_size, looping());
  }

  if(replacement_policy == "FIFO" && workload  == "No-locality"){
    fifo(cache_size, no_locality());
  } else if(replacement_policy == "FIFO" && workload  == "80-20"){
    fifo(cache_size, eighty_twenty());
  } else if (replacement_policy == "FIFO" && workload  == "Looping"){
    fifo(cache_size, looping());
  }

  if(replacement_policy == "Rand" && workload  == "No-locality"){
    random(cache_size, no_locality());
  } else if(replacement_policy == "Rand" && workload  == "80-20"){
    random(cache_size, eighty_twenty());
  } else if (replacement_policy == "Rand" && workload  == "Looping"){
    random(cache_size, looping());
  }
  
  if(replacement_policy == "Clock" && workload  == "No-locality"){
    clock(cache_size, no_locality());
  } else if(replacement_policy == "Clock" && workload  == "80-20"){
    clock(cache_size, eighty_twenty());
  } else if (replacement_policy == "Clock" && workload  == "Looping"){
    clock(cache_size, looping());
  }
  
  
  return 0;
}

int * no_locality(){
  int i;
  int *pages = new int[10000];

  for(i = 0; i < 10000; i++){
    pages[i] = rand()%(100) + 1;
  }
  /*
  for(i = 0; i< 10000; i++){
    cout << pages[i] << " " << endl;
  }
  */
  
  return pages;
}

int * eighty_twenty(){
  int i, num;
  int *pages = new int[10000];
  
  for(i = 0; i < 10000; i++){
    num = rand()%100 + 1;

    if(num <= 80){
      pages[i] = rand()%20 + 1;
    } else {
      pages[i] = rand()%80 + 1;
    }
  }

  /*  for(i = 0; i< 10000; i++){
    cout << pages[i] << " " << endl;
  }
  */
  return pages;
}

int * looping(){
  int i, num = 0;
  int *pages = new int[10000];

  for(i = 0; i < 10000; i++){
    if(num > 49){
      num = 0;
    }
    
    pages[i] = num;
    num++;
  }
  
  return pages;
}

float fifo(int cache_size, int * pages){
  if(cache_size == 0){
    return 0.0;
  }

  int * cache = new int[10000];
  int i, j;
  int page;
  float hits = 0.0;
  bool found;
  int cache_index = 0;
  
  for(i = 0; i < cache_size; i++){
    cache[i] = 0;
  }

  for(i = 0; i < 10000; i++){
    page = pages[i];
    found = false;

    for(j = 0; j < cache_size; j++){
      if(page == cache[j]){
	found = true;
	hits++;
	break;
      }
    }

    if(!found){
      cache[cache_index] = page;
      cache_index++;
    }

    if(cache_index == cache_size){
      cache_index = 0;
    }    
  }
  return (hits/(float)10000.0) * 100;
}

float random(int cache_size, int * pages){
  if(cache_size == 0){
    return 0.0;
  }

  int i, j;
  int page;
  float hits = 0.0;
  int * cache = new int[cache_size];
  bool found;
  
  for(i = 0; i < cache_size; i++){
    cache[i] = 0;
  }

  for(i = 0; i < 10000; i++){
    found = false;
    page = pages[i];

    for(j = 0; j < cache_size; j++){
      if(page == cache[j]){
	found = true;
	hits++;
	break;
      }

      if(!found){
	cache[rand()%cache_size] = page;
      }
    }

    return (hits/10000.0) * 100;
  }
}

float lru(int cache_size, int * pages){
  if(cache_size == 0){
    return 0;
  }
  
  int i, j;
  int page;
  float hits = 0.0;
  list<int> cache;
  bool found;

  for(i = 0; i < 10000; i++){
    found = false;
    page = pages[i];

    for(j = 0; j < cache_size; j++){
      if(find(cache.begin(), cache.end(), page) != cache.end()){
	hits++;
	found = true;
	cache.remove(page);
	cache.push_front(page);
	break;
      }
    }

    if(!found){
      if(cache.size() < cache_size){
	cache.push_front(page);
      } else {
	cache.pop_back();
	cache.push_front(page);
      }
    }

  }

  return (hits/10000) * 100;
}

float clock(int cache_size, int * pages){
  if(cache_size == 0){
    return 0;
  }

  int i, j, k;
  int page;
  float hits = 0.0;
  int* cache = new int[10000];
  int* bits = new int[cache_size];
  bool found;
  int index = 0;

  for(i = 0; i < cache_size; i++){
    cache[i] = 0;
  }
  
  for(i = 0; i < 10000; i++){
    found = false;
    page = pages[i];

    for(j = 0; j < cache_size; j++){
      if(cache[j] = page){
	hits++;
	found = true;
	bits[j] = 1;
	break;
      }
    }

    if(!found){
      while(bits[k] == 1){
	if(k == cache_size){
	  k = 0;
	}
	bits[k] = 0;
	k++;
      }
      cache[k] = page;
      bits[k] = 1;
    }

    if(k == cache_size){
      k = 0;
    }
  }

  return (hits/10000.0) * 100;
}

float optimal(int cache_size, int * pages){
  if(cache_size == 0){
    return 0;
  }

  int i, j;
  int page;
  float hits = 0.0;
  int* cache = new int[cache_size];
  bool found;
  int before_full = 0;
  list<int> list;

  for(i = 0; i < cache_size; i++){
    cache[i] = 0;
  }

  for(i = 0; i < 10000; i++){
    found = false;
    page = pages[i];

    for(j = 0; j < cache_size; j++){
      if(page == cache[j]){
	hits++;
	found = true;
	break;
      }
    }

    if(!found){
      if(before_full < cache_size){ // Before the cache is full we can just add pages to it 
	cache[before_full] = page;
	before_full++;
      } else {
	list.clear();
	// Want to look for page furthest in future
	for(j = 0; j < cache_size; j++){	  
	    list.push_back(cache[j]); // Add all the pages in the cache to a list	    
	}
	
	for(j = i + 1; j < 10000; j++){ // Go through the rest of the reference stream
	  if(list.size() > 1){
	    list.remove(pages[j]); // Remove pages until there is only the furthest used one left
	  } else {
	    break;
	  }
	}

	for(j = 0; j < cache_size; j++){
	  if(cache[j] == list.front()){
	    cache[j] = page;  // Find that final page in the cache and "evict" it
	    break;
	  }
	}
      }
    }
  }

  return (hits/10000.0) * 100;
  
}
