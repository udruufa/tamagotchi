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
bool game_flag;
bool heal_flag;
bool info_flag;

int y;

int lvl_mid = 5;
int lvl_sen = 9;

uint32_t start;
int32_t seconds_passed_here;

typedef struct {
    int32_t health;
    int32_t happiness;
    int32_t hunger;
    int32_t exp;
    int32_t lvl;
    int32_t health_time;
    int32_t happiness_time;
    int32_t hunger_time;
    int32_t exp_time;
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

// GAME

bool game = true;
bool win = false;

int32_t section_x = 40;
int32_t section_y = 8;

int count = 0;

int count_40 = 0;
int count_57 = 0;
int count_74 = 0;
int count_8 = 0;
int count_25 = 0;
int count_42 = 0;
int count_left = 0;
int count_right = 0;

int count_40_rand = 0;
int count_57_rand = 0;
int count_74_rand = 0;
int count_8_rand = 0;
int count_25_rand = 0;
int count_42_rand = 0;
int count_left_rand = 0;
int count_right_rand = 0;

int rand_rows = 9;
int32_t arr_rand[9][2] =
    {{40, 8}, {57, 8}, {74, 8}, {40, 25}, {57, 25}, {74, 25}, {40, 42}, {57, 42}, {74, 42}};

int32_t arr_done[5][2];
int32_t arr_rand_done[4][2];

static void draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);

    const char* name = furi_hal_version_get_ble_local_device_name_ptr();
    char* space = strchr(name, ' ');
    name = space + 1;

    canvas_clear(canvas);

    // if(hello_flag) {
    //     canvas_clear(canvas);
    //     if(sec == 0)
    //         canvas_draw_icon(canvas, 0, 0, &I_h0);
    //     else if(sec == 1)
    //         canvas_draw_icon(canvas, 0, 0, &I_h1);
    //     else if(sec == 2)
    //         canvas_draw_icon(canvas, 0, 0, &I_h2);
    //     else if(sec == 3)
    //         canvas_draw_icon(canvas, 0, 0, &I_h3);
    //     else if(sec == 4)
    //         canvas_draw_icon(canvas, 0, 0, &I_h4);
    //     else if(sec == 5)
    //         canvas_draw_icon(canvas, 0, 0, &I_h5);
    //     else if(sec == 6)
    //         canvas_draw_icon(canvas, 0, 0, &I_h6);
    //     else if(sec == 7)
    //         canvas_draw_icon(canvas, 0, 0, &I_h7);
    //     else if(sec == 8)
    //         canvas_draw_icon(canvas, 0, 0, &I_h8);
    //     else if(sec == 9)
    //         canvas_draw_icon(canvas, 0, 0, &I_h9);
    //     else if(sec == 10)
    //         canvas_draw_icon(canvas, 0, 0, &I_h10);
    //     else if(sec == 11)
    //         canvas_draw_icon(canvas, 0, 0, &I_h11);
    //     else if(sec == 12) {
    //         canvas_draw_icon(canvas, 0, 0, &I_h12);
    //         hello_flag = false;
    //         canvas_clear(canvas);
    //     }
    // }

    // if(hello_flag == false) {
    char str[16];
    char str2[16];
    char str3[16];
    uint32_t now = furi_hal_rtc_get_timestamp();
    seconds_passed_here = now - start;

    if(sec % 4 == 0 || sec == 0) {
        if(game_state.lvl >= lvl_sen) {
            canvas_draw_icon(canvas, 38, 6, &I_dolph_l_2);
        } else if(game_state.lvl >= lvl_mid) {
            canvas_draw_icon(canvas, 50, 14, &I_dolph_m_2);
        } else {
            canvas_draw_icon(canvas, 50, 20, &I_dolph_s_2);
        }
    } else if(sec % 2 == 0) {
        if(game_state.lvl >= lvl_sen) {
            canvas_draw_icon(canvas, 42, 6, &I_dolph_l_0);
        } else if(game_state.lvl >= lvl_mid) {
            canvas_draw_icon(canvas, 50, 14, &I_dolph_m_0);
        } else {
            canvas_draw_icon(canvas, 51, 20, &I_dolph_s_0);
        }
    } else {
        if(game_state.lvl >= lvl_sen) {
            canvas_draw_icon(canvas, 42, 6, &I_dolph_l_1);
        } else if(game_state.lvl >= lvl_mid) {
            canvas_draw_icon(canvas, 50, 14, &I_dolph_m_1);
        } else {
            canvas_draw_icon(canvas, 50, 20, &I_dolph_s_1);
        }
    }

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 9, name);

    canvas_set_font(canvas, FontSecondary);
    if(game_state.exp > 10) game_state.lvl = floor(sqrt(game_state.exp / 10));
    canvas_draw_str(canvas, 4, 18, "lvl");
    snprintf(str3, sizeof(str3), "%ld", game_state.lvl);
    canvas_draw_str(canvas, 14, 18, str3);

    canvas_draw_icon(canvas, 86, 2, &I_states);

    canvas_draw_box(canvas, 95 + (30 - game_state.health), 3, game_state.health, 4);
    canvas_draw_box(canvas, 95 + (30 - game_state.happiness), 10, game_state.happiness, 4);
    canvas_draw_box(canvas, 95 + (30 - game_state.hunger), 17, game_state.hunger, 4);

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

        elements_multiline_text_aligned(canvas, 48, 8, AlignLeft, AlignTop, "Sardine");
        elements_multiline_text_aligned(canvas, 48, 20, AlignLeft, AlignTop, "Squid");
        elements_multiline_text_aligned(canvas, 48, 32, AlignLeft, AlignTop, "Mackerel");
        elements_multiline_text_aligned(canvas, 48, 48, AlignLeft, AlignTop, "Icefish");

        canvas_set_color(canvas, ColorBlack);
        canvas_draw_box(canvas, 0, y - 2, 128, 12);

        canvas_invert_color(canvas);

        if(y == 8)
            elements_multiline_text_aligned(canvas, 48, 8, AlignLeft, AlignTop, "Sardine");
        else if(y == 20)
            elements_multiline_text_aligned(canvas, 48, 20, AlignLeft, AlignTop, "Squid");
        else if(y == 32)
            elements_multiline_text_aligned(canvas, 48, 32, AlignLeft, AlignTop, "Mackerel");
        else if(y == 48)
            elements_multiline_text_aligned(canvas, 48, 48, AlignLeft, AlignTop, "Icefish");

        elements_multiline_text_aligned(canvas, 44, y, AlignRight, AlignTop, ">");
    }
    if(heal_flag) {
        canvas_clear(canvas);

        elements_multiline_text_aligned(canvas, 48, 16, AlignLeft, AlignTop, "Vitamins");
        elements_multiline_text_aligned(canvas, 48, 28, AlignLeft, AlignTop, "Pills");
        elements_multiline_text_aligned(canvas, 48, 40, AlignLeft, AlignTop, "Injection");

        canvas_set_color(canvas, ColorBlack);
        canvas_draw_box(canvas, 0, y - 2, 128, 12);

        canvas_invert_color(canvas);

        if(y == 16)
            elements_multiline_text_aligned(canvas, 48, 16, AlignLeft, AlignTop, "Vitamins");
        else if(y == 28)
            elements_multiline_text_aligned(canvas, 48, 28, AlignLeft, AlignTop, "Pills");
        else if(y == 40)
            elements_multiline_text_aligned(canvas, 48, 40, AlignLeft, AlignTop, "Injection");

        elements_multiline_text_aligned(canvas, 44, y, AlignRight, AlignTop, ">");
    }
    if(info_flag) {
        canvas_clear(canvas);

        canvas_draw_icon(canvas, 90, 2, &I_trarops);

        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str(canvas, 6, 14, name);
        canvas_draw_str(canvas, 46, 14, "| lvl");
        canvas_draw_str(canvas, 66, 14, str3);

        canvas_set_font(canvas, FontSecondary);

        canvas_draw_str(canvas, 8, 24, "exp:");
        snprintf(str2, sizeof(str2), "%ld", game_state.exp);
        canvas_draw_str(canvas, 28, 24, str2);
        canvas_draw_str(canvas, 46, 24, "/");
        snprintf(str, sizeof(str), "%d", (int)pow(game_state.lvl + 1, 2) * 10);
        canvas_draw_str(canvas, 54, 24, str);

        canvas_draw_str(canvas, 8, 40, "health:");
        snprintf(str2, sizeof(str2), "%ld", game_state.health);
        canvas_draw_str(canvas, 38, 40, str2);
        canvas_draw_str(canvas, 50, 40, "/30");

        canvas_draw_str(canvas, 8, 49, "happiness:");
        snprintf(str2, sizeof(str2), "%ld", game_state.happiness);
        canvas_draw_str(canvas, 52, 49, str2);
        canvas_draw_str(canvas, 64, 49, "/30");

        canvas_draw_str(canvas, 8, 58, "hunger:");
        snprintf(str2, sizeof(str2), "%ld", game_state.hunger);
        canvas_draw_str(canvas, 40, 58, str2);
        canvas_draw_str(canvas, 48, 58, "/30");

        // canvas_draw_str(canvas, 8, 32, "sec:");
        // snprintf(str, sizeof(str), "%ld", seconds_passed_here);
        // canvas_draw_str(canvas, 26, 32, str);
    }
    if(game_flag) {
        canvas_clear(canvas);

        canvas_draw_icon(canvas, 0, 0, &I_frame);

        canvas_draw_icon(canvas, section_x, section_y, &I_section);

        for(int i = 0; i < count; i++) {
            canvas_draw_icon(canvas, arr_done[i][0], arr_done[i][1], &I_x);
            canvas_draw_icon(canvas, arr_rand_done[i][0], arr_rand_done[i][1], &I_o);
        }

        if(count == 5 || count_40 == 3 || count_57 == 3 || count_74 == 3 || count_8 == 3 ||
           count_25 == 3 || count_42 == 3 || count_left == 3 || count_right == 3 ||
           count_40_rand == 3 || count_57_rand == 3 || count_74_rand == 3 || count_8_rand == 3 ||
           count_25_rand == 3 || count_42_rand == 3 || count_left_rand == 3 ||
           count_right_rand == 3) {
            canvas_draw_icon(canvas, 0, 0, &I_frame_end);
            game = false;
        }

        if(count_40 == 3 || count_57 == 3 || count_74 == 3 || count_8 == 3 || count_25 == 3 ||
           count_42 == 3 || count_left == 3 || count_right == 3) {
            canvas_set_font(canvas, FontPrimary);
            canvas_draw_str(canvas, 42, 36, "YOU WIN");
            win = true;
        } else if(
            count_40_rand == 3 || count_57_rand == 3 || count_74_rand == 3 || count_8_rand == 3 ||
            count_25_rand == 3 || count_42_rand == 3 || count_left_rand == 3 ||
            count_right_rand == 3) {
            canvas_set_font(canvas, FontPrimary);
            canvas_draw_str(canvas, 38, 36, "YOU LOSE");
        } else if(count == 5) {
            canvas_set_font(canvas, FontPrimary);
            canvas_draw_str(canvas, 42, 36, "THE END");
        }
    }
}
// }

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
    game_state.health = 30;
    game_state.happiness = 30;
    game_state.hunger = 30;
    game_state.exp = 0;
    game_state.lvl = 1;
    game_state.health_time = 0;
    game_state.happiness_time = 60;
    game_state.hunger_time = 30;
    game_state.exp_time = 30;
    game_state.last_save_time = furi_hal_rtc_get_timestamp();
}

int32_t tamagochi_app(void* p) {
    UNUSED(p);

    bool health_plus = false;
    bool happiness_plus = false;
    bool hunger_plus = false;
    bool exp_plus = false;

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
    if(game_state.happiness > 0)
        game_state.happiness -= seconds_passed / game_state.happiness_time;
    if(game_state.hunger > 0) game_state.hunger -= seconds_passed / game_state.hunger_time;

    while(1) {
        furi_check(furi_message_queue_get(event_queue, &event, FuriWaitForever) == FuriStatusOk);

        if(game_state.health > 0 && seconds_passed_here % game_state.health_time == 0 &&
           seconds_passed_here >= game_state.health_time && health_plus == false &&
           game_state.health_time != 0) {
            game_state.health--;
            health_plus = true;
        }
        if(game_state.happiness > 0 && seconds_passed_here % game_state.happiness_time == 0 &&
           seconds_passed_here >= game_state.happiness_time && happiness_plus == false) {
            game_state.happiness--;
            happiness_plus = true;
        }
        if(game_state.hunger > 0 && seconds_passed_here % game_state.hunger_time == 0 &&
           seconds_passed_here >= game_state.hunger_time && hunger_plus == false) {
            game_state.hunger--;
            hunger_plus = true;
        }
        if(seconds_passed_here % game_state.hunger_time != 0) {
            health_plus = false;
            happiness_plus = false;
            hunger_plus = false;
            exp_plus = false;
        }
        if(seconds_passed_here % game_state.exp_time == 0 && exp_plus == false) {
            game_state.exp++;
            exp_plus = true;
        }

        if(game_state.hunger == 0) {
            game_state.health_time = 30;
            game_state.happiness_time = 30;
        }
        if(game_state.happiness == 0) {
            game_state.health_time = 30;
        }

        if(event.type == EventTypeInput) {
            if(event.input.key == InputKeyBack && event.input.type == InputTypePress) {
                if(eat_flag)
                    eat_flag = false;
                else if(heal_flag)
                    heal_flag = false;
                else if(info_flag)
                    info_flag = false;
                else {
                    game_state.last_save_time = furi_hal_rtc_get_timestamp();
                    save_game();
                    break;
                }
            } else if(event.input.key == InputKeyUp && event.input.type == InputTypePress) {
                if(eat_flag) {
                    if(y == 48)
                        y -= 16;
                    else if(y > 8)
                        y -= 12;
                    else
                        y = 48;
                } else if(heal_flag) {
                    if(y > 16)
                        y -= 12;
                    else
                        y = 40;
                } else if(game_flag) {
                    section_y -= 17;
                }
            } else if(event.input.key == InputKeyDown && event.input.type == InputTypePress) {
                if(eat_flag) {
                    if(y == 32)
                        y += 16;
                    else if(y < 48)
                        y += 12;
                    else
                        y = 8;
                } else if(heal_flag) {
                    if(y < 40)
                        y += 12;
                    else
                        y = 16;
                } else if(game_flag) {
                    section_y += 17;
                }
            } else if(event.input.key == InputKeyRight && event.input.type == InputTypePress) {
                if(game_flag)
                    section_x += 17;
                else {
                    if(select == 6) select = 0;
                    select++;
                }
            } else if(event.input.key == InputKeyLeft && event.input.type == InputTypePress) {
                if(game_flag)
                    section_x -= 17;
                else {
                    if(select == 1) select = 7;
                    select--;
                }
            } else if(event.input.key == InputKeyOk && event.input.type == InputTypePress) {
                if(eat_flag) { //EAT
                    game_state.exp += 2;

                    eat_flag = false;
                    if(y == 8) { //Sardine
                        if(game_state.happiness + 1 < 30) //happiness +1
                            game_state.happiness += 1;
                        else
                            game_state.happiness = 30;

                        if(game_state.hunger + 2 < 30) //hunger +2
                            game_state.hunger += 2;
                        else
                            game_state.hunger = 30;
                    }
                    if(y == 20) { //Mackerel
                        if(game_state.hunger + 3 < 30) //hunger +3
                            game_state.hunger += 3;
                        else
                            game_state.hunger = 30;
                    }
                    if(y == 32) { //Squid
                        if(game_state.happiness + 2 < 30) //happiness +2
                            game_state.happiness += 2;
                        else
                            game_state.happiness = 30;

                        if(game_state.hunger + 1 < 30) //hunger +1
                            game_state.hunger += 1;
                        else
                            game_state.hunger = 30;
                    }
                    if(y == 48) { //Icefish
                        if(game_state.health - 2 > 0) //health -2
                            game_state.health -= 2;
                        else
                            game_state.health = 0;

                        if(game_state.happiness + 2 < 30) //happiness +2
                            game_state.happiness += 2;
                        else
                            game_state.happiness = 30;

                        if(game_state.hunger + 4 < 30) //hunger +4
                            game_state.hunger += 4;
                        else
                            game_state.hunger = 30;
                    }
                } else if(select == 1) {
                    eat_flag = true;
                    y = 8;
                }

                else if(select == 2) { //PET
                    game_state.exp += 1;

                    if(game_state.happiness + 1 < 30) //happiness +1
                        game_state.happiness += 1;
                    else
                        game_state.happiness = 30;
                }

                if(game_flag) { //PLAY
                    if(game) {
                        arr_done[count][0] = section_x;
                        arr_done[count][1] = section_y;

                        if(arr_done[count][0] == 40) count_40++;
                        if(arr_done[count][0] == 57) count_57++;
                        if(arr_done[count][0] == 74) count_74++;
                        if(arr_done[count][1] == 8) count_8++;
                        if(arr_done[count][1] == 25) count_25++;
                        if(arr_done[count][1] == 42) count_42++;
                        if(arr_done[count][0] == 40 && arr_done[count][1] == 8) count_left++;
                        if(arr_done[count][0] == 74 && arr_done[count][1] == 42) count_left++;
                        if(arr_done[count][0] == 74 && arr_done[count][1] == 8) count_right++;
                        if(arr_done[count][0] == 40 && arr_done[count][1] == 42) count_right++;
                        if(arr_done[count][0] == 57 && arr_done[count][1] == 25) {
                            count_left++;
                            count_right++;
                        }

                        if(count < 4) {
                            for(int i = 0; i < rand_rows; i++) {
                                if(arr_rand[i][0] == arr_done[count][0] &&
                                   arr_rand[i][1] == arr_done[count][1]) {
                                    for(int j = i; j < rand_rows - 1; j++) {
                                        arr_rand[j][0] = arr_rand[j + 1][0];
                                        arr_rand[j][1] = arr_rand[j + 1][1];
                                    }
                                    rand_rows--;
                                    break;
                                }
                            }

                            int row = rand() % rand_rows;
                            arr_rand_done[count][0] = arr_rand[row][0];
                            arr_rand_done[count][1] = arr_rand[row][1];

                            if(arr_rand_done[count][0] == 40) count_40_rand++;
                            if(arr_rand_done[count][0] == 57) count_57_rand++;
                            if(arr_rand_done[count][0] == 74) count_74_rand++;
                            if(arr_rand_done[count][1] == 8) count_8_rand++;
                            if(arr_rand_done[count][1] == 25) count_25_rand++;
                            if(arr_rand_done[count][1] == 42) count_42_rand++;
                            if(arr_rand_done[count][0] == 40 && arr_rand_done[count][1] == 8)
                                count_left_rand++;
                            if(arr_rand_done[count][0] == 74 && arr_rand_done[count][1] == 42)
                                count_left_rand++;
                            if(arr_rand_done[count][0] == 74 && arr_rand_done[count][1] == 8)
                                count_right_rand++;
                            if(arr_rand_done[count][0] == 40 && arr_rand_done[count][1] == 42)
                                count_right_rand++;
                            if(arr_rand_done[count][0] == 57 && arr_rand_done[count][1] == 25) {
                                count_left_rand++;
                                count_right_rand++;
                            }

                            for(int i = 0; i < rand_rows; i++) {
                                if(arr_rand[i][0] == arr_rand_done[count][0] &&
                                   arr_rand[i][1] == arr_rand_done[count][1]) {
                                    for(int j = i; j < rand_rows - 1; j++) {
                                        arr_rand[j][0] = arr_rand[j + 1][0];
                                        arr_rand[j][1] = arr_rand[j + 1][1];
                                    }
                                    rand_rows--;
                                    break;
                                }
                            }
                        }
                        count++;
                    } else if(!(game)) {
                        section_x = 40;
                        section_y = 8;

                        count = 0;

                        count_40 = 0;
                        count_57 = 0;
                        count_74 = 0;
                        count_8 = 0;
                        count_25 = 0;
                        count_42 = 0;
                        count_left = 0;
                        count_right = 0;

                        count_40_rand = 0;
                        count_57_rand = 0;
                        count_74_rand = 0;
                        count_8_rand = 0;
                        count_25_rand = 0;
                        count_42_rand = 0;
                        count_left_rand = 0;
                        count_right_rand = 0;

                        rand_rows = 9;
                        memcpy(
                            arr_rand,
                            (int32_t[9][2]){
                                {40, 8},
                                {57, 8},
                                {74, 8},
                                {40, 25},
                                {57, 25},
                                {74, 25},
                                {40, 42},
                                {57, 42},
                                {74, 42}},
                            sizeof(arr_rand));
                        game = true;
                        game_flag = false;
                    }
                } else if(select == 3) {
                    game_flag = true;
                    game_state.exp += 4;

                    if(game_state.happiness + 5 < 30) //happiness +5
                        game_state.happiness += 5;
                    else
                        game_state.happiness = 30;

                    if(game_state.hunger - 3 > 0) //hunger -3
                        game_state.hunger -= 3;
                    else
                        game_state.hunger = 0;
                }

                else if(heal_flag) { //HEAL
                    game_state.exp += 1;

                    heal_flag = false;
                    if(y == 16) {
                        if(game_state.happiness - 1 > 0) //happiness -1
                            game_state.happiness -= 1;
                        else
                            game_state.happiness = 0;
                        if(game_state.health + 2 < 30) //health +2
                            game_state.health += 2;
                        else
                            game_state.health = 30;
                    }
                    if(y == 28) {
                        if(game_state.happiness - 2 > 0) //happiness -2
                            game_state.happiness -= 2;
                        else
                            game_state.happiness = 0;
                        if(game_state.health + 4 < 30) //health +4
                            game_state.health += 4;
                        else
                            game_state.health = 30;
                    }
                    if(y == 40) {
                        if(game_state.happiness - 4 > 0) //happiness -4
                            game_state.happiness -= 4;
                        else
                            game_state.happiness = 0;
                        if(game_state.health + 5 < 30) //health +5
                            game_state.health += 5;
                        else
                            game_state.health = 30;
                    }
                } else if(select == 5) {
                    heal_flag = true;
                    y = 16;
                }

                else if(select == 6) {
                    info_flag = true;
                }
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
