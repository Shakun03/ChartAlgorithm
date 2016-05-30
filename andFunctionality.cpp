#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string>
#include <string.h>

using namespace std;

char*textArr[500]; // storing the textual label of every node
char typeArr[500]; // storing the type of each node
int indexArr[500]; // storing the destination point of every node
char*outputArray[600]; //final text description
//char*prefix_pl[10]={"If","After","When","As soon as","In case of"};
//char*postfix_pl[10]={"happened","was typed in","came in","is valid","is unvalid"};
//char*prefix_tr[10]={"I can","I have to","the system must","the activity"};
//char*postfix_tr[1]={"then"};

int j=0;
int i=0;
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
	 //cout<<"test: "<<newNode->text<<endl;
	 //cout<<newNode->prefix;
	 textArr[i++]=newNode->text;
	 typeArr[j++]=newNode->type;
	 indexArr[k++]=newNode->dest;
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
	 //cout<<"the val is: "<<graph->Array[src].head->type;
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

void changeToText(struct Graph*graph)
{
 	int v=0;
        while(v!=graph->V)
        {
        	struct node*crawl = graph->Array[v].head;
        	if(crawl->postfix=="end")
         	{
        		outputArray[l++]="end";
        	}
		 else
        	{
		 	outputArray[l++]=crawl->prefix;
	        	outputArray[l++]=crawl->text;
	        	outputArray[l++]=crawl->postfix;
	        	outputArray[l++]="and";
        	}
        	v++;
     	}
}

int main()
{
	 //Create a graph containing say, two events i.e start and end, and three activities.
	 int V = 3;
	 struct Graph* graph = createGraph(V);
	 struct node*startingEventNode=addNewNode(0,'e',"When","Start","happened");
	 addFlow(graph, 0, 1,'a',"I have to","fill the form","");
	 addFlow(graph,1,2,'a',"the system must","Do activity 2","");
	 addFlow(graph,2,3,'e',"","","end");
	 //Print the adjacency list representation of the above graph
	 cout<<"the data structure info: "<<endl;
	 printGraph(graph);
	 cout<<endl<<"**************************";
	 cout<<endl<<endl<<"The textual info:"<<endl;
	 changeToText(graph);
     	for(int e=0;e<=l;e++)
     	{
     		cout<<" "<< outputArray[e];
     	}
	return 0;
}


