inline const char *PromoRoomName() { return "promo"; }
/* Promotion/campaign narrative screen, including the destructor COMDAT at 0040be9c. All names except shared GUI classes are
 * working labels. The packed arrays follow the original unaligned member offsets. */
#include <string.h>
typedef unsigned long U32;
typedef unsigned char byte;
struct GuiControl; struct GuiPoint; struct InputReq; struct Palette; struct Surface;
struct Timer; void TimerSet(Timer *,U32); int TimerExpired(Timer *);
struct Timer { U32 start,end,duration; Timer(U32 n){TimerSet(this,n);} };
struct StrPool { int Lock(); void Unlock(); char *GetString(long,int=1); };
#pragma pack(push,1)
class GuiScreen { public: byte header[16]; InputReq *input; byte rest[56]; int field4c, finished;
    GuiScreen(GuiControl **,unsigned,U32); ~GuiScreen(){input=0;} void LoadLayout(long); void Open(InputReq *,StrPool *,long,int);
    int IsActive() const {return input != 0;} long HitTest(const GuiPoint *); int Update(); void Draw();
};
struct FileRecord { U32 handle,open,rest[5]; U32 IsOpen() {return open;} };
struct FlicSound;
struct Flic { byte header[32]; FileRecord file; Palette *palette; U32 fields40[3]; long loops; byte tail[64];
    int IsOpen(){return file.IsOpen();} void SetLoops(long n){loops=n;} Palette *GetPalette(){return palette;} Flic(); ~Flic(); int Open(long,int=100,int=0); void Close(); void ApplyPalette(); void SetSounds(FlicSound *); void StopSounds(); U32 Update(Surface *);
};
class PromoScreen : public GuiScreen { public:
    byte speed; int field55; int x[4],y[4],width[4],height[4]; int field99;
    Timer timer; Flic animation;
    int field139,field13d,field141,field145,done,field14d; U32 page,visible,previous,delay,rank; int advance;
    char *text[4]; unsigned short field179[4],field181[4]; int color[4],font[4];
    PromoScreen(); ~PromoScreen() {} void Open(InputReq *,int); void SetPage(); int NextPage(); void SetLayout();
    void Play(const char *); void PlayOnce(const char *,FlicSound *); void DrawText(); void Close(); int Update(); void Render(); void Finish();
};
struct PlayerInfo { byte rank,field1,field2,previousRank; byte rest[44]; };
struct ProfileManager;
struct MenuButton { byte pad[20]; U32 attributes; byte tail[12]; MenuButton(const char *,U32); virtual ~MenuButton(); };
struct MenuPrefixButton { byte pad[20]; U32 attributes; byte tail[16]; MenuPrefixButton(const char *,signed char,signed char,signed char); virtual ~MenuPrefixButton(); void SetText(const char *); };
struct MenuHelpRegion { byte pad[28]; long id; int activated; MenuHelpRegion(); virtual ~MenuHelpRegion(); };
#pragma pack(pop)
extern GuiControl *promoControls[];
extern StrPool promoStrings, gShellStrings;
extern ProfileManager profileManager;
extern MenuPrefixButton promoButton; extern MenuButton promoNumber;
extern long gGuiEffectPriority;
extern int promoStatus, promoPlaying;
extern byte promoFinished;
extern int g4bb78c, g511948, g513888, g4b74f4;
extern "C" void BriefCloseAudio();
void ShellSoundShutdown();
void GuiFilterPalette(Palette *,int);
void SigsStart(void *,byte);
void SurfacePresent(void *);
extern "C" void BriefCloseWait();
extern "C" int BriefDone(const char *);
extern "C" void HercSetMissionStart(void *,int);
void GetPlayerInfo(void *,PlayerInfo *);
extern "C" int BriefRender(int,int,int,int,const char *,int,int,int,int *,int *,int,int,int,int,int,int);
extern "C" void LogicCallMode(int,void *);

PromoScreen::PromoScreen() : GuiScreen(promoControls,3,0), timer(0), animation() {}

/* 004094b9 */
void PromoScreen::Open(InputReq *input, int number) {
    if (!IsActive()) {
        promoStrings.Lock();
        BriefCloseAudio();
        promoStatus=0xc1d;
        done=0; field55=0; promoPlaying=0; promoFinished=0;
        field14d=0; field139=0; visible=0; previous=0;
        LoadLayout((long)"promo.box");
        GuiScreen::Open(input,&gShellStrings,3001,1);
        SetLayout(); promoButton.SetText(" ");
        promoNumber.attributes=(promoNumber.attributes & 0xff1fffffUL)+0xa00000;
        speed=12-gGuiEffectPriority/10;
        delay=100;
        PlayerInfo info; GetPlayerInfo(&profileManager,&info);
        rank=info.rank;
        if (number==999) {
            if (rank==0) page=0;
            else { page=info.previousRank+1; advance=1; }
        } else if (number==99) {page=rank;advance=0;}
        else {advance=0;page=number;}
        if (rank==14) {advance=0;page=rank;}
        SetPage();
    }
}

extern FlicSound promoSounds_0048f6a8[];
extern FlicSound promoSounds_0048f6c8[];
extern FlicSound promoSounds_0048f6f8[];
extern FlicSound promoSounds_0048f728[];
extern FlicSound promoSounds_0048f800[];
extern FlicSound promoSounds_0048f818[];
extern FlicSound promoSounds_0048f838[];
extern FlicSound promoSounds_0048f8a8[];
extern FlicSound promoSounds_0048f8c8[];
extern FlicSound promoSounds_0048f8e8[];
extern FlicSound promoSounds_0048f920[];
extern FlicSound promoSounds_0048f940[];
extern FlicSound promoSounds_0048f9a8[];
extern FlicSound promoSounds_0048f9c8[];
extern FlicSound promoSounds_0048fa10[];
extern FlicSound promoSounds_0048fa30[];
extern FlicSound promoSounds_0048fa68[];
extern FlicSound promoSounds_0048fa88[];
extern FlicSound promoSounds_0048faa8[];
extern FlicSound promoSounds_0048faf0[];
extern FlicSound promoSounds_0048fb20[];
extern FlicSound promoSounds_0048fb40[];
extern FlicSound promoSounds_0048fb90[];
extern FlicSound promoSounds_0048fbb0[];
extern FlicSound promoSounds_0048fbd0[];
extern FlicSound promoSounds_0048fbf8[];
extern FlicSound promoSounds_0048fc18[];
extern FlicSound promoSounds_0048fc38[];
extern FlicSound promoSounds_0048fc58[];
extern FlicSound promoSounds_0048fca0[];
extern FlicSound promoSounds_0048fcb0[];
extern FlicSound promoSounds_0048ff68[];
extern FlicSound promoSounds_00490108[];
extern FlicSound promoSounds_00490138[];
extern FlicSound promoSounds_00490178[];

extern int promoChanged;
void PromoScreen::SetPage() {
    for(int i=0;i<4;++i) {text[i]=0;field179[i]=0;field181[i]=0;color[i]=15;font[i]=1;}
    SetLayout(); field4c=1;
    switch(page) {
    case 0:
        field4c=0;
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=400;
            text[0]=promoStrings.GetString(1);
            text[1]=promoStrings.GetString(2);
            Play("Promo1.flx");
            animation.SetSounds(promoSounds_0048f6a8);
            ++field14d;
            break;
        case 1:
            x[0]=10;
            y[0]=75;
            width[0]=400;
            height[0]=125;
            x[1]=10;
            y[1]=320;
            width[1]=400;
            height[1]=100;
            x[2]=10;
            y[2]=200;
            width[2]=400;
            height[2]=120;
            text[0]=promoStrings.GetString(3);
            text[1]=promoStrings.GetString(5);
            text[2]=promoStrings.GetString(4);
            animation.StopSounds();
            Play("Protos.flx");
            animation.SetSounds(promoSounds_0048f6c8);
            ++field14d;
            break;
        case 2:
            x[0]=10;
            y[0]=10;
            width[0]=590;
            height[0]=140;
            x[1]=10;
            y[1]=300;
            width[1]=600;
            height[1]=30;
            x[2]=10;
            y[2]=334;
            width[2]=312;
            height[2]=133;
            x[3]=320;
            y[3]=334;
            width[3]=315;
            height[3]=133;
            text[0]=promoStrings.GetString(6);
            text[1]=promoStrings.GetString(7);
            text[2]=promoStrings.GetString(8);
            text[3]=promoStrings.GetString(9);
            animation.StopSounds();
            Play("rem_shad.flx");
            animation.SetSounds(promoSounds_0048f6f8);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 1:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=280;
            width[1]=300;
            height[1]=150;
            x[2]=10;
            y[2]=75;
            width[2]=300;
            height[2]=200;
            text[0]=promoStrings.GetString(10);
            text[1]=promoStrings.GetString(12);
            text[2]=promoStrings.GetString(11);
            Play("promo1.flx");
            animation.SetSounds(promoSounds_0048f6a8);
            ++field14d;
            break;
        case 1:
            x[1]=10;
            y[1]=115;
            width[1]=300;
            height[1]=140;
            x[2]=10;
            y[2]=255;
            width[2]=300;
            height[2]=150;
            text[0]=promoStrings.GetString(13);
            text[1]=promoStrings.GetString(14);
            text[2]=promoStrings.GetString(15);
            animation.StopSounds();
            Play("bgms.flx");
            animation.SetSounds(promoSounds_0048f728);
            ++field14d;
            break;
        case 2:
            x[0]=10;
            y[0]=10;
            width[0]=590;
            height[0]=140;
            x[2]=430;
            y[2]=350;
            width[2]=209;
            height[2]=119;
            x[1]=220;
            y[1]=350;
            width[1]=210;
            height[1]=119;
            x[3]=10;
            y[3]=350;
            width[3]=210;
            height[3]=119;
            text[0]=promoStrings.GetString(16);
            text[1]=promoStrings.GetString(18);
            text[2]=promoStrings.GetString(19);
            text[3]=promoStrings.GetString(17);
            animation.StopSounds();
            Play("ogre_etc.flx");
            animation.SetSounds(promoSounds_0048f800);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 2:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=125;
            x[2]=10;
            y[2]=205;
            width[2]=300;
            height[2]=263;
            text[0]=promoStrings.GetString(20);
            text[1]=promoStrings.GetString(21);
            text[2]=promoStrings.GetString(22);
            Play("promo2.flx");
            animation.SetSounds(promoSounds_0048f818);
            ++field14d;
            break;
        case 1:
            x[1]=10;
            y[1]=90;
            width[1]=300;
            height[1]=120;
            x[2]=10;
            y[2]=210;
            width[2]=300;
            height[2]=220;
            text[0]=promoStrings.GetString(23);
            text[1]=promoStrings.GetString(24);
            text[2]=promoStrings.GetString(25);
            animation.StopSounds();
            Play("wm6a.flx");
            animation.SetSounds(promoSounds_0048f838);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 3:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=90;
            x[2]=10;
            y[2]=170;
            width[2]=300;
            height[2]=290;
            text[0]=promoStrings.GetString(26);
            text[1]=promoStrings.GetString(27);
            text[2]=promoStrings.GetString(28);
            Play("promo3.flx");
            animation.SetSounds(promoSounds_0048f8a8);
            ++field14d;
            break;
        case 1:
            x[0]=280;
            y[0]=10;
            width[0]=350;
            height[0]=100;
            x[1]=10;
            y[1]=330;
            width[1]=600;
            height[1]=40;
            x[2]=10;
            y[2]=375;
            width[2]=600;
            height[2]=85;
            text[0]=promoStrings.GetString(29);
            text[1]=promoStrings.GetString(30);
            text[2]=promoStrings.GetString(31);
            animation.StopSounds();
            Play("defender.flx");
            animation.SetSounds(promoSounds_0048f8c8);
            ++field14d;
            break;
        case 2:
            text[0]=promoStrings.GetString(32);
            animation.StopSounds();
            Play("om4a.flx");
            animation.SetSounds(promoSounds_0048f8e8);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 4:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=120;
            x[2]=10;
            y[2]=192;
            width[2]=300;
            height[2]=269;
            text[0]=promoStrings.GetString(33);
            text[1]=promoStrings.GetString(34);
            text[2]=promoStrings.GetString(35);
            Play("promo4.flx");
            animation.SetSounds(promoSounds_0048f920);
            ++field14d;
            break;
        case 1:
            text[0]=promoStrings.GetString(36);
            animation.StopSounds();
            Play("monk.flx");
            animation.SetSounds(promoSounds_0048f940);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 5:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=85;
            x[2]=10;
            y[2]=160;
            width[2]=300;
            height[2]=275;
            text[0]=promoStrings.GetString(37);
            text[1]=promoStrings.GetString(38);
            text[2]=promoStrings.GetString(39);
            animation.StopSounds();
            Play("promo5.flx");
            animation.SetSounds(promoSounds_0048f9a8);
            ++field14d;
            break;
        case 1:
            text[0]=promoStrings.GetString(40);
            animation.StopSounds();
            Play("wm9a.flx");
            animation.SetSounds(promoSounds_0048f9c8);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 6:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=65;
            width[1]=300;
            height[1]=115;
            x[2]=10;
            y[2]=180;
            width[2]=300;
            height[2]=285;
            text[0]=promoStrings.GetString(41);
            text[1]=promoStrings.GetString(42);
            text[2]=promoStrings.GetString(43);
            Play("promo6.flx");
            animation.SetSounds(promoSounds_0048fa10);
            ++field14d;
            break;
        case 1:
            x[0]=280;
            y[0]=10;
            width[0]=340;
            height[0]=100;
            x[1]=10;
            y[1]=400;
            width[1]=600;
            height[1]=60;
            x[2]=10;
            y[2]=320;
            width[2]=600;
            height[2]=80;
            text[0]=promoStrings.GetString(44);
            text[1]=promoStrings.GetString(46);
            text[2]=promoStrings.GetString(45);
            animation.StopSounds();
            Play("destroy.flx");
            animation.SetSounds(promoSounds_0048fa30);
            ++field14d;
            break;
        case 2:
            x[0]=10;
            y[0]=10;
            width[0]=300;
            height[0]=80;
            x[2]=10;
            y[2]=90;
            width[2]=300;
            height[2]=205;
            x[1]=10;
            y[1]=295;
            width[1]=300;
            height[1]=100;
            text[0]=promoStrings.GetString(47);
            text[1]=promoStrings.GetString(49);
            text[2]=promoStrings.GetString(48);
            animation.StopSounds();
            Play("om3a.flx");
            animation.SetSounds(promoSounds_0048fa68);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 7:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=75;
            x[2]=10;
            y[2]=160;
            width[2]=300;
            height[2]=180;
            text[0]=promoStrings.GetString(50);
            text[1]=promoStrings.GetString(51);
            text[2]=promoStrings.GetString(52);
            Play("promo7.flx");
            animation.SetSounds(promoSounds_0048fa88);
            ++field14d;
            break;
        case 1:
            x[1]=10;
            y[1]=250;
            width[1]=300;
            height[1]=150;
            x[2]=10;
            y[2]=90;
            width[2]=300;
            height[2]=165;
            text[0]=promoStrings.GetString(53);
            text[1]=promoStrings.GetString(55);
            text[2]=promoStrings.GetString(54);
            animation.StopSounds();
            Play("wf1a.flx");
            animation.SetSounds(promoSounds_0048faa8);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 8:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=75;
            x[2]=10;
            y[2]=150;
            width[2]=300;
            height[2]=225;
            text[0]=promoStrings.GetString(56);
            text[1]=promoStrings.GetString(57);
            text[2]=promoStrings.GetString(58);
            Play("promo8.flx");
            ++field14d;
            break;
        case 1:
            text[0]=promoStrings.GetString(59);
            animation.StopSounds();
            Play("wm8a.flx");
            animation.SetSounds(promoSounds_0048faf0);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 9:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=85;
            x[2]=10;
            y[2]=160;
            width[2]=300;
            height[2]=280;
            text[0]=promoStrings.GetString(60);
            text[1]=promoStrings.GetString(61);
            text[2]=promoStrings.GetString(62);
            Play("promo9.flx");
            animation.SetSounds(promoSounds_0048fb20);
            ++field14d;
            break;
        case 1:
            x[0]=280;
            y[0]=8;
            width[0]=340;
            height[0]=100;
            x[1]=10;
            y[1]=400;
            width[1]=600;
            height[1]=60;
            x[2]=10;
            y[2]=320;
            width[2]=600;
            height[2]=70;
            text[0]=promoStrings.GetString(63);
            text[1]=promoStrings.GetString(65);
            text[2]=promoStrings.GetString(64);
            animation.StopSounds();
            Play("juggern.flx");
            animation.SetSounds(promoSounds_0048fb40);
            ++field14d;
            break;
        case 2:
            x[1]=10;
            y[1]=170;
            width[1]=300;
            height[1]=200;
            text[0]=promoStrings.GetString(66);
            text[1]=promoStrings.GetString(67);
            animation.StopSounds();
            Play("brain2.flx");
            animation.SetSounds(promoSounds_0048fb90);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 10:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=75;
            x[2]=10;
            y[2]=150;
            width[2]=300;
            height[2]=200;
            text[0]=promoStrings.GetString(68);
            text[1]=promoStrings.GetString(69);
            text[2]=promoStrings.GetString(70);
            Play("promo10.flx");
            animation.SetSounds(promoSounds_0048fbb0);
            ++field14d;
            break;
        case 1:
            x[1]=10;
            y[1]=350;
            width[1]=300;
            height[1]=50;
            x[2]=10;
            y[2]=110;
            width[2]=300;
            height[2]=240;
            text[0]=promoStrings.GetString(71);
            text[1]=promoStrings.GetString(73);
            text[2]=promoStrings.GetString(72);
            animation.StopSounds();
            Play("wm7a.flx");
            animation.SetSounds(promoSounds_0048fbd0);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 11:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=75;
            x[2]=10;
            y[2]=160;
            width[2]=300;
            height[2]=200;
            text[0]=promoStrings.GetString(74);
            text[1]=promoStrings.GetString(75);
            text[2]=promoStrings.GetString(76);
            animation.StopSounds();
            Play("promo11.flx");
            animation.SetSounds(promoSounds_0048fbf8);
            ++field14d;
            break;
        case 1:
            x[2]=10;
            y[2]=75;
            width[2]=300;
            height[2]=200;
            x[1]=10;
            y[1]=275;
            width[1]=300;
            height[1]=175;
            text[0]=promoStrings.GetString(77);
            text[1]=promoStrings.GetString(79);
            text[2]=promoStrings.GetString(78);
            animation.StopSounds();
            Play("baby.flx");
            animation.SetSounds(promoSounds_0048fc18);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 12:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=75;
            width[1]=300;
            height[1]=80;
            x[3]=10;
            y[3]=153;
            width[3]=300;
            height[3]=260;
            x[2]=10;
            y[2]=413;
            width[2]=300;
            height[2]=59;
            text[0]=promoStrings.GetString(80);
            text[1]=promoStrings.GetString(81);
            text[2]=promoStrings.GetString(83);
            text[3]=promoStrings.GetString(82);
            Play("promo12.flx");
            animation.SetSounds(promoSounds_0048fc38);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 13:
        switch(field14d) {
        case 0:
            x[1]=10;
            y[1]=95;
            width[1]=300;
            height[1]=120;
            x[2]=10;
            y[2]=215;
            width[2]=300;
            height[2]=115;
            x[3]=10;
            y[3]=327;
            width[3]=302;
            height[3]=135;
            text[0]=promoStrings.GetString(84);
            text[1]=promoStrings.GetString(85);
            text[2]=promoStrings.GetString(86);
            text[3]=promoStrings.GetString(87);
            Play("promo13.flx");
            animation.SetSounds(promoSounds_0048fc58);
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    case 14:
        switch(field14d) {
        case 0:
            PlayOnce("winninga.flx",promoSounds_0048ff68);
            ++field14d;
            break;
        case 1:
            x[0]=10;
            y[0]=10;
            width[0]=240;
            height[0]=450;
            text[0]=promoStrings.GetString(88);
            Play("winningb.flx");
            animation.SetSounds(promoSounds_00490108);
            ++field14d;
            break;
        case 2:
            PlayOnce("winningc.flx",promoSounds_00490138);
            ++field14d;
            break;
        case 3:
            x[0]=10;
            y[0]=10;
            width[0]=320;
            height[0]=450;
            text[0]=promoStrings.GetString(89);
            Play("winningd.flx");
            animation.SetSounds(promoSounds_00490178);
            ++field14d;
            break;
        default:
            done=1;
            field55=1;
            promoChanged=1;
            break;
        } break;
    case 1000:
        switch(field14d) {
        case 0:
            x[0]=10;
            y[0]=10;
            width[0]=590;
            height[0]=450;
            text[0]=promoStrings.GetString(90);
            Play("loser4b.flx");
            animation.SetSounds(promoSounds_0048fca0);
            ++field14d;
            break;
        case 1:
            PlayOnce("loser4c.flx",promoSounds_0048fcb0);
            ++field14d;
            break;
        default:
            done=1;
            field55=1;
            promoChanged=1;
            break;
        } break;
    case 1001:
        switch(field14d) {
        case 0:
            x[0]=10;
            y[0]=10;
            width[0]=590;
            height[0]=450;
            text[0]=promoStrings.GetString(91);
            Play("loser4b.flx");
            animation.SetSounds(promoSounds_0048fca0);
            ++field14d;
            break;
        case 1:
            PlayOnce("loser4c.flx",promoSounds_0048fcb0);
            ++field14d;
            break;
        default:
            done=1;
            field55=1;
            promoChanged=1;
            break;
        } break;
    case 1002:
        switch(field14d) {
        case 0:
            x[0]=10;
            y[0]=10;
            width[0]=590;
            height[0]=450;
            text[0]=promoStrings.GetString(92);
            Play("loser4b.flx");
            animation.SetSounds(promoSounds_0048fca0);
            ++field14d;
            break;
        case 1:
            PlayOnce("loser4c.flx",promoSounds_0048fcb0);
            ++field14d;
            break;
        default:
            done=1;
            field55=1;
            promoChanged=1;
            break;
        } break;
    case 1003:
        switch(field14d) {
        case 0:
            x[0]=10;
            y[0]=10;
            width[0]=590;
            height[0]=450;
            text[0]=promoStrings.GetString(93);
            Play("loser4b.flx");
            animation.SetSounds(promoSounds_0048fca0);
            ++field14d;
            break;
        case 1:
            PlayOnce("loser4c.flx",promoSounds_0048fcb0);
            ++field14d;
            break;
        default:
            done=1;
            field55=1;
            promoChanged=1;
            break;
        } break;
    case 1004:
        switch(field14d) {
        case 0:
            x[0]=10;
            y[0]=10;
            width[0]=590;
            height[0]=450;
            text[0]=promoStrings.GetString(94);
            Play("loser4b.flx");
            animation.SetSounds(promoSounds_0048fca0);
            ++field14d;
            break;
        case 1:
            PlayOnce("loser4c.flx",promoSounds_0048fcb0);
            ++field14d;
            break;
        default:
            done=1;
            field55=1;
            promoChanged=1;
            break;
        } break;
    case 2000:
        switch(field14d) {
        case 0:
            PlayOnce("credits.flx",0);
            ++field14d;
            break;
        default:
            done=1;
            promoChanged=1;
            break;
        } break;
    case 2200:
        switch(field14d) {
        case 0:
            x[0]=40;
            y[0]=50;
            width[0]=550;
            height[0]=100;
            x[1]=40;
            y[1]=150;
            width[1]=550;
            height[1]=75;
            x[2]=40;
            y[2]=225;
            width[2]=550;
            height[2]=110;
            x[3]=40;
            y[3]=355;
            width[3]=550;
            height[3]=75;
            text[0]=promoStrings.GetString(95);
            text[1]=promoStrings.GetString(96);
            text[2]=promoStrings.GetString(97);
            text[3]=promoStrings.GetString(98);
            Play("tutor3j.flx");
            ++field14d;
            break;
        case 1:
            x[0]=30;
            y[0]=150;
            width[0]=550;
            height[0]=75;
            x[1]=30;
            y[1]=250;
            width[1]=550;
            height[1]=75;
            x[2]=30;
            y[2]=350;
            width[2]=550;
            height[2]=75;
            x[3]=30;
            y[3]=375;
            width[3]=550;
            height[3]=75;
            text[0]=promoStrings.GetString(99);
            text[1]=promoStrings.GetString(100);
            text[2]=promoStrings.GetString(101);
            Play("tutor3a.flx");
            ++field14d;
            break;
        case 2:
            x[0]=30;
            y[0]=215;
            width[0]=550;
            height[0]=130;
            x[1]=30;
            y[1]=350;
            width[1]=550;
            height[1]=75;
            text[0]=promoStrings.GetString(102);
            text[1]=promoStrings.GetString(103);
            Play("tutor3b.flx");
            ++field14d;
            break;
        case 3:
            x[0]=35;
            y[0]=50;
            width[0]=510;
            height[0]=200;
            x[1]=35;
            y[1]=120;
            width[1]=510;
            height[1]=150;
            x[2]=40;
            y[2]=150;
            width[2]=510;
            height[2]=50;
            x[3]=40;
            y[3]=200;
            width[3]=510;
            height[3]=50;
            text[0]=promoStrings.GetString(104);
            text[1]=promoStrings.GetString(105);
            Play("tutor3c.flx");
            ++field14d;
            break;
        case 4:
            x[0]=165;
            y[0]=30;
            width[0]=450;
            height[0]=80;
            x[1]=10;
            y[1]=160;
            width[1]=450;
            height[1]=80;
            x[2]=10;
            y[2]=250;
            width[2]=450;
            height[2]=80;
            x[3]=165;
            y[3]=350;
            width[3]=450;
            height[3]=90;
            text[0]=promoStrings.GetString(106);
            text[1]=promoStrings.GetString(107);
            text[2]=promoStrings.GetString(108);
            text[3]=promoStrings.GetString(109);
            Play("tutor3d.flx");
            ++field14d;
            break;
        case 5:
            x[0]=10;
            y[0]=10;
            width[0]=560;
            height[0]=50;
            x[1]=165;
            y[1]=160;
            width[1]=450;
            height[1]=80;
            x[2]=10;
            y[2]=75;
            width[2]=450;
            height[2]=70;
            x[3]=10;
            y[3]=350;
            width[3]=450;
            height[3]=80;
            text[0]=promoStrings.GetString(110);
            text[1]=promoStrings.GetString(111);
            text[2]=promoStrings.GetString(112);
            text[3]=promoStrings.GetString(113);
            Play("tutor3e.flx");
            ++field14d;
            break;
        case 6:
            x[0]=165;
            y[0]=170;
            width[0]=310;
            height[0]=280;
            text[0]=promoStrings.GetString(114);
            Play("tutor3f.flx");
            ++field14d;
            break;
        case 7:
            x[0]=165;
            y[0]=40;
            width[0]=450;
            height[0]=80;
            x[1]=30;
            y[1]=210;
            width[1]=450;
            height[1]=70;
            x[2]=165;
            y[2]=360;
            width[2]=450;
            height[2]=80;
            x[3]=40;
            y[3]=10;
            width[3]=335;
            height[3]=50;
            text[0]=promoStrings.GetString(115);
            text[1]=promoStrings.GetString(116);
            text[2]=promoStrings.GetString(117);
            Play("tutor3g.flx");
            ++field14d;
            break;
        case 8:
            x[0]=30;
            y[0]=70;
            width[0]=450;
            height[0]=70;
            x[1]=150;
            y[1]=200;
            width[1]=450;
            height[1]=50;
            x[2]=30;
            y[2]=340;
            width[2]=450;
            height[2]=80;
            text[0]=promoStrings.GetString(118);
            text[2]=promoStrings.GetString(119);
            text[1]=promoStrings.GetString(120);
            Play("tutor3h.flx");
            ++field14d;
            break;
        case 9:
            x[0]=150;
            y[0]=120;
            width[0]=450;
            height[0]=70;
            x[1]=30;
            y[1]=290;
            width[1]=450;
            height[1]=100;
            text[0]=promoStrings.GetString(121);
            text[1]=promoStrings.GetString(122);
            Play("tutor3i.flx");
            ++field14d;
            break;
        default:
            done=1;
            break;
        } break;
    default: done=1;promoChanged=1;break;
    }
}


/* 0040b8f6 */
int PromoScreen::NextPage() {
    visible=0; speed=12-gGuiEffectPriority/10;
    SetPage();
    if (done) {
        if (advance) {
            if (rank <= page) return 0;
            field14d=0;field139=0;done=0;++page; SetPage(); return 1;
        } else {
            if (!field55) return 0;
            field14d=0;field139=0;done=0;field55=0;page=2000; SetPage(); return 1;
        }
    }
    return 1;
}

/* 0040b9b2 */
void PromoScreen::SetLayout() {
    x[0]=10;y[0]=10;width[0]=270;height[0]=460;
    x[1]=12;y[1]=10;width[1]=298;height[1]=461;
    x[2]=12;y[2]=10;width[2]=298;height[2]=461;
    x[3]=12;y[3]=10;width[3]=298;height[3]=461;
}

/* 0040ba39 */
void PromoScreen::Play(const char *name) {
    if (name) {
        if (animation.IsOpen()) animation.Close();
        animation.Open((long)name);
        if (strcmp(name,"winningb.flx") && strcmp(name,"winningd.flx")) {BriefCloseWait();BriefCloseAudio();}
        if (animation.Update((Surface *)&g511948)&0x20) { animation.ApplyPalette(); GuiFilterPalette(animation.GetPalette(),2); }
        if (strcmp(name,"loser4c.flx")) animation.SetLoops(-1);
        finished=0;DrawText();
    }
}

/* 0040bb0e */
void PromoScreen::PlayOnce(const char *name, FlicSound *sounds) {
    if (animation.IsOpen()) animation.Close();
    animation.Open((long)name);
    if (sounds) animation.SetSounds(sounds);
    animation.SetLoops(1);
    if (strcmp(name,"winningc.flx")) BriefCloseAudio();
    if (animation.Update((Surface *)&g511948)&0x20) {
        if (!strcmp(name,"winningc.flx")) {
            SigsStart(&g513888,0);SurfacePresent(&g513888);animation.ApplyPalette();SurfacePresent(&g513888);
        } else {animation.ApplyPalette();GuiFilterPalette(animation.GetPalette(),2);}
    }
    if (!strcmp(name,"credits.flx")) HercSetMissionStart(&g4b74f4,0x979);
    promoPlaying=1;
}

/* 0040bc26 */
void PromoScreen::DrawText() {
    visible+=speed;
    int pending=0;
    int a,b;
    for (int i=0;i<4;++i) {
        if (text[i]) {
            if (strlen(text[i])>visible) pending=1;
            BriefRender(x[i]+1,y[i]+1,width[i],height[i],text[i],0,-1,0,&a,&b,0,0,font[i],0,0,visible);
            BriefRender(x[i],y[i],width[i],height[i],text[i],0,-1,0,&a,&b,0,color[i],font[i],0,0,visible);
        }
    }
    previous=visible;
    if (pending && TimerExpired(&timer)) {TimerSet(&timer,delay);LogicCallMode(0x186,0);}
    if (!pending) finished=1;
}

/* 0040bd73 */
void PromoScreen::Close() {
    if (IsActive()) {
        promoStrings.Unlock();ShellSoundShutdown();animation.Close();input=0;BriefCloseAudio();
    }
}

/* 0040bdb5 */
int PromoScreen::Update() {
    if (IsActive()) {
        if (promoFinished==1) {promoFinished=0;return 1;}
        if (HitTest((GuiPoint *)&g4bb78c)<0 && BriefDone(PromoRoomName())>=0) promoStatus=0xc1d;
        return GuiScreen::Update();
    }
    return 0;
}

/* 0040be1b */
void PromoScreen::Render() {
    if (IsActive()) {
        U32 flags=animation.Update((Surface *)&g511948);
        if (flags & 0x20) animation.ApplyPalette();
        if (promoPlaying && (flags & 4)) {promoFinished=1;promoPlaying=0;}
        DrawText();GuiScreen::Draw();
    }
}

/* 0040be8c */
void PromoScreen::Finish() {BriefDone(PromoRoomName());}




 

MenuButton promoNumber(0,0);
MenuPrefixButton promoButton(0,1,2,-1);
MenuHelpRegion promoHelp;
int promoPlaying; byte promoFinished;
PromoScreen g4afbfc;







static char promoSourceFile[] = __FILE__;
GuiControl *promoControls[3] = {
    (GuiControl *)&promoNumber,
    (GuiControl *)&promoHelp,
    (GuiControl *)&promoButton
};
