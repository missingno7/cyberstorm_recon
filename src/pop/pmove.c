/* Movement popup. Original path absent. Working names and packed actor fields. */
typedef unsigned char U8;
typedef unsigned long U32;
struct GuiRect {
    long x,y,width,height;
};
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
struct ActorUnit {
    char storage[267];
};
struct ActorItem {
    char storage[637];
    int turn,move,jump,land;
};
struct ActorIdentity {
    U8 header[5];
    U8 type;
    int IsMobile() {
        return !(type==1 || type==7 || type==14 || type==17 || type==18);
    }
};
struct ActorRecord {
    int id,next,index,kind;
    int start;
    U8 unknown14[4];
    ActorUnit *unit;
    char unknown1c[4];
    ActorIdentity identity;
    U8 unknown26[20];
    GuiPoint position;
    int location,angle;
    U8 unknown4a[12];
    U8 state;
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
            if((unsigned)g4d2498->identity.type==7 || (unsigned)g4d2498->identity.type==6) {
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
            if(!actor->identity.IsMobile()) {
                int cost=0;
                switch(selection) {
                    case 0:case 1:cost=item->turn;
                    break;
                    case 2: {
                        GuiPoint p=moveOffsets[GridClamp(actor->angle)%6];
                        p+=actor->position;
                        cost=f_00432c00(fallbackName,ResolveName(fallbackName,&p),actor->location);
                        break;
                    }
                    case 4:if((unsigned)actor->identity.type!=7)cost=item->jump;
                    break;
                    case 5:if((unsigned)actor->identity.type==7)cost=item->land;
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
    if(g4d2498 && g4d2498->identity.IsMobile())g4d2498->state=3;
    if(all) {
        int owner=gLocalActorKind;
        for(ActorRecord *p=ActorFindByOwner(owner); p; p=ActorFindOwnerNext(p,owner))if(p->identity.IsMobile())p->state=3;
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
            case 0x25:selection=((unsigned)g4d2498->identity.type==7)+4;
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
        case 0: if(!s->focus || s->focus->identity.IsMobile())return;
        SoundPlay(9,0);
        if(!SimTrySpendFocus(s,item->turn))return;
        SimFaceTarget(s,((GridClamp(s->focus->angle)+1)%6)*0x2aaaaabc,item->turn);
        break;
        case 1: if(!s->focus || s->focus->identity.IsMobile())return;
        SoundPlay(9,0);
        if(!SimTrySpendFocus(s,item->turn))return;
        SimFaceTarget(s,((GridClamp(s->focus->angle)-1)%6)*0x2aaaaabc,item->turn);
        break;
        case 2: if(!s->focus || s->focus->identity.IsMobile())return;
        {
            GuiPoint p;
            p=moveOffsets[GridClamp(s->focus->angle)%6];
            p+=s->focus->position;
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
        if(s->focus->location==s->focus->start)return;
        SimTargetAction(s,s->focus->start);
        SimPrepareAction(s,s->focus->start);
        break;
        case 7: {
            SoundPlay(9,0);
            ActorRecord *saved=g4d2498;
            int owner=s->owner;
            for(ActorRecord *p=ActorFindByOwner(owner); p; p=ActorFindOwnerNext(p,owner)) {
                if(p->start && p->location!=p->start) {
                    SimSetFocus((SimGame *)&modeCheckName,p);
                    SimTargetAction(s,p->start);
                    SimPrepareAction(s,p->start);
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
                if(p->start==s->target || p->location==s->target) {
                    if(p->start && p->start!=p->location) {
                        SimSetFocus((SimGame *)&modeCheckName,p);
                        SimTargetAction(s,p->start);
                        SimPrepareAction(s,p->start);
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
