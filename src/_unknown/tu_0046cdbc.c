// ADPCM decoder at 0046cdbc..0046ceec. C++ semantic reconstruction of an assembly-origin object.
// The original ENTER / ES / GS save-and-restore sequence is not reproduced by ordinary C++.
extern unsigned int decodeRemaining,decodeOriginalCount;
extern unsigned short decodeIndexDelta[16],decodeStepSize[89];
struct LinkDecoderState {
    unsigned char *source;
    unsigned char *destination;
    unsigned char pad_08[8];
    unsigned int sample_number;
    int predictor;
    int difference;
    unsigned short encoded;
    unsigned short nibble;
    unsigned short step;
    unsigned short step_index;
    unsigned short format;
};
struct LinkDecoderOwner {
    static unsigned int __cdecl decode_link_samples(LinkDecoderState *, unsigned int);
};
unsigned int __cdecl LinkDecoderOwner::decode_link_samples(
    LinkDecoderState *state, unsigned int sample_count)
{
    unsigned char *source = state->source;
    unsigned char *destination = state->destination;
    unsigned int scaled_count = sample_count;
    decodeOriginalCount = sample_count;
    if ((short)state->format == 0x10)
        scaled_count >>= 1;
    decodeRemaining = scaled_count;
    do {
        unsigned int nibble;
        if ((state->sample_number & 1) == 0) {
            state->encoded = *source;
            source++;
            nibble = state->encoded & 0xf;
            state->nibble = (unsigned short)nibble;
        } else {
            nibble = (state->encoded >> 4) & 0xf;
            state->nibble = (unsigned short)nibble;
        }
        state->difference = 0;
        if (nibble & 4)
            state->difference += state->step;
        if (nibble & 2)
            state->difference += state->step >> 1;
        if (nibble & 1)
            state->difference += state->step >> 2;
        state->difference += state->step >> 3;
        if (nibble & 8)
            state->difference = -state->difference;
        state->predictor += state->difference;
        if (state->predictor >= 0x7fff)
            state->predictor = 0x7fff;
        if (state->predictor <= -0x8000)
            state->predictor = -0x8000;
        if ((short)state->format == 0x10) {
            *(short *)destination = (short)state->predictor;
            destination += 2;
        } else {
            *destination = (unsigned char)(state->predictor >> 8) ^ 0x80;
            destination++;
        }
        state->step_index = (unsigned short)(state->step_index +
            decodeIndexDelta[state->nibble]);
        if (state->step_index < 0x8000) {
            if (state->step_index > 0x58)
                state->step_index = 0x58;
        } else {
            state->step_index = 0;
        }
        state->sample_number++;
        state->step = decodeStepSize[state->step_index];
        decodeRemaining--;
    } while (decodeRemaining != 0);
    state->source = source;
    state->destination = destination;
    return decodeOriginalCount;
}
