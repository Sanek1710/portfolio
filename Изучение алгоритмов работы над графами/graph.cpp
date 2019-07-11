#include "graph.h"
using namespace std;

ostream& operator<<(ostream& os, ADJLIST& al)
{
	for (int i = 0; i < al.v_cnt; i++)
	{
		os << al.v[i].num;
		int j = 0;
		if (al.v[i].deg)
		{
			os << ": " << al.v[i].adj[0];
			if (!al.digraph && al.v[i].adj[0] == al.v[i].num) j++;
		}
		for (j++; j < al.v[i].deg; j++)
		{
			os << ", " << al.v[i].adj[j];
			if (!al.digraph && al.v[i].adj[0] == al.v[i].num) j++;
		}
		os << ';' << endl;
	}
	return os;
}


//private:

//returns founded rec	
GRAPH::NODE_RECORD *	GRAPH::find_rec(NODE node, NODE_RECORD **last)
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
GRAPH::NODE_RECORD *	GRAPH::add_rec(NODE node, NODE_RECORD *before)
{
	NODE_RECORD *NEW_NODE = (NODE_RECORD*)malloc(sizeof(NODE_RECORD));
	NEW_NODE->prv = before->prv;
	NEW_NODE->nxt = before;
	before->prv = NEW_NODE;
	NEW_NODE->prv->nxt = NEW_NODE;

	NEW_NODE->v = node;
	NEW_NODE->deg = 0;
	NEW_NODE->adj = NULL;

	NODE_CNT++;

	return NEW_NODE;
}

//returns next after deleted
GRAPH::NODE_RECORD *	GRAPH::del_rec(NODE_RECORD *to_del)
{
	if (!to_del) return NULL;

	NODE_CNT--;

	NODE_RECORD *to_ret = to_del->nxt;

	if (to_del->deg) free(to_del->adj);

	to_del->nxt->prv = to_del->prv;
	to_del->prv->nxt = to_del->nxt;
	free(to_del);

	return to_ret;
}

char *const GRAPH::name()
{
	return GRAPH_NAME_;
}
//check toread string
int *	GRAPH::check(const char *str)
{
	int *nums = (int *)malloc((3 * strlen(str) / 4) * sizeof(int));
	int temp_idx = 0, num_idx = 1;
	int node_cnt = 0, cur_node = 0;

	int old_num = 0, num = 0;

	while (*str == ' ') str++; //pass ' '

	while (*str == '{')	//pass 'symb'[BLOCK]
	{
		str++;
		node_cnt++;
		//check [BLOCK]

		while (*str == ' ') str++; //pass ' '

		if (!('0' <= *str && *str <= '9')) goto ERROR_END;	//pass num
		while ('0' <= *str && *str <= '9')
		{
			old_num = num;
			num = num * 10 + *str - '0';
			if (old_num > num) goto ERROR_END;
			str++;		//
		}
		nums[num_idx++] = num;
		cur_node = num;
		num = old_num = 0;

		while (*str == ' ') str++; //pass ' '

		if (*str != ':')
		{
			if (*str == '}')
			{
				nums[num_idx++] = 0;
				goto NO_ADJ;	//check 'symb'
			}
		}
		else str++;

		while (*str == ' ') str++; //pass ' '

		temp_idx = num_idx;
		num_idx++;

		if (!('0' <= *str && *str <= '9')) goto ERROR_END;	//pass num
		while ('0' <= *str && *str <= '9')
		{
			old_num = num;
			num = num * 10 + *str - '0';
			if (old_num > num) goto ERROR_END;
			str++;		//
		}
		nums[num_idx++] = num;
		//if (num == cur_node) nums[num_idx++] = num;
		num = old_num = 0;

		while (*str == ' ') str++; //pass ' '

		while (*str == ',' || ('0' <= *str && *str <= '9'))	//pass 'symb'[BLOCK]
		{
			if (*str == ',') str++;
			//check [BLOCK]

			while (*str == ' ') str++; //pass ' '

			if (!('0' <= *str && *str <= '9')) goto ERROR_END;	//pass num
			while ('0' <= *str && *str <= '9')
			{
				old_num = num;
				num = num * 10 + *str - '0';
				if (old_num > num) goto ERROR_END;
				str++;		//
			}
			nums[num_idx++] = num;
			//if (num == cur_node) nums[num_idx++] = num;
			num = old_num = 0;

			while (*str == ' ') str++; //pass ' '
		}

		nums[temp_idx] = num_idx - temp_idx - 1;
	NO_ADJ:

		if (*str != '}') goto ERROR_END;	//check 'symb'
		str++;

		while (*str == ' ') str++; //pass ' '
	}

	if (*str != '\0' && *str != '\n') goto ERROR_END;

	nums[0] = node_cnt;
	nums = (int *)realloc(nums, num_idx * sizeof(int));

	return nums;

ERROR_END:
	free(nums);
	return NULL;
}

//returns (int*) nodes_name array
int *	GRAPH::str_nodes(const char *str, int *node_cnt)
{
	int *nums = (int *)malloc((strlen(str) / 2 + 1) * sizeof(int));
	int num_idx = 0;
	int num = 0, old_num = 0;

	while (*str == ' ') str++; //pass ' '

	if (!('0' <= *str && *str <= '9'))
	{
		if (*str != '\0') goto ERROR_END;	//pass num
		else
		{
			*node_cnt = 0;
			return nums;
		}
	}
	while ('0' <= *str && *str <= '9')
	{
		old_num = num;
		num = num * 10 + *str - '0';
		if (old_num > num) goto ERROR_END;
		str++;		//
	}
	nums[num_idx++] = num;
	num = old_num = 0;

	while (*str == ' ') str++; //pass ' '

	while (*str == ',')	//pass 'symb'[BLOCK]
	{
		str++;
		//check [BLOCK]

		while (*str == ' ') str++; //pass ' '

		if (!('0' <= *str && *str <= '9')) goto ERROR_END;	//pass num
		while ('0' <= *str && *str <= '9')
		{
			old_num = num;
			num = num * 10 + *str - '0';
			if (old_num > num) goto ERROR_END;
			str++;		//
		}
		nums[num_idx++] = num;
		num = old_num = 0;

		while (*str == ' ') str++; //pass ' '
	}

	if (*str != '\0') goto ERROR_END;

	*node_cnt = num_idx;

	nums = (int *)realloc(nums, num_idx * sizeof(int));

	return nums;

ERROR_END:
	free(nums);
	*node_cnt = 0;
	return NULL;
}



int		GRAPH::deg_in(NODE_RECORD *node)
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

int		GRAPH::deg_out(NODE_RECORD *node)
{
	if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
	{
		if (!node) return -1;
		return node->deg;
	}
	return -1;
}

int		GRAPH::deg(NODE_RECORD *node)
{
	if (!node) return -1;

	if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
	{
		return deg_in(node) + deg_out(node);
	}
	else
	{
		return node->deg;
	}
}


void	GRAPH::girth(NODE_RECORD *node1, NODE_RECORD *node2, INT_ARRAY &cur_path, int &opt_dist)
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
int		GRAPH::distance(NODE_RECORD *node1, NODE_RECORD *node2)
{
	INT_ARRAY dist_arr;
	int optimal_distance = NODE_CNT;

	girth(node1, node2, dist_arr, optimal_distance);

	if (optimal_distance == NODE_CNT) return -1;
	return optimal_distance;
}


int		GRAPH::eccentr(NODE_RECORD *node)
{
	int max_dis = -1, dis;

	NODE_RECORD *to_find = BEGIN->nxt;
	while (to_find != END)
	{
		dis = distance(node, to_find);
		if (dis > max_dis) max_dis = dis;

		to_find = to_find->nxt;
	}

	return max_dis;
}

void	GRAPH::ost_d(NODE_RECORD * node1, GRAPH & G)
{
	for (int i = 0; i < node1->deg; i++)
	{
		if (!G.is_contain(node1->adj[i]->v))
		{
			G.add_v(node1->adj[i]->v);

			G.add_e({ node1->v , node1->adj[i]->v });
			ost_d(node1->adj[i], G);
		}
	}
}

void	GRAPH::ost_w(NODE_RECORD * node1, GRAPH & G)
{
	NODE_RECORD temp;
	temp.adj = NULL;
	temp.deg = 0;
	

	for (int i = 0; i < node1->deg; i++)
	{
		if (!G.is_contain(node1->adj[i]->v))
		{
			G.add_v(node1->adj[i]->v);
			G.add_e({ node1->v , node1->adj[i]->v });
			temp.insert(node1->adj[i]);
		}
	}

	for (int i = 0; i < temp.deg; i++)
	{
		ost_w(temp.adj[i], G);
	}

	if (temp.deg > 0) free(temp.adj);
}

void	GRAPH::init(const char *name, BYTE FLAGS)
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


//public:

GRAPH::GRAPH()
{
	init("Graph", gra::GFLAG_SIMPLE);
}

GRAPH::GRAPH(const char *name, BYTE FLAGS)
{
	init(name, FLAGS);
}

GRAPH::GRAPH(BYTE FLAGS)
{
	init("Graph", FLAGS);
}

GRAPH::GRAPH(const char *name)
{
	init(name, gra::GFLAG_SIMPLE);
}

GRAPH::GRAPH(const GRAPH &G)
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
		if (TMP_G->deg) TMP->adj = (NODE_RECORD**)malloc(TMP_G->deg * sizeof(NODE_RECORD*));
		TMP->deg = TMP_G->deg;
		for (int i = 0; i < TMP_G->deg; i++)
		{
			find_rec(TMP_G->adj[i]->v, &fnd);
			TMP->adj[i] = fnd;
		}

		TMP_G = TMP_G->nxt;
		TMP = TMP->nxt;
	}
}

BYTE		GRAPH::set_flag(BYTE FLAGS)
{
	STAT_FLAG_ |= FLAGS;
	if (STAT_FLAG_ & gra::GFLAG_SIMPLE) STAT_FLAG_ = gra::GFLAG_SIMPLE;
	return STAT_FLAG_;
}

BYTE		GRAPH::clr_flag(BYTE FLAGS)
{
	STAT_FLAG_ &= ~FLAGS;
	return STAT_FLAG_;
}

int		GRAPH::n()
{
	return NODE_CNT;
}

int		GRAPH::m()
{
	int res = 0;

	BEG_FORALL_V(*this, 0)
	{
		res += CUR_V(0)->deg;
	}
	END_FORALL_V(*this, 0)

		return (STAT_FLAG_ & gra::GFLAG_DIGRAPH) ? (res) : (res / 2);
}

bool	GRAPH::is_contain(NODE v)
{
	return find_rec(v, &LAST_USED) ? true : false;
}

//returns added count
int		GRAPH::add_v(int count, NODE *v)
{
	NODE_RECORD *founded;
	int add_cnt = 0;

	for (int i = 0; i < count; i++)
	{
		if (!find_rec(v[i], &founded))
		{
			add_cnt++;
			add_rec(v[i], founded);
		}
	}

	return add_cnt;
}

int		GRAPH::add_v(NODE v)
{
	return add_v(1, &v);
}

int		GRAPH::add_v(NODE v, int adj_num, NODE *adj_v)
{
	NODE_RECORD *node1;

	if (!find_rec(v, &node1))
	{
		node1 = add_rec(v, node1);
	}

	if (adj_num <= 0) return 1;

	for (int i = 0; i < adj_num; i++)
	{
		NODE_RECORD *node2;
		if (!find_rec(adj_v[i], &node2)) continue;

		if (!(STAT_FLAG_ & gra::GFLAG_LOOP) && node1->v == adj_v[i]) continue;

		if (!(STAT_FLAG_ & gra::GFLAG_MULTIEDGE) && (node1->find(node2) >= 0)) continue;

		node1->insert(node2);
		if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH)) node2->insert(node1);
		node1->adj[0];
		node1->adj[1];
		node1->adj[2];
	}

	return 1;
}

int		GRAPH::add_v(NODE v, const char *str)
{
	int node_cnt = 0;
	int *nodes = str_nodes(str, &node_cnt);

	if (!nodes) return 0;

	add_v(v, node_cnt, nodes);
	free(nodes);
	return node_cnt;
}

int		GRAPH::del_v(NODE v)
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


int		GRAPH::add_e(EDGE e)
{
	if (!is_contain(e.v1)) return -1;

	return add_v(e.v1, 1, &e.v2);
}

int		GRAPH::del_e(EDGE e)
{
	NODE_RECORD *node1, *node2;

	if (!find_rec(e.v1, &node1)) return 0;
	if (!find_rec(e.v2, &node2)) return 0;

	if (node1->take(node2) < 0) return 0;

	if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH)) node2->take(node1);

	return 1;
}


int		GRAPH::deg_in(NODE v)
{
	if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
	{
		NODE_RECORD *founded;

		if (!find_rec(v, &founded)) return -1;
		return deg_in(founded);
	}
	return -1;
}

int		GRAPH::deg_out(NODE v)
{
	if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
	{
		NODE_RECORD *founded;

		if (!find_rec(v, &founded)) return -1;
		return founded->deg;
	}
	return -1;
}

int		GRAPH::deg(NODE v)
{
	NODE_RECORD *founded;

	if (!find_rec(v, &founded)) return -1;
	return deg(founded);
}

NODE	**	GRAPH::A(BYTE FLAGS, int *sizeN)
{
	int ** A_matrix;
	int heading = (FLAGS & gra::DFLAG_HEADING) ? 1 : 0;
	int size_A = NODE_CNT + heading;
	INT_ARRAY  verties;

	A_matrix = (int**)malloc(size_A * sizeof(int*));
	for (int i = heading; i < size_A; i++)
	{
		A_matrix[i] = (int*)malloc(size_A * sizeof(int));
		memset(A_matrix[i], 0, size_A * sizeof(int));
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
			A_matrix[idx_FROM][idx_TO] += 1;
		}
	}
	END_FORALL_V(*this, 0);

	if (FLAGS & gra::DFLAG_PRINT)
	{
		if (heading)
		{
			cout << "adjacency matrix : " << endl;
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
				printf("%2d ", A_matrix[i][j]);
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

NODE	**	GRAPH::I(BYTE FLAGS, int *sizeN, int *sizeM)
{
	int ** I_matrix;
	int heading = (FLAGS & gra::DFLAG_HEADING) ? 1 : 0;
	int size_N = NODE_CNT + heading;
	int size_M = m() + heading;

	INT_ARRAY  verties;

	I_matrix = (int**)malloc(size_N * sizeof(int*));
	for (int i = 0; i < size_N; i++)
	{
		I_matrix[i] = (int*)malloc(size_M * sizeof(int));
		memset(I_matrix[i], 0, size_M * sizeof(int));
	}

	if (heading) verties.push(0);
	BEG_FORALL_V(*this, 0)
	{
		verties.push(CUR_V(0)->v);
	}
	END_FORALL_V(*this, 0);

	if (heading)
	{
		for (int j = 0; j < size_M; j++)
		{
			I_matrix[0][j] = j;
		}
		for (int i = 1; i < size_N; i++)
		{
			I_matrix[i][0] = verties.arr[i];
		}
	}

	int idx_FROM, idx_TO;
	int cur_edge_idx = heading;
	BEG_FORALL_V(*this, 0)
	{
		idx_FROM = verties.find(CUR_V(0)->v);
		CUR_V(0)->v;
		for (int i = 0; i < CUR_V(0)->deg; i++)
		{
			idx_TO = verties.find(CUR_V(0)->adj[i]->v);
			if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
			{
				if (idx_FROM == idx_TO)
				{
					I_matrix[idx_FROM][cur_edge_idx] = 2;
					if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH)) i++;
				}
				else
				{
					I_matrix[idx_FROM][cur_edge_idx] = 1;
					I_matrix[idx_TO][cur_edge_idx] = -1;
				}
			}
			else
			{
				if (CUR_V(0)->adj[i]->v < CUR_V(0)->v) continue;
				I_matrix[idx_FROM][cur_edge_idx] += 1;
				I_matrix[idx_TO][cur_edge_idx] += 1;

				if (idx_FROM == idx_TO && !(STAT_FLAG_ & gra::GFLAG_DIGRAPH)) i++;
			}

			cur_edge_idx++;
		}
	}
	END_FORALL_V(*this, 0);

	if (FLAGS & gra::DFLAG_PRINT)
	{
		if (heading)
		{
			cout << "incidence matrix : " << endl;
			printf("v\\e ", I_matrix[0][0]);
			for (int j = 1; j < size_M; j++)
			{

				printf("%2d ", I_matrix[0][j]);
			}
			cout << endl;
		}
		for (int i = heading; i < size_N; i++)
		{
			if (heading) printf("%2d: ", I_matrix[i][0]);
			for (int j = heading; j < size_M; j++)
			{
				printf("%2d ", I_matrix[i][j]);
			}
			cout << endl;
		}
	}

	verties.clear();
	if (FLAGS & gra::DFLAG_RETURN)
	{
		if (sizeN) *sizeN = size_N;
		if (sizeM) *sizeM = size_M;
		return I_matrix;
	}
	for (int i = 0; i < size_N; i++)
	{
		free(I_matrix[i]);
		I_matrix[i] = NULL;
	}
	free(I_matrix);
	return NULL;
}

ADJLIST GRAPH::adj_list(BYTE FLAGS)
{
	ADJLIST adjlist;

	adjlist.v_cnt = NODE_CNT;
	adjlist.digraph = !!(STAT_FLAG_ & gra::GFLAG_DIGRAPH);
	adjlist.v = (ADJLIST::ADJNODE*)malloc(NODE_CNT * sizeof(ADJLIST::ADJNODE));

	int i = 0;
	BEG_FORALL_V(*this, 0)
	{
		adjlist.v[i].num = CUR_V(0)->v;
		adjlist.v[i].deg = CUR_V(0)->deg;
		adjlist.v[i].adj = (NODE*)malloc(CUR_V(0)->deg * sizeof(NODE));

		for (int j = 0; j < CUR_V(0)->deg; j++)
		{
			adjlist.v[i].adj[j] = CUR_V(0)->adj[j]->v;
		}

		i++;
	}
	END_FORALL_V(*this, 0);

	if (FLAGS & gra::DFLAG_PRINT)
	{
		if (FLAGS & gra::DFLAG_HEADING) cout << "adjacency list : " << endl;

		ostream &os = cout;
		ADJLIST al = adjlist;

		for (int i = 0; i < al.v_cnt; i++)
		{
			os << al.v[i].num;
			int j = 0;
			if (al.v[i].deg)
			{
				os << ": " << al.v[i].adj[j];
				if (!al.digraph && al.v[i].adj[j] == al.v[i].num) j++;
			}
			for (j++; j < al.v[i].deg; j++)
			{
				os << ", " << al.v[i].adj[j];
				if (!al.digraph && al.v[i].adj[j] == al.v[i].num) j++;
			}
			os << ';' << endl;
		}
		//cout << adjlist;
	}

	if (FLAGS & gra::DFLAG_RETURN) return adjlist;
	else adjlist.clear();

	return adjlist;
}

NODE *	GRAPH::leaf(BYTE FLAGS, int *size)//stop//
{
	INT_ARRAY leaves;
	NODE_RECORD *to_find = BEGIN->nxt;

	while (to_find != END)
	{
		if (deg(to_find) == 1) leaves.push(to_find->v);
		to_find = to_find->nxt;
	}

	if (FLAGS & gra::DFLAG_PRINT)
	{
		if (FLAGS & gra::DFLAG_HEADING) cout << "leaves : ";
		cout << "{";
		if (leaves.size) cout << ' ' << leaves.arr[0];
		for (int i = 1; i < leaves.size; i++)
		{
			cout << ", " << leaves.arr[i];
		}
		cout << " }" << endl;
	}

	if (FLAGS & gra::DFLAG_RETURN)
	{
		if (size) *size = leaves.size;
		return leaves.arr;
	}
	else leaves.clear();
	return NULL;
}

NODE *	GRAPH::isolated(BYTE FLAGS, int *size)
{
	INT_ARRAY isol;
	NODE_RECORD *to_find = BEGIN->nxt;

	while (to_find != END)
	{
		if (deg(to_find) == 0) isol.push(to_find->v);
		to_find = to_find->nxt;
	}

	if (FLAGS & gra::DFLAG_PRINT)
	{
		if (FLAGS & gra::DFLAG_HEADING) cout << "isolated : ";
		cout << "{";
		if (isol.size) cout << ' ' << isol.arr[0];
		for (int i = 1; i < isol.size; i++)
		{
			cout << ", " << isol.arr[i];
		}
		cout << " }" << endl;
	}

	if (FLAGS & gra::DFLAG_RETURN)
	{
		if (size) *size = isol.size;
		return isol.arr;
	}
	else isol.clear();
	return NULL;
}

int *	GRAPH::deg_seq(BYTE FLAGS, int *size)
{
	if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH))
	{
		INT_ARRAY degs;
		NODE_RECORD *to_find = BEGIN->nxt;

		while (to_find != END)
		{
			degs.push(to_find->deg);
			to_find = to_find->nxt;
		}

		if (FLAGS & gra::DFLAG_PRINT)
		{
			if (FLAGS & gra::DFLAG_HEADING) cout << "deg_seq : ";
			cout << "{";
			if (degs.size) cout << ' ' << degs.arr[0];
			for (int i = 1; i < degs.size; i++)
			{
				cout << ", " << degs.arr[i];
			}
			cout << " }" << endl;
		}

		if (FLAGS & gra::DFLAG_RETURN)
		{
			if (size) *size = degs.size;
			return degs.arr;
		}
		else degs.clear();
	}
	else if (FLAGS & gra::DFLAG_PRINT)
	{
		INT_ARRAY degs;
		NODE_RECORD *to_find = BEGIN->nxt;

		while (to_find != END)
		{
			degs.push(deg_in(to_find));
			degs.push(deg_out(to_find));
			to_find = to_find->nxt;
		}

		if (FLAGS & gra::DFLAG_PRINT)
		{
			if (FLAGS & gra::DFLAG_HEADING) cout << "deg_seq (in, out): ";
			cout << "{";
			if (degs.size) cout << " (" << degs.arr[0] << ", " << degs.arr[1] << ")";
			for (int i = 2; i < degs.size; i += 2)
			{
				cout << ", (" << degs.arr[i] << ", " << degs.arr[i + 1] << ")";
			}
			cout << " }" << endl;
		}

		if (FLAGS & gra::DFLAG_RETURN)
		{
			if (size) *size = degs.size;
			return degs.arr;
		}
		else degs.clear();
	}

	return NULL;
}

NODE *	GRAPH::source(BYTE FLAGS, int *size)
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

NODE *	GRAPH::sink(BYTE FLAGS, int *size)
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


int		GRAPH::distance(NODE v1, NODE v2)
{
	NODE_RECORD *node1, *node2;
	if (!find_rec(v1, &node1)) return -1;
	if (!find_rec(v2, &node2)) return -1;

	return distance(node1, node2);
}

int		GRAPH::eccentr(NODE v)
{
	int max = -1;
	NODE_RECORD *node;
	if (!find_rec(v, &node)) return -1;

	return eccentr(node);
}

int		GRAPH::R()
{
	int min_ecc = NODE_CNT, ecc;
	NODE_RECORD *prev_node = BEGIN->nxt;

	NODE_RECORD *to_find = BEGIN->nxt;
	while (to_find != END)
	{
		ecc = eccentr(to_find);
		if (ecc < min_ecc) min_ecc = ecc;
		if (distance(to_find, prev_node) == -1) return -1;

		to_find = to_find->nxt;
	}

	return min_ecc;
}

int		GRAPH::D()
{
	int max_ecc = -1, ecc;
	if (NODE_CNT == 0) return 0;

	NODE_RECORD *prev_node = BEGIN->nxt;

	NODE_RECORD *to_find = BEGIN->nxt;
	while (to_find != END)
	{
		ecc = eccentr(to_find);
		if (ecc > max_ecc) max_ecc = ecc;
		if (distance(to_find, prev_node) == -1) return -1;

		to_find = to_find->nxt;
	}

	return max_ecc;
}

NODE *	GRAPH::centre(BYTE FLAGS, int *size)
{
	INT_ARRAY cntr;
	NODE_RECORD *to_find = BEGIN->nxt;

	int Radius = R();

	if (Radius != -1)
	{
		while (to_find != END)
		{
			if (eccentr(to_find) == Radius) cntr.push(to_find->v);
			to_find = to_find->nxt;
		}
	}

	if (FLAGS & gra::DFLAG_PRINT)
	{
		if (FLAGS & gra::DFLAG_HEADING) cout << "centre : ";
		cout << "{";
		if (cntr.size) cout << ' ' << cntr.arr[0];
		for (int i = 1; i < cntr.size; i++)
		{
			cout << ", " << cntr.arr[i];
		}
		cout << " }" << endl;
	}

	if (FLAGS & gra::DFLAG_RETURN)
	{
		if (size) *size = cntr.size;
		return cntr.arr;
	}
	else cntr.clear();
	return NULL;
}

NODE *	GRAPH::periph(BYTE FLAGS, int *size)
{
	INT_ARRAY prph;
	NODE_RECORD *to_find = BEGIN->nxt;

	int Diametre = D();

	if (Diametre != -1)
	{
		while (to_find != END)
		{
			if (eccentr(to_find) == Diametre) prph.push(to_find->v);
			to_find = to_find->nxt;
		}
	}

	if (FLAGS & gra::DFLAG_PRINT)
	{
		if (FLAGS & gra::DFLAG_HEADING) cout << "peripheral : ";
		cout << "{";
		if (prph.size) cout << ' ' << prph.arr[0];
		for (int i = 1; i < prph.size; i++)
		{
			cout << ", " << prph.arr[i];
		}
		cout << " }" << endl;
	}

	if (FLAGS & gra::DFLAG_RETURN)
	{
		if (size) *size = prph.size;
		return prph.arr;
	}
	else prph.clear();
	return NULL;
}


int		GRAPH::subdivide(EDGE e)
{
	if (!del_e(e)) return -1;

	NODE w = 1;
	NODE_RECORD *to_find = BEGIN->nxt;

	while (to_find != END && to_find->v == w)
	{
		w++;
		to_find = to_find->nxt;
	}

	add_v(w, 1, &e.v2);
	add_e({ e.v1, w });

	return w;
}

/*
int		GRAPH::identify(NODE v1, NODE v2)
{
	NODE_RECORD *node1 = NULL, *node2 = NULL;

	if (!find_rec(v1, &node1)) return 0;
	if (!find_rec(v2, &node2)) return 0;

	if (!(STAT_FLAG_ & gra::GFLAG_LOOP))
	{
		while (node1->take(node2) >= 0);
		while (node2->take(node1) >= 0);
	}

	for (int i = 0; i < node2->deg; i++)
	{
		node1->insert(node2->adj[i]);
	}
	if (!(STAT_FLAG_ & gra::GFLAG_MULTIEDGE)) node1->uniq();

	if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
	{
		NODE_RECORD *to_find = BEGIN->nxt;
		while (to_find != END)
		{
			while (to_find->take(node2) >= 0)
			{
				to_find->insert(node1);
			}

			if (!(STAT_FLAG_ & gra::GFLAG_MULTIEDGE))
			{
				to_find->uniq();
			}

			to_find = to_find->nxt;
		}
	}
	else
	{
		for (int i = 0; i < node2->deg; i++)
		{
			while (node2->adj[i]->take(node2) >= 0)
			{
				node2->adj[i]->insert(node1);
			}

			if (!(STAT_FLAG_ & gra::GFLAG_MULTIEDGE))
			{
				node2->adj[i]->uniq();
			}
		}
	}

	del_v(v2);

	return 1;
}
*/

int		GRAPH::identify(NODE v1, NODE v2)
{
	NODE_RECORD *node1 = NULL, *node2 = NULL;

	if (v1 == v2) return 0;
	if (!find_rec(v1, &node1)) return 0;
	if (!find_rec(v2, &node2)) return 0;

	while (node1->take(node2) != -1);
	while (node2->take(node1) != -1);

	while(node2->deg > 0)
	{
		//if (CUR_V(0) == node2) CONTINUE_V(0);
		//указатели на удаляемую

		NODE_RECORD *cur_node1 = (node2->adj[0] == node2) ? (node1) : (node2->adj[0]);
		NODE_RECORD *cur_node2 = node2->adj[0];
		
		int k = 0;
		k = node1->find(cur_node1);

		if (k != -1)
		{
			while (node1->adj[k] == cur_node1 && node2->take(cur_node2) != -1)
			{
				k++;
				if (k == node1->deg) break;
			}
		}

		while (node2->take(cur_node2) != -1)
		{
			node1->insert(cur_node1);
		}
	}

	BEG_FORALL_V(*this, 0)
	{
		if (CUR_V(0) == node2) CONTINUE_V(0);
		//указатели на удаляемую

		int k = 0;
		k = CUR_V(0)->find(node1);

		if (k != -1)
		{
			while (CUR_V(0)->adj[k] == node1 && CUR_V(0)->take(node2) != -1)
			{
				k++;
				if (k == CUR_V(0)->deg) break;
			}
		}

		while (CUR_V(0)->take(node2) != -1)
		{
			CUR_V(0)->insert(node1);
		}
	}
	END_FORALL_V(*this, 0);

	del_v(v2);
}

int		GRAPH::contruct(EDGE e)
{
	if (!del_e(e)) return 0;

	return identify(e.v1, e.v2);
}


int		GRAPH::duplicate(NODE v)
{
	NODE w = 1;
	NODE_RECORD *node;

	if (!find_rec(v, &node)) return -1;

	NODE_RECORD *to_find = BEGIN->nxt;
	if (to_find->v == 0) to_find = to_find->nxt;

	while (to_find != END && to_find->v == w)
	{
		w++;
		to_find = to_find->nxt;
	}

	NODE_RECORD *New_Node = add_rec(w, to_find);

	if (STAT_FLAG_ & gra::GFLAG_DIGRAPH)
	{
		to_find = BEGIN->nxt;
		while (to_find != END)
		{
			int find_idx = to_find->find(node);
			if (find_idx >= 0)
			{
				while (find_idx < to_find->deg && to_find->adj[find_idx] == node)
				{
					if (to_find == node) New_Node->insert(New_Node);
					else to_find->insert(New_Node);
					find_idx++;
				}
			}

			to_find = to_find->nxt;
		}

		for (int i = 0; i < node->deg; i++)
		{
			if (node->adj[i] != node)
			{
				New_Node->insert(node->adj[i]);
			}
		}
	}
	else
	{
		for (int i = 0; i < node->deg; i++)
		{
			if (node->adj[i] == node)
			{
				New_Node->insert(New_Node);
				New_Node->insert(New_Node);
			}
			else
			{
				New_Node->insert(node->adj[i]);
				node->adj[i]->insert(New_Node);
			}
		}
	}

	return w;
}

int		GRAPH::reproduct(NODE v)
{
	NODE w = duplicate(v);
	if (w < 0) return -1;

	add_e({ v, w });

	if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH))
	{

	}
	else
	{

	}

	return w;
}


void	GRAPH::gfree()
{
	NODE_RECORD *to_free = BEGIN->nxt;

	while (to_free != END)
	{
		if (to_free->deg > 0)
		{
			free(to_free->adj);
			to_free->adj = NULL;
		}

		to_free = del_rec(to_free);
	}
}


void	GRAPH::print()
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
			if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH) && to_print->adj[0] == to_print) i++;
		}
		for (i++; i < to_print->deg; i++)
		{
			printf(", %d", to_print->adj[i]->v);
			if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH) && to_print->adj[i] == to_print) i++;
		}
		printf(" } ");

		to_print = to_print->nxt;
	}

	printf("\n");
}

void	GRAPH::fprint(const char *fname)
{
	NODE_RECORD *to_print = BEGIN->nxt;

	FILE *file = fopen(fname, "w");

	if (to_print == END) fprintf(file, "{ }");

	while (to_print != END)
	{
		fprintf(file, "{ %d", to_print->v);

		int i = 0;
		if (to_print->deg)
		{
			fprintf(file, ": %d", to_print->adj[i]->v);
			if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH) && to_print->adj[0] == to_print) i++;
		}
		for (i++; i < to_print->deg; i++)
		{
			fprintf(file, ", %d", to_print->adj[i]->v);
			if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH) && to_print->adj[0] == to_print) i++;
		}
		fprintf(file, " } ");

		to_print = to_print->nxt;
	}

	fclose(file);
}

int		GRAPH::read(const char *format)
{
	gfree();

	int *nodes; // { 1: 2, 3 } { 2: 2 }-> [ncnt]2, [num]1, [adj_cnt]2, [adj:]2, 3,  2, 1, 2
	nodes = check(format);

	if (nodes == NULL) return 0;

	int j = 1;
	for (int i = 0; i < *nodes; i++)
	{
		add_v(nodes[j], 0, NULL);
		j += nodes[j + 1] + 2;
	}

	j = 1;
	for (int i = 0; i < *nodes; i++)
	{
		add_v(nodes[j], nodes[j + 1], &nodes[j + 2]);
		j += nodes[j + 1] + 2;
	}

	free(nodes);

	return 1;
}

int		GRAPH::fread(const char *fname)
{
	gfree();

	FILE *file = fopen(fname, "r");
	if (!file) return 0;

	int type = fgetc(file);
	int N, M;
	int **A_mrx, **I_mrx;

	switch (type)
	{
	case 'A':
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
				if (fscanf(file, "%d ", &A_mrx[i][j]) != 1) goto MATRIX_A_ERROR;
				
				if (A_mrx[i][j] < 0) goto MATRIX_A_ERROR;
				if (i == j && A_mrx[i][j])
				{
					if (!(STAT_FLAG_ & gra::GFLAG_LOOP) || A_mrx[i][j] & 1 == 1) goto MATRIX_A_ERROR;
				}

				if (!(STAT_FLAG_ & gra::GFLAG_MULTIEDGE))
				{
					if (i != j && A_mrx[i][j] > 1) goto MATRIX_A_ERROR;
					if (i == j && A_mrx[i][j] > 2) goto MATRIX_A_ERROR;
				}
			}
		}
		

		for (int i = 1; i <= N; i++) add_v(i);

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				while (A_mrx[i][j]--) add_e({ i + 1, j + 1 });
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

	case 'I': 
	{
		if (fscanf(file, " %d %d\n", &N, &M) != 2) return 0;

		I_mrx = (int **)malloc(N * sizeof(int*));
		for (int i = 0; i < N; i++)
		{
			I_mrx[i] = (int *)malloc(M * sizeof(int));
		}

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				if (fscanf(file, "%d ", &I_mrx[i][j]) != 1) goto MATRIX_I_ERROR;
			}
		}

		for (int j = 0; j < M; j++)
		{
			int cnt = 0, sum = 0;
			for (int i = 0; i < N; i++)
			{
				if (I_mrx[i][j])
				{
					cnt++;
					sum += I_mrx[i][j];
				}
			
				if (I_mrx[i][j] < -1 || 2 < I_mrx[i][j]) goto MATRIX_I_ERROR;
				if (I_mrx[i][j] == 2 && !(STAT_FLAG_ & gra::GFLAG_LOOP)) goto MATRIX_I_ERROR;
				if (I_mrx[i][j] == -1 && !(STAT_FLAG_ & gra::GFLAG_DIGRAPH)) goto MATRIX_I_ERROR;
			}
			if (sum == 2)
			{
				if (STAT_FLAG_ & gra::GFLAG_DIGRAPH && cnt != 1) goto MATRIX_I_ERROR;
				if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH) && cnt != 1 && cnt != 2) goto MATRIX_I_ERROR;
			}
			else if (sum == 0)
			{
				if (STAT_FLAG_ & gra::GFLAG_DIGRAPH && cnt != 2) goto MATRIX_I_ERROR;
			}
			else goto MATRIX_I_ERROR;


		}

		for (int i = 1; i <= N; i++) add_v(i);

		for (int j = 0; j < M; j++)
		{
			NODE v1 = -1, v2 = -1;
			for (int i = 0; i < N; i++)
			{
				if (I_mrx[i][j] == -1) v2 = i + 1;
				else if (I_mrx[i][j] == 1)
				{
					if (v1 == -1) v1 = i + 1;
					else v2 = i + 1;
				}
				else if (I_mrx[i][j] == 2) v1 = v2 = i + 1;
			}
			if (v1 != -1 && v2 != -1) add_e({ v1, v2 });
		}

		fclose(file);

		for (int i = 0; i < N; i++) free(I_mrx[i]);
		free(I_mrx);
		return 1;
	MATRIX_I_ERROR:
		fclose(file);

		for (int i = 0; i < N; i++) free(I_mrx[i]);
		free(I_mrx);
	} return 0;

	default:
	{
		int *nodes; // { 1: 2, 3 } { 2: 2 }-> [ncnt]2, [num]1, [adj_cnt]2, [adj:]2, 3,  2, 1, 2
		char format[10000];
		fgets(format + 1, 99999, file);
		format[0] = type;
		fclose(file);

		nodes = check(format);

		if (nodes == NULL) return 0;


		int j = 1;
		for (int i = 0; i < *nodes; i++)
		{
			add_v(nodes[j], 0, NULL);
			j += nodes[j + 1] + 2;
		}

		j = 1;
		for (int i = 0; i < *nodes; i++)
		{
			int k = 0;
			if (!(STAT_FLAG_ & gra::GFLAG_DIGRAPH))
			{
				while (nodes[j + 2 + k] < nodes[j]) k++;
			}
			
			add_v(nodes[j], nodes[j + 1] - k, &nodes[j + 2 + k]);
			j += nodes[j + 1] + 2;
		}

		free(nodes);

		return 1;
	} break;	
	}

	return 1;


}

void	GRAPH::order(NODE start_val)
{
	int new_node_num = start_val;
	BEG_FORALL_V(*this, 0)
	{
		CUR_V(0)->v = new_node_num;
		new_node_num++;
	}
	END_FORALL_V(*this, 0)
}


GRAPH	GRAPH::	operator =(GRAPH G)
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
		}

		TMP_G = TMP_G->nxt;
		TMP = TMP->nxt;
	}

	return *this;
}

GRAPH	&GRAPH::operator << (NODE v)
{
	this->add_v(v);
	return *this;
}

GRAPH	&GRAPH::operator >> (NODE v)
{
	this->del_v(v);
	return *this;
}

GRAPH	&GRAPH::operator << (EDGE e)
{
	this->add_e(e);
	return *this;
}

GRAPH	&GRAPH::operator >>	(EDGE e)
{
	this->del_e(e);
	return *this;
}


GRAPH	GRAPH::ost_d(NODE v)
{
	GRAPH depth_ost;
	NODE_RECORD *node;

	if (NODE_CNT == 0) return depth_ost;
	if (v < 1 || find_rec(v, &node)) node = BEGIN->nxt;

	depth_ost.add_v(node->v);
	
	ost_d(node, depth_ost);

	return depth_ost;
}

GRAPH	GRAPH::ost_d()
{
	GRAPH depth_ost;
	NODE_RECORD *node;

	if (NODE_CNT == 0) return depth_ost;

	BEG_FORALL_V(*this, 0)
	{
		find_rec(CUR_V(0)->v, &node);
		if (!depth_ost.is_contain(CUR_V(0)->v))
		{
			depth_ost.add_v(node->v);
			ost_d(node, depth_ost);
		}
	}
	END_FORALL_V(*this, 0);

	return depth_ost;
}

GRAPH	GRAPH::ost_w(NODE v)
{
	GRAPH depth_ost;
	NODE_RECORD *node;

	if (NODE_CNT == 0) return depth_ost;
	if (v < 1 || find_rec(v, &node)) node = BEGIN->nxt;

	depth_ost.add_v(node->v);
	ost_w(node, depth_ost);

	return depth_ost;
}

GRAPH	GRAPH::ost_w()
{
	GRAPH depth_ost;
	NODE_RECORD *node;

	if (NODE_CNT == 0) return depth_ost;

	BEG_FORALL_V(*this, 0)
	{
		find_rec(CUR_V(0)->v, &node);
		if (!depth_ost.is_contain(CUR_V(0)->v))
		{
			depth_ost.add_v(node->v);
			ost_w(node, depth_ost);
		}
	}
	END_FORALL_V(*this, 0);

	return depth_ost;
}


GRAPH::~GRAPH()
{
	gfree();
	free(BEGIN);
	free(END);
	free(GRAPH_NAME_);
}

ostream& operator<<(ostream& os, GRAPH& gr)
{
	gr.print();
	return os;
}