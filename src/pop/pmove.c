/* Movement popup. Original path absent. Working names and packed actor fields. */
typedef unsigned char U8;
typedef unsigned long U32;
#include "actor.h"


struct GuiPoint {
    long x,y;
    GuiPoint(long a=0,long b=0):x(a),y(b) {
    }
    GuiPoint &operator+=(const GuiPoint &p) {
        x+=p.x;
        y+=p.y;
        return *this;
    }
};
struct InputReq {
    U32 mask,state;
};
struct Timer {
    U32 start,end,duration;
};
struct StrPool {
};
class GuiControl:public GuiRect {
    public: U8 style[4];
    U32 attributes,shortcut;
    GuiControl(U32,U32);
    virtual ~GuiControl();
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
    virtual int HitTest(const GuiPoint *);
};
class GuiScreen:public GuiRect {
    public:InputReq *input;
    GuiControl **controls;
    StrPool *strings;
    unsigned count,focus;
    long hovered;
    unsigned pressed;
    U32 options;
    long firstString;
    int navigation,delayed;
    unsigned delayedControl;
    Timer repeatTimer;
    GuiScreen(GuiControl **,unsigned,U32);
    void LoadLayout(long);
    void Open(InputReq *,StrPool *,long,int);
    int Update();
    void Draw();
    long HitTest(const GuiPoint *);
    int IsActive() {
        return input!=0;
    }
};
class SimImageGadget:public GuiControl {
    public: U32 state;
    long image,repeat;
    SimImageGadget(long,long,U32);
    virtual ~SimImageGadget();
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
    virtual int HitTest(const GuiPoint *);
};
class MenuButton:public GuiControl {
    public: U32 state;
    const char *text;
    MenuButton(const char * =0,U32=0);
    virtual ~MenuButton();
    virtual void Reset(unsigned);
    virtual void Draw(unsigned);
    virtual int Event(unsigned,U32);
    void Enable(int disabled) {
        attributes=(attributes&0xff1fffffUL)+((disabled&7)<<21);
    }
    void Invalidate() {
        attributes&=~0x1000000UL;
    }
};
#pragma pack(push,1)



struct ActorItem {
    char storage[637];
    int turn,move,jump,land;
};




















struct SimGame {
    U8 head[20];
    int target;
    U8 unknown18[32];
    ActorRecord *focus;
    U8 untilOwner[0x942];
    int owner;
};
#pragma pack(pop)
struct SoundSettings;
extern int SoundPlay(unsigned,const SoundSettings *);
extern ActorItem *PopFindItem(ActorUnit *,int=0,int=-1);
extern long GridClamp(U32);
extern "C" int ResolveName(const char *,const GuiPoint *);
extern "C" int f_00432c00(const char *,int,int);
extern int SimPreviewCost(SimGame *,int,int);
extern int SimFocusReady(SimGame *);
extern int SimTrySpendFocus(SimGame *,int);
extern void SimFaceTarget(SimGame *,int,int);
extern void SimTargetAction(SimGame *,int);
extern void SimPrepareAction(SimGame *,int);
extern void SimSetFocus(SimGame *,ActorRecord *);
extern ActorRecord *ActorFindByOwner(int);
extern ActorRecord *ActorFindOwnerNext(ActorRecord *,int);
extern "C" int f_00455d60(ActorRecord *,int);
extern "C" int f_00455e50(ActorRecord *,int);
extern void GridClearSelection(void *);
extern ActorRecord *g4d2498;
extern int gLocalActorKind,targetLimits,moveDefaultCost;
extern U32 gIn80,gInA4;
extern GuiPoint g4bb78c;
extern StrPool targetBriefResource;
extern char fallbackName[];
struct ModeSystem;
extern ModeSystem modeCheckName;
static char pmoveSourceFile[] = __FILE__;
extern GuiControl *moveControls[];
class MovementPopup:public GuiScreen {
    public:int changed;
    MovementPopup();
    ~MovementPopup();
    void OpenMove(InputReq *);
    void Close();
    int Process();
    void Render();
    void Invalidate();
};
SimImageGadget moveImageA(145,1000,0),moveImageB(148,1000,0),moveImageC(151,1000,0),moveImageD(154,1000,0);
MenuButton moveJump,moveLand,moveButton2,moveButton3,moveButton4;
MovementPopup simPanel;
GuiPoint moveOffsets[6]= {
    GuiPoint(0,-2),GuiPoint(-1,-1),GuiPoint(-1,1),GuiPoint(0,2),GuiPoint(1,1),GuiPoint(1,-1)
};
MovementPopup::MovementPopup():GuiScreen(moveControls,9,0) {
}
void MovementPopup::OpenMove(InputReq *request) {
    if(!IsActive()) {
        LoadLayout((long)"POPMOVE.BOX");
        Open(request,&targetBriefResource,501,0);
        changed=1;
    }
}
void MovementPopup::Close() {
    if(IsActive())input=0;
}
int MovementPopup::Process() {
    if(IsActive()) {
        if(changed) {
            if((unsigned)g4d2498->action.state==7 || (unsigned)g4d2498->action.state==6) {
                moveJump.Enable(5);
                moveLand.Enable(0);
            }
            else {
                moveLand.Enable(5);
                moveJump.Enable(0);
            }
            moveJump.Invalidate();
            moveLand.Invalidate();
            changed=0;
        }
        int selection=HitTest(&g4bb78c);
        if(selection>=0) {
            targetLimits=selection+400;
            ActorRecord *actor=g4d2498;
            ActorUnit *unit=actor->unit;
            ActorItem *item=PopFindItem(unit,0);
            if(!actor->action.IsMoving()) {
                int cost=0;
                switch(selection) {
                    case 0:case 1:cost=item->turn;
                    break;
                    case 2: {
                        GuiPoint p=moveOffsets[GridClamp(actor->action.direction)%6];
                        p+=(GuiPoint &)actor->action.target;
                        cost=f_00432c00(fallbackName,ResolveName(fallbackName,&p),actor->action.resource);
                        break;
                    }
                    case 4:if((unsigned)actor->action.state!=7)cost=item->jump;
                    break;
                    case 5:if((unsigned)actor->action.state==7)cost=item->land;
                    break;
                    case 6:cost=moveDefaultCost;
                    break;
                }
                if(cost)SimPreviewCost((SimGame *)&modeCheckName,cost,1);
            }
        }
        return Update();
    }
    return 0;
}
void SimPanelDraw(MovementPopup *p) {
    if(p->IsActive())p->Draw();
}
void MovementPopup::Invalidate() {
    changed=1;
}
void StopMoving(void *,int all) {
    if(g4d2498 && g4d2498->action.IsMoving())g4d2498->action.special=3;
    if(all) {
        int owner=gLocalActorKind;
        for(ActorRecord *p=ActorFindByOwner(owner); p; p=ActorFindOwnerNext(p,owner))if(p->action.IsMoving())p->action.special=3;
    }
}
void MovementCommand(SimGame *s) {
    int selection=simPanel.Process()-1;
    if(selection<0) {
        switch(gIn80) {
            case 1:selection=3;
            break;
            case 0x39:selection=6;
            break;
            case 0x21:selection=8;
            break;
            case 0x17:selection=2;
            break;
            case 0x24:selection=0;
            break;
            case 0x26:selection=1;
            break;
            case 0x25:selection=((unsigned)g4d2498->action.state==7)+4;
            break;
        }
        if(selection<0)return;
    }
    if(gInA4) {
        switch(selection) {
            case 6:selection=7;
            break;
            case 8:selection=10;
            break;
            case 4:selection=11;
            break;
            case 5:selection=12;
            break;
        }
    }
    if(!SimFocusReady(s))return;
    ActorItem *item=PopFindItem(s->focus->unit);
    switch(selection) {
        case 0: if(!s->focus || s->focus->action.IsMoving())return;
        SoundPlay(9,0);
        if(!SimTrySpendFocus(s,item->turn))return;
        SimFaceTarget(s,((GridClamp(s->focus->action.direction)+1)%6)*0x2aaaaabc,item->turn);
        break;
        case 1: if(!s->focus || s->focus->action.IsMoving())return;
        SoundPlay(9,0);
        if(!SimTrySpendFocus(s,item->turn))return;
        SimFaceTarget(s,((GridClamp(s->focus->action.direction)-1)%6)*0x2aaaaabc,item->turn);
        break;
        case 2: if(!s->focus || s->focus->action.IsMoving())return;
        {
            GuiPoint p;
            p=moveOffsets[GridClamp(s->focus->action.direction)%6];
            p+=(GuiPoint &)s->focus->action.target;
            GridClearSelection(fallbackName);
            SimPrepareAction(s,ResolveName(fallbackName,&p));
            SoundPlay(9,0);
        }
        break;
        case 3:SoundPlay(9,0);
        StopMoving(s,1);
        break;
        case 4:if(f_00455d60(s->focus,0))SoundPlay(9,0);
        simPanel.changed=1;
        break;
        case 5:if(f_00455e50(s->focus,0))SoundPlay(9,0);
        simPanel.changed=1;
        break;
        case 6:SoundPlay(9,0);
        if(s->focus->action.resource==s->focus->target)return;
        SimTargetAction(s,s->focus->target);
        SimPrepareAction(s,s->focus->target);
        break;
        case 7: {
            SoundPlay(9,0);
            ActorRecord *saved=g4d2498;
            int owner=s->owner;
            for(ActorRecord *p=ActorFindByOwner(owner); p; p=ActorFindOwnerNext(p,owner)) {
                if(p->target && p->action.resource!=p->target) {
                    SimSetFocus((SimGame *)&modeCheckName,p);
                    SimTargetAction(s,p->target);
                    SimPrepareAction(s,p->target);
                }
            }
            SimSetFocus((SimGame *)&modeCheckName,saved);
        }
        break;
        case 8:if(s->target) {
            SimTargetAction(s,s->target);
            SimPrepareAction(s,s->target);
        }
        else SoundPlay(7,0);
        break;
        case 10:if(!s->target) {
            SoundPlay(7,0);
            break;
        }
        {
            ActorRecord *saved=g4d2498;
            SoundPlay(9,0);
            int owner=s->owner;
            for(ActorRecord *p=ActorFindByOwner(owner); p; p=ActorFindOwnerNext(p,owner)) {
                if(p->target==s->target || p->action.resource==s->target) {
                    if(p->target && p->target!=p->action.resource) {
                        SimSetFocus((SimGame *)&modeCheckName,p);
                        SimTargetAction(s,p->target);
                        SimPrepareAction(s,p->target);
                    }
                }
                else {
                    SimSetFocus((SimGame *)&modeCheckName,p);
                    SimTargetAction(s,s->target);
                    SimPrepareAction(s,s->target);
                }
            }
            SimSetFocus((SimGame *)&modeCheckName,saved);
        }
        break;
        case 11: {
            SoundPlay(9,0);
            int owner=s->owner;
            for(ActorRecord *p=ActorFindByOwner(owner); p; p=ActorFindOwnerNext(p,owner))f_00455d60(p,0);
            simPanel.changed=1;
        }
        break;
        case 12: {
            SoundPlay(9,0);
            int owner=s->owner;
            for(ActorRecord *p=ActorFindByOwner(owner); p; p=ActorFindOwnerNext(p,owner))f_00455e50(p,0);
            simPanel.changed=1;
        }
        break;
    }
}
MovementPopup::~MovementPopup() {
    input=0;
}

GuiControl *moveControls[] = {&moveImageA,&moveImageB,&moveImageC,&moveImageD,&moveJump,&moveLand,&moveButton2,&moveButton3,&moveButton4};
