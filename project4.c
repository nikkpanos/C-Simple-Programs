#include <stdio.h>
#include <stdlib.h> // for malloc use
#include <string.h> // for strcpy use
#include <float.h> // for DBL_MAX use

#define DIJ 1 // Dijkstra algorithm choice 
#define BF 2 // Bellman-Ford algorithm choice
#define INF DBL_MAX/2 // Will be used to resemble infinity
#define YES 2
#define NO 1

// Every node has a name consisted of 1 character (capital latin letter) and a distance (this is essential for the algorithms).
// It also has a pointer to each previous node (also essential for the algorithms).
struct node{
    double dist;
    char name;
    struct node * prev;
};

// Every edge has name consisted of 2 letters (one for each node on its ends), and a weight (cost) which will be calculated using bit rate.
// Also it has 2 pointers, one for each node.
struct edge{
    double weight;
    char name[3];
    struct node * node1;
    struct node * node2;
};

// This will consist of 2 arrays (of dynamically allocated size) of pointers.
// One array will be pointing at the nodes of the network, and the other will be pointing at the edges of the network. 
struct network{
    struct node **nodes;
    struct edge **edges;
};

int areConnected(struct network graph, int nodeIndex1, int nodeIndex2, int numOfEdges); // To find if two nodes are connected


/*
======================================================================================================================================================================
========================================================================= MAIN =======================================================================================
======================================================================================================================================================================
*/

int main(){
    /*=================================== User must give network's amount of nodes and edges ==================================*/

    int numOfNodes;
    printf("How many nodes are there in the network? Enter a number: ");
    scanf("%d", &numOfNodes); while ((getchar()) != '\n'); // while ((getchar()) != '\n'); THIS must be used after taking character input to clear the buffer, 
                                                           // but I used it after every scanf just in case :)
    while(numOfNodes<2 && numOfNodes>24){ // Limit to 24 because the nodes will be represented with 1 capital letter
        printf("Please enter a valid number: ");
        scanf("%d", &numOfNodes); while ((getchar()) != '\n');
    }// while

    int numOfEdges;
    // Assuming that all nodes have links with all other nodes, then the max number of links would be the the sum that is
    // given by the series of i, from i=1 to n, which is n(n+1)/2, where n in our case is numOfNodes-1.
    int maxNumOfEdges = (numOfNodes-1)*(numOfNodes)/2;
    printf("How many links are there in the network? Enter a number: ");
    scanf("%d", &numOfEdges); while ((getchar()) != '\n');
    while(numOfEdges > maxNumOfEdges){
        printf("You entered too many edges. Please enter a valid number: ");
        scanf("%d", &numOfEdges); while ((getchar()) != '\n');
    }// while
    if (numOfEdges < 0) numOfEdges = 0; // Dijkstra's algorithm calculates cost between not connected nodes,
                                        // thats why we do not necessarily need edges in the graph, so 0 is a valid value.
    
    /*====================== Initializing the graph, with two arrays. One array has pointers to nodes, the other has pointers to edges ======================*/
    struct network graph;
    graph.nodes = malloc(numOfNodes*sizeof(struct node *));
    graph.edges = malloc(numOfEdges*sizeof(struct edge *));
    if (graph.nodes == NULL) {printf("Memory allocation process failed!"); exit(0);}

    /*========================================= Node and Edge information gathering and storing them into the graph =========================================*/
    
    // User gives node information
    // Naming the nodes using latin capital letters
    printf("\nThe network's nodes are: ");
    struct node tempNodes[numOfNodes]; // Used to temporarily store full information of a node before connecting its memory space with the graph
    for(int i=0; i<numOfNodes; i++){
        tempNodes[i].name = 65+i;
        graph.nodes[i] = &tempNodes[i];
        printf("%c ", graph.nodes[i]->name);
        // Only names are given now. Distances and connections with previous node will be calculated during the algorithm
    }// for i

    // User gives edge information
    printf("\n\nNow you must give the necessary information for each edge.");
    struct edge tempEdges[numOfEdges]; // Used to temporarily store full information of an edge before connecting its memory space with the graph
    for(int i=0; i<numOfEdges; i++){
        char nodeName1, nodeName2;
        double bitRate, weight;
        // First user gives the node names for the edge's ends. If one of the names does not match the names created in the previous step, user tries again.
        int goodInput = 0;
        while (goodInput < 2){
            printf("\n\nEnter the node's letter for the one end of the edge No%d.\n(Please be very careful! Letters must be latin and must match exactly!): ", i+1);
            scanf("%c", &nodeName1); while ((getchar()) != '\n');
            printf("Enter the node's letter for the other end of the edge No%d.\n(Please be very careful! Letters must be latin and must match exactly!): ", i+1);
            scanf("%c", &nodeName2); while ((getchar()) != '\n');
            for(int j=0; j<numOfNodes; j++){
                if (graph.nodes[j]->name == nodeName1) {tempEdges[i].node1 = graph.nodes[j]; goodInput++;}
                if (graph.nodes[j]->name == nodeName2) {tempEdges[i].node2 = graph.nodes[j]; goodInput++;}
            }// j
            if (goodInput < 2){
                printf("You must enter valid node names. Please try again.");
                goodInput = 0;
            }// if
        }// while

        char name[3] = {nodeName1, nodeName2, '\0'};
        strcpy(tempEdges[i].name, name);
         
        printf("Enter the bit rate of the link in Mbps: ");
        scanf("%lf", &bitRate); while ((getchar()) != '\n');
        while (bitRate<0){
            printf("\nYou must enter a valid bit rate value: ");
            scanf("%lf", &bitRate); while ((getchar()) != '\n');
        }// while
        weight = 1000/bitRate; //To make higher bit rates have less weight. 1000 was a random choice here, I could have used any number.
        tempEdges[i].weight = weight;
        graph.edges[i] = &tempEdges[i]; // Finally making the graph's edge point at the newly created edge
    }// for i
    
    // Just printing the constructed graph
    printf("\n\n\nThe information of the nodes and links you entered are: ");
    for(int i=0; i<numOfNodes; i++){
        printf("\nNode No%d is %c", i+1, graph.nodes[i]->name);
    }// for i
    for(int i=0; i<numOfEdges; i++){
        printf("\nLink No%d is %s, has a weight of %.2f and the its two ends are the nodes %c and %c.", i+1, 
                        graph.edges[i]->name, graph.edges[i]->weight, graph.edges[i]->node1->name, graph.edges[i]->node2->name);
    }// for i

    int stop = NO;
    while (stop == NO){
    /*====================================================== Routing Algorithm and starting node selection =========================================================*/
    
        printf("\n\nYou must choose a routing algorithm for the construction of the routing table.");
        printf("\nEnter \"1\" for Dijkstra's (link-state) algorithm or \"2\" for Bellman-Ford's (distance vector) algorithm: ");
        int alg;
        scanf("%d", &alg); while ((getchar()) != '\n');
        while(alg != DIJ && alg != BF){
            printf("Please read the instructions and enter again: ");
            scanf("%d", &alg); while ((getchar()) != '\n');
        }// while
        int start;
        printf("Enter the number of node for which you want to calculate the routing table.\n(Careful. We need the number, not the letter!): ");
        scanf("%d", &start); while ((getchar()) != '\n');
        while(start<1 || start>numOfNodes){
            printf("You must enter a valid number. Please try again: ");
            scanf("%d", &start); while ((getchar()) != '\n');
        }// 
        start--; // Decrementing because will be used as array indexing.
        graph.nodes[start]->prev = graph.nodes[start];

        /*================================================================= DIJKSTRA'S ALGORITHM ======================================================================*/

        if (alg == DIJ){
            // First we assign distances to each node. The starting node gets 0, and the others get infinity.
            double nodeCheck[numOfNodes]; // This will be used to track the nodes which the algorithm still has to examine.
                                        // Now we will assign the distances to this array, and later if the examination of a node is completed, we will assign -1.
            for (int i=0; i<numOfNodes; i++){
                if (i == start) graph.nodes[i]->dist = 0;
                else graph.nodes[i]->dist = INF;
                nodeCheck[i] = graph.nodes[i]->dist;
            }// for i

            printf("\nStep 0: \t\t  ");
            for (int j=0; j<numOfNodes; j++){
                    printf("dist(%c):", graph.nodes[j]->name);
                    if (graph.nodes[j]->dist > INF/2) printf("inf\t");
                    else printf("%.2f\t", graph.nodes[j]->dist);
            }// for j

            for (int j=0; j<numOfNodes; j++){
                double minDist=INF;
                int curNode; // Index of node to be examined
                for (int i=0; i<numOfNodes; i++) { // To find the node with the currently min distanse
                    if (nodeCheck[i] < 0) continue; // We do not care about nodes that are already examined
                    if (nodeCheck[i] < minDist){
                        minDist = nodeCheck[i];
                        curNode = i;
                    }// if
                }// for i
                nodeCheck[curNode] = -1; // So that the same node will not be examined again
                for (int i=0; i<numOfNodes; i++) {
                    if (nodeCheck[i] < 0) continue; // We do not care about nodes that are already examined
                    int curEdge = areConnected(graph, curNode, i, numOfEdges);
                    if (curEdge == -1) continue; // We do not care about nodes that are not connected with the node we examine right now
                    
                    // Found a node which is not yet examined AND is connected with the currently examined node
                    double curDist = graph.nodes[i]->dist;
                    double newDist = graph.nodes[curNode]->dist + graph.edges[curEdge]->weight;
                    if (newDist < curDist){
                        graph.nodes[i]->dist = newDist;
                        graph.nodes[i]->prev = graph.nodes[curNode];
                        nodeCheck[i] = newDist;
                    }// if
                }// for i

                printf("\nStep %d (examined node %c): ", j+1, graph.nodes[curNode]->name);
                for (int i=0; i<numOfNodes; i++){
                    printf("dist(%c):", graph.nodes[i]->name);
                    if (graph.nodes[i]->dist > INF/2) printf("inf\t");
                    else printf("%.2f\t", graph.nodes[i]->dist);
                }// for i
            }// for j

            printf("\n\n\nAt the end of the algorithm, the routing table is the following: ");
            printf("\nDestination node\tNext-Hop\tCost");
            for(int i=0; i<numOfNodes; i++){
                struct node * nextHop = graph.nodes[i];
                while(1){
                    if (nextHop->prev->name == graph.nodes[start]->name) break;
                    nextHop = nextHop->prev;
                }// while

                printf("\n\t%c\t\t   %c\t\t%.2f", graph.nodes[i]->name, nextHop->name, graph.nodes[i]->dist);

            }// for i

            
        }// if (DIJ)

        /*================================================================= BELLMAN-FORD'S ALGORITHM ==================================================================*/

        if (alg == BF){

            // Initialize distances. Value 0 for the start, infinity for all the others.
            for (int i=0; i<numOfNodes; i++){
                if (i == start) graph.nodes[i]->dist = 0;
                else graph.nodes[i]->dist = INF;
            }// for i

            // Print step 0 distances
            printf("\nStep 0: ");
                for (int j=0; j<numOfNodes; j++){
                    printf("dist(%c):", graph.nodes[j]->name);
                    if (graph.nodes[j]->dist > INF/2) printf("inf\t");
                    else printf("%.2f\t", graph.nodes[j]->dist);
            }// for j

            // Outer loop must execute for numOfNodes-1 times to be sure that the min distance was found for every node.
            for (int i=0; i<numOfNodes-1; i++){
                // Every time we find the edges (and nodes) that are connected with the one we examine each time.
                int relaxedSomething = 0; // Will become 1 if relaxation happen on one of the nodes during one loop walkthrough
                for (int j=0; j<numOfNodes; j++){
                    char curNode = graph.nodes[j]->name; 
                    double minDist = graph.nodes[j]->dist;
                    struct node * prev; // Will keep track of previous nodes where needed
                    int relaxedNode = 0; // Will become 1 if relaxation happens to the specific node

                    // Finding what edges are connected to the curNode
                    for(int k=0; k<numOfEdges; k++){
                        if (curNode != graph.edges[k]->node1->name && curNode != graph.edges[k]->node2->name) continue; // We do not care about not connected edges/nodes.

                        // Finding the other end of the edge and calculating the new estimated distance for the curNode
                        double prevDist, estimDist, edgeWeight;
                        if (curNode == graph.edges[k]->node1->name){
                            prevDist = graph.edges[k]->node2->dist;
                            prev = graph.edges[k]->node2;
                        }else if (curNode == graph.edges[k]->node2->name){
                            prevDist = graph.edges[k]->node1->dist;
                            prev = graph.edges[k]->node1;
                        }// if-else if
                        edgeWeight = graph.edges[k]->weight;
                        estimDist = prevDist + edgeWeight;

                        // Keep track of the minimum of the new estimated distances for the curNode
                        if (estimDist < minDist){
                            minDist = estimDist;
                            relaxedNode = 1; // this is for current node
                            relaxedSomething = 1; // this is for all nodes of current examination
                        }// if
                    }// for k

                    // If a better estimation for the new distance has been found, then we assign new distance and previous node to the examined one.
                    if (relaxedNode){ 
                        graph.nodes[j]->dist = minDist;
                        graph.nodes[j]->prev = prev;
                    }// if
                }// for j
                
                // Print every step (after step 0) distances
                printf("\nStep %d: ", i+1);
                for (int j=0; j<numOfNodes; j++){
                    printf("dist(%c):", graph.nodes[j]->name);
                    if (graph.nodes[j]->dist > INF/2) printf("inf\t");
                    else printf("%.2f\t", graph.nodes[j]->dist);
                }// for j

                if (!relaxedSomething) break; // If no relaxation occured, then the shortest paths have been calculated and the algorithm can stop.
            }// for i
            printf("\n\n\nAt the end of the algorithm, the routing table is the following: ");
            printf("\nDestination node\tNext-Hop\tCost");
            for(int i=0; i<numOfNodes; i++){
                struct node * nextHop = graph.nodes[i];
                while(1){
                    if (nextHop->prev->name == graph.nodes[start]->name) break;
                    nextHop = nextHop->prev;
                }// while

                printf("\n\t%c\t\t   %c\t\t%.2f", graph.nodes[i]->name, nextHop->name, graph.nodes[i]->dist);

            }// for i
        }// if (BF)

        printf("\n\n\nWould you like to try a adifferent algorithm and/or with a different start node?\nEnter \"1\" for YES\nEnter \"2\" for NO\n");
        scanf("%d", &stop);
        while(stop != NO && stop != YES){
            printf("\nPlease enter a valid number: ");
            scanf("%d", &stop);
        }// if


    }// while (algorithm and starting node selection)
    // Must free the memory that was allocated with malloc
    free(graph.nodes);
    free(graph.edges);


}// main

/*
======================================================================================================================================================================
====================================================================== FUNCTIONS =====================================================================================
======================================================================================================================================================================
*/

int areConnected(struct network graph, int nodeIndex1, int nodeIndex2, int numOfEdges){
    char nodeName1 = graph.nodes[nodeIndex1]->name;
    char nodeName2 = graph.nodes[nodeIndex2]->name;
    for (int i=0; i<numOfEdges; i++){
        char name1 = graph.edges[i]->node1->name;
        char name2 = graph.edges[i]->node2->name;
        if((nodeName1 == name1 && nodeName2 == name2) || (nodeName1 == name2 && nodeName2 == name1)) return i;
    }// for i
    return -1;
}// areConnected