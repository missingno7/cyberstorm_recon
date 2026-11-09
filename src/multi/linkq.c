/* multi\linkq.c -- thread-safe doubly linked queue used by the multiplayer link (cs11 0046e0c0..0046e570).
 * One critical section guards every queue. Names are working labels. */
#include <windows.h>
#include <string.h>

static char linkqFile[] = __FILE__;

struct QueueNode {
    QueueNode *prev;
    QueueNode *next;
    void *data;
    int Key();
};

class Queue {
public:
    QueueNode *head;
    QueueNode *tail;

    Queue();
    ~Queue();
    static void Lock();
    static void Unlock();
    void MoveHead(QueueNode *node, Queue *dest);
    int IsEmpty();
    QueueNode *First();
    QueueNode *Append(QueueNode *node);
    QueueNode *Add(unsigned size, void *source);
    void Remove(QueueNode *node);
    void RemoveAll();
    QueueNode *Insert(QueueNode *node);
    QueueNode *AddSorted(unsigned size, void *source);
    QueueNode *Find(int key);
    void Rewind(int unused, int *key);
    int Less(void *a, void *b);

private:
    static int sLockReady;
    static CRITICAL_SECTION sLock;
};

class KeyedQueue : public Queue {
public:
    int key;
    KeyedQueue(int key);
};

int Queue::sLockReady;
CRITICAL_SECTION Queue::sLock;

void Queue::MoveHead(QueueNode *node, Queue *dest)
{
    Lock();
    if (node == head) {
        if ((head = node->next) != 0)
            head->prev = 0;
        if (node->data)
            dest->Append(node);
    }
    Unlock();
}

int Queue::IsEmpty()
{
    return head == 0;
}

QueueNode *Queue::First()
{
    if (head)
        return head;
    return 0;
}

QueueNode *Queue::Append(QueueNode *node)
{
    if (!node->data)
        return 0;
    Lock();
    node->next = 0;
    node->prev = tail;
    if (head == 0)
        head = node;
    if (tail)
        tail->next = node;
    tail = node;
    Unlock();
    return node;
}

QueueNode *Queue::Add(unsigned size, void *source)
{
    QueueNode *node = new QueueNode;
    if (node == 0)
        return 0;
    char *data = new char[size];
    if (data == 0) {
        delete node;
        return 0;
    }
    if (source)
        memcpy(data, source, size);
    else
        memset(data, 0, size);
    node->data = data;
    Append(node);
    return node;
}

void Queue::Remove(QueueNode *node)
{
    Lock();
    if (node && node->data && head) {
        delete[] node->data;
        node->data = 0;
        if (node->prev)
            node->prev->next = node->next;
        else
            head = node->next;
        if (node->next)
            node->next->prev = node->prev;
        else
            tail = node->prev;
        delete node;
        Unlock();
    }
}

void Queue::RemoveAll()
{
    Lock();
    while (head)
        Remove(head);
    Unlock();
}

void Queue::Lock()
{
    EnterCriticalSection(&sLock);
}

void Queue::Unlock()
{
    LeaveCriticalSection(&sLock);
}

Queue::Queue()
{
    if (sLockReady == 0)
        InitializeCriticalSection(&sLock);
    sLockReady = 1;
}

Queue::~Queue()
{
    if (sLockReady)
        DeleteCriticalSection(&sLock);
    sLockReady = 0;
}

KeyedQueue::KeyedQueue(int key)
{
    this->key = key;
}

QueueNode *Queue::Insert(QueueNode *node)
{
    QueueNode *after;
    Lock();
    if (head == 0 || Less(node->data, tail->data) == 0) {
        Append(node);
    } else {
        after = tail;
        while ((after = after->prev) != 0 && Less(node->data, after->data))
            ;
        if (after) {
            node->prev = after;
            node->next = after->next;
            after->next = node;
        } else {
            node->prev = head->prev;
            node->next = head;
            head = node;
        }
    }
    Unlock();
    return node;
}

QueueNode *Queue::AddSorted(unsigned size, void *source)
{
    QueueNode *node = new QueueNode;
    if (node == 0)
        return 0;
    char *data = new char[size];
    if (data == 0) {
        delete node;
        return 0;
    }
    if (source)
        memcpy(data, source, size);
    else
        memset(data, 0, size);
    node->data = data;
    Insert(node);
    return node;
}

QueueNode *Queue::Find(int key)
{
    QueueNode *node;
    Lock();
    node = head;
    while (node) {
        if (node->Key() == key)
            break;
        node = node->next;
    }
    Unlock();
    return node;
}

void Queue::Rewind(int unused, int *key)
{
    QueueNode *node;
    Lock();
    node = tail;
    while (node) {
        if (node->Key() != *key)
            break;
        (*key)--;
        node = node->prev;
    }
    Unlock();
}

