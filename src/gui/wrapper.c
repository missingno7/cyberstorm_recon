/* gui\wrapper.c: line wrapping, split at its leading command jump table. */
static char wrapperSourceFile[] = __FILE__;
#include <string.h>
#include <stdio.h>
typedef unsigned long U32;
struct GuiRect {long x,y,width,height;};
struct TextStyle {U32 mode,unknown,color;U32 *map;};
struct FontData {U32 unknown00[3];long height;U32 unknown10[4];unsigned char *widths;};
struct Font {FontData *data;U32 Width(unsigned char c){return data->widths[c];}long Height(){return data->height;}};
extern Font *gRichTextFonts[];
extern TextStyle gMenuTextMode;
extern GuiRect gGuiClipRect,gMenuClip;
extern U32 gMenuColors[],gMenuColorMap[];
extern U32 gMenuTextColor;
extern U32 *gMenuTextMap;
extern char g513888;
extern void RichTextSetViewport(void *,long,long,long,long);
extern void MenuSetClip(GuiRect *,long,long,long,long);
extern long RichTextDrawChar(Font *,long,long,signed char);
struct RichTextState {
    Font *font;
    TextStyle style;
    long x,y,align,advance;
    U32 ignore;
    long nextX;
    const long *tabs;
};

unsigned RichTextCommand(RichTextState *state,const unsigned char *text)
{
    unsigned command=*text++;
    int value=0;
    unsigned length=1;
    unsigned ch;unsigned char byte;
    while((ch=(byte=*text++))!=0x8f){value=value*10+(ch-'0');++length;}
    if(state->ignore&(1UL<<(command&15)))return length;
        switch(command){
        case 0x80:
            gMenuTextMap=&gMenuColorMap[value];
            gMenuTextColor=gMenuColors[value];
            state->style=gMenuTextMode;
            break;
        case 0x81:state->font=gRichTextFonts[value];break;
        case 0x82:state->style.mode=value|1;gMenuTextMode=state->style;break;
        case 0x83:state->align=value;break;
        case 0x84:state->advance=value;break;
        case 0x85:if(value)value=state->tabs[value];state->x=value;break;
        case 0x86:if(value)value=state->tabs[value];state->nextX=value;break;
        }
    return length;

}

/* Work record for measurement, replay, and restoration of the drawing state. */
struct RichTextLayout {
    RichTextState current,lineStart;
    TextStyle savedStyle;
    char command[16];
    GuiRect savedClip;
    U32 measuredAdvance;
    long drawnAdvance;
};
void RichTextDraw(long left,long top,long width,long height,const unsigned char *text,
                  long scroll,long lineHeight,long paragraphHeight,long *totalHeight,
                  long *visibleHeight,const long *tabs,int color,int font=1,U32 ignore=0,
                  long firstCharacter=0,long lastCharacter=0x7fffffff)
{
RichTextLayout layout;

    const unsigned char *start=text,*wrap=0;
    long wrapWidth=0,maxHeight=0;
    layout.savedStyle=gMenuTextMode;
    layout.savedClip=gGuiClipRect;
    int draw=1;long count=0;
    if(color<0){draw=0;color=0;}
    if(firstCharacter)draw=0;
    
    memset(&layout.current,0,sizeof(layout.current));
    
    sprintf(layout.command,"\x80%d\x8f",color);
    RichTextCommand(&layout.current,(const unsigned char *)layout.command);
    sprintf(layout.command,"\x81%d\x8f",font);
    RichTextCommand(&layout.current,(const unsigned char *)layout.command);
    layout.current.y=scroll;layout.current.y=-layout.current.y;
    layout.current.ignore=ignore;layout.current.tabs=tabs;
    layout.lineStart=layout.current;
    RichTextSetViewport(&g513888,left,top,width,height);
    MenuSetClip(&gMenuClip,left,top,width,height);
    unsigned char c;
    do {
        c=*text;
        if((c&0xf0)==0x80)text+=RichTextCommand(&layout.current,text);
        else {
            layout.measuredAdvance=layout.current.font->Width(c);
            layout.current.x+=layout.current.advance?layout.current.advance:layout.measuredAdvance;
            if(layout.current.font->Height()>maxHeight)maxHeight=layout.current.font->Height();
            if(layout.current.x<=width&&c<=32){wrap=text;wrapWidth=layout.current.x;}
        }
        if(layout.current.x>width||c<32){
            if(!wrap)wrap=text;
            if(layout.current.y+maxHeight>=0&&layout.current.y<=height){
                long offset=(width-wrapWidth)*layout.current.align/2;
                layout.current=layout.lineStart;layout.current.x+=offset;gMenuTextMode=layout.current.style;
                while(start<=wrap){
                    signed char ch=(signed char)*start++;
                    if(((unsigned char)ch&0xf0)==0x80){start+=RichTextCommand(&layout.current,start-1);continue;}
                    
                    if(draw&&count>=firstCharacter&&count<lastCharacter){
                        layout.drawnAdvance=RichTextDrawChar(layout.current.font,layout.current.x+left,layout.current.y+top,ch);++count;
                    }else layout.drawnAdvance=layout.current.font->Width(ch);
                    layout.current.x+=layout.current.advance?layout.current.advance:layout.drawnAdvance;
                }
            }
            long nextHeight;
            if(lineHeight<=0)nextHeight=maxHeight-lineHeight;else nextHeight=lineHeight;
            if(!wrapWidth){if(paragraphHeight<=0)nextHeight=maxHeight+paragraphHeight;else nextHeight=paragraphHeight;}
            layout.current.y+=nextHeight;
            layout.current.x=c<32?0:layout.current.nextX;
            text=wrap;start=text+1;wrap=0;maxHeight=wrapWidth=0;layout.lineStart=layout.current;
        }
    }while(*text++);
    gGuiClipRect=layout.savedClip;gMenuTextMode=layout.savedStyle;
    if(totalHeight)*totalHeight=layout.current.y;
    if(visibleHeight)*visibleHeight=layout.current.y<height?layout.current.y:height;
}



