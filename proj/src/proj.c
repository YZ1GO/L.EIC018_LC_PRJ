//headers for lcf
#include <lcom/lcf.h>
#include <lcom/proj.h>

//headers for game engine
#include "game.h"
#include "xpm_templates.h"

//headers for devices
#include "rtc.h"
#include "graphic.h"
#include "mouse.h"
#include "keyboard.h"
#include "i8254.h"
#include "utils.h"
#include "list.h"

//extern variables
extern int kb_count;
extern uint8_t scancode;
extern uint8_t bytes[2];
extern int size;
extern int size_temp;
extern bool flag_complete_kb;
extern uint32_t count;
extern int hook_id;
extern int hook_id_kb;
extern int hook_id_mouse;
extern int count_mouse;
extern int bytes_m[3];
extern int byte_m;
extern bool flag_complete;
extern int hook_id_timer;
extern uint32_t count_timer;
extern uint32_t count_elapsed_time;
int elapsed_time = 0;
int last_collision_time = -COOLDOWN_PERIOD;
int explosion_time = -COOLDOWN_PERIOD;

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/g6/proj/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/g6/proj/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
    return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
    vg_init(0x105);
    sprite_t *cursor = sprite_ctor(mouse_icon_xpm);
    sprite_t *logo = sprite_ctor(logo_xpm);
    sprite_t *play = sprite_ctor(PLAY_xpm);
    sprite_t *exit = sprite_ctor(EXIT_xpm);
    sprite_t *player = sprite_ctor(player_xpm);
    //sprite_t *playerLeft = sprite_ctor(leftPlayer_xpm);
    //sprite_t *playerRight = sprite_ctor(rightPlayer_xpm);
    sprite_t *arena = sprite_ctor(arena_xpm);
    sprite_t *enemies[4];
    enemies[0] = sprite_ctor(topDownEnemy_xpm);
    sprite_t *verticalEnemy = enemies[0];
    enemies[1] = sprite_ctor(topDownEnemy_xpm);
    sprite_t *verticalEnemy2 = enemies[1];
    enemies[2] = sprite_ctor(leftRightEnemy_xpm);
    sprite_t *leftRightEnemy = enemies[2];
    enemies[3] = sprite_ctor(rightLeftEnemy_xpm);
    sprite_t *rightLeftEnemy = enemies[3];
    sprite_t *explosion = sprite_ctor(explosion_xpm);
     sprite_t *textScore = sprite_ctor(textScore_xpm);
    sprite_t *scorexpm = sprite_ctor(score_xpm);
    sprite_t *healthxpm = sprite_ctor(health_xpm);
    sprite_t *clockxpm = sprite_ctor(clock_xpm);
    sprite_t *timexpm = sprite_ctor(time_xpm);
    sprite_set_pos(scorexpm, 670, 50);
    sprite_set_pos(healthxpm, 670, 200);
    sprite_set_pos(timexpm, 670, 350);
    sprite_set_pos(clockxpm, 670, 500);
    sprite_set_pos(textScore, 50, 50);
    sprite_set_pos(enemies[0], V_ENEMY1_X, V_ENEMY1_Y);
    sprite_set_pos(enemies[1], V_ENEMY2_X, V_ENEMY2_Y);
    sprite_set_pos(enemies[2], LR_ENEMY_X, LR_ENEMY_Y);
    sprite_set_pos(enemies[3], RL_ENEMY_X, RL_ENEMY_Y);
    sprite_set_pos(arena, 0, 0);
    sprite_set_pos(logo, 100, 100);
    sprite_set_pos(cursor, 100, 100);
    sprite_set_pos(play, 450, 300);
    sprite_set_pos(exit, 450, 400);
    sprite_set_pos(player, PLAYER_X, PLAYER_Y);
    drawMenu(play, exit, cursor, logo);

    int ipc_status;
    message msg;
    int r;

    uint8_t bit_no_timer;
    if (timer_subscribe_int(&bit_no_timer)) {
        return 1;
    }
    uint32_t irq_set_timer = BIT(0);

    uint8_t bit_no_kbd;
    if (kb_subscribe_int(&bit_no_kbd)) {
        return 1;
    }
    uint32_t irq_set_kb = BIT(1);

    uint8_t bit_no_m;
    if (send_command(0xF4)) {
        return 1;
    }
    if (mouse_subscribe_int(&bit_no_m)) {
        return 1;
    }
    uint32_t irq_set_m = BIT(12); 
    uint32_t number_packets = 0;
    struct packet mouse_packet;

    uint8_t bit_no_rtc;
    if (rtc_subscribe_int(&bit_no_rtc)) {
        return 1;
    }
    //uint32_t irq_set_rtc = BIT(8);

    int state = 0;
    int good = 1;
    game_t game;
    game.health = 100;
    game.score = 0;
    while(good) { 
        /* You may want to use a different condition */
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & irq_set_timer) { /* subscribed interrupt */
                        timer_int_handler();
                        if (count_timer % 1 == 0) {
                            if (state == 1) {
                                vg_draw_rectangle(verticalEnemy->x, verticalEnemy->y, verticalEnemy->w, verticalEnemy->h, BLACK);
                                vg_draw_rectangle(verticalEnemy2->x, verticalEnemy2->y, verticalEnemy2->w, verticalEnemy2->h, BLACK);
                                vg_draw_rectangle(leftRightEnemy->x, leftRightEnemy->y, leftRightEnemy->w, leftRightEnemy->h, BLACK);
                                vg_draw_rectangle(rightLeftEnemy->x, rightLeftEnemy->y, rightLeftEnemy->w, rightLeftEnemy->h, BLACK);
                                handleVerticalMovementEnemy(verticalEnemy, verticalEnemy2, elapsed_time);
                                handleHorizontalMovementEnemy(leftRightEnemy, rightLeftEnemy, elapsed_time);
                                sprite_draw(verticalEnemy);
                                sprite_draw(verticalEnemy2);
                                sprite_draw(leftRightEnemy);
                                sprite_draw(rightLeftEnemy);
                                sprite_draw(arena); 
                                if (elapsed_time - explosion_time > 0.1) {
                                    vg_draw_rectangle(explosion->x, explosion->y, explosion->w, explosion->h, BLACK);
                                    explosion_time = 9999;
                                }
                                if (count_elapsed_time % 60 == 0) {
                                    elapsed_time++;
                                    game.score += 50*elapsed_time + 10; 
                                    sprite_draw(scorexpm);
                                    draw_numbers(game.score, 110, 950);
                                    sprite_draw(healthxpm);
                                    vg_draw_rectangle(750, 260, 150, 50, BLACK);
                                    draw_numbers(game.health, 260, 950);
                                    sprite_draw(timexpm);
                                    draw_numbers(elapsed_time, 410, 950);
                                    sprite_draw(clockxpm);
                                    char* string = "00:00";
                                    rtc_read_time(string);
                                    draw_numbers_time(string, 560, 750);
                                }
                                if (game.health <= 0) {
                                    vg_draw_rectangle(0, 0, 1024, 768, BLACK);
                                    state = 2;
                                    elapsed_time = 0;
                                    drawRetryMenu(play, exit, cursor, textScore, game.score);
                                }

                            }
                        }
                    }		
                    if (msg.m_notify.interrupts & irq_set_kb) { /* subscribed interrupt */
                        kb_interupt_handler();
                        size_temp--;
                        if (flag_complete_kb) {
                            size = 0;
                            flag_complete_kb = false;
                            if (state == 0) {
                                vg_draw_rectangle(cursor->x,cursor->y, cursor->w, cursor->h, BLACK);
                                handleMoviment(scancode, cursor, 0);
                                drawMenu(play, exit, cursor, logo);
                                handleClick(scancode, cursor, play, exit, &state, &good, &game);
                            }
                            if (state == 1) {
                                handleClick(scancode, cursor, play, exit, &state, &good, &game);
                                vg_draw_rectangle(player->x, player->y, player->w, player->h, BLACK);
                                handleMoviment(scancode, player, 1);
                                sprite_draw(player);
                            }
                            if (state == 2) {
                                vg_draw_rectangle(cursor->x,cursor->y, cursor->w, cursor->h, BLACK);
                                handleMoviment(scancode, cursor, 0);
                                drawRetryMenu(play, exit, cursor, textScore, game.score);
                                handleClick(scancode, cursor, play, exit, &state, &good, &game);
                            }
                        }
                    }
                    if (state == 1) {
                        for (int i = 0; i < 4; i++) {
                            if (check_collision(player, explosion, enemies[i], enemies, i, &last_collision_time, &explosion_time, elapsed_time)) {
                                game.health -= 30;
                            }
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_m) { /* subscribed interrupt */
                        mouse_ih();
                        if (flag_complete) {
                            build_packet(&mouse_packet);
                            number_packets++;
                            flag_complete = false;
                            count_mouse = 0;
                            if (state == 0) {
                                vg_draw_rectangle(cursor->x,cursor->y, cursor->w, cursor->h, BLACK);
                                handleMovimentCursorMouse(&mouse_packet, cursor);
                                drawMenu(play, exit, cursor, logo);
                            }
                        }
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */	
            }
        } else { 
            /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    }
    if(timer_unsubscribe_int()) {
        return 1;
    }
    if (kb_unsubscribe_int()) {
        return 1;
    }
    if (mouse_unsubscribe_int()) {
        return 1;
    }
    if (send_command(0xF5)) {
        return 1;
    }
    if (rtc_unsubscribe_int()) {
        return 1;
    }
    if (vg_exit()) {
        return 1;
    }
    return 0;
}
