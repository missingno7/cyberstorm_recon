/* gui\menu.c: inferred shared control names; Watcom 10.6 C++, plain + -bm.
 * Packed control fields and text buffers follow the observed data layouts. */
#include <string.h>
typedef unsigned long U32;
struct GuiRect {long x,y,width,height;};
struct GuiPoint {long x,y;};
struct InputReq {U32 mask,state;int HelpPressed(){return (state&0xa0)==0x20;}};
struct Timer;extern void TimerSet(Timer *,U32);extern int TimerExpired(Timer *);
struct Timer {U32 start,end,duration;Timer(U32 n){TimerSet(this,n);}};
struct FontData {unsigned char unknown[32];unsigned char *widths;};
struct Font {FontData *data;};
struct StrPool {char *GetString(long,int=0);char *Prefix(long id){return GetString(id,0);}};
struct SoundSettings;extern int SoundPlay(unsigned,const SoundSettings *);
extern "C" void AssertFail(const char *,int,const char *);
extern InputReq *gGuiCurrentInput;
extern U32 gIn80,gIn84;inline U32 MenuInputChar(){return gIn84;}
extern Font *gMenuFonts[5];
extern U32 gMenuColors[30],gMenuColorMap[30];
extern char g513888;
extern U32 gMenuForeground,gMenuBackground,gMenuTextMode,gMenuTextColor;
extern U32 *gMenuTextMap;
extern GuiRect gMenuClip;
extern StrPool *gGuiStringPool;extern long gGuiStringBase;
struct MenuImage {U32 offset;unsigned short width,height;U32 flags;};
struct MenuImages {U32 count;MenuImage images[1];};
extern MenuImages *gMenuImages;
extern char g511948,gMenuSavedSurface;
extern StrPool gMenuPrefixStrings;
extern void MenuFill(void *,long,long,long,long);
extern void MenuLine(void *,long,long,long,long);
extern void MenuBevel(void *,int,int,long,long,long,long);
extern void MenuSetColors(void *,U32,long);
extern void MenuDrawImage(void *,const MenuImage *,long,long);
extern void MenuSetClip(GuiRect *,long,long,long,long);
extern long MenuTextWidth(Font *,const char *);
extern void MenuDrawText(Font *,long,long,const char *);
extern void MenuDrawWrapped(const char *,long,long,long,long,const GuiRect *,int=0,int=0);
extern void MenuDrawPanel(const MenuImage *,const GuiRect *);
extern void MenuRestore(void *,void *,const GuiRect *,long,long);
extern void MenuHelpSetTopic(long);extern void MenuHelpOpenTopic(long);
extern const char *MenuFormatNumber(long,long=0,long=16);
extern int MenuControlChanged(void *,unsigned,U32 *);
struct GuiControlData:GuiRect {unsigned char style[4];U32 attributes,shortcut;};
class GuiControl:public GuiControlData {
public:
    GuiControl(U32,U32);virtual ~GuiControl();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);virtual int HitTest(const GuiPoint *);
void Clip(){long left=x,top=y,w=width,h=height;MenuSetClip(&gMenuClip,left,top,w,h);}void ResetCore(){attributes&=~0x1060000UL;}int Dirty(){return (attributes&0x1000000UL)==0;}void Clean(){attributes|=0x1000000UL;}
};
#pragma pack(push,1)
class MenuLabel:public GuiControl {
public:
    const char *text;long align;unsigned char font;int background;long color;MenuLabel(const char *,long,unsigned char,long,int);virtual ~MenuLabel();virtual void Reset(unsigned);virtual void Draw(unsigned);void SetFont(unsigned char);void SetText(const char *);void SetColor(long);void SetAlign(long);void SetBackground(int);
};
class MenuHelpRegion:public GuiControl {
public:
    long id;int activated;MenuHelpRegion();virtual ~MenuHelpRegion();virtual void Draw(unsigned);virtual int Event(unsigned,U32);
};
class MenuCaption:public GuiControl {
public:
    const char *text;signed char align;unsigned char reserved[2],drawn;MenuCaption(const char *,signed char);virtual ~MenuCaption();virtual void Reset(unsigned);virtual void Draw(unsigned);void SetText(const char *);
};
class MenuEdit:public GuiControl {
public:
    U32 state;char *text;U32 capacity,cursor;MenuEdit(char *,U32);virtual ~MenuEdit();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);char *GetText();void SetText(const char *);
};
class MenuButton:public GuiControl {
public:
    U32 state;const char *text;MenuButton(const char *,U32);virtual ~MenuButton();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);
};
class MenuTextButton:public GuiControl {
public:
    U32 state,unused;const char *text;signed char font,color,align;MenuTextButton(const char *,U32,signed char,signed char,signed char);virtual ~MenuTextButton();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);void SetColor(signed char);void SetAlign(signed char);void SetFont(signed char);
};
class MenuListButton:public GuiControl {
public:
    U32 state;MenuListButton();virtual ~MenuListButton();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);
};
class MenuImageButton:public GuiControl {
public:
    U32 state;long image,repeat;MenuImageButton(long=0,long=0,U32=0);virtual ~MenuImageButton(){}virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);
};
class MenuImageLabel:public GuiControl {
public:
    const char *text;long image;signed char alignX,alignY,font,unused,color,bevel;MenuImageLabel(long=0,signed char=0,signed char=0,signed char=1,signed char=0,signed char=11,signed char=0);virtual ~MenuImageLabel(){}virtual void Draw(unsigned);void SetStyle(signed char,signed char,signed char,signed char,signed char,signed char);
};
class MenuSpacer:public GuiControl {
public:
    MenuSpacer();virtual ~MenuSpacer(){}virtual void Draw(unsigned);
};
class MenuPrefixButton:public GuiControl {
public:
    const char *text;U32 state;signed char font,color,align,prefix;MenuPrefixButton(const char *,signed char,signed char,signed char);virtual ~MenuPrefixButton();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);void SetText(const char *);void SetColor(signed char);void SetAlign(signed char);void SetPrefix(signed char);void SetFont(signed char);
};
class MenuPanel:public GuiControl {
public:
    const char *text;MenuPanel();virtual ~MenuPanel();virtual void Draw(unsigned);
};
class MenuNumber:public GuiControl {
public:
    U32 unused;const char *text;long value;signed char font,color,flash,align;MenuNumber(signed char,signed char);virtual ~MenuNumber();virtual void Reset(unsigned);virtual void Draw(unsigned);void SetValue(long,signed char);void SetFont(signed char);
};
struct MenuMessage {unsigned char bytes[476];MenuMessage(unsigned,const char *);};
struct MenuNetwork {int Count();};extern MenuNetwork gMenuNetwork;extern void MenuSend(const MenuMessage *,int);extern int MenuControlKey();
extern char gMenuHistory[2048],gMenuVisible[48];
class MenuChat:public GuiControl {
public:
    U32 state,scrollStart,maxPosition,visibleLength,cursor;Timer sendTimer,blinkTimer,forceTimer;int pending;signed char font;long caretColor;MenuChat(U32);virtual ~MenuChat();virtual void Reset(unsigned);virtual void Draw(unsigned);virtual int Event(unsigned,U32);char *GetText();void Clear();void Send();void ClearCore(){state=0;memset(gMenuHistory,0,2048);memset(gMenuVisible,0,48);scrollStart=0;cursor=0;gMenuHistory[0]=26;}void SendCore(){if(pending){if(gMenuNetwork.Count()>1){MenuMessage message(0x1002,gMenuVisible);MenuSend(&message,0);}pending=0;}}
};
#pragma pack(pop)
static unsigned char editKeys[9]={28,1,14,71,79,75,77,83,82};
static unsigned char chatKeys[9]={28,1,14,71,79,75,77,83,82};
extern int gMenuInsert,gMenuChatInsert;
 static char menuSourceFile[] = __FILE__;
MenuLabel::MenuLabel(const char *s,long a,unsigned char f,long c,int b):GuiControl(32,0),text(s),align(a),font(f),background(b),color(c) {
}
void MenuLabel::SetFont(unsigned char f) {
    font=f;
}
void MenuLabel::SetText(const char *s) {
    text=s;
}
void MenuLabel::SetColor(long c) {
    color=c;
}
void MenuLabel::SetAlign(long a) {
    align=a;
}
void MenuLabel::Reset(unsigned) {
    ResetCore();
    text=0;
}
void MenuLabel::SetBackground(int b) {
    background=b;
}
void MenuLabel::Draw(unsigned) {
    if(background) {
        gMenuBackground=gMenuForeground=gMenuColors[0];
        MenuFill(&g513888,x,y,width,height);
        Clip();
    }
    if(gMenuFonts[font]&&text) {
        gMenuTextMode=1;
        gMenuTextMap=&gMenuColorMap[color];
        gMenuTextColor=gMenuColors[color];
        long left=x;
        if(align>=0) {
            long extra=width-MenuTextWidth(gMenuFonts[font],text);
            if(align<=0)extra>>=1;
            left+=extra;
        }
        MenuDrawText(gMenuFonts[font],left,y,text);
    }
}
MenuHelpRegion::MenuHelpRegion():GuiControl(3,0),id(-1),activated(0) {
}
void MenuHelpRegion::Draw(unsigned) {
    if(id!=-1) {
        MenuHelpSetTopic(id+gGuiStringBase);
        if(activated)MenuHelpOpenTopic(id+gGuiStringBase);
    }
    activated=0;
}
int MenuHelpRegion::Event(unsigned,U32) {
    if(gGuiCurrentInput->HelpPressed())activated=1;
    return 0;
}
MenuCaption::MenuCaption(const char *s,signed char a):GuiControl(0,0),text(s),align(a),drawn(0) {
}
void MenuCaption::SetText(const char *s) {
    text=s;
    drawn=0;
}
void MenuCaption::Reset(unsigned) {
    ResetCore();
    drawn=0;
    text=0;
}
void MenuCaption::Draw(unsigned) {
    if(!drawn) {
        gMenuTextMode=1;
        gMenuTextMap=&gMenuColorMap[2];
        gMenuTextColor=gMenuColors[2];
        Clip();
        long left=x;
        if(align>=0) {
            long extra=width-MenuTextWidth(gMenuFonts[2],text);
            if(align<=0)extra>>=1;
            left+=extra;
        }
        MenuDrawText(gMenuFonts[2],left,y,text);
        drawn=1;
    }
}
MenuEdit::MenuEdit(char *s,U32 n):GuiControl(3,0),state(0),text(s),capacity(n) {
}
char *MenuEdit::GetText() {
    return text;
}
void MenuEdit::SetText(const char *s) {
    strcpy(text,s);
    cursor=strlen(text);
}
void MenuEdit::Draw(unsigned) {
    if(gMenuFonts[2]&&text&&!(state&1)) {
        state|=1;
        gMenuForeground=gMenuColors[15];
        gMenuBackground=gMenuColors[8];
        MenuFill(&g513888,x,y,width,height);
        Clip();
        gMenuTextMap=&gMenuColorMap[15];
        gMenuTextColor=gMenuColors[15];
        gMenuTextMode=1;
        MenuDrawText(gMenuFonts[2],x+10,y,text);
        char c=text[cursor];
        text[cursor]=0;
        long left=MenuTextWidth(gMenuFonts[2],text)+x+9;
        text[cursor]=c;
        gMenuForeground=gMenuColors[16];
        MenuLine(&g513888,left,y,left,y+height-1);
    }
}
int MenuEdit::Event(unsigned,U32) {
    int changed=0;
    U32 key=gIn80;
    signed char c=(signed char)MenuInputChar();
    U32 length=strlen(text);
    

































if(length<cursor||length>capacity)AssertFail(menuSourceFile,__LINE__,0);
    if(memchr(editKeys,key,9)) {
        switch(key) {
            case 28:return 1;
            case 1:return 2;
            case 14:if(cursor>0) {
                memmove(&text[cursor-1],text+cursor,length-cursor+1);
                --length;
                --cursor;
                changed=1;
            }
            break;
            case 71:cursor=0;
            changed=1;
            break;
            case 79:cursor=length;
            changed=1;
            break;
            case 75:if(cursor>0) {
                --cursor;
                changed=1;
            }
            break;
            case 77:if(cursor<length) {
                ++cursor;
                changed=1;
            }
            break;
            case 83:if((long)length>0&&cursor<length) {
                memmove(text+cursor,&text[cursor+1],length-cursor);
                --length;
                changed=1;
            }
            break;
            case 82:gMenuInsert^=1;
            break;
        }
    }
    else if(c>=(signed char)32&&c<=(signed char)126&&length<capacity) {
        long size=MenuTextWidth(gMenuFonts[2],text);
        size+=gMenuFonts[2]->data->widths[(unsigned char)c];
        if(size<width-2) {
            U32 next=length+1;
            if(gMenuInsert) {
                memmove(&text[cursor+1],text+cursor,length-cursor+1);
                length=next;
            }
            else if(length==cursor)length=next;
            text[cursor++]=c;
            changed=1;
        }
    }
    if(changed) {
        text[length]=0;
        state&=~1UL;
    }
    return 0;
}
MenuButton::MenuButton(const char *s,U32 key):GuiControl(16,key),state(0),text(s) {
}
void MenuButton::Reset(unsigned) {
    ResetCore();
    state=0;
    text=0;
}
void MenuButton::Draw(unsigned index) {
    U32 previous=state;
    if(MenuControlChanged(this,index,&state)) {
        long image=18,color=20;
        if(state&0x80000UL) {
            image=36;
            if(state&0x20000UL) {
                if(!(previous&0x20000UL))SoundPlay(415,0);
                image=27;
                color=19;
            }
            else if(!(previous&0x80000UL))SoundPlay(392,0);
        }
        MenuDrawPanel(&gMenuImages->images[image],this);
        Clip();
        const char *s=text;
        if(!s)s=gGuiStringPool->GetString(gGuiStringBase+index,0);
        if(s)MenuDrawWrapped(s,0,0,1,color,this,0,0);
    }
}
int MenuButton::Event(unsigned index,U32) {
    return index+1;
}
MenuTextButton::MenuTextButton(const char *s,U32 key,signed char a,signed char c,signed char f):GuiControl(16,key),state(0),unused(0),text(s),font(f),color(c),align(a) {
}
void MenuTextButton::Reset(unsigned) {
    ResetCore();
    state=unused=0;
    text=0;
}
void MenuTextButton::SetColor(signed char c) {
    color=c;
    state&=~0x1000000UL;
}
void MenuTextButton::SetAlign(signed char a) {
    align=a;
    state&=~0x1000000UL;
}
void MenuTextButton::SetFont(signed char f) {
    font=f;
    state&=~0x1000000UL;
}
void MenuTextButton::Draw(unsigned index) {
    if(MenuControlChanged(this,index,&state)) {
        const char *s=text;
        if(!s)s=gGuiStringPool->GetString(gGuiStringBase+index,0);
        if(s) {
            MenuDrawWrapped(s,font,-1,align,(state&0x480000UL)?15:color,this);
            Clip();
        }
    }
}
MenuListButton::MenuListButton():GuiControl(16,0),state(0) {
}
void MenuListButton::Draw(unsigned index) {
    U32 previous=state;
    if(MenuControlChanged(this,index,&state)) {
        MenuRestore(&g511948,&gMenuSavedSurface,this,x,y);
        Clip();
        const char *s=gGuiStringPool->GetString(gGuiStringBase+index,0);
        if(s) {
            GuiRect r=*this;
            r.y--;
            r.x+=30;
            MenuDrawWrapped(s,-1,-1,2,(state&0x480000UL)?15:2,&r);
            if(state&0x480000UL) {
                if((state&0x20000UL)&&!(previous&0x20000UL))SoundPlay(5,0);
                else if(!(previous&0x480000UL))SoundPlay(392,0);
            }
        }
    }
}
MenuImageButton::MenuImageButton(long n,long delay,U32 key):GuiControl(28,key),state(0),image(n),repeat(delay) {
}
void MenuImageButton::Draw(unsigned index) {
    U32 previous=state;
    if(MenuControlChanged(this,index,&state)) {
        Clip();
        if((state&0x20000UL)&&(state&0x80000UL)) {
            if(!(previous&0x20000UL)&&!(previous&0x80000UL))SoundPlay(415,0);
            MenuDrawImage(&g513888,&gMenuImages->images[image+1],x,y);
        }
        else MenuDrawImage(&g513888,&gMenuImages->images[image],x,y);
        if(state&0x80000UL) {
            if(!(previous&0x80000UL)&&!(state&0x20000UL))SoundPlay(392,0);
            MenuSetColors(&g513888,gMenuColors[15],-1);
            MenuFill(&g513888,x,y,width,height);
        }
    }
}
int MenuImageButton::Event(unsigned index,U32 event) {
    static Timer repeatTimer(0);
    int result=index+1;
    if(repeat) {
        if(event&4) {
            TimerSet(&repeatTimer,repeat*500/1000);
            return result;
        }
        if((event&8)&&TimerExpired(&repeatTimer)) {
            SoundPlay(392,0);
            TimerSet(&repeatTimer,repeat*200/1000);
            return result;
        }
    }
    else if(event&16)return result;
    return 0;
}
MenuImageLabel::MenuImageLabel(long n,signed char ax,signed char ay,signed char f,signed char u,signed char c,signed char b):GuiControl(32,0),text(0),image(n),alignX(ax),alignY(ay),font(f),unused(u),color(c),bevel(b) {
}
void MenuImageLabel::SetStyle(signed char ax,signed char ay,signed char f,signed char u,signed char c,signed char b) {
    alignX=ax;
    alignY=ay;
    font=f;
    unused=u;
    color=c;
    bevel=b;
}
void MenuImageLabel::Draw(unsigned index) {
    if(Dirty()) {
        Clean();
        if(bevel)MenuBevel(&g513888,1,0,x,y,width,height);
        Clip();
        if(image) {
            if(gMenuImages&&image>=0&&(U32)image<gMenuImages->count) {
                MenuImage *p=&gMenuImages->images[image];
                long left=x+(width-p->width)*(alignX+1)/2;
                long top=y+(height-p->height)*(alignY+1)/2;
                MenuDrawImage(&g513888,p,left,top);
            }
        }
        else {
            const char *s=text;
            if(!s)s=gGuiStringPool->GetString(gGuiStringBase+index,0);
            if(s) {
                if(color==11)MenuDrawWrapped(s,alignX,alignY,font,3,this,1,1);
                MenuDrawWrapped(s,alignX,alignY,font,color,this,0,0);
            }
        }
    }
}
MenuSpacer::MenuSpacer():GuiControl(32,0) {
}
void MenuSpacer::Draw(unsigned) {
    if(Dirty())Clean();
}
MenuPrefixButton::MenuPrefixButton(const char *s,signed char a,signed char c,signed char f):GuiControl(16,0),text(s),state(0),font(f),color(c),align(a),prefix(-1) {
}
void MenuPrefixButton::SetText(const char *s) {
    text=s;
    state&=~0x1000000UL;
}
void MenuPrefixButton::SetColor(signed char c) {
    color=c;
    state&=~0x1000000UL;
}
void MenuPrefixButton::SetAlign(signed char a) {
    align=a;
    state&=~0x1000000UL;
}
void MenuPrefixButton::SetPrefix(signed char p) {
    prefix=p;
    state&=~0x1000000UL;
}
void MenuPrefixButton::Reset(unsigned) {
    ResetCore();
    state=0;
}
void MenuPrefixButton::Draw(unsigned index) {
    U32 previous=state;
    char buffer[128];
    if(MenuControlChanged(this,index,&state)) {
        const char *s=text;
        if(!s)s=gGuiStringPool->GetString(gGuiStringBase+index,0);
        if(s) {
            if(prefix>=0) {
                const char *p=gMenuPrefixStrings.Prefix(prefix+1102);
                if(p) {
                    strcpy(buffer,p);
                    strcat(buffer,s);
                    s=buffer;
                }
            }
            GuiRect r=*this;
            --r.y;
            MenuDrawWrapped(s,font,-1,align,(state&0x480000UL)?15:color,&r);
            if((state&0x480000UL)&&!(previous&0x480000UL))SoundPlay(392,0);
            else if((state&0x20000UL)&&!(previous&0x20000UL))SoundPlay(5,0);
            Clip();
        }
    }
}
void MenuPrefixButton::SetFont(signed char f) {
    font=f;
    state&=~0x1000000UL;
}
MenuPanel::MenuPanel():GuiControl(32,0),text(0) {
}
void MenuPanel::Draw(unsigned index) {
    const char *s;
    if(Dirty()) {
        Clean();
        unsigned char font,color;
        color=18;
        font=2;
        MenuDrawPanel(&gMenuImages->images[0],this);
        Clip();
        s=text;
        if(!s)s=gGuiStringPool->GetString(gGuiStringBase+index,0);
        if(s)MenuDrawWrapped(s,-1,0,font,color,this);
    }
}
MenuNumber::MenuNumber(signed char f,signed char c):GuiControl(32,0),text(0),value(-1),font(f),color(c),align(1) {
}
void MenuNumber::SetValue(long n,signed char flashOnce) {
    value=n;
    flash=flashOnce;
    attributes&=~0x1000000UL;
}
void MenuNumber::SetFont(signed char f) {
    font=f;
}
void MenuNumber::Draw(unsigned index) {
    if(Dirty()) {
        Clean();
        MenuDrawPanel(&gMenuImages->images[9],this);
        Clip();
        const char *s=text;
        if(!s)s=gGuiStringPool->GetString(gGuiStringBase+index,0);
        if(s)MenuDrawWrapped(s,font,0,align,color,this);
        if(value>=0) {
            if(flash) {
                color=16;
                flash=0;
                attributes&=~0x1000000UL;
            }
            MenuDrawWrapped(MenuFormatNumber(value),1,0,align,color,this);
        }
    }
}
MenuChat::MenuChat(U32 length):GuiControl(3,0),state(0),visibleLength(length),sendTimer(0),blinkTimer(0),forceTimer(0) {
    font=4;
    scrollStart=0;
    maxPosition=46;
    gMenuHistory[2047]=0;
    visibleLength=48;
    memset(gMenuHistory,0,2048);
    memset(gMenuVisible,0,48);
    cursor=0;
    gMenuHistory[0]=26;
}
char *MenuChat::GetText() {
    return gMenuVisible;
}
void MenuChat::Reset(unsigned) {
    ResetCore();
    ClearCore();
}
void MenuChat::Clear() {
    ClearCore();
}
void MenuChat::Draw(unsigned index) {
    int blink=0;
    if(TimerExpired(&blinkTimer)) {
        TimerSet(&blinkTimer,500);
        blink=1;
    }
    if(!blink) {
        if(!gMenuFonts[font]||(state&1))return;
        state|=1;
    }
    MenuDrawPanel(&gMenuImages->images[9],this);
    Clip();
    const char *s=gMenuVisible;
    if(!s)s=gGuiStringPool->GetString(gGuiStringBase+index,0);
    if(s)MenuDrawWrapped(s,-1,0,font,15,this);
    char c=gMenuVisible[cursor];
    gMenuVisible[cursor]=0;
    long left=MenuTextWidth(gMenuFonts[font],gMenuVisible)+x+4;
    gMenuVisible[cursor]=c;
    if(blink) {
        if(caretColor==15)caretColor=3;
        else caretColor=15;
    }
    gMenuForeground=gMenuColors[caretColor];
    MenuLine(&g513888,left,y+15,left+7,y+15);
}
void MenuChat::Send() {
    if(pending) {
        if(gMenuNetwork.Count()>1) {
            MenuMessage message(0x1002,gMenuVisible);
            MenuSend(&message,0);
        }
        pending=0;
    }
}
int MenuChat::Event(unsigned,U32) {
    char *history=gMenuHistory;
    char *visible=gMenuVisible;
    if(TimerExpired(&sendTimer)) {
        SendCore();
        TimerSet(&sendTimer,200);
    }
    if(TimerExpired(&forceTimer)) {
        pending=1;
        SendCore();
        TimerSet(&forceTimer,2000);
    }
    int changed=0;
    U32 key=gIn80;
    U32 input=MenuInputChar();
    signed char c=(signed char)input;
    U32 length=strlen(gMenuHistory),oldLength=length;
    if(memchr(chatKeys,key,9)) {
        switch(key) {
            case 14:if(MenuControlKey()) {
                memset(gMenuHistory,0,2048);
                cursor=scrollStart=0;
                gMenuHistory[0]=26;
                pending=1;
                changed=1;
            }
            else if(cursor>0) {
                memmove(&gMenuHistory[cursor-1],&gMenuHistory[cursor],oldLength-cursor+1);
                --cursor;
                if(cursor<scrollStart)scrollStart=cursor;
                pending=1;
                changed=1;
            }
            break;
            case 71:scrollStart=cursor=0;
            gMenuHistory[0]=' ';
            pending=1;
            changed=1;
            break;
            case 79:cursor=oldLength;
            changed=1;
            pending=1;
            break;
            case 83:if(cursor>0) {
                memmove(&gMenuHistory[cursor],&gMenuHistory[cursor+1],oldLength-cursor);
                changed=1;
                pending=1;
            }
            break;
            case 82:gMenuChatInsert^=1;
            break;
            case 28:
            case 75:
            break;
        }
    }
    else if(!(((signed char)input>=0&&(signed char)input<=7)||(c>=9&&c<=31)||((int)c>=127&&(int)c<=144)||((int)c>=152&&(int)c<=182)||((int)c>=185&&(int)c<=190)||(int)c==197||(int)c==208||(int)c==215||(int)c==216||(int)c==222||(int)c==229||(int)c==247||(int)c==248||(int)c==254)) {
        U32 offset=cursor-scrollStart;
        if(offset>=visibleLength)++scrollStart;
        if(cursor>=2046) {
            memmove(history,history+1,2046);
            cursor=2045;
            scrollStart=cursor-visibleLength+1;
        }
        if(gMenuChatInsert)memmove(&history[cursor+1],history+cursor,length-cursor+1);
        else if(cursor==length)++length;
        gMenuHistory[cursor++]=c;
        changed=1;
        pending=1;
    }
    if(changed) {
        memmove(visible,history+scrollStart,visibleLength);
        gMenuVisible[visibleLength]=0;
        state&=~1UL;
    }
    return 0;
}
