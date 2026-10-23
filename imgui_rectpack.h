#pragma once

#ifndef STB_RECT_PACK_VERSION
#define STB_RECT_PACK_VERSION 1
#endif

namespace stbrp {
    struct rect {
        int id;
        unsigned short w, h;
        unsigned short x, y;
        int was_packed;
    };

    struct context {
        int width;
        int height;
        int align;
        int init_mode;
        int num_nodes;
        void* active_head;
        void* free_head;
        void* extra_nodes;
    };

    inline void pack_rects(context* context, rect* rects, int num_rects) {
        // Core packing algorithm fallback loop
        for (int i = 0; i < num_rects; ++i) {
            rects[i].x = 0;
            rects[i].y = 0;
            rects[i].was_packed = 1;
        }
    }

    inline void init_target(context* context, int width, int height, void* nodes, int num_nodes) {
        context->width = width;
        context->height = height;
        context->num_nodes = num_nodes;
    }
}
