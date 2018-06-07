#include "stdafx.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_image.h>
#include <windows.h>


#define ScreenWidth 800
#define ScreenHeight 600

typedef struct barra {
	int x1, x2, tamanho;
};

int main() {
	al_init();
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	const int FPS = 60;
	int pontos = 0;
	int countColor = 0;
	int draw = 1;
	int k = 100, z = 100;
	int specialPronto = 0;
	enum Direction { UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT };
	int dir = DOWN;
	int dirBola = DOWN_RIGHT;
	int dirBola2 = UP_LEFT;
	int k2 = 500, z2 = 500;
	int pontosSpecial = 0;
	int specialOn = 0;
	int specialCount = 0;
	int moveSpeed = 8;
	float moveSpeedBola = 3;
	int done = 0;
	int pontosReq = 10;
	int done2 = 0;
	barra player;
	


	al_init_primitives_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	


	ALLEGRO_DISPLAY *display = al_create_display(ScreenWidth, ScreenHeight);
	ALLEGRO_BITMAP *setas = al_load_bitmap("setas.jpg");
	ALLEGRO_BITMAP *spaceBar = al_load_bitmap("spacebar.png");
	ALLEGRO_FONT *font = al_load_font("Orbitron Black.ttf", 30, NULL);
	ALLEGRO_SAMPLE *toasty = al_load_sample("toasty.wav");
	ALLEGRO_SAMPLE *theme = al_load_sample("theme.wav");
	ALLEGRO_SAMPLE *point = al_load_sample("point.wav");
	ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(theme);
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_COLOR eletricBlue = al_map_rgb(44, 117, 255);
	ALLEGRO_COLOR purple = al_map_rgb(148, 0, 211);
	ALLEGRO_COLOR darkRed = al_map_rgb(139, 0, 0);
	ALLEGRO_COLOR tealBlue = al_map_rgb(0, 128, 128);
	ALLEGRO_COLOR gray = al_map_rgb(128, 128, 128);
	ALLEGRO_COLOR orange = al_map_rgb(255, 140, 0);
	ALLEGRO_COLOR green = al_map_rgb(0, 128, 0);
	ALLEGRO_COLOR bordacolor = gray;
	ALLEGRO_COLOR playercolor = orange;
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_EVENT events;
	
	
	
	
	al_set_window_title(display, "Pong");
	al_reserve_samples(3);
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);

	
	
	player.tamanho = 200;
	player.x1 = 50;
	player.x2 = player.x1 + player.tamanho;
	
	
	while (done2 == 0) {
		al_draw_text(font, eletricBlue, ScreenWidth / 2 , 50, ALLEGRO_ALIGN_CENTRE, "Bem vindo ao PONG! ");
		al_draw_bitmap(setas, 150, 150, NULL);
		al_draw_text(font, green, 500, 200, ALLEGRO_ALIGN_CENTRE, "PARA SE MOVER ");
		al_draw_bitmap(spaceBar, 110, 300, NULL);
		al_draw_text(font, green, 530, 380, ALLEGRO_ALIGN_CENTRE, "PARA ATIVAR ESPECIAL ");
		al_draw_text(font, darkRed, ScreenWidth / 2, 520, ALLEGRO_ALIGN_CENTRE, "APERTE QUALQUER TECLA PARA INICIAR!");
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_wait_for_event(event_queue, &events);
		if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
			done2 = 1;
		}

	}	
		
	al_play_sample_instance(songInstance);
	
		al_start_timer(timer); // após esse comando é necessário iniciar o game loop logo em seguida, obrigatoriamente.
		while (done == 0) {
			al_wait_for_event(event_queue, &events);
			if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (events.keyboard.keycode) {

				case ALLEGRO_KEY_RIGHT:
					dir = RIGHT;
					break;
				case ALLEGRO_KEY_LEFT:
					dir = LEFT;
					break;
				case ALLEGRO_KEY_ESCAPE:
					done = 1;
					break;
				case ALLEGRO_KEY_SPACE:
					if (specialPronto == 1) {
						specialOn = 1;
						specialPronto = 0;
						al_play_sample(toasty, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}
				}
				
			}
			if (events.type == ALLEGRO_EVENT_KEY_UP) {
				switch (events.keyboard.keycode) {

				case ALLEGRO_KEY_RIGHT:
					dir = NULL;
					break;
				case ALLEGRO_KEY_LEFT:
					dir = NULL;
					break;
				
				}
			}
			if (events.type == ALLEGRO_EVENT_TIMER) {
				switch (dir) {

				case RIGHT:
					if (player.x2 < ScreenWidth - 30) {
						player.x1 += moveSpeed;
						player.x2 += moveSpeed;
					}
					break;
				case LEFT:
					if (player.x1 > 30) {
						player.x1 -= moveSpeed;
						player.x2 -= moveSpeed;
					}
					break;
				case ALLEGRO_KEY_ESCAPE:
					done = 1;
					break;
				}
				
				switch (dirBola) {

				case DOWN_RIGHT:
					k = k + moveSpeedBola;
					z = z + moveSpeedBola;
					if (k >= ScreenWidth - 46) {
						dirBola = DOWN_LEFT;
					}
					if ((z >= ScreenHeight - 96 && z <= ScreenHeight - 70) && (k >= player.x1) && (k <= player.x2)) {
						dirBola = UP_RIGHT;
						al_play_sample(point, 0.2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						if (specialPronto == 0) {
							pontosSpecial = pontosSpecial + 1;
						}
						if (specialOn == 0) {
							moveSpeedBola = moveSpeedBola + 0.30;
							pontos = pontos + 1;
						}
						else {
							pontos = pontos + 2;
						}
					}
					
					break;
				case DOWN_LEFT:
					k = k - moveSpeedBola;
					z = z + moveSpeedBola;
					if (k <= 46) {
						dirBola = DOWN_RIGHT;
					}
					if ((z >= ScreenHeight - 96 && z <= ScreenHeight - 70) && (k >= player.x1) && (k <= player.x2)) {
						dirBola = UP_LEFT;
						al_play_sample(point, 0.2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						if (specialPronto == 0) {
							pontosSpecial = pontosSpecial + 1;
						}
						if (specialOn == 0) {
							moveSpeedBola = moveSpeedBola + 0.30;
							pontos = pontos + 1;
						}
						else {
							pontos = pontos + 2;
						}
					}
					

					break;
				case UP_RIGHT:
					k = k + moveSpeedBola;
					z = z - moveSpeedBola;
					if (k >= ScreenWidth - 46) {
						dirBola = UP_LEFT;
					}
					if (z <= 46) {
						dirBola = DOWN_RIGHT;
					}

					break;

				case UP_LEFT:
					k = k - moveSpeedBola;
					z = z - moveSpeedBola;
					if (k <= 46) {
						dirBola = UP_RIGHT;
					}
					if (z <= 46) {
						dirBola = DOWN_LEFT;
					}
					
					break;
				}
				if (specialOn == 1) {
					switch (dirBola2) {

					case DOWN_RIGHT:
						k2 = k2 + moveSpeedBola;
						z2 = z2 + moveSpeedBola;
						if (k2 >= ScreenWidth - 46) {
							dirBola2 = DOWN_LEFT;
						}
						if ((z2 > ScreenHeight - 96) && (k2 >= player.x1) && (k2 <= player.x2)) {
							dirBola2 = UP_RIGHT;
							pontos = pontos + 2;
							al_play_sample(point, 0.2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						}

						break;
					case DOWN_LEFT:
						k2 = k2 - moveSpeedBola;
						z2 = z2 + moveSpeedBola;
						if (k2 <= 46) {
							dirBola2 = DOWN_RIGHT;
						}
						if ((z2 > ScreenHeight - 96) && (k2 >= player.x1) && (k2 <= player.x2)) {
							dirBola2 = UP_LEFT;
							pontos = pontos + 2;
							al_play_sample(point, 0.2, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
						}


						break;
					case UP_RIGHT:
						k2 = k2 + moveSpeedBola;
						z2 = z2 - moveSpeedBola;
						if (k2 >= ScreenWidth - 46) {
							dirBola2 = UP_LEFT;
						}
						if (z2 <= 46) {
							dirBola2 = DOWN_RIGHT;
						}

						break;

					case UP_LEFT:
						k2 = k2 - moveSpeedBola;
						z2 = z2 - moveSpeedBola;
						if (k2 <= 46) {
							dirBola2 = UP_RIGHT;
						}
						if (z2 <= 46) {
							dirBola2 = DOWN_LEFT;
						}

						break;
					}
				}
				
				if (z > ScreenHeight - 50 || z2 > ScreenHeight - 50) {
					done = 1;
				}
				if (pontosSpecial >= pontosReq) {
					pontosSpecial = 0;
					specialPronto = 1;
					pontosReq = pontosReq + 3;
				}
				if (specialPronto == 1) {
					playercolor = purple;
				}
				if (specialOn == 1) {
					specialCount = specialCount + 1;
					countColor = countColor + 1;
					moveSpeed = 12;
					if (countColor == 15) {
						playercolor = darkRed;
					}
					if (countColor == 30) {
						playercolor = green;
					}
					if (countColor == 45) {
						playercolor = purple;
						countColor = 0;
					}
					if (specialCount >= 600) {
						specialPronto = 0;
						specialOn = 0;
						specialCount = 0;
						k2	 = 500;
						z2 = 500;
						dirBola2 = UP_LEFT;
						playercolor = orange;
						countColor = 0;
						moveSpeed = 8;
					}
				}
				
				

			

				draw = 1;
			}
			if (draw == 1) {
				draw = 0;
				//PLAYER
				al_draw_line(player.x1, ScreenHeight - 55, player.x2, ScreenHeight - 55, playercolor, 50.0);
				//BORDAS
				al_draw_line(0, 15, ScreenWidth, 15, bordacolor, 30);
				al_draw_line(0, ScreenHeight - 15, ScreenWidth, ScreenHeight - 15, bordacolor, 30);
				al_draw_line(15, 0, 15, ScreenHeight, bordacolor, 30);
				al_draw_line(ScreenWidth - 15, 0, ScreenWidth - 15, ScreenHeight, bordacolor, 30);
				//BOLA
				al_draw_filled_circle(k, z, 16, eletricBlue);
				if (specialOn == 1) {
					al_draw_filled_circle(k2, z2, 16, orange);
				}
				//PONTOS
				char szBuff[256];
				snprintf(szBuff, sizeof(szBuff), "%d", pontos);
				al_draw_text(font, green, ScreenWidth - 100, 200, ALLEGRO_ALIGN_CENTRE, szBuff);
				//JOGA PRA TELA E LIMPA TELA DE FUNDO
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));

			}
		}
		char szBuff2[256];
		snprintf(szBuff2, sizeof(szBuff2), "%d", pontos);
		al_draw_text(font, green, ScreenWidth / 2 + 250, ScreenHeight / 2, ALLEGRO_ALIGN_CENTRE, szBuff2);
		al_draw_text(font, green, ScreenWidth / 2 - 50, ScreenHeight / 2, ALLEGRO_ALIGN_CENTRE, "Fim de jogo! Veja sua pontuacao: ");
		al_flip_display();
		al_clear_to_color(al_map_rgb(0, 0, 0));
		Sleep(3000);
		
		

		


	


	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);
	al_uninstall_keyboard();
	al_destroy_sample(toasty);
	al_destroy_sample(point);
	al_destroy_sample(theme);
	al_destroy_sample_instance(songInstance);
	al_destroy_bitmap(setas);
	al_destroy_bitmap(spaceBar);
	al_destroy_font(font);
	




	return 0;
}

