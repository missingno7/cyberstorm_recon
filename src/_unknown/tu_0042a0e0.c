/* Palette fades used by the shared GUI; original source filename unknown. */
typedef unsigned long U32;
struct PaletteColor {unsigned char red,green,blue,flags;};
struct Palette {PaletteColor entries[256];unsigned char unknown[82];};
struct Timer;extern void TimerSet(Timer *,U32);extern int TimerExpired(Timer *);
struct Timer {U32 start,end,duration;Timer(U32 n){TimerSet(this,n);}};
extern U32 TimeNow();
extern void HelpClearTopic();
extern long gGuiEffectPriority;
extern Palette gGuiPalette,gGuiFadePalette;
extern long gGuiFadeStep,gGuiFadeFirst,gGuiFadeLast;
extern int gGuiPaletteFaded,gGuiPartialFade;
extern U32 gGuiFadeColor;
extern char g513888;
extern void GuiPumpMessages();
extern void SurfacePresent(void *);
extern void PaletteFade(Palette *,int=0,int=256,int=256,const U32 * =0);
extern void ApplyPaletteData(void *,int=0,int=256);

void GuiFadeOut()
{
    HelpClearTopic();
    if(gGuiEffectPriority>=40){
        Palette palette=gGuiPalette;
        if(gGuiFadeStep<2)gGuiFadeStep=2;
        Timer timer(0);
        U32 start=TimeNow();
        for(int amount=0;amount<256;amount+=gGuiFadeStep){
            do{GuiPumpMessages();}while(!TimerExpired(&timer));
            TimerSet(&timer,50);
            gGuiFadePalette=palette;
            PaletteFade(&gGuiFadePalette,0,256,amount,&gGuiFadeColor);
            ApplyPaletteData(&gGuiFadePalette);
        }
        PaletteFade(&palette,0,256,256,&gGuiFadeColor);
        ApplyPaletteData(&palette);
        U32 elapsed=TimeNow()-start;
        if(elapsed>250)gGuiFadeStep+=2;else if(elapsed<250)gGuiFadeStep-=2;
        gGuiPaletteFaded=1;
    }
}
void GuiFadeOutRange(int first,int last)
{
    Palette palette=gGuiPalette;
    if(gGuiFadeStep<2)gGuiFadeStep=2;
    Timer timer(0);
    U32 start=TimeNow();
    for(int amount=0;amount<256;amount+=gGuiFadeStep){
        do{GuiPumpMessages();}while(!TimerExpired(&timer));
        TimerSet(&timer,50);
        gGuiFadePalette=palette;
        PaletteFade(&gGuiFadePalette,first,last-first,amount,&gGuiFadeColor);
        ApplyPaletteData(&gGuiFadePalette);
    }
    PaletteFade(&palette,first,last-first,256,&gGuiFadeColor);
    ApplyPaletteData(&palette);
    U32 elapsed=TimeNow()-start;
    if(elapsed>250)gGuiFadeStep+=2;else if(elapsed<250)gGuiFadeStep-=2;
    gGuiPaletteFaded=1;gGuiPartialFade=1;gGuiFadeFirst=first;gGuiFadeLast=last;
}
void GuiFadeIn()
{
    Palette palette=gGuiPalette;
    if(gGuiFadeStep<2)gGuiFadeStep=2;
    Timer timer(0);
    U32 start=TimeNow(),finish;
    if(gGuiPartialFade){
        for(int amount=256;amount>=0;amount-=gGuiFadeStep){
            do{GuiPumpMessages();}while(!TimerExpired(&timer));
            TimerSet(&timer,50);
            gGuiFadePalette=palette;
            PaletteFade(&gGuiFadePalette,gGuiFadeFirst,gGuiFadeLast-gGuiFadeFirst,amount,&gGuiFadeColor);
            ApplyPaletteData(&gGuiFadePalette);
            if(amount==256)SurfacePresent(&g513888);
        }
        PaletteFade(&palette,gGuiFadeFirst,gGuiFadeLast-gGuiFadeFirst,0,&gGuiFadeColor);
        ApplyPaletteData(&palette);
        finish=TimeNow();gGuiPartialFade=0;
    }else{
        for(int amount=256;amount>=0;amount-=gGuiFadeStep){
            do{GuiPumpMessages();}while(!TimerExpired(&timer));
            TimerSet(&timer,50);
            gGuiFadePalette=palette;
            PaletteFade(&gGuiFadePalette,0,256,amount,&gGuiFadeColor);
            ApplyPaletteData(&gGuiFadePalette);
            if(amount==256)SurfacePresent(&g513888);
        }
        PaletteFade(&palette,0,256,0,&gGuiFadeColor);
        ApplyPaletteData(&palette);finish=TimeNow();
    }
    U32 elapsed=finish-start;
    if(elapsed>250)gGuiFadeStep+=2;else if(elapsed<250)gGuiFadeStep-=2;
    gGuiPaletteFaded=0;
}
