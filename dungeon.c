#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_W 80
#define MAP_H 25

#define MAX_ROOMS     20
#define ROOM_MIN_SIZE 4
#define ROOM_MAX_SIZE 10

typedef enum {
    TILE_WALL,
    TILE_FLOOR
} TileType;

typedef struct {
    int x, y;   // top-left
    int w, h;   // width, height
} Room;

TileType map[MAP_H][MAP_W];
Room rooms[MAX_ROOMS];
int room_count = 0;

void init_map(void) {
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            map[y][x] = TILE_WALL;
        }
    }
}

int rects_intersect(Room a, Room b) {
    // Add a 1-tile margin so rooms aren't touching
    if (a.x - 1 > b.x + b.w) return 0;
    if (b.x - 1 > a.x + a.w) return 0;
    if (a.y - 1 > b.y + b.h) return 0;
    if (b.y - 1 > a.y + a.h) return 0;
    return 1;
}

void carve_room(Room room) {
    for (int y = room.y; y < room.y + room.h; y++) {
        for (int x = room.x; x < room.x + room.w; x++) {
            if (x > 0 && x < MAP_W && y > 0 && y < MAP_H) {
                map[y][x] = TILE_FLOOR;
            }
        }
    }
}

void carve_horz_corridor(int x1, int x2, int y) {
    if (x2 < x1) { int tmp = x1; x1 = x2; x2 = tmp; }
    for (int x = x1; x <= x2; x++) {
        if (x >= 0 && x < MAP_W && y >= 0 && y < MAP_H) {
            map[y][x] = TILE_FLOOR;
        }
    }
}

void carve_vert_corridor(int y1, int y2, int x) {
    if (y2 < y1) { int tmp = y1; y1 = y2; y2 = tmp; }
    for (int y = y1; y <= y2; y++) {
        if (x >= 0 && x < MAP_W && y >= 0 && y < MAP_H) {
            map[y][x] = TILE_FLOOR;
        }
    }
}

void generate_dungeon(void) {
    init_map();
    room_count = 0;

    for (int i = 0; i < MAX_ROOMS; i++) {
        Room new_room;

        new_room.w = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        new_room.h = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);

        new_room.x = rand() % (MAP_W - new_room.w - 1);
        new_room.y = rand() % (MAP_H - new_room.h - 1);

        int failed = 0;
        for (int j = 0; j < room_count; j++) {
            if (rects_intersect(new_room, rooms[j])) {
                failed = 1;
                break;
            }
        }
        if (failed) continue;

        carve_room(new_room);

        int center_x = new_room.x + new_room.w / 2;
        int center_y = new_room.y + new_room.h / 2;

        if (room_count > 0) {
            // Try to connect to previous room
            Room prev = rooms[room_count - 1];
            int prev_center_x = prev.x + prev.w / 2;
            int prev_center_y = prev.y + prev.h / 2;

            if (rand() % 2) {
                carve_horz_corridor(prev_center_x, center_x, prev_center_y);
                carve_vert_corridor(prev_center_y, center_y, center_x);
            } else {
                carve_vert_corridor(prev_center_y, center_y, prev_center_x);
                carve_horz_corridor(prev_center_x, center_x, center_y);
            }
        }

        rooms[room_count++] = new_room;
    }
}

void print_map(void) {
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            char c = '#';
            if (map[y][x] == TILE_FLOOR) {
                c = '.';
                printf("%c", c);
            } else {
                printf("\033[36m%c\033[0m", c);
            }
        }
        putchar('\n');
    }
}

int main(void) {
    srand((unsigned)time(NULL));

    generate_dungeon();
    print_map();

    return 0;
}

