#include <iostream>
#include <queue>
#include <stack>
#include <omp.h>
using namespace std;

class Node
{
public:
    Node *right;
    Node *left;
    int data;

    Node(int data) : data(data), left(nullptr), right(nullptr) {}
};

class Search
{
private:
    Node *root;

public:
    Search() : root(nullptr) {}
    void createTree(); // Function to manually create the tree structure
    void bfs();
    void dfs();
};

void Search::createTree()
{
    int rootData;
    cout << "Enter the root value: ";
    cin >> rootData;
    root = new Node(rootData);

    queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        Node *currNode = q.front();
        q.pop();

        int leftData, rightData;
        cout << "Enter left child value of " << currNode->data << " (-1 if none): ";
        cin >> leftData;

        if (leftData != -1)
        {
            Node *leftNode = new Node(leftData);
            currNode->left = leftNode;
            q.push(leftNode);
        }

        cout << "Enter right child value of " << currNode->data << " (-1 if none): ";
        cin >> rightData;

        if (rightData != -1)
        {
            Node *rightNode = new Node(rightData);
            currNode->right = rightNode;
            q.push(rightNode);
        }
    }
}

void Search::bfs()
{
    if (!root)
    {
        cout << "Tree is empty\n";
        return;
    }

    queue<Node *> q;
    q.push(root);

    double startTime = omp_get_wtime();
    while (!q.empty())
    {
        int qSize = q.size();

#pragma omp parallel for
        for (int i = 0; i < qSize; i++)
        {
            Node *currNode;
#pragma omp critical
            {
                currNode = q.front();
                q.pop();
            }

            // cout << currNode->data << "\t";

#pragma omp critical
            {
                cout << currNode->data << "\t";
            }

#pragma omp critical
            {
                if (currNode->left)
                    q.push(currNode->left);

                if (currNode->right)
                    q.push(currNode->right);
            }
        }
    }
    double endTime = omp_get_wtime();
    cout << "";
    cout << "Time required for bfs: " << endTime - startTime << endl;
}

void Search::dfs()
{
    if (!root)
    {
        cout << "Tree is empty\n";
        return;
    }

    stack<Node *> s;
    s.push(root);
    double startTime = omp_get_wtime();
    while (!s.empty())
    {
        Node *currNode;
#pragma omp critical
        {
            currNode = s.top();
            s.pop();
        }

        cout << currNode->data << "\t";

#pragma omp critical
        {
            if (currNode->right)
                s.push(currNode->right);

            if (currNode->left)
                s.push(currNode->left);
        }
    }
    double endTime = omp_get_wtime();
    cout << "";
    cout << "Time required for dfs: " << endTime - startTime << endl;
}

int main()
{
    Search tree;
    tree.createTree();

    cout << "BFS traversal:\n";
    tree.bfs();
    cout << "\n\n";

    cout << "DFS traversal:\n";
    tree.dfs();

    return 0;
}

/*
Run Commands:
1) g++ -fopenmp Search.cpp -o search
2) ./search
*/
/*
1
/ \
2   3
/   / \
4   5   6

Enter the root value: 1
Enter left child value of 1 (-1 if none): 2
Enter right child value of 1 (-1 if none): 3
Enter left child value of 2 (-1 if none): 4
Enter right child value of 2 (-1 if none): -1
Enter left child value of 3 (-1 if none): 5
Enter right child value of 3 (-1 if none): 6
Enter left child value of 4 (-1 if none): -1
Enter right child value of 4 (-1 if none): -1
Enter left child value of 5 (-1 if none): -1
Enter right child value of 5 (-1 if none): -1
Enter left child value of 6 (-1 if none): -1
Enter right child value of 6 (-1 if none): -1


BFS traversal:
1	2	3	4	5	6
Time required for bfs: 0.000123 s

DFS traversal:
1	2	4	3	5	6
Time required for dfs: 0.000111 s
*/