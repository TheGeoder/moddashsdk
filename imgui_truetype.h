#pragma once

struct stbtt_fontinfo {
    void* userdata;
    unsigned char* data;
    int fontstart;
    int numGlyphs;
    int loca, head, glyf, hmtx, kern, gpos, svg;
    int index_map;
    int indexToLocFormat;
};

struct stbtt_pack_context {
    void* user_alloc_context;
    void* pack_info;
    int width;
    int height;
    unsigned int stride_in_bytes;
    unsigned int padding;
    unsigned int skip_missing;
    unsigned int h_oversample, v_oversample;
    unsigned char* pixels;
    void* nodes;
};

struct stbtt_packedchar {
    unsigned short x0, y0, x1, y1;
    float xoff, yoff, xadvance;
    float xoff2, yoff2;
};

struct stbtt_pack_range {
    float font_size;
    int first_unicode_codename_in_range;
    int* array_of_unicode_codepoints;
    int num_chars;
    stbtt_packedchar* chardata_for_range;
    unsigned char h_oversample, v_oversample;
};

namespace stbtt {
    inline int InitFont(stbtt_fontinfo* info, const unsigned char* data, int offset) { return 1; }
    inline int PackBegin(stbtt_pack_context* spc, unsigned char* pixels, int width, int height, int stride_in_bytes, int padding, void* alloc_context) { return 1; }
    inline void PackEnd(stbtt_pack_context* spc) {}
    inline int PackFontRange(stbtt_pack_context* spc, const unsigned char* fontdata, int font_index, float size, int first_char, int num_chars, stbtt_packedchar* chardata) { return 1; }
}
