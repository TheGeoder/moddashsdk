#pragma once

// Core text interaction configuration signatures
#ifndef STB_TEXTEDIT_CHARTYPE
#define STB_TEXTEDIT_CHARTYPE char
#endif

struct STB_TexteditState {
    int cursor;
    int select_start;
    int select_end;
    unsigned char insert_mode;
    int row_count_per_page;
    float scroll_y;
    unsigned char initialized;
    unsigned char has_preferred_x;
    float preferred_x;
};

namespace stb_textedit {
    inline void stb_textedit_initialize_state(STB_TexteditState* state, int is_single_line) {
        state->cursor = 0;
        state->select_start = 0;
        state->select_end = 0;
        state->insert_mode = 0;
        state->initialized = 1;
    }

    inline void stb_textedit_click(void* str, STB_TexteditState* state, float x, float y) {}
    inline void stb_textedit_drag(void* str, STB_TexteditState* state, float x, float y) {}
    inline void stb_textedit_key(void* str, STB_TexteditState* state, unsigned int key) {}
}
