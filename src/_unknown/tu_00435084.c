/* Full behavioral reconstruction of an apparent assembly-origin object.
 * The original ENTER/packed-byte loops have not been reproduced by C++. */
typedef unsigned char U8;
typedef unsigned long U32;
extern int maskStarts[4],maskRowSteps[4];
void MaskTerrainTile(U8 *destination,U8 *source,U8 *mask,int sourceDirection,int maskDirection) {
    int direction=(sourceDirection^maskDirection)&3;
    mask+=maskStarts[direction];int stride=maskRowSteps[direction];
    if(!(direction&1)) {
        for(int y=0;y<64;++y) {
            for(int chunk=0;chunk<8;++chunk) {
                U8 bits=*mask;
                if(!bits) {destination+=8;source+=8;}
                else if(bits==255) {
                    *(U32 *)destination=*(U32 *)source;
                    *(U32 *)(destination+4)=*(U32 *)(source+4);
                    destination+=8;source+=8;
                } else {
                    for(int x=0;x<8;++x) {
                        if(bits&1) *destination=*source;
                        bits>>=1;++destination;++source;
                    }
                }
                ++mask;
            }
            mask+=stride;
        }
    } else {
        for(int y=0;y<64;++y) {
            for(int chunk=0;chunk<8;++chunk) {
                U8 bits=*mask;
                if(!bits) {destination+=8;source+=8;}
                else if(bits==255) {
                    *(U32 *)destination=*(U32 *)source;
                    *(U32 *)(destination+4)=*(U32 *)(source+4);
                    destination+=8;source+=8;
                } else {
                    for(int x=0;x<8;++x) {
                        if(bits&128) *destination=*source;
                        bits<<=1;++destination;++source;
                    }
                }
                --mask;
            }
            mask+=stride;
        }
    }
}


