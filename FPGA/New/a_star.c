#include "a_star.h"

static const int dx[4] = {-1, 1, 0, 0};
static const int dy[4] = {0, 0, -1, 1};


static uint32_t g_cost[TAMANHO_MALHA_TOTAL];
static uint32_t f_cost[TAMANHO_MALHA_TOTAL];
static uint8_t parent_dir[TAMANHO_MALHA_TOTAL];
static uint8_t open_flag[TAMANHO_MALHA_TOTAL];
static uint8_t closed_flag[TAMANHO_MALHA_TOTAL];
static int32_t heap_index[TAMANHO_MALHA_TOTAL];
static int32_t heap_data[TAMANHO_MALHA_TOTAL];
static int heap_size;
static uint8_t path_rev[TAMANHO_MALHA_TOTAL];

/* ---------------- Heap ---------------- */
static inline void heap_swap(int i, int j) {
    int a = heap_data[i];
    int b = heap_data[j];
    heap_data[i] = b;
    heap_data[j] = a;
    heap_index[a] = j;
    heap_index[b] = i;
}
static void heapify_up(int idx) {
    while (idx > 0) {
        int p = (idx - 1) >> 1;
        if (f_cost[heap_data[idx]] < f_cost[heap_data[p]]) {
            heap_swap(idx, p);
            idx = p;
        } else break;
    }
}
static void heapify_down(int idx) {
    while (1) {
        int l = (idx << 1) + 1;
        int r = l + 1;
        int s = idx;
        if (l < heap_size && f_cost[heap_data[l]] < f_cost[heap_data[s]]) s = l;
        if (r < heap_size && f_cost[heap_data[r]] < f_cost[heap_data[s]]) s = r;
        if (s != idx) {
            heap_swap(idx, s);
            idx = s;
        } else break;
    }
}
static inline void heap_insert(int n) {
    heap_data[heap_size] = n;
    heap_index[n] = heap_size;
    heapify_up(heap_size++);
}
static inline int heap_extract_min(void) {
    if (!heap_size) return -1;
    int m = heap_data[0];
    heap_size--;
    if (heap_size) {
        heap_data[0] = heap_data[heap_size];
        heap_index[heap_data[0]] = 0;
        heapify_down(0);
    }
    heap_index[m] = -1;
    return m;
}

/* ---------------- Heurística ---------------- */
static inline uint32_t heuristica(int x1, int y1, int x2, int y2) {
    int dxh = x1 - x2; if (dxh < 0) dxh = -dxh;
    int dyh = y1 - y2; if (dyh < 0) dyh = -dyh;
    return (uint32_t)(dxh + dyh);
}

/* ---------------- A* ---------------- */
int aStar_direct(Mapa *mapa,
                 int sx, int sy,
                 int dx_t, int dy_t,
                 uint8_t *saida)
{
    const int start = IDX(sx, sy, TAMANHO_MALHA);
    const int destino = IDX(dx_t, dy_t, TAMANHO_MALHA);
    const int total = TAMANHO_MALHA_TOTAL;

    int i;
    for (i = 0; i < total; i++) {
        g_cost[i] = INF32;
        f_cost[i] = INF32;
        open_flag[i] = 0;
        closed_flag[i] = 0;
        heap_index[i] = -1;
        parent_dir[i] = 0xFF;
    }
    heap_size = 0;

    g_cost[start] = 0;
    f_cost[start] = heuristica(sx, sy, dx_t, dy_t);
    heap_insert(start);
    open_flag[start] = 1;

    while (heap_size > 0) {
        int cur = heap_extract_min();
        closed_flag[cur] = 1;

        if (cur == destino) break;

        int cx = cur / TAMANHO_MALHA;
        int cy = cur % TAMANHO_MALHA;
        uint32_t gcur = g_cost[cur];

        int d;
        for (d = 0; d < 4; d++) {
            int nx = cx + dx[d];
            int ny = cy + dy[d];
            if (nx < 0 || nx >= TAMANHO_MALHA || ny < 0 || ny >= TAMANHO_MALHA) continue;

            int nidx = IDX(nx, ny, TAMANHO_MALHA);
            uint8_t v = mapa->malha[nidx];
            if (v == DESCONHECIDO || v == OCUPADO) continue;
            if (closed_flag[nidx]) continue;

            uint32_t tent_g = gcur + 1;
            if (!open_flag[nidx] || tent_g < g_cost[nidx]) {
                parent_dir[nidx] = (uint8_t)d;
                g_cost[nidx] = tent_g;
                f_cost[nidx] = tent_g + heuristica(nx, ny, dx_t, dy_t);
                if (!open_flag[nidx]) {
                    heap_insert(nidx);
                    open_flag[nidx] = 1;
                } else {
                    int hi = heap_index[nidx];
                    if (hi >= 0) heapify_up(hi);
                }
            }
        }
    }

    if (parent_dir[destino] == 0xFF) {
        ((uint32_t*)saida)[0] = 0xFFFFFFFFU;
        return 0;
    }

    int cur = destino;
    int steps = 0;
    while (cur != start && steps < total) {
        uint8_t dir = parent_dir[cur];
        path_rev[steps++] = dir;
        int cx = cur / TAMANHO_MALHA;
        int cy = cur % TAMANHO_MALHA;
        cur = IDX(cx - dx[dir], cy - dy[dir], TAMANHO_MALHA);
    }

    ((uint32_t*)saida)[0] = (uint32_t)steps;
    uint8_t *out = (uint8_t*)(saida + 4);

    for (i = 0; i < steps; i++)
        out[i] = path_rev[steps - 1 - i];

    return 1;

}


