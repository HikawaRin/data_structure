// Red Black Tree implementation
// Solution for Luogu P3369
// 2-3-4 tree approch, insert rotate at most twice while delete rotate at most three times
#include <stdio.h>

const int maxn = 1e5+10;

struct {
  int e[4];
  int key, cnt, size;
}nodes[maxn];
static int node_cnt{1};
static int root{0};

int *ls(int n) { return &(nodes[n].e[0]); }
int *rs(int n) { return &(nodes[n].e[1]); }
int *p(int n) { return &(nodes[n].e[2]); }
int *color(int n) { return &(nodes[n].e[3]); }

void push_up(int n) { nodes[n].size = nodes[n].cnt + nodes[*ls(n)].size + nodes[*rs(n)].size; }

int rotate(int n, bool is_left) { 
  int nn = (is_left)? *rs(n): *ls(n);
  int ms = (is_left)? *ls(nn): *rs(nn);
  if (*p(n) != 0) { 
    if (*ls(*p(n)) == n) { *ls(*p(n)) = nn; }
    else { *rs(*p(n)) = nn; }
  } else { root = nn; }
  int tmp = *color(nn); *color(nn) = *color(n); *color(n) = tmp;
  *p(nn) = *p(n);  *p(n) = nn;   
  if (ms != 0) { *p(ms) = n; }
  if (is_left) { *rs(n) = ms; *ls(nn) = n; }
  else { *ls(n) = ms; *rs(nn) = n; }

  push_up(n); push_up(nn);
  return nn;
}

void fix_insert(int n) { 
  if (root == n) { return; }
  int pa = *p(n), gp = *p(pa);
  if (*color(pa) == 1) { 
    int uncle = (*ls(gp) == pa)? *rs(gp): *ls(gp);
    if (*color(uncle) == 1) { 
      *color(pa) = 0; *color(uncle) = 0; *color(gp) = 1; 
      fix_insert(gp);
    } else { 
      if ((*ls(gp) == pa) ^ (*ls(pa) == n)) { 
        pa = rotate(pa, (*ls(gp) == pa));
      }
      
      rotate(gp, (*ls(gp) != pa));
    }
  }  
}

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
  *p(*ip) = pa; *color(*ip) = 1;
  nodes[*ip].key = key; nodes[*ip].cnt = 1; nodes[*ip].size = 1;
  fix_insert(*ip);
  *color(root) = 0;
}

void fix_delete(int n) { 
  if (n == root) { return; }
  int pa = *p(n), sib = (*ls(pa) == n)? *rs(pa): *ls(pa);
  if (*color(sib) == 1) { 
    rotate(pa, (*ls(pa) == n));
    sib = (*ls(pa) == n)? *rs(pa): *ls(pa);
  }
  if (*color(*ls(sib)) == 0 && *color(*rs(sib)) == 0) { 
    *color(sib) = 1;
    if (*color(pa) == 1) { *color(pa) = 0; }
    else { fix_delete(pa); }
  } else { 
    int po_son = (*ls(pa) == n)? *rs(sib): *ls(sib);
    if (*color(po_son) == 0) { 
      sib = rotate(sib, (*ls(pa) != n));
      po_son = (*ls(pa) == n)? *rs(sib): *ls(sib);
    }
    rotate(pa, (*ls(pa) == n));
    *color(po_son) = 0;
  }
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
    bool need_fix = !(*color(*ip));
    if (*ls(*ip) == 0) { 
      need_fix = need_fix && !(*color(*rs(*ip)));
      if (*rs(*ip) != 0) { *p(*rs(*ip)) = *p(*ip); }
      *ip = *rs(*ip);
      *color(*ip) = 0;
    } else if (*rs(*ip) == 0) { 
      need_fix = need_fix && !(*color(*ls(*ip)));
      *p(*ls(*ip)) = *p(*ip);
      *ip = *ls(*ip);
      *color(*ip) = 0;
    } else { 
      int *sip = rs(*ip);
      while (*ls(*sip) != 0) { sip = ls(*sip); }
      nodes[*ip].key = nodes[*sip].key; nodes[*ip].cnt = nodes[*sip].cnt;
      need_fix = !(*color(*sip)) && !(*color(*rs(*sip)));
      ip = rs(*ip);
      while (ip != sip) { nodes[*ip].size -= nodes[*sip].cnt; ip = ls(*ip); }
      if (*rs(*ip) != 0) { *p(*rs(*ip)) = *p(*ip); }
      *ip = *rs(*ip);
      *color(*ip) = 0;
    }
    if (need_fix) { fix_delete(*ip); }
    *color(root) = 0;
  }
}

int query_rank(int key) { 
  int rank = 1, it = root;
  while (it != 0) { 
    int cmp = key - nodes[it].key;
    if (cmp > 0) { 
      rank += (nodes[*ls(it)].size + nodes[it].cnt);
      it = *rs(it);
    } else { it = *ls(it); }
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
    } else { it = *ls(it); }
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
