// Binary Search Tree(BST) implementation
// record the number of duplicate key, hence can be used as map
// support operation
//  insert, 
//  remove, 
//  find key`s rank(even not exist in tree), 
//  find node by rank
//  get previous and successor
#include <stdio.h>

const int maxn = 1e5+10;
struct {
  int e[3];
  int key, cnt, size;
}nodes[maxn];
static int node_cnt{1};
static int root{0};

int *ls(int n) { return &(nodes[n].e[0]); }
int *rs(int n) { return &(nodes[n].e[1]); }
int *p(int n) { return &(nodes[n].e[2]); }

void insert(int key) { 
  int *ip = &root, pa = 0;
  while (*ip != 0) { 
    ++nodes[*ip].size;
    int cmp = key - nodes[*ip].key;
    if (cmp < 0) { pa = *ip; ip = ls(pa); }
    else if (cmp > 0) { pa = *ip; ip = rs(pa); }
    else { ++nodes[*ip].cnt; return; }
  }

  *ip = node_cnt++;
  *p(*ip) = pa;
  nodes[*ip].key = key; nodes[*ip].cnt = 1; nodes[*ip].size = 1;
}

void remove(int key) { 
  int *ip = &root;
  while (*ip != 0) { 
    --nodes[*ip].size;
    int cmp = key - nodes[*ip].key;
    if (cmp < 0) { ip = ls(*ip); }
    else if (cmp > 0) { ip = rs(*ip); }
    else { --nodes[*ip].cnt; break; }
  }
  if (nodes[*ip].cnt == 0) { 
    if (*ls(*ip) == 0) { 
      if (*rs(*ip) != 0) { *p(*rs(*ip)) = *p(*ip); }
      
      *ip = *rs(*ip);
    } else if (*rs(*ip) == 0) { 
      *p(*ls(*ip)) = *p(*ip); 

      *ip = *ls(*ip);
    } else { 
      int *sip = rs(*ip);
      while (*ls(*sip) != 0) { sip = ls(*sip); }
      nodes[*ip].key = nodes[*sip].key; nodes[*ip].cnt = nodes[*sip].cnt;
      ip = rs(*ip);
      while (ip != sip) { nodes[*ip].size -= nodes[*sip].cnt; ip = ls(*ip); }

      if (*rs(*ip) != 0) { *p(*rs(*ip)) = *p(*ip); }

      *ip = *rs(*ip);
    }
  }
}

int query_rank(int key) { 
  int rank = 1, it = root;
  while (it != 0) { 
    int cmp = key - nodes[it].key;
    if (cmp <= 0) { it = *ls(it); }
    else { 
      rank += (nodes[*ls(it)].size + nodes[it].cnt);
      it = *rs(it); 
    }
  }
  return rank;
}

int query(int rank) { 
  int it = root;
  while (it != 0) { 
    if (rank > nodes[*ls(it)].size) { 
      rank -= (nodes[*ls(it)].size + nodes[it].cnt);
      if (rank <= 0) { return it; }
      it = *rs(it); 
    } else { 
      it = *ls(it);
    }
  }
  return -1;
}

int main() { 
  int n, op, val, res;
  scanf("%d", &n);
  for (int i = 0; i < n; ++i) { 
    scanf("%d %d", &op, &val);
    switch (op) { 
      case 1: insert(val); break;
      case 2: remove(val); break;
      case 3: res = query_rank(val); break;
      case 4: res = nodes[query(val)].key; break;
      case 5: res = nodes[query(query_rank(val)-1)].key; break;
      case 6: res = nodes[query(query_rank(val+1))].key; break;
    }
    if (op > 2) { printf("%d\n", res); }
  }
  return 0;
}
