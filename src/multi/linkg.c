// Unnamed transport object. Working types recovered from 36-byte peer records.
#include <string.h>
// Use the established assembler name for the data-message constructor entry.
#pragma aux message_data_ctor_name "W?MenuMessageData$n(pn$MenuMessage$$iiii)pn$1$";
struct MenuMessage;
struct QueueNode { QueueNode *prev,*next; MenuMessage *data; int Key(); MenuMessage *Data() { return data; } };
class Queue { public: QueueNode *head,*tail; Queue(); ~Queue();
 void Lock(); void Unlock(); int IsEmpty(); QueueNode *First();
 void Remove(QueueNode*); void RemoveAll(); QueueNode *Find(int); QueueNode *Add(unsigned,void*);
 int Less(void*,void*); void DiscardThrough(int);
};
struct Timer { unsigned long start,end,duration; Timer(); };
void TimerSet(Timer*,unsigned long);
inline Timer::Timer() { TimerSet(this,0); }
#pragma pack(push,1)
struct PeerQueue { int acknowledged,received,sequence,unreliable; Timer timer; Queue queue;
 PeerQueue() {} ~PeerQueue() {} void Clear();
};
struct Transport { char flags; PeerQueue peers[33]; Transport() {} ~Transport() {} };
struct MenuMessage { int type; unsigned checksum; int sequence,ackLow,ackHigh; unsigned size; int from,to; char payload[442]; MenuMessage(); __pragma("message_data_ctor_name") MenuMessage(int,int,int,int); int Sequence() { return sequence; } int From() { return from; } int To() { return to; } };
#pragma pack(pop)
extern Transport networkQueue;
extern Queue gReceiveQueue;
struct PlayerState { int state; char unknown04[164]; int peer; char unknownac[16]; };
extern PlayerState chat_player_state_table[];
int PeerAlive(int);
int IsSystemMessage(MenuMessage*);
int MessageDeliver(MenuMessage*);
void MessageAcknowledge(MenuMessage*,char*);
MenuMessage *MenuMessageData(MenuMessage*,int,int,int,int);
int MenuSend(MenuMessage*,int);
inline PlayerState *GetPlayerState(int i) { PlayerState *p=chat_player_state_table; p+=i; return p; }
Transport networkQueue;

void PeerQueue::Clear() { memset(this,0,28); if(!queue.IsEmpty()) queue.RemoveAll(); }
void ResetQueue(void *object) { Transport *t=(Transport*)object; for(int i=0;i<33;i++) t->peers[i].Clear(); }
void ValidateSlot(int) {}
void Queue::DiscardThrough(int sequence) {
 Lock();
 for(;;) { QueueNode *n=First(); MenuMessage *m=n?n->Data():0;
  if(!m || m->Sequence()>sequence) break; Remove(n);
 }
 Unlock();
}
int OldestAcknowledgement(Transport *t) {
 int peer=0,sequence=0x7fffffff;
 for(int i=1;i<=9;i++) {
  if(GetPlayerState(i)->state==2) {  int seq=t->peers[GetPlayerState(i)->peer].acknowledged;
   if(seq<sequence) { peer=GetPlayerState(i)->peer; sequence=seq; }
   else if(seq==sequence) peer=0;
  }
 }
 if(peer) PeerAlive(peer);
 return sequence;
}
int QueueAcknowledge(char *object,int peer,int low,int high) {
 Transport *t=(Transport*)object; Queue *queue=&t->peers[peer].queue; queue->DiscardThrough(low);
 if(high>t->peers[peer].acknowledged) t->peers[peer].acknowledged=high;
 Queue *all=&t->peers[0].queue; int oldest=OldestAcknowledgement(t); all->DiscardThrough(oldest); return 1;
}
inline int DeliverSequence(Queue *queue,int sequence) { QueueNode *n=queue->Find(sequence); if(!n) return 0; return MessageDeliver(n->Data()); }
int QueueRedeliver(char *object,int peer,int sequence) { Transport *t=(Transport*)object; return DeliverSequence(&t->peers[peer].queue,sequence); }
int QueueSequence(char *object,int peer) { Transport *t=(Transport*)object; return ++t->peers[peer].sequence; }
int QueueSent(char *object,MenuMessage *m) { Transport *t=(Transport*)object; int peer=m->To(); if(!m->Sequence()) return 0; t->peers[peer].queue.Add(474,m); return 1; }
int QueueReceive(char *object,MenuMessage *m) {
 int sequence=m->Sequence();
 if(sequence && !IsSystemMessage(m)) {
  int current; int peer=m->From(), directed=m->To();
  PeerQueue *p=&((Transport*)object)->peers[peer]; int *received=directed?&p->received:&p->unreliable;
  if(!*received) *received=sequence-1;
  current=*received; if(sequence>current) {
   if(current+1==m->Sequence()) {
    MenuMessage *copy=new MenuMessage;
    *copy=*m; gReceiveQueue.Add(474,copy); ++*received;
   } else { int type=directed?0x1004:0x1005; MenuMessage request(type,current+1,sequence,0); MenuSend(&request,peer); }
  }
  if(directed) MessageAcknowledge(m,object);
 }
 return 1;
}
int QueueNode::Key() { MenuMessage *m=Data(); return m?m->Sequence():0; }
int Queue::Less(void *a,void *b) { if(!a || !b) return 0; return ((MenuMessage*)a)->Sequence()<((MenuMessage*)b)->Sequence(); }
int QueueResend(char *object,int peer,int first,int last) { for(int i=first;i<=last;) QueueRedeliver(object,peer,i++); return 1; }
int QueueRelease(char *object,int first,int last) { for(int i=first;i<=last;) QueueRedeliver(object,0,i++); return 1; }
int QueueUpdate(char *object,int peer,int high,int low,int a,int b) {
 Transport *t=(Transport*)object; PeerQueue *p=&t->peers[peer];
 if(p->received<b) { MenuMessage request(0x1004,p->received+1,p->received+1,0); MenuSend(&request,peer); }
 if(p->unreliable && p->unreliable<a) { MenuMessage request(0x1005,p->unreliable+1,p->unreliable+1,0); MenuSend(&request,peer); }
 return 1;
}

void QueueStatus(char *object,int peer,int *a,int *b,int *high,int *low) {
 Transport *t=(Transport*)object; PeerQueue *p=&t->peers[peer];
 *a=t->peers[0].sequence; *b=p->sequence; *high=p->unreliable; *low=p->received;
}
void QueueAcks(char *object,int peer,int *high,int *low) {
 if(peer) { int a,b; QueueStatus(object,peer,&a,&b,high,low); }
 else { *low=0; *high=0; }
}
static char xSourceFile[] = __FILE__;
