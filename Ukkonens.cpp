
#include <bits/stdc++.h>

using namespace std;

// Suffix Tree Node Structure
struct Node
{
    int start;     // starting index of the Node
    int *end;      // end Index of the Node
    int LeafIndex; // LeafIndex is stored at the Leaf ( depend on the Edge );
    // At max 256 edges can be created
    struct Node *child[256];  // Array of Edges going out of the Node
    struct Node *Suffix_Link; // Suffic Link pointing from Active Node to the Suffix Node
};

int Count_Nodes = 0;    // Total Number of Nodes in the Suffix Tree
int Internal_Nodes = 0; // Total Number of Internal Nodes in the Suffix Tree
int Leaf_Nodes = 0;     // Total Number of Leaf Nodes in the Suffix Tree

Node *RootNode = NULL; // Root Node of the suffix Tree

string Input_String;                            // input string to make the suffix tree
string Previous_Edge;                           // Previous Edge of the Current Node
string temp;                                    // temporary string
string temp_string = " Connected to the root "; // temporary string for showing that the edge is connected to the root Node

Node *Last_Created_Node = NULL; // Last created Node for creating the suffix link from that Node to the current Node
Node *Active_Node = NULL;       // Active Node of the Suffix Tree

int Active_Edge = -1;  // Active Edge representing the index number of the Input string
int Active_Length = 0; // Active Length is the length of the Edge after the Active Node

int Remaining_Nodes = 0; // Remaining Node is for checking how many nodes to be created
int leafEnd = -1;        
int *Root_End = NULL;    
int *Divide_Node = NULL; // Divide Node is for splitting the Node from the Edge or from already created Node.
int size = -1;           //

// For creating or initialising the new Suffix Tree Node
Node *Create_Node(int start, int *end)
{
    Node *node = (Node *)malloc(sizeof(Node));
    for (int i = 0; i < 256; i++)
        node->child[i] = NULL; // first we initialise all the child of the Node with Null
    node->start = start;
    node->end = end;
    node->Suffix_Link = RootNode; 

    node->LeafIndex = -1; 
    return node;
}

int Edge_Length(Node *n)
{
    return *(n->end) - (n->start) + 1;
}
// Next_Char is for checking that We have to jump from A Node to move to the next character
// or we just we have to increase the active length in the Currect Active Node
int Next_Char(Node *currNode)
{

    if (Active_Length >= Edge_Length(currNode)) // if Active length is greater the Edge lenth of the current Active Node then we have to jump a Node
    {
        Active_Edge = Active_Edge + Edge_Length(currNode);
        Active_Length -= Edge_Length(currNode);
        Active_Node = currNode;
        return 1;
    }
    return 0;
}

void Suffix_Indexing(Node *n, int H)
{
    if (n == NULL)
        return; //  if current Node is NULL the we return

    if (n->start != -1)
    {
        int i = n->start;
        int j = *(n->end);

        int k;
        for (k = i; k <= j; k++) {
            cout<<Input_String[k];
            Previous_Edge+=(Input_String[k]);
        }
    }
    int leaf = 1;
    for (int i = 0; i < 256; i++)
    {
        if (n->child[i] != NULL)
        {
            if (leaf == 1 && n->start != -1) {
                printf(" [%d]", n->LeafIndex);
                Internal_Nodes++;
                cout<<" --> Internal Node"<<endl;
            }

            leaf = 0;
            Suffix_Indexing(n->child[i], H + Edge_Length(n->child[i]));
        }
    }
    if (leaf == 1)
    {
       n->LeafIndex = size - H;
        printf(" [%d]", n->LeafIndex);
        Leaf_Nodes++;
        cout<<" --> Leaf Node\n";
       Previous_Edge.erase();
    }
}

// Debugging the code phase by phase
Debug_by_phase(int i)
{
    cout << "\nPhase " << i << endl;
    cout << "Remaining_Nodes = " << Remaining_Nodes << "  Active_Length = " << Active_Length << endl;

    if (Active_Edge == -1)
        cout << "Active_Edge = " << Active_Edge << endl;
    else
        cout << "Active_Edge = " << Active_Edge << "  Active_Character  = " << Input_String[Active_Edge] << endl;
}

int main()
{
    Previous_Edge = "";
    Input_String = "banana$"; // input string

    size = Input_String.size(); // size of the input string
    Root_End = (int *)malloc(sizeof(int));
    *Root_End = -1; // starting with the global end -1

    RootNode = Create_Node(-1, Root_End); // RootNode(-1,-1);

    Active_Node = RootNode; // Root Node is the Active Node before starting the 0th phase

    // Phases for making the Tree
    for (int i = 0; i < size; i++)
    {
        Debug_by_phase(i); // for debugging the code phase by phase

        leafEnd = i;
        Remaining_Nodes++;
        Last_Created_Node = NULL;

        while (Remaining_Nodes > 0)
        {

            if (Active_Length == 0)
            {
                Active_Edge = i;
            }
            if (Active_Node->child[Input_String[Active_Edge]] == NULL)
            {
                Active_Node->child[Input_String[Active_Edge]] = Create_Node(i, &leafEnd);

                if (Last_Created_Node != NULL)
                {
                    Last_Created_Node->Suffix_Link = Active_Node;
                    Last_Created_Node = NULL;
                }
            }
            else
            {
                Node *next = Active_Node->child[Input_String[Active_Edge]];
                if (Next_Char(next))
                {
                    continue;
                }

                if (Input_String[next->start + Active_Length] == Input_String[i])
                {
                    if (Last_Created_Node != NULL && Active_Node != RootNode)
                    {
                        Last_Created_Node->Suffix_Link = Active_Node;
                        Last_Created_Node = NULL;
                    }
                    Active_Length++;
                    break;
                }

                Divide_Node = (int *)malloc(sizeof(int));
                *Divide_Node = next->start + Active_Length - 1;

                Node *split = Create_Node(next->start, Divide_Node);
                Active_Node->child[Input_String[Active_Edge]] = split;

                split->child[Input_String[i]] = Create_Node(i, &leafEnd);
                next->start += Active_Length;
                split->child[Input_String[Active_Edge]] = next;

                if (Last_Created_Node != NULL)
                {
                    Last_Created_Node->Suffix_Link = split;
                }

                Last_Created_Node = split;
            }

            Remaining_Nodes--;
            if (Active_Node == RootNode && Active_Length > 0)
            {
                Active_Length--;
                Active_Edge = i - Remaining_Nodes + 1;
            }

            else if (Active_Node != RootNode)
            {
                Active_Node = Active_Node->Suffix_Link;
            }
        }
    }

    cout << endl
         << endl;

    int H = 0; // labeling at the leaf index

    Suffix_Indexing(RootNode, H);

    cout << endl
         << endl;

    Count_Nodes = Internal_Nodes + Leaf_Nodes;
    cout << "Number of nodes created -> " << Count_Nodes << endl;
    cout << "Number of Internal_Nodes created -> " << Internal_Nodes << endl;
    cout << "Number of Leaf_Nodes created --> " << Leaf_Nodes << endl;

    return 0;
}
