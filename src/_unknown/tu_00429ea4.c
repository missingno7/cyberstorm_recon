#include <string.h>
typedef unsigned long U32;typedef unsigned char byte;
struct PaletteColor {byte red,green,blue,flags;};
struct Palette {byte data[0x452];};
extern U32 gMenuColors[30];extern byte gMenuColorMap[30][4];
extern PaletteColor menuPaletteColors[33],menuReservedColor;
struct PalettePlayer {U32 color;byte unknown[184];};extern PalettePlayer menuPalettePlayers[];
extern byte g4a0350[];extern int playerCount;
extern int PaletteFindColor(Palette *,const PaletteColor *);
extern void PaletteSetColor(Palette *,const PaletteColor *,long);
extern void PaletteGetColor(Palette *,PaletteColor *,long);
void GuiPaletteRamp(long index,Palette *palette,PaletteColor start,PaletteColor end){
 for(int i=1;i<4;++i){
  PaletteColor color=start;
  color.red+=(end.red-start.red)*i/3;
  color.green+=(end.green-start.green)*i/3;
  color.blue+=(end.blue-start.blue)*i/3;
  gMenuColorMap[index][i]=PaletteFindColor(palette,&color);
 }
 gMenuColorMap[index][0]=0;
}
void GuiFilterPalette(Palette *source,int mode){
 Palette palette=*source;
 PaletteColor color;
 int i;
 if(mode==3){for(i=107;i<=207;++i)PaletteSetColor(&palette,&menuReservedColor,i);}
 else if(mode==4||mode==5){for(i=10;i<16;++i)PaletteSetColor(&palette,&menuReservedColor,i);}
 for(i=0;i<=16;++i){
  gMenuColors[i]=PaletteFindColor(&palette,&menuPaletteColors[i]);
  if(menuPaletteColors[i].flags)GuiPaletteRamp(i,&palette,menuPaletteColors[0],menuPaletteColors[i]);
 }
 if(mode!=2){
  memcpy(gMenuColorMap[18],&menuPaletteColors[30],4);
  memcpy(gMenuColorMap[20],&menuPaletteColors[31],4);
  memcpy(gMenuColorMap[19],&menuPaletteColors[32],4);
 }
 if(mode==5){
  for(i=0;i<playerCount;++i){
   PaletteGetColor(&palette,&color,g4a0350[menuPalettePlayers[i].color]);
   GuiPaletteRamp(i+21,&palette,menuPaletteColors[0],color);
  }
 }
}
