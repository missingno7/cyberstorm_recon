// Working layout: Options is 66 bytes; constructor leaves header and tail untouched.
#pragma pack(push,2)
struct Options {
 int header, enabled, sound, difficulty, money, unknown14, unknown18, unknown1c;
 int unknown20, unknown24, unknown28, unknown2c, unknown30, unknown34;
 char tail[10];
 Options(); void Reset();
};
#pragma pack(pop)
extern Options defaultOptions;
Options::Options() {
 enabled=1; sound=1; difficulty=3; money=100000; unknown14=1;
 unknown18=0; unknown1c=0; unknown20=120; unknown24=4; unknown28=1;
 unknown2c=-1; unknown30=0; unknown34=0;
}
void Options::Reset() { *this=defaultOptions; }
