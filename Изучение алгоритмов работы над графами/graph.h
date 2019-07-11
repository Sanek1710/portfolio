#pragma once
#define _CRT_SECURE_NO_WARNINGS


#include <stdlib.h>
#include <iostream>
#include <string>

#define BEG_FORALL_V(G, id) \
			{ NODE_RECORD *___TO_RUN_##id = (G).BEGIN->nxt;\
			while ((___TO_RUN_##id) != (G).END) {

#define END_FORALL_V(G, id) \
			(___TO_RUN_##id) = (___TO_RUN_##id)->nxt; } }
//
#define CUR_V(id)		(___TO_RUN_##id)

#define CONTINUE_V(id)	{ ___TO_RUN_##id = ___TO_RUN_##id->nxt; continue; }

#define BEG_FORINT_V(id, beg, end) \
			{ NODE_RECORD *___TO_RUN_##id = beg;\
			while (___TO_RUN_##id != end) {

#define END_FORINT_V(id)	END_FORALL_V(NULL, id)


using namespace std;

typedef unsigned char BYTE;
namespace gra
{
	static const BYTE	//Graph Flags for Graph initialisation
		GFLAG_SIMPLE	= 0b10000000,
		GFLAG_DIGRAPH	= 0b00000001,
		GFLAG_LOOP		= 0b00000010,
		GFLAG_MULTIEDGE = 0b00000100,
		GFLAG_WEIGHT	= 0b00001000;

	static const BYTE	//Data Flags for functions
		DFLAG_PRINT		= 0b00001000,
		DFLAG_HEADING	= 0b00010000,
		DFLAG_RETURN	= 0b00100000;
}


typedef int NODE, VERTICE;
typedef int WEIGHT;

typedef struct EDGE
{
	int v1, v2;
}EDGE, ARROW;

typedef struct INT_ARRAY
{
	int *arr = NULL;
	int size = 0;

	int push(int elem)
	{
		if (size == 0) arr = (int*)malloc(1 * sizeof(int));
		else arr = (int*)realloc(arr, (size + 1) * sizeof(int));

		arr[size] = elem;
		size++;

		return size;
	}

	int pop(int *elem)
	{
		if (size == 0) return -1;

		size--;
		if (elem) *elem = arr[size];

		if (size == 0)
		{
			free(arr);
			arr = NULL;
		}
		else arr = (int*)realloc(arr, size * sizeof(int));
		return size;
	}

	int	find(int elem)
	{
		int idx = size - 1;
		while (idx >= 0 && arr[idx] != elem) idx--;

		return idx;
	}

	void sort()
	{
		for (int i = size - 1; i >= 1; i--)
		{
			int max_idx = i;
			for (int j = 0; j <= i; j++)
			{
				if (arr[j] > arr[max_idx]) max_idx = j;
			}
			if (max_idx < i)
			{
				swap<int>(arr[i], arr[max_idx]);
			}
		}
	}

	void clear()
	{
		if (size) free(arr);
		arr = NULL;
		size = 0;
	}
	
	void print()
	{
		for (int i = 0; i < size; i++)
		{
			cout << arr[i] << ' ';
		}
		cout << endl;
	}

}INT_ARRAY;


typedef struct ADJLIST
{
	struct ADJNODE
	{
		NODE num;
		int deg = 0;
		NODE *adj = NULL;
	}*v = NULL;

	int v_cnt = 0;
	bool digraph;

	void clear()
	{
		for (int i = 0; i < v_cnt; i++)
		{
			if (v[i].deg) free(v[i].adj);
		}
		free(v);
	}

}ADJLIST;

ostream& operator<<(ostream& os, ADJLIST& al);


class GRAPH
{
private:
	char *GRAPH_NAME_;
	BYTE  STAT_FLAG_;

	typedef struct NODE_RECORD
	{
		NODE v;	//name
		int  deg;
		NODE_RECORD **adj;	//adjasent
		NODE_RECORD *nxt, *prv;

		//returns deg (cnt)
		int	insert(NODE_RECORD *node)
		{
			int i = deg;

			adj = (NODE_RECORD**)realloc(adj, (deg + 1) * sizeof(NODE_RECORD*));

			while (i > 0 && adj[i - 1]->v > node->v)
			{
				adj[i] = adj[i - 1];
				i--;
			}
			adj[i] = node;

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

			return deg;
		}

		void uniq()
		{
			int j = 1, i = 0;

			while (j < deg)
			{
				while (j < deg && adj[i]->v == adj[j]->v) j++;
				if (j < deg) adj[++i] = adj[j++];
			}
			i++;

			if (i < deg)
			{
				deg = i;
				adj = (NODE_RECORD**)realloc(adj, i * sizeof(NODE_RECORD*));
			}
		}

	}NODE_RECORD;

	int NODE_CNT = 0;
	NODE_RECORD *BEGIN = NULL, *END = NULL;
	NODE_RECORD *LAST_USED = NULL;

	NODE_RECORD *	find_rec(NODE node, NODE_RECORD **last);
	NODE_RECORD *	add_rec(NODE node, NODE_RECORD *before);
	NODE_RECORD *	del_rec(NODE_RECORD *to_del);


	void	init		(const char *name, BYTE FLAGS);

	int *	check		(const char *str);

	int *	str_nodes	(const char *str, int *node_cnt);

	int		deg_in		(NODE_RECORD *node);
	int		deg_out		(NODE_RECORD *node);
	int		deg			(NODE_RECORD *node);

	void	girth		(NODE_RECORD *node1, NODE_RECORD *node2, INT_ARRAY &cur_path, int &opt_dist);
	int		distance	(NODE_RECORD *node1, NODE_RECORD *node2);

	void	ost_d		(NODE_RECORD * node1, GRAPH & G);
	void	ost_w		(NODE_RECORD * node1, GRAPH & G);

	int		eccentr		(NODE_RECORD *node);

public:
			GRAPH		();
			GRAPH		(const char *name, BYTE FLAGS);
			GRAPH		(BYTE FLAGS);
			GRAPH		(const char *name);
			GRAPH		(const GRAPH &G);

	char * const	name();
	BYTE	set_flag	(BYTE FLAGS);
	BYTE	clr_flag	(BYTE FLAGS);

	int		n();
	int		m();

	bool	is_contain	(NODE v);
	int		add_v		(int count, NODE *v);
	int		add_v		(NODE v);
	int		add_v		(NODE v, int adj_num, NODE *adj_v);
	int		add_v		(NODE v, const char *str);

	int		del_v		(NODE v);
	int		add_e		(EDGE e);
	int		del_e		(EDGE e);

	int		deg_in		(NODE v);
	int		deg_out		(NODE v);
	int		deg			(NODE v);

	NODE **	A			(BYTE FLAGS, int *sizeN);
	NODE **	I			(BYTE FLAGS, int *sizeN, int *sizeM);

	ADJLIST adj_list	(BYTE FLAGS);

	NODE *	leaf		(BYTE FLAGS, int *size);
	NODE *	isolated	(BYTE FLAGS, int *size);

	int *	deg_seq		(BYTE FLAGS, int *size);

	NODE *	source		(BYTE FLAGS, int *size);
	NODE *	sink		(BYTE FLAGS, int *size);

	int		distance	(NODE v1, NODE v2);

	int		eccentr		(NODE v);

	int		R();
	int		D();

	NODE *	centre		(BYTE FLAGS, int *size);
	NODE *	periph		(BYTE FLAGS, int *size);

	int		subdivide	(EDGE e);

	int		identify	(NODE v1, NODE v2);
	int		contruct	(EDGE e);

	int		duplicate	(NODE v);
	int		reproduct	(NODE v);

	void	gfree();

	void	order	(NODE start_val);

	GRAPH	&operator << (NODE v);
	GRAPH	&operator >> (NODE v);
	GRAPH	&operator << (EDGE e);
	GRAPH	&operator >> (EDGE e);

	GRAPH			operator =(GRAPH G);
	GRAPH			operator ~()
	{
		NODE_RECORD TMP_NODE;

		GRAPH rev_G = *this; //START

		BEG_FORALL_V(rev_G, 0)
		{
			TMP_NODE.deg = 0;
			TMP_NODE.adj = NULL;

			BEG_FORALL_V(rev_G, 1)
			{
				if (CUR_V(0)->find(CUR_V(1)) == -1 && CUR_V(1) != CUR_V(0))
				{
					TMP_NODE.insert(CUR_V(1));
				}
			}
			END_FORALL_V(rev_G, 1);

			if (CUR_V(0)->deg) free(CUR_V(0)->adj);
			CUR_V(0)->deg = TMP_NODE.deg;
			CUR_V(0)->adj = TMP_NODE.adj;
		}
		END_FORALL_V(rev_G, 0);



		return rev_G;
	}

	friend GRAPH	operator |(GRAPH G1, GRAPH G2)
	{
		if (G1.STAT_FLAG_ != G2.STAT_FLAG_)
		{
			cout << "union G1 | G2 : DIFFERENT GFLAGS ERROR" << endl;
			return G1;
		}

		G1.order(1);
		G2.order(G1.NODE_CNT + 1);

		BEG_FORALL_V(G2, 0)
		{
			G1.add_v(CUR_V(0)->v);
		}
		END_FORALL_V(G2, 0);

		BEG_FORALL_V(G2, 0)
		{
			NODE_RECORD* to_find;
			G1.find_rec(CUR_V(0)->v, &to_find);
			
			NODE_RECORD** &arr1 = to_find->adj; //arr1
			NODE_RECORD** &arr2 = CUR_V(0)->adj;//arr2
			
			NODE_RECORD* to_insert;
			int i = 0, j = 0;
			
			while (i < CUR_V(0)->deg)
			{
				while (j < to_find->deg && arr1[j]->v < arr2[i]->v) j++;
				while (j < to_find->deg && i < CUR_V(0)->deg && arr1[j]->v == arr2[i]->v) i++, j++;
				while (i < CUR_V(0)->deg && (j == to_find->deg || arr2[i]->v < arr1[j]->v))
				{
					G1.find_rec(arr2[i]->v, &to_insert);
					to_find->insert(to_insert);
					i++, j++;
				}
			}

		}
		END_FORALL_V(G2, 0);

		return G1;
	}
	friend GRAPH	operator &(GRAPH G1, GRAPH G2)
	{

	}

	friend GRAPH	operator +(GRAPH G1, GRAPH G2)
	{
		if (G1.STAT_FLAG_ != G2.STAT_FLAG_)
		{
			cout << "summ G1 + G2 : DIFFERENT GFLAGS ERROR" << endl;
			return G1;
		}

		G1.order(1);
		G2.order(G1.NODE_CNT + 1);

		GRAPH sum_G = (G1 | G2);

		NODE_RECORD *MIDLE;
		sum_G.find_rec(G2.BEGIN->nxt->v, &MIDLE);

		BEG_FORINT_V(left, sum_G.BEGIN->nxt, MIDLE)
		{
			BEG_FORINT_V(right, MIDLE, sum_G.END)
			{
				sum_G.add_e({ CUR_V(left)->v, CUR_V(right)->v });
			}
			END_FORINT_V(right)
		}
		END_FORINT_V(left)

			return sum_G;
	}
	friend GRAPH	operator *(GRAPH G1, GRAPH G2)
	{
		if (G1.STAT_FLAG_ != G2.STAT_FLAG_)
		{
			cout << "mult G1 * G2 : DIFFERENT GFLAGS ERROR" << endl;
			return G1;
		}

		int pre_name = G1.NODE_CNT;

		NODE vnum = 1;
		BEG_FORALL_V(G2, 0)
		{
			CUR_V(0)->v = vnum;
			vnum++;
		}
		END_FORALL_V(G2, 0);

		GRAPH mlt_G("mlt_G", G1.STAT_FLAG_);

		for (int i = 0; i < G2.NODE_CNT; i++)
		{
			vnum = 1;
			BEG_FORALL_V(G1, 0)
			{
				CUR_V(0)->v = i * pre_name + vnum;
				vnum++;
			}
			END_FORALL_V(G1, 0);
			mlt_G = mlt_G | G1;
		}

		NODE_RECORD *to_find;
		BEG_FORALL_V(mlt_G, 0)
		{
			___TO_RUN_0->v;
			G2.find_rec(CUR_V(0)->v / pre_name + 1, &to_find);

			for (int i = 1; i <= pre_name; i++)
			{
				for (int j = 0; j < to_find->deg; j++)
				{
					NODE_RECORD *to_find_in;
					mlt_G.find_rec((to_find->adj[j]->v - 1)*pre_name + i, &to_find_in);
					CUR_V(0)->insert(to_find_in);
				}
				END_FORALL_V(mlt_G, 0)
			}
		}

		return mlt_G;
	}

	GRAPH ost_d(NODE v);
	GRAPH ost_d();

	GRAPH ost_w(NODE v);
	GRAPH ost_w();


	/*friend GRAPH	operator +(GRAPH G1, GRAPH G2)
	{
		if (G1.STAT_FLAG_ != G2.STAT_FLAG_)
		{
			cout << "summ G1 + G2 : DIFFERENT GFLAGS ERROR" << endl;
			return G1;
		}

		GRAPH sum_G = (G1 | G2);

		BEG_FORALL_V(sum_G, 0)
		{
			if (G1.is_contain(CUR_V(0)->v))
			{
				if (G2.is_contain(CUR_V(0)->v)) CONTINUE_V(0);
				else
				{
					BEG_FORALL_V(sum_G, 1)
					{
						if (G2.is_contain(CUR_V(1)->v) && !G1.is_contain(CUR_V(1)->v))
						{
							sum_G.add_e({ CUR_V(0)->v, CUR_V(1)->v });
						}
					}
					END_FORALL_V(sum_G, 1)
				}
			}
		}
		END_FORALL_V(sum_G, 0)

		return sum_G;
	}*/

	void	print();
	void	fprint(const char *fname);

	int		read(const char *format);
	int		fread(const char *fname); 

	~GRAPH();
};

ostream& operator<<(ostream& os, GRAPH& gr);
