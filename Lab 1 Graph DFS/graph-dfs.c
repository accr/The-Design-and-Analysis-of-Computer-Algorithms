#include<stdio.h>
#include<string.h>
#include<malloc.h>

#define MAX_VEX_NUM 20
#define TRUE 1
#define FALSE 0

#define OK 1
#define FAIL 0

int visited[MAX_VEX_NUM];
typedef int status;
typedef int q_elemtype;

// 邻接表的数据结构
enum GraphyKind
{
    DG,
    DN,
    UDG,
    UDN
};

struct  ArcNode
{
    int adjvex_no;
    void *arcinfo;
    struct ArcNode *nextarc;
};

struct VNode
{
    char data[3];
    struct ArcNode *firstarc;
};

struct ALGraphy
{
    int vexnum;
    int arcnum;
    enum GraphyKind kind; 
    struct VNode *HeadNode;
};

/* 链队列的数据结构  */
struct QNode
{
    q_elemtype data; 
    struct QNode *next;
};

struct linked_queue
{
    struct QNode *front;
    struct QNode *rear;
};

status init_queue(struct linked_queue *Q)
{
    (*Q).front = (*Q).rear = NULL;

    return OK;
}

int queue_empty(struct linked_queue Q)
{
    if (!Q.rear && !Q.front)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

status enqueue(struct linked_queue *Q, q_elemtype elem)
{
    struct QNode *p;

    p = (struct QNode*)malloc(sizeof(struct QNode));
    if (!p)
    {
        return FAIL;
    }

    p->data = elem;
    p->next = NULL;
 
    if (queue_empty(*Q))
    {
        (*Q).front = (*Q).rear = p;
    }
    else
    {
        (*Q).rear->next = p; 
        (*Q).rear = p;
    }
   
    return OK;
} 

status dequeue(struct linked_queue *Q, q_elemtype *elem)
{
    struct QNode *p;

    if (queue_empty(*Q))
    {
        return FAIL;
    }
    else
    {
        p = (*Q).front;
        (*elem) = p->data;
        (*Q).front = p->next;

        if (!p->next)
        {
            (*Q).rear = NULL;
        }

        free(p);

        return OK;
    }
}

int get_first_adjvex(struct ALGraphy G, int vex_no)
{
    if (G.HeadNode[vex_no].firstarc)
    {
        return G.HeadNode[vex_no].firstarc->adjvex_no; 
    }

    return -1;
}

int get_next_adjvex(struct ALGraphy G, int vex_no, int w)
{
    struct ArcNode *p;

    p = G.HeadNode[vex_no].firstarc;

    while (p && p->adjvex_no != w)
    {
        p = p->nextarc;
    }

    if (p && p->nextarc) 
    {
        return p->nextarc->adjvex_no; 
    }
    else
    {
        return -1;
    }
}

void visit(struct ALGraphy G, int vex_no)
{
    printf("visit %s\n", G.HeadNode[vex_no].data);
}

void DFS(struct ALGraphy G, int vex_no)
{
    int i;

    visited[vex_no] = TRUE;
    visit(G, vex_no);

    for (i = get_first_adjvex(G, vex_no); i != -1; i = get_next_adjvex(G, vex_no, i))
    {
        if (!visited[i])
        {
            DFS(G, i);
        }
    } 
}

void DFS_traverse(struct ALGraphy G)
{
    int i;

    for (i = 0; i < G.vexnum; i++)
    {
        visited[i] = FALSE;
    }

    i = 0;

    /* !不要误解这个for循环，这个只是针对非连通图而设置，对于连通图，只须DFS(G,0)即可 */
    //for (i = 0; i < G.vexnum; i++)
    //{
    //    if (!visited[i])
    //    {
            DFS(G, i);
    //    }
    //}
}

void BFS_traverse(struct ALGraphy G)
{
    int i;
    q_elemtype vex_no = 0;
    struct linked_queue Q;

    for (i = 0; i < G.vexnum; i++)
    {
        visited[i] = FALSE;
    }

    init_queue(&Q);

    /* !这里没有考虑非连通图的情况，反而便于读者理解，理解了后就很容易去理解非连通图的处理了 */
    visited[vex_no] = TRUE;
    visit(G, vex_no);
    enqueue(&Q, vex_no);

    while (!queue_empty(Q))
    {
        dequeue(&Q, &vex_no);

        for (i = get_first_adjvex(G, vex_no); i != -1; i = get_next_adjvex(G, vex_no, i))
        {
            if (!visited[i])
            {
                visited[i] = TRUE;
                visit(G, i);
                enqueue(&Q, i);
            }
        }
    }


}

status create_graphy(struct ALGraphy *G)
{
    int i;
    int v;
    struct ArcNode *pArcNode;
    char name[3] = {0};

    printf("please input vexnum:");
    scanf("%d", &(*G).vexnum);
    printf("please input graphy kind:(0(DG),1(DN),2(UDG),3(UDN):");
    scanf("%d", &(*G).kind);

    (*G).arcnum = 0;

    (*G).HeadNode = (struct VNode*)malloc(sizeof(struct VNode) * (*G).vexnum);
    if (!(*G).HeadNode)
    {
        printf("%s: malloc failed\n", __FUNCTION__);
        return FAIL;
    }

    for (i = 0; i < (*G).vexnum; i++)
    {
        sprintf(name, "v%d", i);
        strcpy((*G).HeadNode[i].data, name); 
        printf("data:%s\n",(*G).HeadNode[i].data); 

        (*G).HeadNode[i].firstarc = NULL;

        printf("please input v%d's last adjvex(or input -1 to exit the loop):", i);
        scanf("%d", &v);

        while (v != -1)
        {
            pArcNode = (struct ArcNode*)malloc(sizeof(struct ArcNode));
            if (!pArcNode)
            {
                printf("%s: malloc failed\n", __FUNCTION__);
                return FAIL;
            }

            (*G).arcnum++;

            pArcNode->adjvex_no = v;
            pArcNode->arcinfo = NULL;
            pArcNode->nextarc = (*G).HeadNode[i].firstarc; 
            (*G).HeadNode[i].firstarc = pArcNode;

            printf("please input v%d's another front adjvex(or input -1 to exit the loop):", i);
            scanf("%d", &v);
        }
    }

    return OK;
}

void print_graphy(struct ALGraphy G)
{
    int i;

    printf("vexnum:%3d\n", G.vexnum);
    printf("arcnum:%3d\n", G.arcnum);
    printf("kind:  %3d\n", G.kind);

    for (i = 0; i < G.vexnum; i++) //for循环老喜欢加;号阿，影响程序调试，罪过!
    {
        printf("Head data:%s\n", G.HeadNode[i].data);
    }

    //其余信息读者根据需要自己添加把，TODO
}

int main(void)
{
    struct ALGraphy  G;

    (void)create_graphy(&G);

    print_graphy(G);

    printf("Depth First Search:\n");
    DFS_traverse(G);

    printf("Breadth First Search:\n");
    BFS_traverse(G);

    return 0;
}





#define MAXV           100  /*顶点最大个数*/      
#define MAXDEGREE      100  /*最大出度*/        
        
typedef struct {        
    int edges[MAXV+1][MAXE]；      /*邻接边信息*/ 
    int degree[MAXV+1];            /*出度*/   
    int nvertices;  
    int nedges; 
} graph;        
        
read_graph(graph *g, bool directed)     
{       
    int i;  
    int m;               /* 边数 */   
    int x, y;            /*edge(x,y)*/  
        
    initialize_graph(g);    
        
    输入本图的顶点个数和边数；   
        
    for (i=1; i<=m; i++) {  
        scanf("%d %d", &x, &y);
        insert_edge(g, x, y, directed);
    }   
}       
        
initialize_graph(graph *g)      
{       
    int i;  
    g->nvertices=0; 
    g->nedges=0;    
    for (i=1; i<=MAXV; i++) g->degree[i]=0; 
}       
        
insert_edge(graph *g, int x, int y, bool directed)      
{       
    if (g->degree[x]>MAXDEGREE) printf("Warning: exceeds MAX degree\n",x,y);    
    g->edges[x][g->degree[x]]=y;    
    g->degree[x]++; 
        
    if (directed==FALSE)    
         insert_edge(g, x, y, TRUE);
    else    
        g->nedges ++;
}       
