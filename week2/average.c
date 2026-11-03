#include <stdio.h>
#include <stddef.h>

#define WINDOW 5

typedef struct {
    float data[WINDOW];
    size_t count;
    size_t next;
} Ring;

static void ring_push(Ring *r, float v) {
    r->data[r->next] = v;
    r->next = (r->next + 1) % WINDOW;
    if (r->count < WINDOW) {
        r->count++;
    }
}

static float ring_average(const Ring *r) {
    if (r->count == 0) {
        return 0.0f;
    }
    float sum = 0.0f;

    for (size_t i=0; i < r->count; i++) {
        sum += r->data[i];
    }
    return sum / (float)r->count;
}

static float fake_read(void) {
    static int step = 0;
    step++;
    return 20.0f + (float)(step % 7) * 0.5f;
}

int main(void) {
    Ring ring = {{0.0f}, 0, 0};
    for (int i=0; i < 12; i++) {
        float v = fake_read();
        ring_push(&ring, v);
        printf("read %5.2f   count=%zu   average=%.2f\n",
        v, ring.count, ring_average(&ring));
    }

    return 0;
}