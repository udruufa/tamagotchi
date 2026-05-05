#include <stdlib.h>
#include <stdio.h>
#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_rtc.h>
#include <gui/gui.h>
#include <gui/elements.h>
#include <input/input.h>
#include <storage/storage.h>
#include <storage/filesystem_api_defines.h>
#include <notification/notification_messages.h>
#include <tamagochi_icons.h>

bool hello_flag = true;

int sec = 0;
int dolphin = 0;
int select = 1;

bool eat_flag;
bool pet_flag;

int y = 16;

uint32_t start;
int32_t seconds_passed_here;

typedef struct {
    int32_t health;
    int32_t hunger;
    uint32_t last_save_time;
} TamagochiState;

TamagochiState game_state;

typedef enum {
    EventTypeTick,
    EventTypeInput,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} TamagochiEvent;

static void draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);

    const char* name = furi_hal_version_get_ble_local_device_name_ptr();
    char* space = strchr(name, ' ');
    name = space + 1;

    canvas_clear(canvas);

    if(hello_flag) {
        canvas_clear(canvas);
        if(sec == 0)
            canvas_draw_icon(canvas, 0, 0, &I_h0);
        else if(sec == 1)
            canvas_draw_icon(canvas, 0, 0, &I_h1);
        else if(sec == 2)
            canvas_draw_icon(canvas, 0, 0, &I_h2);
        else if(sec == 3)
            canvas_draw_icon(canvas, 0, 0, &I_h3);
        else if(sec == 4)
            canvas_draw_icon(canvas, 0, 0, &I_h4);
        else if(sec == 5)
            canvas_draw_icon(canvas, 0, 0, &I_h5);
        else if(sec == 6)
            canvas_draw_icon(canvas, 0, 0, &I_h6);
        else if(sec == 7)
            canvas_draw_icon(canvas, 0, 0, &I_h7);
        else if(sec == 8)
            canvas_draw_icon(canvas, 0, 0, &I_h8);
        else if(sec == 9)
            canvas_draw_icon(canvas, 0, 0, &I_h9);
        else if(sec == 10)
            canvas_draw_icon(canvas, 0, 0, &I_h10);
        else if(sec == 11)
            canvas_draw_icon(canvas, 0, 0, &I_h11);
        else if(sec == 12) {
            canvas_draw_icon(canvas, 0, 0, &I_h12);
            hello_flag = false;
            canvas_clear(canvas);
        }
    }

    if(hello_flag == false) {
        char str[16];
        uint32_t now = furi_hal_rtc_get_timestamp();
        seconds_passed_here = now - start;
        snprintf(str, sizeof(str), "%ld", seconds_passed_here);
        canvas_draw_str(canvas, 16, 20, str);

        if(sec % 4 == 0 || sec == 0) {
            if(dolphin == 0) {
                canvas_draw_icon(canvas, 50, 20, &I_dolph_s_2);
            } else if(dolphin == 1) {
                canvas_draw_icon(canvas, 50, 14, &I_dolph_m_2);
            } else if(dolphin == 2) {
                canvas_draw_icon(canvas, 38, 6, &I_dolph_l_2);
            }
        } else if(sec % 2 == 0) {
            if(dolphin == 0) {
                canvas_draw_icon(canvas, 51, 20, &I_dolph_s_0);
            } else if(dolphin == 1) {
                canvas_draw_icon(canvas, 50, 14, &I_dolph_m_0);
            } else if(dolphin == 2) {
                canvas_draw_icon(canvas, 42, 6, &I_dolph_l_0);
            }
        } else {
            if(dolphin == 0) {
                canvas_draw_icon(canvas, 50, 20, &I_dolph_s_1);
            } else if(dolphin == 1) {
                canvas_draw_icon(canvas, 50, 14, &I_dolph_m_1);
            } else if(dolphin == 2) {
                canvas_draw_icon(canvas, 42, 6, &I_dolph_l_1);
            }
        }

        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 2, 9, name);
        canvas_draw_icon(canvas, 86, 2, &I_states);

        // if(game_state.health <= 10 && game_state.health >= 0)
        canvas_draw_box(canvas, 95 + (10 - game_state.health) * 3, 3, game_state.health * 3, 4);
        // if(game_state.hunger <= 10 && game_state.hunger >= 0)
        canvas_draw_box(canvas, 95 + (10 - game_state.hunger) * 3, 10, game_state.hunger * 3, 4);

        canvas_draw_icon(canvas, 8, 50, &I_eat);
        canvas_draw_icon(canvas, 28, 50, &I_pet);
        canvas_draw_icon(canvas, 48, 50, &I_poop);
        canvas_draw_icon(canvas, 68, 50, &I_sleep);
        canvas_draw_icon(canvas, 88, 50, &I_heal);
        canvas_draw_icon(canvas, 108, 50, &I_info);

        if(select == 1)
            canvas_draw_icon(canvas, 7, 49, &I_eat_pressed);
        else if(select == 2)
            canvas_draw_icon(canvas, 27, 49, &I_pet_pressed);
        else if(select == 3)
            canvas_draw_icon(canvas, 47, 49, &I_poop_pressed);
        else if(select == 4)
            canvas_draw_icon(canvas, 67, 49, &I_sleep_pressed);
        else if(select == 5)
            canvas_draw_icon(canvas, 87, 49, &I_heal_pressed);
        else if(select == 6)
            canvas_draw_icon(canvas, 107, 49, &I_info_pressed);

        canvas_set_font(canvas, FontSecondary);
        if(eat_flag) {
            canvas_clear(canvas);

            elements_multiline_text_aligned(canvas, 48, 16, AlignLeft, AlignTop, "Pumpkin");
            elements_multiline_text_aligned(canvas, 48, 28, AlignLeft, AlignTop, "Pumpkin 2");
            elements_multiline_text_aligned(canvas, 48, 40, AlignLeft, AlignTop, "Pumpkin 3");

            canvas_set_color(canvas, ColorBlack);
            canvas_draw_box(canvas, 0, y - 2, 128, 12);

            canvas_invert_color(canvas);

            if(y == 16)
                elements_multiline_text_aligned(canvas, 48, 16, AlignLeft, AlignTop, "Pumpkin");
            else if(y == 28)
                elements_multiline_text_aligned(canvas, 48, 28, AlignLeft, AlignTop, "Pumpkin 2");
            else if(y == 40)
                elements_multiline_text_aligned(canvas, 48, 40, AlignLeft, AlignTop, "Pumpkin 3");

            elements_multiline_text_aligned(canvas, 44, y, AlignRight, AlignTop, ">");
        }
    }
}

static void input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    FuriMessageQueue* event_queue = ctx;

    TamagochiEvent event = {.type = EventTypeInput, .input = *input_event};
    furi_message_queue_put(event_queue, &event, FuriWaitForever);
}

static void timer_callback(void* ctx) {
    furi_assert(ctx);
    FuriMessageQueue* event_queue = ctx;

    TamagochiEvent event = {.type = EventTypeTick};
    furi_message_queue_put(event_queue, &event, 0);
}

bool save_file_exists(void) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    File* file = storage_file_alloc(storage);

    bool exists = storage_file_open(
        file, "/ext/apps_data/tamagochi/save.dat", FSAM_READ, FSOM_OPEN_EXISTING);

    if(exists) {
        storage_file_close(file);
    }

    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);

    return exists;
}

void save_game() {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    File* file = storage_file_alloc(storage);

    if(storage_file_open(
           file, "/ext/apps_data/tamagochi/save.dat", FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        storage_file_write(file, &game_state, sizeof(TamagochiState));
        storage_file_close(file);
    }

    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
}

void load_game() {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    File* file = storage_file_alloc(storage);

    start = furi_hal_rtc_get_timestamp();

    if(storage_file_open(
           file, "/ext/apps_data/tamagochi/save.dat", FSAM_READ, FSOM_OPEN_EXISTING)) {
        storage_file_read(file, &game_state, sizeof(TamagochiState));
        storage_file_close(file);
    }

    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
}

void init_new_game() {
    game_state.health = 10;
    game_state.hunger = 10;
    game_state.last_save_time = furi_hal_rtc_get_timestamp();
}

int32_t tamagochi_app(void* p) {
    UNUSED(p);

    bool hunger_plus = false;
    bool health_plus = false;

    TamagochiEvent event;
    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(TamagochiEvent));

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, draw_callback, NULL);
    view_port_input_callback_set(view_port, input_callback, event_queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    load_game();
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    FuriTimer* timer = furi_timer_alloc(timer_callback, FuriTimerTypePeriodic, event_queue);
    furi_timer_start(timer, 300);

    if(!save_file_exists()) init_new_game();

    uint32_t now = furi_hal_rtc_get_timestamp();
    int32_t seconds_passed = now - game_state.last_save_time;
    if(game_state.health > 0) game_state.health -= seconds_passed / 60;
    if(game_state.hunger > 0) game_state.hunger -= seconds_passed / 30;

    while(1) {
        furi_check(furi_message_queue_get(event_queue, &event, FuriWaitForever) == FuriStatusOk);

        if(game_state.health > 0 && seconds_passed_here % 60 == 0 && seconds_passed_here >= 60 &&
           health_plus == false) {
            game_state.health--;
            health_plus = true;
        }
        if(game_state.hunger > 0 && seconds_passed_here % 30 == 0 && seconds_passed_here >= 30 &&
           hunger_plus == false) {
            game_state.hunger--;
            hunger_plus = true;
        }
        if(seconds_passed_here % 30 != 0) {
            health_plus = false;
            hunger_plus = false;
        }

        if(event.type == EventTypeInput) {
            if(event.input.key == InputKeyBack && event.input.type == InputTypePress) {
                if(eat_flag)
                    eat_flag = false;
                else {
                    game_state.last_save_time = furi_hal_rtc_get_timestamp();
                    save_game();
                    break;
                }
            } else if(event.input.key == InputKeyUp && event.input.type == InputTypePress) {
                if(dolphin < 2) dolphin++;
                if(eat_flag) y -= 12;
            } else if(event.input.key == InputKeyDown && event.input.type == InputTypePress) {
                if(dolphin > 0) dolphin--;
                if(eat_flag) y += 12;
            } else if(event.input.key == InputKeyRight && event.input.type == InputTypePress) {
                if(select == 6) select = 0;
                select++;
            } else if(event.input.key == InputKeyLeft && event.input.type == InputTypePress) {
                if(select == 1) select = 7;
                select--;
            } else if(event.input.key == InputKeyOk && event.input.type == InputTypePress) {
                if(eat_flag) {
                    eat_flag = false;
                    if(game_state.hunger < 10) game_state.hunger++;
                } else if(select == 1)
                    eat_flag = true;
                else if(select == 2)
                    // pet_flag = true;
                    if(game_state.health < 10) game_state.health++;
            }
        } else if(event.type == EventTypeTick) {
            sec++;
        }
    }

    furi_timer_free(timer);

    furi_message_queue_free(event_queue);

    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close(RECORD_GUI);

    return 0;
}
