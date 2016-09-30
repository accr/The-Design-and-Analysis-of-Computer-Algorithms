/** 
 * 无向图的DFS算法 
 */  
public class GraphDFS {  
    private boolean[] visited;  
      
    //邻接矩阵  
    private int [][] matrix;      
      
    //pre和post指针  
    private int _pre_post = 1;  
      
    //连通分量指针,指向每个连通分量在顶点集合nodes里的开始位置  
    private int _cc = 0;  
      
    //所有顶点的集合  
    private GraphNode[] nodes ;   
      
    //连通分量集合  
    private List<GraphNode[]> cc = new ArrayList<GraphNode[]>();  
      
    /** 
     * 初始化 
     * @param matrix 图的邻接矩阵 
     */  
    public GraphDFS(int[][] matrix){  
        visited = new boolean[matrix.length];  
          
        List<GraphNode> list = new ArrayList<GraphNode>();         
        for(int i=0;i<matrix.length;i++){  
            list.add(new GraphNode());  
        }  
        nodes = list.toArray(new GraphNode[0]);  
        //nodes = new GraphNode[v];  
          
        this.matrix = matrix;  
    }     
      
    /** 
     * 对某个连通分量进行深度优先搜索 
     * @param v 第几个顶点 
     */  
    public void DFSTraverse(int v){  
        visited[v] = true;  
        nodes[_cc].setName(v);  
        int tmp = _cc;  
          
        //记录当前遍历到的顶点的pre  
        nodes[tmp].setPre(_pre_post++);  
        _cc++;  
          
        //对当前顶点的邻接顶点再进行深度优先搜索  
        for(int i=0; i<matrix.length; i++){  
            if(matrix[v][i]==1 && !visited[i])  
                DFSTraverse(i);  
        }  
          
        //记录当前遍历到的顶点的post  
        nodes[tmp].setPost(_pre_post++);  
    }  
      
    /** 
     * 对所有连通分量进行深度优先搜索 
     */  
    public void dfs(){  
        for(int i=0; i<nodes.length; i++){  
            if(!visited[i]){  
                //该连通分量在nodes集合的开始位置  
                int start = _cc;  
                //对该连通分量进行深度遍历  
                DFSTraverse(i);  
                //该连通分量的大小  
                int count = _cc - start;  
                  
                //该连通分量包含的所有顶点  
                GraphNode[] newNodes = new GraphNode[count];  
                for(int j=0; j<count;j++){  
                    int v = start+j;  
                    newNodes[j] = nodes[v];  
                }             
                  
                cc.add(newNodes);  
            }  
        }  
    }  
      
    //cc: Connected Component,连通分量  
    public List<GraphNode[]> getCC(){  
        return cc;  
    }  
  
}  


/** 
 * 图的顶点 
 */  
class GraphNode{  
    private int pre ;  
    private int post ;  
      
    //顶点名  
    private int name;  
      
    //顶点信息  
    public String getInfo(){  
        return "vartex: "+name+"\t pre= "+pre+", post= "+post;  
    }  
      
    public int getName() {  
        return name;  
    }  
    public void setName(int name) {  
        this.name = name;  
    }     
    public int getPre() {  
        return pre;  
    }  
    public void setPre(int pre) {  
        this.pre = pre;  
    }  
    public int getPost() {  
        return post;  
    }  
    public void setPost(int post) {  
        this.post = post;  
    }     
}  


public static void main(String[] args) {  
    String path = "D:\\graph";  
      
    try(Scanner scanner = new Scanner(new File(path, "tinyG.txt"));){  
          
        //第一行的数字是顶点的数目  
        int v = scanner.nextInt();  
        //第二行的数字是边的数目  
        int e = scanner.nextInt();  
        //创建一个无向图  
        GraphRepresentation graph = new GraphRepresentation(v, e);  
          
        //读取每条边对应的两个顶点,设置邻接矩阵的值  
        for (int i = 0; i < e; i++) {                  
            int v1 = scanner.nextInt();  
            int v2 = scanner.nextInt();  
            graph.addEdge(v1, v2);  
        }  
          
        //根据graph的邻接矩阵,对其进行深度优先搜索  
        GraphDFS graphDFS = new GraphDFS(graph.getAdjacencyMatrix());  
        graphDFS.dfs();       
          
        //连通分量集合  
        List<GraphNode[]> cc = graphDFS.getCC();  
        int count = cc.size();  
        System.err.println("********** 共有"+count+"个连通分量,分别为: ********** ");  
          
        //存储遍历结果  
        StringBuffer sb = new StringBuffer();  
          
        for(int i=0; i<count; i++){  
            GraphNode[] c = cc.get(i);  
            System.err.println("第"+(i+1)+"个连通分量:");  
            for(GraphNode node : c){  
                System.err.println(node.getInfo());  
                sb.append(node.getName()+"\t");  
            }  
            System.err.println();  
            sb.append("\n");  
        }  
          
        System.err.println("********** 遍历结果:**********\n" + sb.toString());  
          
    } catch (Exception e) {  
        throw new RuntimeException(e.getMessage());  
    }     
}     
