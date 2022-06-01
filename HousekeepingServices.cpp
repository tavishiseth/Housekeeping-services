#include <bits/stdc++.h>
using namespace std;


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct AdjListNode
{
	int dest;
	int weight;
	struct AdjListNode* next;
};


struct AdjList
{
	struct AdjListNode *head;
};


struct Graph
{
	int V;
	struct AdjList* array;
};


struct AdjListNode* newAdjListNode(int dest, int weight)
{
	struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

struct Graph* createGraph(int V)
{
	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
	graph->V = V;
	graph->array = (struct AdjList*)
	malloc(V * sizeof(struct AdjList));
	for (int i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

void addEdge(struct Graph* graph, int src,int dest, int weight)
{
	struct AdjListNode* newNode = newAdjListNode(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;
	newNode = newAdjListNode(src, weight);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}

struct MinHeapNode
{
	int v;
	int dist;
};

struct MinHeap
{

	int size;
	int capacity;
	int *pos;
	struct MinHeapNode **array;
};

struct MinHeapNode* newMinHeapNode(int v,int dist)
{
	struct MinHeapNode* minHeapNode = (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap =(struct MinHeap*) malloc(sizeof(struct MinHeap));
	minHeap->pos = (int *)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array =(struct MinHeapNode**)malloc(capacity *
	sizeof(struct MinHeapNode*));
	return minHeap;
}

void swapMinHeapNode(struct MinHeapNode** a,struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

void minHeapify(struct MinHeap* minHeap,int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist )   smallest = left;

	if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist )  smallest = right;

	if (smallest != idx)
	{
		MinHeapNode *smallestNode = minHeap->array[smallest];
		MinHeapNode *idxNode = minHeap->array[idx];

		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		swapMinHeapNode(&minHeap->array[smallest],&minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

int isEmpty(struct MinHeap* minHeap)
{
	return minHeap->size == 0;
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	if (isEmpty(minHeap))
		return NULL;

	struct MinHeapNode* root = minHeap->array[0];

	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	minHeap->pos[root->v] = minHeap->size-1;
	minHeap->pos[lastNode->v] = 0;

	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
	int i = minHeap->pos[v];

	minHeap->array[i]->dist = dist;

	while (i && minHeap->array[i]->dist <
		minHeap->array[(i - 1) / 2]->dist)
	{
		minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
		minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
		swapMinHeapNode(&minHeap->array[i],&minHeap->array[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

bool isInMinHeap(struct MinHeap *minHeap, int v)
{
	if (minHeap->pos[v] < minHeap->size)  return true;
	return false;
}

int dijkstra(struct Graph* graph, vector<int> bookings, int src, int dest)
{
	int V = graph->V;
	int dist[V];
	struct MinHeap* minHeap = createMinHeap(V);

	for (int v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v,dist[v]);
		minHeap->pos[v] = v;
	}
	minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);
	minHeap->size = V;
	while (!isEmpty(minHeap))
	{
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v;
		struct AdjListNode* pCrawl = graph->array[u].head;
		while (pCrawl != NULL)
		{
			int v = pCrawl->dest;
			if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v])
			{
				dist[v] = dist[u] + pCrawl->weight;
				decreaseKey(minHeap, v, dist[v]);
			}
			pCrawl = pCrawl->next;
		}
	}
	return dist[dest];
}

void ETA_high_priority(struct Graph* graph, vector<int> bookings, int V, int service_time)
{
	int time_to_travel=0,h=0,m=0;
	int curr_loc=bookings.front();
	while(!bookings.empty())
	{
	    bookings.erase(bookings.begin());
	    if(bookings.empty()) break;
		int dest_loc=bookings.front();
		time_to_travel=time_to_travel+dijkstra(graph,bookings,curr_loc,dest_loc);
		m=0+time_to_travel;
        h=8+time_to_travel/60;
	    m=m%60;
	    if(m<10)
        cout<<"Expected time of arrival for location "<<curr_loc<<" to "<<dest_loc<<" is "<<h<<":0"<<m<<" ("<<time_to_travel<<" min)."<<endl;
        else
        cout<<"Expected time of arrival for location "<<curr_loc<<" to "<<dest_loc<<" is "<<h<<':'<<m<<" ("<<time_to_travel<<" min)."<<endl;
		curr_loc=dest_loc;
		time_to_travel+=service_time;
		cout<<endl;
	}
}

int* dijkstra_returning_array_LP(struct Graph* graph, vector<int> bookings, int src, int dest,int * dist)
{
	int V = graph->V;
	struct MinHeap* minHeap = createMinHeap(V);
	for (int v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v,dist[v]);
		minHeap->pos[v] = v;
	}
	minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);
	minHeap->size = V;
	while (!isEmpty(minHeap))
	{
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v;
		struct AdjListNode* pCrawl = graph->array[u].head;
		while (pCrawl != NULL)
		{
			int v = pCrawl->dest;
			if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v])
			{
				dist[v] = dist[u] + pCrawl->weight;
				decreaseKey(minHeap, v, dist[v]);
			}
			pCrawl = pCrawl->next;
		}
	}
	return dist;
}

int min_distance_LP(struct Graph* graph,int curr_loc,vector<int> LP_bookings,int V)
{
	int temp_arr[V];
	int* arr=dijkstra_returning_array_LP(graph,LP_bookings,curr_loc,curr_loc,temp_arr); //here keeping destination same as we just want the minimum distance array.
	int dest_loc=LP_bookings.front();
	int min_val=arr[dest_loc];
	for(int a=0;a<LP_bookings.size();a++)
	{
		for(int i=0; i<V; i++)
		{
			if(LP_bookings[a]==i && min_val>arr[i])
			{
				min_val=arr[i];
				dest_loc=i;
			}
		}
	}
	return dest_loc;
}

void ETA_low_priority(struct Graph* graph, vector<int> LP_bookings, int V, int service_time)
{
	int curr_loc=LP_bookings.front();
	LP_bookings.erase(LP_bookings.begin());
	int time_to_travel=0,h=0,m=0;
	while(!LP_bookings.empty())
	{
		int dest_loc=min_distance_LP(graph,curr_loc,LP_bookings,V);
		int dist = dijkstra(graph,LP_bookings,curr_loc,dest_loc);
		time_to_travel=time_to_travel+dist;
		m=0+time_to_travel;
        h=8+time_to_travel/60;
	    m=m%60;
	    if(m<10)
        cout<<"Expected time of arrival for location "<<dest_loc<<" is "<<h<<":0"<<m<<" ("<<time_to_travel<<" min)."<<endl;
        else
        cout<<"Expected time of arrival for location "<<dest_loc<<" is "<<h<<':'<<m<<" ("<<time_to_travel<<" min)."<<endl;
		LP_bookings.erase(std::remove(LP_bookings.begin(), LP_bookings.end(), dest_loc), LP_bookings.end());
		curr_loc=dest_loc;
		time_to_travel+=service_time;
		cout<<endl;
	}
}



// Driver code
int main()
{
	// create the graph given in above figure
	int v = 9;
	int b;
	fstream f;
	int n1,n2,n3;
	f.open("input.txt");
    struct Graph* graph = createGraph(v);
	while(!f.eof())
    {
        f>>n1>>n2>>n3;
        addEdge(graph,n1,n2,n3);
    }
	int service_time=100;   //the amount of time taken to provide service to each customer.

	vector<int> bookings_l,bookings_h;
	bookings_l.push_back(0);
	bookings_h.push_back(0);

	 int ch=0;
	  while(ch!=5)
      {
          cout<<"**HOUSEKEEPING SERVICES**\nBook your (1 day prior) 100 min slot for deep cleaning NOW\n(City map consists of 8 houses numbered from 1 to 8 and service headquarters at node 0)\n\n1.Add a Low Priority Booking and view ETA\n2.Remove a Low Priority Booking\n3.Add a High Priority Booking and view ETA\n4.FAQs\n5.Exit\n\nEnter choice: ";
          cin>>ch;
          if(ch==1) {
                cout<<"Enter location: ";
                cin>>b;
                cout<<endl;
                bookings_l.push_back(b);
                ETA_low_priority(graph,bookings_l,v,service_time);
                cout<<"Price: Rs. 300\n";
            }
          if(ch==2) {
                cout<<"Enter location to be removed: ";
                cin>>b;
                cout<<endl;
                bookings_l.erase(std::remove(bookings_l.begin(), bookings_l.end(), b), bookings_l.end());
            }
          if(ch==3) {
                cout<<"Enter location: ";
                cin>>b;
                cout<<endl;
                bookings_h.push_back(b);
                ETA_high_priority(graph,bookings_h,v,service_time);
                cout<<"Price: Rs. 400\n";
          }
          if(ch==4) {
            cout<<"______________________________________________\n1. What is low priority booking?\n\nSlots alloted such that overall path of the service provider is minimum.\nNote that these slots are alloted dynamically and are subject to change.\nBookings are finalized at the end of the day.\nPlease ensure to check your slot after 9 pm on the same day.\n______________________________________________";
            cout<<"\n2. What is high priority booking?\n\nSlots alloted on first-come-first-serve basis.\n______________________________________________";
            cout<<"\n3. Maximum of how many slots can be booked in one day?\n\nYou can book one high and one low priority slot in a single day.\n______________________________________________";
            cout<<"\n4. Can you cancel a high priority booking?\n\nNo, only a low priority booking can be deleted.\n______________________________________________\n\n";
          }

              if(ch!=5)
              {
                  cout << "Press Enter to Continue ";
                  cin.get();
                  cin.ignore();
                  system("CLS");
              }
      }


	return 0;
}
