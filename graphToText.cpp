#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <string>
//#define NDEBUG
using namespace std;

char*textArr[500]; // storing the textual label of every node
char typeArr[500]; // storing the type of each node
int indexArr[500]; // storing the destination point of every node
char*outputArray[600]; //final text description
//prefixForEvent={"If","After","When","As soon as","In case of"};
//postfixForEvent={"happened","was typed in","came in","is valid","is unvalid"};
//prefixForActivity={"I can","I have to","the system must","the activity"};

int j=0;
int k=0;
int l=0;

struct node
{
	 char type;           // 'a' for activity and 'e' for event.
	 int dest;           // every node (be it an activity or an event), is assigned a number starting from 0
	 char*text;         // a string containing the text inside the activity box/event.
	 char*prefix;          //1)the system? me? or simply, then? 2) If? After? As soon as? In case of?
	 char*postfix; //happened? typed? came? valid? unvalid?
	 struct node*next; //pointer to the next node
};

struct nodeList     //each such list contains the collection of nodes to which the current node is pointing to.
{
 	struct node*head;  //starting point of every list.
};

// A structure to represent a graph. A graph is an array of lists.
// Size of array will be V (number of nodes in graph)
// first index of array consists of a list of all nodes to which the first node points to.
// first index of array consists of a list of all nodes to which the first node points to.
// similarly, second index of array consists of a list of all nodes to which the second node points to.
// and so on.

struct Graph
{
 	int V;
 	struct nodeList*Array;
};

//A function to create a new node
struct node*addNewNode(int dest,char type,char const*prefix,char const*text,char const*postfix)
{
    struct node*newNode=(struct node*)malloc(sizeof(struct node));
    newNode->dest=dest;
    newNode->type=type;
    newNode->text = (char*)malloc(sizeof(char) * 1024);
    strncpy(newNode->text,text,strlen(text) + 1);
    newNode->prefix = (char*)malloc(sizeof(char) * 1024);
    strncpy(newNode->prefix,prefix,strlen(prefix) + 1);
    newNode->postfix = (char*)malloc(sizeof(char) * 1024);
    strncpy(newNode->postfix,postfix,strlen(postfix) + 1);
    newNode->next=NULL;
    if(newNode->dest==0)
    {
    	outputArray[l++]=newNode->prefix;
    	outputArray[l++]=newNode->text;
    	outputArray[l++]=newNode->postfix;
    }
    return newNode;
}

//A function to create a graph
struct Graph*createGraph(int V)
{
	struct Graph*graph=(struct Graph*)malloc(sizeof(struct Graph));
	graph->V=V;
	graph->Array=(struct nodeList*)malloc(V*sizeof(struct nodeList));
	int i;
	for (i = 0; i < V; ++i)
	   graph->Array[i].head = NULL;
	return graph;
}

void addFlow(struct Graph* graph, int src, int dest,char type,char const*prefix,char const*text,char const*postfix)
{
	// Add an edge from src to dest.  A new node is added to the
	// list of src.
	struct node* newNode = addNewNode(dest,type,prefix,text,postfix);
	newNode->next = graph->Array[src].head;
	graph->Array[src].head = newNode; //The node is added at the beginning
}

// A function to print the list representation of graph
void printGraph(struct Graph*graph)
{
	int v;
	for (v = 0; v< graph->V; ++v)
	{
		struct node*crawl = graph->Array[v].head;
		printf("\n List of vertex %d\n head ", v);
		while (crawl)
        {
            printf("-> %d", crawl->dest);
            crawl = crawl->next;
		}
    }
}

int check(struct Graph*g,int n)
{
    struct node*crawl=g->Array[n].head;
    int count=1;
    crawl=crawl->next;
    while(crawl)
    {
        crawl=crawl->next;
        count+=1;
    }
    if(count<=1)
        return -1;
    else
    {
        struct node*mov=g->Array[n].head;
        outputArray[l++]=mov->prefix;
        outputArray[l++]=mov->text;
        outputArray[l++]=mov->postfix;
        outputArray[l++]="or";
        outputArray[l++]=mov->next->prefix;
        outputArray[l++]=mov->next->text;
        outputArray[l++]=mov->next->postfix;
        return mov->dest;
    }
}

void changeToText(struct Graph*graph)
{
	int v=0;
    while(v!=graph->V)
    {
        struct node*crawl = graph->Array[v].head;
        if(crawl->postfix=="end")
        {
            
            outputArray[l++]="end";
            break;
        }
        else
        {
            if(crawl->type=='e')
            {
                outputArray[l++]=crawl->prefix;
                outputArray[l++]=crawl->text;
                outputArray[l++]=crawl->postfix;
                v=crawl->dest;
            }
            else if(crawl->type=='a')
            {
                int k=check(graph,crawl->dest);
                if(k==-1)
                {
                    outputArray[l++]=crawl->prefix;
                    outputArray[l++]=crawl->text;
                    outputArray[l++]=crawl->postfix;
                    outputArray[l++]="and";
                    v=crawl->dest;
                }
                else if( k!=-1)
                {
                    v=k;
                }
            }
        }
    }
}

int main()
{
	//Create a graph containing say, two events i.e start and end, and three activities.
    int V = 7;
    struct Graph* graph = createGraph(V);
    struct node*startingEventNode=addNewNode(0,'e',"When","Start","happened");
    string output;
    string expected_output=" When Start happened I have to fill the form  and I have to  pay by cash  or I have to  pay by cheque  After payment is valid the system must verify the documents  and   end";
    addFlow(graph, 0, 1,'a',"I have to","fill the form","");
    addFlow(graph,1,2,'a',"the system must","render a fee slip","");
    addFlow(graph,2,3,'a',"I have to ","pay by cheque","");
    addFlow(graph,2,4,'a',"I have to ","pay by cash","");
    addFlow(graph,3,5,'e',"After","payment","is valid");
    addFlow(graph,4,5,'e',"After","payment","is valid");
    addFlow(graph,5,6,'a',"the system must","verify the documents","");
    addFlow(graph,6,7,'e',"","","end");

    cout<<"the data structure info: "<<endl;
    printGraph(graph);
    cout<<endl<<"**************************";
    cout<<endl<<endl<<"The textual info:"<<endl;
    changeToText(graph);
    for(int e=0;e<l;e++)
    {
        output=output+" "+outputArray[e];
    }
    cout<<endl<<output;
    assert(output.compare(expected_output)==0);
    cout<<endl<<"The result matches the expected output.";
    return 0;
}
