
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    // Member Initialization Avoids default construction + reassignment. Directly initializes in memory.
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// ! nullptr vs NULL
// In C++ nullptr is literally an integer constant 0.
// void f(int);
// void f(void*);

// f(NULL);   // calls f(int) because NULL = 0
// f(nullptr);  // calls f(void*) correctly 

/*
//! Initializer List vs initialize inside constructor body
class Person {
    const string name;  // Immutable after construction
    int age;
    public:
    Person(string n, int a) {
    name = n;  //! Error: cannot assign to const member
    age = a; //!age is default-initialized (indeterminate) then assigned 10, extra work, slower.
    }
    Person(string n, int a) : name(n), age(a) {}
};
*/
// Const data members have no default constructor value.
// If not initialized in the initializer list → compile-time error.
// Initializer List runs before body
// Assignment in Body	runs after members default-constructed

void displayLL(ListNode *p)
{
    ListNode *head = p;
    while (head)
    {
        cout << head->val << "->";
        head = head->next;
    }
    cout << "NULL\n";
    return;
}

// Create a linked list from an array
ListNode *createLL(vector<int> arr)
{
    ListNode *head = new ListNode(arr[0]);
    ListNode *last = head;
    int n = arr.size();
    for (int i = 1; i < n; i++)
    {
        last->next = new ListNode(arr[i]);
        last = last->next;
    }
    return head;
}

// Insert at End
ListNode *insertAtEnd(ListNode *head, int val)
{
    ListNode *newNode = new ListNode(val);
    if (!head)
        return newNode;
    ListNode *temp = head;
    while (temp->next)
        temp = temp->next;
    temp->next = newNode;
    return head;
}

// Insert at Pos
ListNode *insertAtPos(ListNode *head, int val, int pos)
{
    ListNode *newNode = new ListNode(val);
    if (!head)
        return newNode;

    if (pos == 0)
    {
        newNode->next = head;
        head = newNode;
        return head;
    }
    ListNode *curr = head;
    for (int i = 0; curr && i < pos - 1; i++)
    {
        curr = curr->next;
    }

    if (!curr)
        return head;
    newNode->next = curr->next;
    curr->next = newNode;
    return head;
}

// Delete at Pos
ListNode *deleteAtPosition(ListNode *head, int pos)
{
    if (!head)
        return nullptr;
    if (pos == 0)
    {
        ListNode *temp = head;
        head = head->next;
        delete temp;
        return head;
    }
    ListNode *curr = head;
    for (int i = 0; curr && i < pos - 1; i++)
    {
        curr = curr->next;
    }

    if (!curr || !curr->next)
        return head;
    ListNode *toDelete = curr->next;
    curr->next = curr->next->next;
    delete toDelete;
    return head;
}

// Reverse List
ListNode *reverseList(ListNode *head)
{
    ListNode *prev = nullptr;
    ListNode *curr = head;
    while (curr)
    {
        ListNode *next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

// Detect Cycle
bool hasCycle(ListNode *head)
{
    ListNode *slow = head;
    ListNode *fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            return true;
    }
    return false;
}

// Find Middle
ListNode *findMiddle(ListNode *head)
{
    ListNode *slow = head;
    ListNode *fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// Merge Two Sorted Lists
ListNode *mergeTwoLists(ListNode *list1, ListNode *list2)
{
    ListNode *temp = new ListNode();
    ListNode *ans = temp;

    while (list1 && list2)
    {
        if (list1->val <= list2->val)
        {
            temp->next = list1;
            list1 = list1->next;
        }
        else
        {
            temp->next = list2;
            list2 = list2->next;
        }
        temp = temp->next;
    }
    temp->next = (list1 ? list1 : list2);
    return ans->next;
}

// Remove Nth Node From End
ListNode *removeNthFromEnd(ListNode *head, int n)
{
    ListNode *t = head;
    for (int i = 0; i < n; i++)
    {
        t = t->next;
    }

    ListNode *prev = nullptr;
    ListNode *curr = head;
    if (!t)
    {
        t = head;
        head = head->next;
        t->next = nullptr;
        return head;
    }

    while (t)
    {
        t = t->next;
        prev = curr;
        curr = curr->next;
    }

    prev->next = curr->next;
    curr->next = nullptr;

    return head;
}

// Check Palindrome
bool isPalindrome(ListNode *head)
{
    if (!head || !head->next)
        return true;
    ListNode *mid = findMiddle(head);
    ListNode *secondHalf = reverseList(mid);
    ListNode *p1 = head;
    ListNode *p2 = secondHalf;

    bool isPalin = true;
    while (p2)
    {
        if (p1->val != p2->val)
        {
            isPalin = false;
            break;
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    // Restore the list
    reverseList(secondHalf);
    return true;
}

// Detect Start of Cycle
ListNode *detectCycle(ListNode *head)
{
    ListNode *slow = head;
    ListNode *fast = head;
    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
            break;
    }
    if (!fast || !fast->next)
        return nullptr;
    slow = head;
    while (slow != fast)
    {
        slow = slow->next;
        fast = fast->next;
    }
    return slow;
}

// Intersection point of Y linked list: Method-1
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB)
{
    int l1 = 0, l2 = 0;
    ListNode *p1 = headA;
    ListNode *p2 = headB;

    // Calculate lengths
    while (p1)
    {
        l1++;
        p1 = p1->next;
    }
    while (p2)
    {
        l2++;
        p2 = p2->next;
    }

    // Align both pointers
    int diff = l1 - l2;
    while (diff > 0)
    {
        headA = headA->next;
        diff--;
    }
    while (diff < 0)
    {
        headB = headB->next;
        diff++;
    }

    // Traverse together
    while (headA && headB)
    {
        if (headA == headB)
            return headA;
        headA = headA->next;
        headB = headB->next;
    }

    return nullptr;
}

ListNode *getIntersectionNode2(ListNode *headA, ListNode *headB)
{
    ListNode *p1 = headA;
    ListNode *p2 = headB;

    while (p1 != p2)
    {
        p1 = p1 ? p1->next : headB;
        p2 = p2 ? p2->next : headA;
    }

    return p1; // Can be intersection node or nullptr
}

ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
{
    ListNode *dummy = new ListNode(); // Dummy node for easy result building
    ListNode *last = dummy;
    int carry = 0;

    while (l1 || l2 || carry)
    {
        int s1 = l1 ? l1->val : 0;
        int s2 = l2 ? l2->val : 0;
        int sum = s1 + s2 + carry;

        last->next = new ListNode(sum % 10);
        last = last->next;
        carry = sum / 10;

        if (l1)
            l1 = l1->next;
        if (l2)
            l2 = l2->next;
    }

    return dummy->next;
}

ListNode *rotateRight(ListNode *head, int k)
{
    if (!head || !head->next || k == 0)
        return head;

    // Step 1: Find length and last node
    ListNode *curr = head;
    int length = 1;
    while (curr->next)
    {
        curr = curr->next;
        length++;
    }

    // Step 2: Calculate effective rotations
    k %= length;
    if (k == 0)
        return head;

    // Step 3: Find new tail (at position length - k)
    curr->next = head; // make it circular
    int stepsToNewTail = length - k;
    ListNode *newTail = head;
    for (int i = 1; i < stepsToNewTail; i++)
    {
        newTail = newTail->next;
    }

    // Step 4: Break the circle and return new head
    head = newTail->next;
    newTail->next = nullptr;
    return head;
}

// Method-1
ListNode *reverseKGroup(ListNode *head, int k)
{
    if (!head || k == 1)
        return head;

    // Step 1: Store values in a vector
    vector<int> values;
    ListNode *curr = head;
    while (curr)
    {
        values.push_back(curr->val);
        curr = curr->next;
    }

    // Step 2: Reverse every k-group in the vector
    for (int i = 0; i + k <= values.size(); i += k)
    {
        reverse(values.begin() + i, values.begin() + i + k);
    }

    // Step 3: Write back the values into the linked list
    curr = head;
    int i = 0;
    while (curr)
    {
        curr->val = values[i++];
        curr = curr->next;
    }
    return head;
}

// Optimal time:O(n) space:O(1)
ListNode *reverseKGroup(ListNode *head, int k)
{
    if (!head || k == 1)
        return head;

    ListNode *prev = new ListNode(0);
    ListNode *ans = prev;
    prev->next = head;
    while (head)
    {
        ListNode *curr = head;
        for (int i = 0; i < k - 1; i++)
        {
            head = head->next;
            if (!head)
                return ans->next;
        }
        ListNode *next = head->next;
        head->next = nullptr;
        prev->next = reverseList(curr);
        curr->next = next;
        prev = curr;
        head = next;
    }
    return ans->next;
}

// Method-1 merge k sorted lists
// SC: O(N) extra for the vector
// TC: O(N)  iterates over the N nodes + O(N log N) Sort nodes

struct Node
{
    int data;
    int val;
    struct Node *next;
    struct Node *bottom;
    struct Node *random;

    Node(int x)
    {
        data = x;
        next = NULL;
        bottom = NULL;
    }
};
static bool comp(Node *a, Node *b)
{
    return a->data < b->data;
}

Node *flatten(Node *root)
{
    vector<Node *> v;
    Node *p = root;
    while (p)
    {
        Node *q = p;
        while (q)
        {
            v.push_back(q);
            q = q->bottom;
        }
        p = p->next;
    }

    sort(v.begin(), v.end(), comp);

    Node *dummy = new Node(0);
    Node *curr = dummy;
    for (Node *node : v)
    {
        curr->bottom = new Node(node->data);
        curr = curr->bottom;
    }

    return dummy->bottom;
}

// efficient TC: O(N * log k)
// SC: O(log k) (due to recursion stack)
// Where N is total nodes, k is the number of vertical lists.
Node *merge(Node *a, Node *b)
{
    Node *dummy = new Node(0);
    Node *res = dummy;

    while (a && b)
    {
        if (a->data <= b->data)
        {
            res->bottom = a;
            a = a->bottom;
        }
        else
        {
            res->bottom = b;
            b = b->bottom;
        }
        res = res->bottom;
    }

    if (a)
        res->bottom = a;
    else
        res->bottom = b;

    return dummy->bottom;
}

Node *flatten(Node *root)
{
    if (!root || !root->next)
        return root;

    Node *mergeWithRoot = flatten(root->next); // flatten the rest
    return merge(root, mergeWithRoot);         // merge current with flattened rest
}

// Copy list with random pointer
Node *copyRandomList(Node *head)
{
    Node *p = head;
    Node *res = new Node(0);
    Node *newHead = res;
    unordered_map<Node *, Node *> m;
    while (p)
    {
        m[p] = new Node(p->val);
        p = p->next;
    }
    p = head;
    while (p)
    {
        Node *pp = m[p];
        if (p->random)
        {
            pp->random = m[p->random];
        }
        res->next = pp;
        res = res->next;
        p = p->next;
    }
    return newHead->next;
}

Node *copyRandomList2(Node *head)
{
    if (!head)
        return nullptr;

    Node *p = head;

    // Step 1: Interleave cloned nodes
    while (p)
    {
        Node *t = new Node(p->val);
        t->next = p->next;
        p->next = t;
        p = t->next;
    }

    // Step 2: Set random pointers of copied nodes
    p = head;
    while (p)
    {
        if (p->random)
        {
            p->next->random = p->random->next;
        }
        p = p->next->next;
    }

    // Step 3: Separate the two lists
    p = head;
    Node *copyHead = head->next;
    Node *t = copyHead;

    while (p)
    {
        p->next = p->next->next;
        t->next = t->next ? t->next->next : nullptr;
        p = p->next;
        t = t->next;
    }

    return copyHead;
}

// LRU cache (doubly linked list)
int main()
{
    vector<int> v = {1, 2, 3, 4, 5};
    ListNode *head = createLL(v);
    displayLL(head);
    deleteAtPosition(head, 1);
    displayLL(head);
    insertAtEnd(head, 6);
    insertAtPos(head, 2, 1);
    displayLL(head);

    return 0;
}
