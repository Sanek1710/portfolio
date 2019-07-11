#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "graph.h"

using namespace gra;
using namespace std;

#define INF			-1
#define MIN(a, b)	(((a) < (b) && (a) >= 0 || (b) < 0) ? (a) : (b))
#define MAX(a, b)	(((a) >= (b) && (a) >= 0 || (b) < 0) ? (a) : (b))
#define LESS(a, b)	(((a) < (b) && (a) >= 0 || (b) < 0))
#define MORE(a, b)	(((a) > (b) && (a) >= 0 || (b) < 0))
#define SUM(a, b)	(((a) == INF || (b) == INF) ? (INF) : ((a) + (b)))
#define DIF(a, b)	(((a) == INF || (b) == INF) ? (INF) : ((a) - (b)))

typedef struct WEDGE
{
	NODE v1, v2;
	WEIGHT w;
	WEIGHT w_used;

	WEDGE(int v1, int v2, int w)
	{
		this->v1 = v1;
		this->v2 = v2;
		this->w = w;
		this->w_used = 0;
	}
}WEDGE, WARROW;

typedef struct WEDGE_PATH
{
	int sizeN;
	WEDGE *edge;
	bool *flag;
	int sizeM;
	NODE source;
	NODE sink;

	void init(WEDGE *edge, int sizeM, int sizeN, NODE src, NODE snk)
	{
		source = src;
		sink = snk;
		this->edge = edge;
		this->sizeM = sizeM;
		flag = (bool*)malloc(sizeM * sizeof(bool));
		memset(flag, 0, sizeM * sizeof(bool));
	}

	void set(int v)
	{
		flag[v] = true;
	}

	void clr(int v)
	{
		flag[v] = false;
	}

	int find(int v1)
	{
		for (int i = 0; i < sizeM; i++)
		{
			if (edge[i].v1 == v1) return i;
		}
		return -1;
	}

	int find(int v1, int v2)
	{
		for (int i = 0; i < sizeM; i++)
		{
			if (edge[i].v1 == v1 && edge[i].v2 == v2) return i;
		}
		return -1;
	}

	int find_free(int v1)
	{
		for (int i = 0; i < sizeM; i++)
		{
			if (!was_set(edge[i].v1) && edge[i].v1 == v1) return i;
		}
		return -1;
	}

	inline bool dir_able(int i)
	{
		return edge[i].w > edge[i].w_used;
	}

	inline bool rev_able(int i)
	{
		return edge[i].w_used > 0;
	}

	inline bool was_set(int v)
	{
		return flag[v];
	}

	inline void add_used_w(int idx, int value)
	{
		edge[idx].w_used += value;
	}

	inline void dif_used_w(int idx, int value)
	{
		edge[idx].w_used -= value;
	}


	int max_stream()
	{
		int res = 0;
		for (int i = find(source); i < sizeM; i++)
		{
			if (edge[i].v1 != source) break;
			res += edge[i].w_used;
		}

		return res;
	}

	void print()
	{
		cout << "source: " << source << endl;
		cout << "sink: " << sink << endl;

		for (int i = 0; i < sizeM; i++)
		{
			printf("%d -> %d (%d/%d)\n", edge[i].v1, edge[i].v2, edge[i].w_used, edge[i].w);
		}

		cout << "max stream = " << max_stream() << endl;
	}
}WEDGE_ARR;

ostream &operator << (ostream &os, WEDGE &we)
{
	os << "(" << we.v1 << ", " << we.v2 << "):" << we.w;
	return os;
}

typedef struct WPATH
{
	NODE *node;
	int *Dist;
	NODE *Par;
	int size;

	void init(int NodeCnt)
	{
		NODE *v;
		size = NodeCnt;
		node = (NODE*)malloc(NodeCnt * sizeof(NODE));
		Dist = (int*)malloc(NodeCnt * sizeof(int));
		Par = (NODE*)malloc(NodeCnt * sizeof(NODE));
		memset(Dist, INF, NodeCnt * sizeof(int));
		memset(Par, INF, NodeCnt * sizeof(int));

		for (int i = 0; i < NodeCnt; i++)
		{
			node[i] = i + 1;
		}
	}

	inline int &D(NODE v)
	{
		return Dist[v - 1];
	}

	inline int &P(NODE v)
	{
		return Par[v - 1];
	}

	inline int find(NODE v)
	{
		return v - 1;
	}

	void print()
	{
		cout << "v : ";
		for (int n = 0; n < size; n++)
		{
			printf("%2d ", n + 1);
		}
		cout << endl;
		cout << "D : ";
		for (int n = 0; n < size; n++)
		{
			printf("%2d ", Dist[n]);
		}
		cout << endl;
		cout << "P : ";
		for (int n = 0; n < size; n++)
		{
			printf("%2d ", Par[n]);
		}
		cout << endl;
	}
}WPATH ;

typedef struct WPLAN
{
	NODE *node;
	int *T_earl;
	int *T_late;
	int Critical;
	int size;
	WEDGE *edge;
	int *R;
	int *r;
	int m;


	void init(int NodeCnt, int critical, int M)
	{
		NODE *v;
		size = NodeCnt;
		Critical = critical;
		m = M;
		node = (NODE*)malloc(size * sizeof(NODE));
		T_earl = (int*)malloc(size * sizeof(int));
		R = (int*)malloc(m * sizeof(int));
		r = (int*)malloc(m * sizeof(int));
		T_late = (NODE*)malloc(size * sizeof(NODE));
		memset(T_earl, INF, size * sizeof(int));

		for (int i = 0; i < NodeCnt; i++)
		{
			node[i] = i + 1;
			T_late[i] = Critical;
		}
	}

	inline int &T_e(NODE v)
	{
		return T_earl[v - 1];
	}

	inline int &T_l(NODE v)
	{
		return T_late[v - 1];
	}

	inline int find(NODE v)
	{
		return v - 1;
	}

	inline void set_Critical()
	{
		int MaxTe = -1;

		for (int i = 0; i < size; i++)
		{
			if (T_earl[i] > MaxTe) MaxTe = T_earl[i];
			T_late[i] -= Critical;
		}

		Critical = MaxTe;
		for (int i = 0; i < size; i++)
		{
			T_late[i] += Critical;
		}
	}

	void eval()
	{
		for (int i = 0; i < m; i++)
		{
			R[i] = T_l(edge[i].v2) - T_e(edge[i].v1) - edge[i].w;
			r[i] = T_e(edge[i].v2) - T_e(edge[i].v1) - edge[i].w;
		}
	}

	void print()
	{
		cout << "v   : ";
		for (int n = 0; n < size; n++)
		{
			printf("%2d ", n + 1);
		}
		cout << endl << "T_e : ";
		for (int n = 0; n < size; n++)
		{
			printf("%2d ", T_earl[n]);
		}
		cout << endl << "T_l : ";

		for (int n = 0; n < size; n++)
		{
			printf("%2d ", T_late[n]);
		}
		cout << endl << endl;

		printf(" i ->  j   w  Te_i  Te_j  Tl_i  Tl_j   R   r\n");
		for (int i = 0; i < m; i++)
		{

			printf("%2d -> %2d  %2d    %2d    %2d    %2d    %2d  %2d  %2d",
				   edge[i].v1, edge[i].v2, edge[i].w,
				   T_e(edge[i].v1), T_e(edge[i].v2),
				   T_l(edge[i].v1), T_l(edge[i].v2),
				   R[i], r[i]
			);

			printf("%s\n", (R[i] == 0 && r[i] == 0) ? " <--" : "");
		}
		cout << endl;
	}
}WPLAN ;

class WGRAPH
{
private:
	char *GRAPH_NAME_;
	BYTE  STAT_FLAG_;

	typedef struct NODE_RECORD
	{
		NODE v;	//name
		int  deg;
		NODE_RECORD **adj;	//adjasent
		WEIGHT *w;
		NODE_RECORD *nxt, *prv;
		bool flag;
		//returns deg (cnt)
		int	insert(NODE_RECORD *node, WEIGHT wgt)
		{
			int i = deg;

			adj = (NODE_RECORD**)realloc(adj, (deg + 1) * sizeof(NODE_RECORD*));
			w = (WEIGHT*)realloc(w, (deg + 1) * sizeof(WEIGHT));

			while (i > 0 && adj[i - 1]->v > node->v)
			{
				adj[i] = adj[i - 1];
				w[i] = w[i - 1];
				i--;
			}
			adj[i] = node;
			w[i] = wgt;

			deg++;

			return (deg);
		}

		//reterns found index, -1 - else
		int	find(NODE_RECORD * node)
		{
			int lft = 0, rgt = deg - 1;
			int idx;

			idx = (lft + rgt) >> 1;
			while (lft < rgt)
			{
				if (node->v < adj[idx]->v)		rgt = idx - 1;
				else if (adj[idx]->v < node->v)		lft = idx + 1;
				else if (node->v == adj[idx]->v)	rgt = idx;
				idx = (lft + rgt) >> 1;
			}

			if (lft > rgt || node->v != adj[idx]->v) return -1;
			return idx;
		}

		//returns taken index, -1 - else
		int	take(NODE_RECORD * node)
		{
			int i = find(node);

			if (i < 0) return -1;

			int j = i + 1;

			//while (j < deg && adj[j]->v == node->v) j++;

			while (j < deg) adj[i++] = adj[j++];

			deg = i;
			adj = (NODE_RECORD**)realloc(adj, deg * sizeof(NODE_RECORD*));
			w = (WEIGHT*)realloc(w, deg * sizeof(WEIGHT));

			return deg;
		}

		void uniq()
		{
			int j = 1, i = 0;

			while (j < deg)
			{
				while (j < deg && adj[i]->v == adj[j]->v) j++;
				if (j < deg)
				{
					i++;
					adj[i] = adj[j];
					w[i] = w[j];
					j++;
				}
			}
			i++;

			if (i < deg)
			{
				deg = i;
				adj = (NODE_RECORD**)realloc(adj, i * sizeof(NODE_RECORD*));
				w = (WEIGHT*)realloc(w, i * sizeof(WEIGHT));
			}
		}

	}NODE_RECORD;

	int NODE_CNT = 0;
	NODE_RECORD *BEGIN = NULL, *END = NULL;

	NODE_RECORD *	find_rec(NODE node, NODE_RECORD **last)
	{
		NODE_RECORD *start_point = BEGIN->nxt;

		while (start_point != END && start_point->v <= node)
		{
			if (node == start_point->v)
			{
				if (last) *last = start_point;
				return start_point;
			}
			start_point = start_point->nxt;
		}

		if (last) *last = start_point;

		return NULL;
	}

	//returns added rec
	NODE_RECORD *	add_rec(NODE node, NODE_RECORD *before)
	{
		NODE_RECORD *NEW_NODE = (NODE_RECORD*)malloc(sizeof(NODE_RECORD));
		NEW_NODE->prv = before->prv;
		NEW_NODE->nxt = before;
		before->prv = NEW_NODE;
		NEW_NODE->prv->nxt = NEW_NODE;

		NEW_NODE->v = node;
		NEW_NODE->deg = 0;
		NEW_NODE->adj = NULL;
		NEW_NODE->w = NULL;

		NODE_CNT++;

		return NEW_NODE;
	}

	//returns next after deleted
	NODE_RECORD *	del_rec(NODE_RECORD *to_del)
	{
		if (!to_del) return NULL;

		NODE_CNT--;

		NODE_RECORD *to_ret = to_del->nxt;

		if (to_del->deg)
		{
			free(to_del->adj);
			free(to_del->w);
		}

		to_del->nxt->prv = to_del->prv;
		to_del->prv->nxt = to_del->nxt;
		free(to_del);

		return to_ret;
	}

	void	init(const char *name, BYTE FLAGS)
	{
		GRAPH_NAME_ = (char *)malloc(strlen(name) + 1);
		strcpy(GRAPH_NAME_, name);

		if (FLAGS & gra::GFLAG_SIMPLE || FLAGS == 0) STAT_FLAG_ = gra::GFLAG_SIMPLE;
		else STAT_FLAG_ = FLAGS;

		NODE_CNT = 0;
		BEGIN = (NODE_RECORD*)malloc(sizeof(NODE_RECORD));
		END = (NODE_RECORD*)malloc(sizeof(NODE_RECORD));

		BEGIN->v = END->v = -1;
		BEGIN->nxt = END;
		END->prv = BEGIN;
		BEGIN->prv = END->nxt = NULL;
	}

	void	girth(NODE_RECORD *node1, NODE_RECORD *node2, INT_ARRAY &cur_path, int &opt_dist)
	{
		if (node1->v == node2->v)
		{
			if (cur_path.size < opt_dist) opt_dist = cur_path.size;
			return;
		}

		if (cur_path.size >= opt_dist) return;

		cur_path.push(node1->v);

		for (int i = 0; i < node1->deg; i++)
		{
			if (cur_path.find(node1->adj[i]->v) < 0)
			{
				girth(node1->adj[i], node2, cur_path, opt_dist);
			}
		}

		cur_path.pop(NULL);
	}
	int		distance(NODE_RECORD *node1, NODE_RECORD *node2)
	{
		INT_ARRAY dist_arr;
		int optimal_distance = NODE_CNT;

		girth(node1, node2, dist_arr, optimal_distance);

		if (optimal_distance == NODE_CNT) return -1;
		return optimal_distance;
	}

	int		deg_in(NODE_RECORD *node)
	{
		if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
		{
			if (!node) return -1;

			NODE_RECORD *to_find = BEGIN->nxt;
			int res_deg_in = 0;

			while (to_find != END)
			{
				int idx = to_find->find(node);

				if (idx >= 0)
				{
					while (idx < to_find->deg && to_find->adj[idx++] == node)
					{
						res_deg_in++;
					}
				}

				to_find = to_find->nxt;
			}

			return res_deg_in;
		}
		return -1;
	}

	int		deg_out(NODE_RECORD *node)
	{
		if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
		{
			if (!node) return -1;
			return node->deg;
		}
		return -1;
	}

	void	clear_flags()
	{
		BEG_FORALL_V(*this, 0)
		{
			CUR_V(0)->flag = false;
		}
		END_FORALL_V(*this, 0);
	}

	WEDGE *	edge_arr(int *cnt)
	{
		int M = m();
		if (M == 0) return NULL;
		*cnt = M;

		WEDGE * we = (WEDGE*)malloc(M * sizeof(WEDGE));

		int i = 0;
		BEG_FORALL_V(*this, 0)
		{
			for (int j = 0; j < CUR_V(0)->deg; j++)
			{
				if (CUR_V(0)->v > CUR_V(0)->adj[j]->v && !(STAT_FLAG_ & gra::GFLAG_DIGRAPH)) continue;

				we[i] = { CUR_V(0)->v, CUR_V(0)->adj[j]->v, CUR_V(0)->w[j] };

				i++;
			}
		}
		END_FORALL_V(*this, 0);

		return we;
	}

	int		edge_girth(NODE v1, NODE v2, WEDGE_ARR &used, int &min_weight)
	{
		if (v1 == v2) return 1;

		used.set(v1);

		for (int i = 0; i < used.sizeM; i++)
		{
			if (used.edge[i].v1 == v1 && !used.flag[used.edge[i].v2] && used.dir_able(i))
			{
				int local_min = MIN(used.edge[i].w - used.edge[i].w_used, min_weight);

				if (edge_girth(used.edge[i].v2, v2, used, local_min) == 1)
				{
					min_weight = local_min;
					used.clr(v1);
					used.add_used_w(i, min_weight);
					return 1;
				}
				//used.clr(i);
			}
			else if (used.edge[i].v2 == v1 && !used.flag[used.edge[i].v1] && used.rev_able(i))
			{
				int local_min = MIN(used.edge[i].w_used, min_weight);

				if (edge_girth(used.edge[i].v1, v2, used, local_min) == 1)
				{
					min_weight = local_min;
					used.clr(v1);
					used.dif_used_w(i, min_weight);
					return 1;
				}
			}
		}

		used.clr(v1);

		return -1;
	}

public:

	WGRAPH()
	{
		init("Graph", gra::GFLAG_SIMPLE);
	}

	WGRAPH(const char *name, BYTE FLAGS)
	{
		init(name, FLAGS);
	}

	WGRAPH(BYTE FLAGS)
	{
		init("Graph", FLAGS);
	}

	WGRAPH(const char *name)
	{
		init(name, gra::GFLAG_SIMPLE);
	}

	WGRAPH(const WGRAPH &G)
	{
		init(G.GRAPH_NAME_, G.STAT_FLAG_);

		NODE_RECORD* TMP_G = G.BEGIN->nxt;
		while (TMP_G != G.END)
		{
			add_rec(TMP_G->v, END);
			TMP_G = TMP_G->nxt;
		}

		NODE_RECORD* fnd;
		NODE_RECORD* TMP = BEGIN->nxt;
		TMP_G = G.BEGIN->nxt;
		while (TMP != END)
		{
			if (TMP_G->deg)
			{
				TMP->adj = (NODE_RECORD**)malloc(TMP_G->deg * sizeof(NODE_RECORD*));
				TMP->w = (WEIGHT*)malloc(TMP_G->deg * sizeof(WEIGHT));
			}
			TMP->deg = TMP_G->deg;
			for (int i = 0; i < TMP_G->deg; i++)
			{
				find_rec(TMP_G->adj[i]->v, &fnd);
				TMP->adj[i] = fnd;
				TMP->w[i] = TMP_G->w[i];
			}

			TMP_G = TMP_G->nxt;
			TMP = TMP->nxt;
		}
	}

	int		n()
	{
		return NODE_CNT;
	}

	int		m()
	{
		int res = 0;

		BEG_FORALL_V(*this, 0)
		{
			res += CUR_V(0)->deg;
		}
		END_FORALL_V(*this, 0)

			return (STAT_FLAG_ & gra::GFLAG_DIGRAPH) ? (res) : (res / 2);
	}

	int		add_v(NODE v)
	{
		NODE_RECORD *founded;

		if (!find_rec(v, &founded))
		{
			add_rec(v, founded);
			return 1;
		}

		return 0;
	}

	int		del_v(NODE v)
	{
		NODE_RECORD *founded;

		if (!find_rec(v, &founded)) return -1;

		if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
		{
			NODE_RECORD *to_find = BEGIN->nxt;

			while (to_find != END)
			{
				while (to_find->take(founded) >= 0);

				to_find = to_find->nxt;
			}
		}
		else
		{
			for (int i = 0; i < founded->deg; i++)
			{
				while (founded->adj[i]->take(founded) >= 0);
			}
		}

		del_rec(founded);
	}

	int		add_e(WEDGE e)
	{
		NODE_RECORD *node1;
		NODE_RECORD *node2;

		if (!find_rec(e.v1, &node1)) return 0;
		if (!find_rec(e.v2, &node2)) return 0;

		node1->insert(node2, e.w);
		if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH)) node2->insert(node1, e.w);

		return 1;
	}

	int		del_e(WEDGE e)
	{
		NODE_RECORD *node1, *node2;

		if (!find_rec(e.v1, &node1)) return 0;
		if (!find_rec(e.v2, &node2)) return 0;

		if (node1->take(node2) < 0) return 0;
		if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH)) node2->take(node1);

		return 1;
	}

	int		weight()
	{
		int res = 0;

		BEG_FORALL_V(*this, 0)
		{
			for (int i = 0; i < CUR_V(0)->deg; i++)
			{
				res += CUR_V(0)->w[i];
			}
		}
		END_FORALL_V(*this, 0)

			return (STAT_FLAG_ & gra::GFLAG_DIGRAPH) ? (res) : (res / 2);
	}

	NODE	**	W(BYTE FLAGS, int *sizeN)
	{
		int ** A_matrix;
		int heading = (FLAGS & gra::DFLAG_HEADING) ? 1 : 0;
		int size_A = NODE_CNT + heading;
		INT_ARRAY  verties;

		A_matrix = (int**)malloc(size_A * sizeof(int*));
		for (int i = heading; i < size_A; i++)
		{
			A_matrix[i] = (int*)malloc(size_A * sizeof(int));
			memset(A_matrix[i], -1, size_A * sizeof(int));
		}

		if (heading) verties.push(0);
		BEG_FORALL_V(*this, 0)
		{
			verties.push(CUR_V(0)->v);
		}
		END_FORALL_V(*this, 0);

		if (heading)
		{
			A_matrix[0] = verties.arr;
			for (int i = 1; i < size_A; i++)
			{
				A_matrix[i][0] = A_matrix[0][i];
			}
		}

		int idx_FROM, idx_TO;
		BEG_FORALL_V(*this, 0)
		{
			idx_FROM = verties.find(CUR_V(0)->v);

			for (int i = 0; i < CUR_V(0)->deg; i++)
			{
				idx_TO = verties.find(CUR_V(0)->adj[i]->v);
				A_matrix[idx_FROM][idx_TO] = CUR_V(0)->w[i];
			}
		}
		END_FORALL_V(*this, 0);

		if (FLAGS & gra::DFLAG_PRINT)
		{
			if (heading)
			{
				cout << "weight matrix : " << endl;
				printf("v\\v ", A_matrix[0][0]);
				for (int j = 1; j < size_A; j++)
				{
					printf("%2d ", A_matrix[0][j]);
				}
				cout << endl;
			}
			for (int i = heading; i < size_A; i++)
			{
				if (heading) printf("%2d: ", A_matrix[i][0]);
				for (int j = heading; j < size_A; j++)
				{
					if (A_matrix[i][j] >= 0) printf("%2d ", A_matrix[i][j]);
					else printf(" - ");
				}
				cout << endl;
			}
		}

		if (FLAGS & gra::DFLAG_RETURN)
		{
			if (sizeN) *sizeN = size_A;
			return A_matrix;
		}
		for (int i = 0; i < size_A; i++)
		{
			free(A_matrix[i]);
			A_matrix[i] = NULL;
			if (!heading) verties.clear();
		}
		free(A_matrix);
		return NULL;
	}

	NODE	**	W_max_stream(BYTE FLAGS, int *size)
	{
		int ** A_matrix;
		int heading = (FLAGS & gra::DFLAG_HEADING) ? 1 : 0;
		int size_A = NODE_CNT + heading;
		INT_ARRAY  verties;
		WEDGE_PATH edge = max_stream();

		A_matrix = (int**)malloc(size_A * sizeof(int*));
		for (int i = heading; i < size_A; i++)
		{
			A_matrix[i] = (int*)malloc(2 * size_A * sizeof(int));
			memset(A_matrix[i], -1, 2 * size_A * sizeof(int));
		}

		if (heading) 
		{ 
			verties.push(0); 
			
		}
		BEG_FORALL_V(*this, 0)
		{
			verties.push(CUR_V(0)->v);
		
		}
		END_FORALL_V(*this, 0);

		if (heading)
		{
			A_matrix[0] = verties.arr;
			for (int i = 1; i < size_A; i++)
			{
				A_matrix[i][0] = A_matrix[0][i];
			}
		}

		int idx_FROM, idx_TO;
		BEG_FORALL_V(*this, 0)
		{
			idx_FROM = verties.find(CUR_V(0)->v);

			for (int i = 0; i < CUR_V(0)->deg; i++)
			{
				idx_TO = verties.find(CUR_V(0)->adj[i]->v);
				A_matrix[idx_FROM][2*idx_TO] = CUR_V(0)->w[i];
				int idx = edge.find(CUR_V(0)->v, CUR_V(0)->adj[i]->v);
				A_matrix[idx_FROM][2*idx_TO + 1] = edge.edge[idx].w_used;
			}
		}
		END_FORALL_V(*this, 0);

		if (FLAGS & gra::DFLAG_PRINT)
		{
			if (heading)
			{
				cout << "max stream matrix w(w_use): " << endl;
				printf("  v\\v ", A_matrix[0][0]);
				for (int j = 1; j < size_A; j++)
				{
					if (A_matrix[0][j] == edge.source) printf(" S:");
					else if (A_matrix[0][j] == edge.sink) printf(" T:");
					else printf("   ");
					printf("%d ", A_matrix[0][j]);
				}
				cout << endl;
			}
			for (int i = heading; i < size_A; i++)
			{
				if (heading)
				{
					if (A_matrix[i][0] == edge.source) printf(" S:");
					else if (A_matrix[i][0] == edge.sink) printf(" T:");
					else printf("   ");
					printf("%d: ", A_matrix[i][0]);
				}
				for (int j = 2 * heading; j < 2*size_A; j += 2)
				{
					if (A_matrix[i][j] >= 0)
					{
						printf("%1d(%1d) ", A_matrix[i][j], A_matrix[i][j + 1]);
					}
					else printf("   - ");
				}
				cout << endl;
			}
		}

		if (FLAGS & gra::DFLAG_RETURN)
		{
			if (size) *size = size_A;
			return A_matrix;
		}
		for (int i = 0; i < size_A; i++)
		{
			free(A_matrix[i]);
			A_matrix[i] = NULL;
			if (!heading) verties.clear();
		}
		free(A_matrix);
		return NULL;
	}

	NODE *	source(BYTE FLAGS, int *size)
	{
		if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
		{
			INT_ARRAY src;

			NODE_RECORD *to_find = BEGIN->nxt;
			while (to_find != END)
			{
				if (deg_in(to_find) == 0 && deg_out(to_find) > 0) src.push(to_find->v);
				to_find = to_find->nxt;
			}

			if (FLAGS & gra::DFLAG_PRINT)
			{
				if (FLAGS & gra::DFLAG_HEADING) cout << "source : ";
				cout << "{";
				if (src.size) cout << ' ' << src.arr[0];
				for (int i = 1; i < src.size; i++)
				{
					cout << ", " << src.arr[i];
				}
				cout << " }" << endl;
			}

			if (FLAGS & gra::DFLAG_RETURN)
			{
				if (size) *size = src.size;
				return src.arr;
			}
			else src.clear();
		}
		else if (FLAGS & gra::DFLAG_PRINT)
		{
			cout << "source : GRAPH_ERROR" << endl;
		}

		return NULL;
	}

	int		deg_in(NODE v)
	{
		if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
		{
			NODE_RECORD *founded;

			if (!find_rec(v, &founded)) return -1;
			return deg_in(founded);
		}
		return -1;
	}

	int		deg_out(NODE v)
	{
		if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
		{
			NODE_RECORD *founded;

			if (!find_rec(v, &founded)) return -1;
			return founded->deg;
		}
		return -1;
	}

	NODE *	sink(BYTE FLAGS, int *size)
	{
		if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
		{
			INT_ARRAY src;

			NODE_RECORD *to_find = BEGIN->nxt;
			while (to_find != END)
			{
				if (deg_out(to_find) == 0 && deg_in(to_find) > 0) src.push(to_find->v);
				to_find = to_find->nxt;
			}

			if (FLAGS & gra::DFLAG_PRINT)
			{
				if (FLAGS & gra::DFLAG_HEADING) cout << "sink : ";
				cout << "{";
				if (src.size) cout << ' ' << src.arr[0];
				for (int i = 1; i < src.size; i++)
				{
					cout << ", " << src.arr[i];
				}
				cout << " }" << endl;
			}

			if (FLAGS & gra::DFLAG_RETURN)
			{
				if (size) *size = src.size;
				return src.arr;
			}
			else src.clear();
		}
		else if (FLAGS & gra::DFLAG_PRINT)
		{
			cout << "sink : GRAPH_ERROR" << endl;
		}

		return NULL;
	}

	WGRAPH	ost_prim()
	{
		WGRAPH ost_p;

		int max = 0;
		if (NODE_CNT > 0 && BEGIN->nxt->w) max = BEGIN->nxt->w[0];
		else return ost_p;

		BEG_FORALL_V(*this, 0)
		{
			for (int i = 0; i < CUR_V(0)->deg; i++)
			{
				if (max < CUR_V(0)->w[i])
				{
					max = CUR_V(0)->w[i];
				}
			}
		}
		END_FORALL_V(*this, 0);

		ost_p.add_v(BEGIN->nxt->v);

	REPEAT:

		BEG_FORALL_V(*this, 0)
		{
			int min = max + 1;
			int min_i = 0;
			NODE_RECORD *node_min = NULL;
			BEG_FORALL_V(*this, 1)
			{
				if (ost_p.find_rec(CUR_V(1)->v, NULL))
				{
					for (int i = 0; i < CUR_V(1)->deg; i++)
					{
						if (!ost_p.find_rec(CUR_V(1)->adj[i]->v, NULL))
						{
							if (min > CUR_V(1)->w[i])
							{
								min = CUR_V(1)->w[i];
								min_i = i;
								node_min = CUR_V(1);
							}
						}
					}
				}
			}
			END_FORALL_V(*this, 1);
			if (node_min)
			{
				ost_p.add_v(node_min->adj[min_i]->v);
				ost_p.add_e({ node_min->v, node_min->adj[min_i]->v, min });
			}
		}
		END_FORALL_V(*this, 0);

		BEG_FORALL_V(*this, 0)
		{
			if (!ost_p.find_rec(CUR_V(0)->v, NULL))
			{
				ost_p.add_v(CUR_V(0)->v);
				goto REPEAT;
			}
		}
		END_FORALL_V(*this, 0)

			return ost_p;
	}

	WGRAPH	ost_kruscal()
	{
		WGRAPH ost_c;
		WEDGE *we;
		int M;

		we = edge_arr(&M);
		if (we == NULL) return ost_c;

		for (int j = 0; j < M; j++)
		{
			if (distance(ost_c.find_rec(we[j].v2, NULL), ost_c.find_rec(we[j].v1, NULL)) == -1)
			{
				ost_c.add_e(we[j]);
			}
		}

		if (M > 0) free(we);

		return ost_c;
	}

	void	order(NODE base)
	{
		BEG_FORALL_V(*this, 0)
		{
			CUR_V(0)->v = base;
			CUR_V(0)->flag = false;
			base++;
		}
		END_FORALL_V(*this, 0);
	}

	void	Dijkstra(NODE v)
	{
		order(1);

		NODE_RECORD *next_node;
		WPATH path;
		int min;

		if (!find_rec(v, &next_node)) return;

		path.init(NODE_CNT);

		next_node->flag = true;
		path.D(next_node->v) = 0;
		path.P(next_node->v) = next_node->v;

		for (int n = 0; n < NODE_CNT; n++)
		{
			min = -1;
			BEG_FORALL_V(*this, 0)  //
			{						//
				if (CUR_V(0)->flag) //для каждой помеченной вершины 
				{
					NODE v1 = CUR_V(0)->v;

					for (int i = 0; i < CUR_V(0)->deg; i++) //смотрим соседей
					{
						if (!CUR_V(0)->adj[i]->flag)		//если они еще не добавлены
						{
							NODE v2 = CUR_V(0)->adj[i]->v; //запоминаем вершину

							if (LESS(path.D(v1) + CUR_V(0)->w[i], path.D(v2)))	//если новый путь до нее меньше текущего
							{
								path.D(v2) = path.D(v1) + CUR_V(0)->w[i];		//
								path.P(v2) = CUR_V(0)->v;			//то меняем путь

							}
							if (LESS(CUR_V(0)->w[i], min))	//если среди всех путей это минимальный
							{
								min = path.D(v2);				//
								next_node = CUR_V(0)->adj[i];	//запоминаем его
							}
						}
					}
				}
			}
			END_FORALL_V(*this, 0);

			next_node->flag = true;
		}

		path.print();

	}

	void	topolog_sort()
	{
		WGRAPH wg = *this;
		INT_ARRAY named;

		while (wg.NODE_CNT > 1)
		{
			NODE_RECORD *to_find = wg.END->prv;
			while (to_find != wg.BEGIN && !(wg.deg_in(to_find) > 0 && wg.deg_out(to_find) == 0))
			{
				to_find = to_find->prv;
			}
			if (to_find != wg.BEGIN)
			{
				cout << to_find->v << ' ';
				named.push(to_find->v);
				wg.del_v(to_find->v);
			}
			else break;
		}
		named.push(wg.BEGIN->nxt->v);

		NODE_RECORD TMP = { 0, 0, NULL, NULL };

		BEG_FORALL_V(*this, 0)
		{
			named.pop(&CUR_V(0)->v);
			TMP.insert(CUR_V(0), 0);
		}
		END_FORALL_V(*this, 0);

		BEGIN->nxt = END;
		END->prv = BEGIN;

		for (int i = 0; i < TMP.deg; i++)
		{
			TMP.adj[i]->nxt = END;
			TMP.adj[i]->prv = END->prv;
			END->prv->nxt = TMP.adj[i];
			END->prv = TMP.adj[i];
		}
		if (TMP.deg > 0)
		{
			free(TMP.adj);
			free(TMP.w);
		}
	}

	WPLAN	network_plan(NODE v)
	{
		WPLAN wp;

		order(1);
		wp.init(NODE_CNT, 2 * weight(), m());
		wp.T_e(v) = 0;

		BEG_FORALL_V(*this, 0)
		{
			for (int i = 0; i < CUR_V(0)->deg; i++)
			{
				int &T_e1 = wp.T_e(CUR_V(0)->adj[i]->v);
				int &T_e2 = wp.T_e(CUR_V(0)->v);
			//	int &T_l1 = wp.T_l(CUR_V(0)->v);
				//int &T_l2 = wp.T_l(CUR_V(0)->adj[i]->v);
				int w = CUR_V(0)->w[i];

				T_e1 = MAX(T_e1, SUM(T_e2, w));
				//T_l1 = MIN(T_l1, DIF(T_l2, w));
			}
		}
		END_FORALL_V(*this, 0);

		wp.set_Critical();
		{
			NODE_RECORD *to_run = END->prv;
			while (to_run != BEGIN)
			{
				for (int i = 0; i < to_run->deg; i++)
				{
					int &T_l1 = wp.T_l(to_run->v);
					int &T_l2 = wp.T_l(to_run->adj[i]->v);
					int w = to_run->w[i];
					T_l1 = MIN(T_l1, DIF(T_l2, w));
				}

				to_run = to_run->prv;
			}
		}

		wp.edge = edge_arr(&wp.m);

		wp.eval();

		wp.print();

		return wp;
	}

	WEDGE_PATH	max_stream()
	{
		int src_cnt, snk_cnt;
		NODE * src = source(DFLAG_RETURN, &src_cnt);
		NODE * snk = sink(DFLAG_RETURN, &snk_cnt);
		NODE sr = src[0];
		NODE sn = snk[0];
		
		int M = m();
		int N = n();
		WEDGE_PATH used;

		used.init(edge_arr(&M), M, N, sr, sn);

		const int max_weight = weight();
		int min_weight;

		do 
		{
			min_weight = max_weight;
			used.print();
		} 
		while (edge_girth(sr, sn, used, min_weight) == 1);

		return used;
	}

	WGRAPH	operator =(WGRAPH G)
 //make copy of G
	{
		gfree();

		STAT_FLAG_ = G.STAT_FLAG_;

		NODE_RECORD* TMP_G = G.BEGIN->nxt;
		while (TMP_G != G.END)
		{
			add_rec(TMP_G->v, END);
			TMP_G = TMP_G->nxt;
		}

		NODE_RECORD* fnd;
		NODE_RECORD* TMP = BEGIN->nxt;
		TMP_G = G.BEGIN->nxt;
		while (TMP != END)
		{
			if (TMP_G->deg) TMP->adj = (NODE_RECORD**)malloc(TMP_G->deg * sizeof(NODE_RECORD*));
			TMP->deg = TMP_G->deg;
			for (int i = 0; i < TMP_G->deg; i++)
			{
				find_rec(TMP_G->adj[i]->v, &fnd);
				TMP->adj[i] = fnd;
				TMP->w[i] = TMP_G->w[i];
			}

			TMP_G = TMP_G->nxt;
			TMP = TMP->nxt;
		}

		return *this;
	}

	void	print()
	{
		NODE_RECORD *to_print = BEGIN->nxt;

		printf("%s (%d): ", GRAPH_NAME_, NODE_CNT);

		if (to_print == END) printf("{ }");

		while (to_print != END)
		{
			printf("{ %d", to_print->v);

			int i = 0;
			if (to_print->deg)
			{
				printf(": %d", to_print->adj[i]->v);
				printf("(%d)", to_print->w[i]);
				if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH) && to_print->adj[0] == to_print) i++;
			}
			for (i++; i < to_print->deg; i++)
			{
				printf(", %d", to_print->adj[i]->v);
				printf("(%d)", to_print->w[i]);
				if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH) && to_print->adj[i] == to_print) i++;
			}
			printf(" } ");

			to_print = to_print->nxt;
		}

		printf("\n");
	}

	int		fread(const char *fname)
	{
		STAT_FLAG_ = 0;
		gfree();

		FILE *file = fopen(fname, "r");
		if (!file) return 0;

		int type = fgetc(file);
		int N, M;
		int **A_mrx, **I_mrx;

		switch (type)
		{
		case 'S': STAT_FLAG_ |= gra::GFLAG_DIGRAPH;
		case 'W':
		{
			if (fscanf(file, " %d\n", &N) != 1) return 0;

			A_mrx = (int **)malloc(N * sizeof(int*));
			for (int i = 0; i < N; i++)
			{
				A_mrx[i] = (int *)malloc(N * sizeof(int));
			}

			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (fscanf(file, "%d ", &A_mrx[i][j]) != 1)
					{
						A_mrx[i][j] = -1;
						fgetc(file);
					}
				}
			}


			for (int i = 1; i <= N; i++) add_v(i);

			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (A_mrx[i][j] > 0)
					{
						if (type == 'S' || i <= j)
						{
							add_e({ i + 1, j + 1, A_mrx[i][j] });
						}
					}
					else if (A_mrx[i][j] == 0)
					{
						if (i != j)
						{
							add_e({ i + 1, j + 1, A_mrx[i][j] });
						}
					}
				}
			}

			fclose(file);

			for (int i = 0; i < N; i++) free(A_mrx[i]);
			free(A_mrx);
			return 1;

		MATRIX_A_ERROR:
			fclose(file);

			for (int i = 0; i < N; i++) free(A_mrx[i]);
			free(A_mrx);
		} return 0;
		}

		return 1;


	}

	void	gfree()
	{
		NODE_RECORD *to_free = BEGIN->nxt;

		while (to_free != END)
		{
			if (to_free->deg > 0)
			{
				free(to_free->adj);
				free(to_free->w);
				to_free->adj = NULL;
				to_free->w = NULL;
			}

			to_free = del_rec(to_free);
		}
	}

	~WGRAPH()
	{
		gfree();
		free(BEGIN);
		free(END);
		free(GRAPH_NAME_);
	}
};

void
main()
{
	WGRAPH w;

	w.fread("1.txt");

	w.W(DFLAG_HEADING | DFLAG_PRINT, NULL);
	cout << endl;

	w.max_stream().print();
	cout << endl;

	w.W_max_stream(DFLAG_HEADING | DFLAG_PRINT, NULL);
}
