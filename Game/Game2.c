#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
int score = 0;
int width = 640;
int height = 480;
int xSel = 55;
int ySel = 330;
int xSelOp = 0;
int ySelOp = 0;
int xGrid = 0;
int yGrid = 0;
int xPix = 0;
int yPix = 0;
int pos[4];
int values[2];
int tileset[14][7];
int helper[14][7];
int state[14][7];
int change[5];
int line[7];
int column[14];
int flagtimer = 1;

Uint32 startTime = 0;
Uint32 currentTime;

char str[2];
char scorestr[5];
//sprintf(str, "%u", num); converter o uint em string

SDL_Rect selector;
SDL_Rect placer;
SDL_Rect auxplacer;
SDL_Rect auxspriteNav;
SDL_Rect spriteNav;
SDL_Rect textNav;
SDL_Rect textplacer;
SDL_Rect scoreplacer;
SDL_Rect scoreNav;

SDL_Renderer* renderer;

SDL_Window* gWindow = NULL;

SDL_Surface* gBGS1 = NULL;
SDL_Surface* gLoading = NULL;
SDL_Surface* gSelector = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gHelloWorld = NULL;
SDL_Surface* gOptionsBG = NULL;
SDL_Surface* gOptionsSelector = NULL;
SDL_Surface* gSprites = NULL;
SDL_Surface* gNumbers = NULL;
SDL_Surface* gTime = NULL;
SDL_Surface* gScore = NULL;
SDL_Surface* gEndScreen = NULL;
SDL_Surface* gAutors = NULL;

Mix_Music* gMusic = NULL;

Mix_Chunk* gBeep = NULL;
Mix_Chunk* gConfirm = NULL;

int rearranjaPecas();
int checkMoves();
int movimentoValido();
int preenche();
int inicializa();
int carregaMidia();
void clearHelper();
void fecha();

int main(int argc, char* args[]) {
	SDL_Init(SDL_INIT_VIDEO);
	int i, j;
	int xAux, yAux;
	int selected = 0;
	int isAdjacente = 0;
	int nonrepeat = 0;
	int flagMovimento;
	int cont = 0;
	int convert;
	int sair = 0;
	int menu = 1;
	//
	int continua = 1;
	int musica_tocando = 1;
	spriteNav.w = 32;
	spriteNav.h = 32;
	spriteNav.y = 0;
	textNav.y = 0;
	textNav.w = 32;
	textNav.h = 32;
	selector.x = xSel;
	selector.y = ySel;
	renderer = SDL_CreateRenderer(gWindow, -1, 0);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Event e;
	if (!inicializa()) {
		printf("Falha ao iniciar");
	}
	else {
		if (!carregaMidia()) {
			printf("Falha ao carregar midia");
		}
		else {
			while (!sair) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						sair = 1;
					}
					else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
							case SDLK_RIGHT:
								if (selector.x > 400) {
									selector.x = 55;
									menu = 1;
								}
								else {
									selector.x += 218;
									menu++;
								}
								Mix_PlayChannel(-1, gBeep, 0);
								break;
							case SDLK_LEFT:
								if (selector.x == 55) {
									selector.x = 491;
									menu = 3;
								}
								else {
									selector.x -= 218;
									menu--;
								}
								Mix_PlayChannel(-1, gBeep, 0);
								break;
							case SDLK_z:
								switch(menu){
									case 1:
										nonrepeat = 0;
										//the gaem
										Mix_PlayChannel(-1, gConfirm, 0);
										SDL_BlitSurface(gLoading, NULL, gScreenSurface, NULL);
										SDL_UpdateWindowSurface(gWindow);
										SDL_Delay(1000);
										SDL_BlitSurface(gBGS1, NULL, gScreenSurface, NULL);
										textplacer.y = 300;
										textplacer.x = 125;
										SDL_BlitSurface(gScore, NULL, gScreenSurface, &textplacer);
										textplacer.y = 100;
										SDL_BlitSurface(gTime, NULL, gScreenSurface, &textplacer);
										SDL_UpdateWindowSurface(gWindow);
										preenche();
										xPix = 380;
										yPix = 18;
										startTime = SDL_GetTicks();

										while (flagtimer == 1 && sair == 0 && nonrepeat == 0) {
											yAux = 18;
											for (i = 0; i < 14; i++) {
												xAux = 380;
												for (j = 0; j < 7; j++) {

													if (selected == 2) {
														flagMovimento = movimentoValido();
														if (flagMovimento == 1) {
															checkMoves();
														}
														selected = 0;
													}

													spriteNav.x = (tileset[i][j] * 32) + (128 * state[i][j]);
													placer.x = xAux;
													placer.y = yAux;
													SDL_BlitSurface(gSprites, &spriteNav, gScreenSurface, &placer);
													SDL_UpdateWindowSurface(gWindow);
													xAux += 32;
												}
												yAux += 32;
											}

											currentTime = (SDL_GetTicks() - startTime) / 1000;
											currentTime = 60 - currentTime;
											sprintf(str, "%u", currentTime);
											textplacer.x = 145;
											textplacer.y = 150;
											for (i = 0; i < 2; i++) {
												if (currentTime >= 10 || i == 0) {
													convert = str[i] - '0';
												}
												else {
													convert = 10;
												}
												textNav.x = 0 + 32 * convert;
												SDL_BlitSurface(gNumbers, &textNav, gScreenSurface, &textplacer);
												textplacer.x += 32;
											}

											
											sprintf(scorestr, "%d", score);
											scoreplacer.x = 110;
											scoreplacer.y = 350;
											scoreNav.y = 0;
											for (i = 0; i < strlen(scorestr); i++) {
												convert = scorestr[i] - '0';
												scoreNav.x = 0 + (32 * (int)convert);
												scoreNav.h = 32;
												scoreNav.w = 32;
												SDL_BlitSurface(gNumbers, &scoreNav, gScreenSurface, &scoreplacer);
												scoreplacer.x += 32;
											}

											SDL_UpdateWindowSurface(gWindow);
											cont = 0;
											if (currentTime == 0) {
												flagtimer = 0;
											}
											while (SDL_PollEvent(&e) != 0) {
												if (e.type == SDL_QUIT) {
													sair = 1;
												}
												if (e.type == SDL_KEYDOWN) {
													switch (e.key.keysym.sym) {
														case SDLK_LEFT:

															if (state[yGrid][xGrid] != 2) {
																state[yGrid][xGrid] = 0;
															}

															if (xGrid == 0) {
																xGrid = 6;
															}
															else {
																xGrid--;
															}

															if (state[yGrid][xGrid] != 2) {
																state[yGrid][xGrid] = 1;
															}

															break;
														case SDLK_RIGHT:

															if (state[yGrid][xGrid] != 2) {
																state[yGrid][xGrid] = 0;
															}

															if (xGrid == 6) {
																xGrid = 0;
															}
															else {
																xGrid++;
															}

															if (state[yGrid][xGrid] != 2) {
																state[yGrid][xGrid] = 1;
															}

															break;
														case SDLK_UP:

															if (state[yGrid][xGrid] != 2) {
																state[yGrid][xGrid] = 0;
															}

															if (yGrid == 0) {
																yGrid = 13;
															}
															else {
																yGrid--;
															}

															if (state[yGrid][xGrid] != 2) {
																state[yGrid][xGrid] = 1;
															}

															break;
														case SDLK_DOWN:

															if (state[yGrid][xGrid] != 2) {
																state[yGrid][xGrid] = 0;
															}

															if (yGrid == 13) {
																yGrid = 0;
															}
															else {
																yGrid++;
															}
															
															if (state[yGrid][xGrid] != 2) {
																state[yGrid][xGrid] = 1;
															}
														
															break;
														case SDLK_z:
															xPix = 380 + (xGrid * 32);
															yPix = 18 + (yGrid * 32);
															state[yGrid][xGrid] = 2;
															selected++;
															break;
													}
												}
											}
											while (flagtimer == 0) {
												SDL_BlitSurface(gEndScreen, NULL, gScreenSurface, NULL);
												SDL_UpdateWindowSurface(gWindow);
												while (SDL_PollEvent(&e) != 0) {
													if (e.type == SDL_QUIT) {
														sair = 1;
													}
													if (e.type == SDL_KEYDOWN) {
														switch (e.key.keysym.sym) {
														case SDLK_x:
															flagtimer = 1;
															nonrepeat = 1;
															break;
														case SDLK_z:
															flagtimer = 1;
															SDL_BlitSurface(gBGS1, NULL, gScreenSurface, NULL);
															textplacer.y = 300;
															textplacer.x = 125;
															SDL_BlitSurface(gScore, NULL, gScreenSurface, &textplacer);
															textplacer.y = 100;
															SDL_BlitSurface(gTime, NULL, gScreenSurface, &textplacer);
															SDL_UpdateWindowSurface(gWindow);
															preenche();
															xPix = 380;
															yPix = 18;
															startTime = SDL_GetTicks();
															break;
														}
													}
												}
											}
										}
										break;

									case 2:
										//options menu
										continua = 1;	
										Mix_PlayChannel(-1, gConfirm, 0);
										selector.y = 65;
										int soltou = 1;
										SDL_BlitSurface(gLoading, NULL, gScreenSurface, NULL);
										SDL_UpdateWindowSurface(gWindow);
										SDL_Delay(2000);
										while (continua){
											SDL_BlitSurface(gOptionsBG, NULL, gScreenSurface, NULL);
											SDL_BlitSurface(gSelector,NULL,gScreenSurface,&selector);
											SDL_UpdateWindowSurface(gWindow);
											SDL_PollEvent(&e);
											/*
												y = 65 
													opções
												y = 170
													sobre os autores
												y = 270
													desabilitar musica 
												y = 465
													voltar
											*/
											if (e.type == SDL_KEYDOWN && soltou == 1)
											{	
												switch(e.key.keysym.sym)
												{
													case SDLK_DOWN:
														if (selector.y == 65)
														{	
															Mix_PlayChannel(-1, gBeep, 0);
															selector.y = 170;
															soltou = 0;
														}
														else if (selector.y == 170)
														{	
															Mix_PlayChannel(-1, gBeep, 0);
															selector.y = 270;
															soltou = 0;
														}
														else if (selector.y == 270)
														{	
															Mix_PlayChannel(-1, gBeep, 0);
															selector.y = 465;
															soltou = 0;
														}
														else if (selector.y == 465)
														{	
															Mix_PlayChannel(-1, gBeep, 0);
															selector.y = 65;
															soltou = 0;
														}
													break;
													case SDLK_UP:
														if (selector.y == 465)
														{	
															Mix_PlayChannel(-1, gBeep, 0);
															selector.y = 270;
															soltou = 0;
														}
														if (selector.y == 270)
														{	
															Mix_PlayChannel(-1, gBeep, 0);
															selector.y = 170;
															soltou = 0;
														}
														else if(selector.y == 170)
														{	
															Mix_PlayChannel(-1, gBeep, 0);
															selector.y = 65;
															soltou = 0;
														}
														else if (selector.y == 65)
														{	
															Mix_PlayChannel(-1, gBeep, 0);
															selector.y = 270;
															soltou = 0;
														}
													break;
													case SDLK_z:
														if (selector.y == 65)
														{
															soltou = 0;
														}
														else if (selector.y == 170)
														{	
															int olhando_autores = 1;
															Mix_PlayChannel(-1, gBeep, 0);
															while(olhando_autores) {
																soltou = 1;
																SDL_BlitSurface(gAutors, NULL, gScreenSurface, NULL);
																SDL_UpdateWindowSurface(gWindow);
																SDL_PollEvent(&e);
																if (e.type == SDL_KEYDOWN && soltou == 1)
																{
																	switch(e.key.keysym.sym)
																	{
																		case SDLK_z:
																			Mix_PlayChannel(-1, gBeep, 0);
																			olhando_autores = 0;
																			break;
																		break;
																	}
																}
																if (e.type == SDL_QUIT) {
																	fecha();
																}
																SDL_Delay(16);
															}
															soltou = 0;
														}
														else if (selector.y == 270)
														{	
															//Botão de pausar musica
															if (musica_tocando == 1)
															{
																//pausa aqui
																Mix_PlayChannel(-1, gBeep, 0);
																musica_tocando = 0;
																Mix_PauseMusic();
															}
															else
															{
																//volta a tocar
																Mix_PlayChannel(-1, gBeep, 0);
																musica_tocando = 1;
																Mix_ResumeMusic();
															}
															soltou = 0;
														}
														else if (selector.y == 465)
														{	
															//Botão de voltar
															Mix_PlayChannel(-1, gBeep, 0);
															selector.y = ySel;
															continua = 0;
															soltou = 0;
															break;
														}	
													break;
												}
											}
											if (e.type == SDL_KEYUP)
											{
												soltou = 1;
											}

											if (e.type == SDL_QUIT)
											{
												continua = 0;
												sair = 1;
												fecha();
											}
											SDL_Delay(16);
										}
										break;
									case 3:
										sair = 1;
										break;
								}
								break;
							case SDLK_ESCAPE:
								sair = 1;
								break;
						}
					}
				}
				SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
				SDL_BlitSurface(gSelector, NULL, gScreenSurface, &selector);
				SDL_UpdateWindowSurface(gWindow);

				if (Mix_PlayingMusic() == 0) {
					if (musica_tocando == 1){
						Mix_PlayMusic(gMusic, -1);
					}
				}
				else {
					if (Mix_PausedMusic() == 1 && musica_tocando == 1) {
						Mix_ResumeMusic();
					}
				}
			}
		}
	}
	fecha();
	return 0;
}

void clearHelper() {
	int i, j;
	for (i = 0; i < 14; i++) {
		for (j = 0; j < 7; j++) {
			helper[i][j] = 0;
		}
	}
}

int movimentoValido() {
	int i, j;
	int aux;
	int auxX, auxY;
	int start = 0;
	int loop;
	int removeFlag;
	int flag = 0;
	int xM, xL, yM, yL;
	int xEqual = 0, yEqual = 0;
	for (i = 0; i < 14; i++) {
		for (j = 0; j < 7; j++) {
			helper[i][j] = 0;
		}
	}
	for (i = 0; i < 14; i++) {
		for (j = 0; j < 7; j++) {

			if (state[i][j] == 2) {

				if (j != 6) {
					xM = j + 1;
				}
				else {
					xM = 0;
				}

				if (j != 0) {
					xL = j - 1;
				}
				else {
					xL = 6;
				}

				if (i != 14) {
					yM = i + 1;
				}
				else {
					yM = 0;
				}

				if (i != 0) {
					yL = i - 1;
				}
				else {
					yL = 13;
				}

				if (state[i][xM] == state[i][j]) {
					flag = 1;
					xEqual = xM;
					yEqual = i;
				}

				if (state[i][xL] == state[i][j]) {
					flag = 1;
					xEqual = xL;
					yEqual = i;
				}

				if (state[yM][j] == state[i][j]) {
					flag = 1;
					xEqual = j;
					yEqual = yM;
				}

				if (state[yL][j] == state[i][j]) {
					flag = 1;
					xEqual = j;
					yEqual = yM;
				}

				//the fun swapping part
				if (flag == 1) {
					aux = tileset[yEqual][xEqual];
					helper[i][j] = 1;
					helper[yEqual][xEqual] = 1;
					tileset[yEqual][xEqual] = tileset[i][j];
					tileset[i][j] = aux;
				}
				state[yEqual][xEqual] = 0;
				state[i][j] = 0;
				auxspriteNav.x = (tileset[i][j] * 32) + (128 * state[i][j]);
				auxplacer.x = 380 + (32 * j);
				auxplacer.y = 18 + (32 * i);
				SDL_BlitSurface(gSprites, &auxspriteNav, gScreenSurface, &auxplacer);
				auxspriteNav.x = (tileset[yEqual][xEqual] * 32) + (128 * state[i][j]);
				auxplacer.x = 380 + (32 * xEqual);
				auxplacer.y = 18 + (32 * yEqual);
				SDL_BlitSurface(gSprites, &auxspriteNav, gScreenSurface, &auxplacer);
				pos[0] = i;
				pos[1] = j;
				pos[2] = yEqual;
				pos[3] = xEqual;
			}
		}
	}
	return flag;
}
	
int checkMoves() {
	int i, j, k, aux, jM, iM, kaux;
	int h[2];
	int flag = 0;
	int hc = 0;
	int count = 0;
	int auxcount = 0;
	hc = 0;
	for (i = 0; i < 14; i++) {
		for (j = 0; j < 7; j++) {
			if (helper[i][j] == 1) {
				h[hc] = tileset[i][j];
				hc++;
				for (k = 0; k < 7; k++) {
					if (tileset[i][k] == tileset[i][j]) {
						state[i][k] = 3;
					}
				}
				count = 0;
				for (k = 0; k < 7; k++) {
					if (state[i][k] == 3) {
						count++;
					}
					if (k == 6) {
						if (count < 3) {
							auxcount = count;
							kaux = k;
							while (auxcount >= 0) {
								state[i][kaux--] = 0;
								auxcount--;
							}
							count = 0;
						}
						else if (count >= 3) {
							auxcount = count;
							kaux = k;
							while (auxcount > 0) {
								if (helper[i][--kaux] == 1) {
									flag = 1;
								}
								auxcount--;
							}
							if (flag != 1) {
								auxcount = count;
								kaux = k;
							}
							while (auxcount >= 0 && flag != 1) {
								state[i][--kaux] = 0;
								auxcount--;
							}
							count = 0;
							flag = 0;
						}
					}
					if (state[i][k] == 0 && count < 3) {
						auxcount = count;
						kaux = k;
						while (auxcount > 0) {
							state[i][--kaux] = 0;
							auxcount--;
						}
						count = 0;
					}
					else if (state[i][k] == 0 && count >= 3) {
						auxcount = count;
						kaux = k;
						while (auxcount > 0) {
							if (helper[i][--kaux] == 1) {
								flag = 1;
							}
							auxcount--;
						}
						if (flag != 1) {
							auxcount = count;
							kaux = k;
						}
						while (auxcount > 0 && flag != 1) {
							state[i][--kaux] = 0;
							auxcount--;
						}
						count = 0;
						flag = 0;
					}
					else if (state[i][k] == 0) {
						count = 0;
					}
				}
				for (k = 0; k < 14; k++) {
					if (tileset[k][j] == tileset[i][j]) {
						state[k][j] = 3;
					}
				}
				count = 0;
				for (k = 0; k < 14; k++) {
					if (state[k][j] == 3) {
						count++;
					}
					if (k == 13) {
						if (count < 3) {
							auxcount = count;
							kaux = k;
							while (auxcount >= 0) {
								state[kaux--][j] = 0;
								auxcount--;
							}
							count = 0;
						}
						else if (count >= 3) {
							auxcount = count;
							kaux = k;
							while (auxcount > 0) {
								if (helper[kaux--][j] == 1) {
									flag = 1;
								}
								auxcount--;
							}
							if (flag != 1) {
								auxcount = count;
								kaux = k;
							}
							while (auxcount >= 0 && flag != 1) {
								state[kaux--][j] = 0;
								auxcount--;
							}
							count = 0;
							flag = 0;
						}
					}
					if (state[k][j] == 0 && count < 3) {
						auxcount = count;
						kaux = k;
						while (auxcount > 0) {
							state[--kaux][j] = 0;
							auxcount--;
						}
						count = 0;
					}
					else if (state[k][j] == 0 && count >= 3) {
						auxcount = count;
						kaux = k;
						while (auxcount > 0) {
							if (helper[--kaux][j] == 1) {
								flag = 1;
							}
							auxcount--;
						}
						if (flag != 1) {
							auxcount = count;
							kaux = k;
						}
						while (auxcount > 0 && flag != 1) {
							state[--kaux][j] = 0;
							auxcount--;
						}
						count = 0;
						flag = 0;
					}
					else if (state[k][j] == 0) {
						count = 0;
					}
				}
			}
		}
	}
	for (i = 0; i < 14; i++) {
		for (j = 0; j < 7; j++) {
			if (tileset[i][j] != h[0] && tileset[i][j] != h[1]) {
				state[i][j] = 0;
			}
		}
	}
	rearranjaPecas();
	return 0;
}

int rearranjaPecas() {
	int i, j, k, m;
	int aux;
	int flag = 0;
	for (i = 0; i < 14; i++) {
		for (j = 0; j < 7; j++) {
			if (state[i][j] == 3 || helper[i][j] == 1) {
				tileset[i][j] = 17;
				auxspriteNav.x = (tileset[i][j] * 32);
				auxplacer.x = 380 + (32 * j);
				auxplacer.y = 18 + (32 * i);
				auxplacer.w = 32;
				auxplacer.h = 32;
				SDL_BlitSurface(gSprites, &auxspriteNav, gScreenSurface, &auxplacer);
				SDL_UpdateWindowSurface(gWindow);
				state[i][j] = 0;
				score += 10;
			}
		}
	}
	for (i = 13; i >= 0; i--) {
		for (j = 0; j < 7; j++) {
			if (tileset[i][j] == 17 && tileset[i-1][j] != 17 && i > 0) {
				aux = tileset[i][j];
				tileset[i][j] = tileset[i - 1][j];
				tileset[i - 1][j] = aux;
			}
		}
	}
	for (i = 0; i < 14; i++) {
		for (j = 0; j < 7; j++) {
			if (tileset[i][j] == 17) {
				srand(time(NULL));
				tileset[i][j] = rand() % 4;
				/* auxspriteNav.x = (tileset[i][j] * 32) + (128 * state[i][j]);
				auxplacer.x = 380 + (32 * j);
				auxplacer.y = 18 + (32 * i);
				auxplacer.w = 32;
				auxplacer.h = 32;
				SDL_BlitSurface(gSprites, &auxspriteNav, gScreenSurface, &auxplacer);
				SDL_UpdateWindowSurface(gWindow);*/
			}
		}
	}
	clearHelper();
	return 1;
}

int clearVectors() {
	int i;
	for (i = 0; i < 5; i++) {
		change[i] = 9;
	}
	return 1;
}

int preenche() {
	int i, j;
	FILE* fp;
	int x = 380;
	int y = 18;
	fp = fopen("control/layout.txt", "w");
	srand(time(NULL));
	for (i = 0; i < 14; i++) {
		for (j = 0; j < 7; j++) {
			tileset[i][j] = rand()%4;
			state[i][j] = 0;
			spriteNav.x = 32 * tileset[i][j];
			placer.x = x;
			placer.y = y;
			SDL_BlitSurface(gSprites, &spriteNav, gScreenSurface, &placer);
			x += 32;
			fprintf(fp, "%d", tileset[i][j]);
		}
		x = 380;
		y += 32;
		fprintf(fp, "\n");
	}
	SDL_UpdateWindowSurface(gWindow);
	fclose(fp);
	return 1;
}

int inicializa() {
	int flag = 1;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL nao inicializou. Erro %s\n", SDL_GetError());
		flag = 0;
	}
	else {
		gWindow = SDL_CreateWindow("AToSaS (Beta Version)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN); //criar variaveis pra resolucao e uma opcao pra trocar.
		if (gWindow == NULL) {
			printf("Janela nao criada! Erro %s\n", SDL_GetError());
			flag = 0;
		}
		else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
				printf("O Mixer SDL nao pode ser iniciado! Erro SDL_mixer: %s\n", Mix_GetError());
				flag = 0;
			}
		}
	}
	return flag;
}

int carregaMidia() {
	int flag = 1;

	gHelloWorld = SDL_LoadBMP("images/titlescreen.bmp");
	if (gHelloWorld == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}
	gAutors = SDL_LoadBMP("images/Autores.bmp");
	if (gAutors == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gSelector = SDL_LoadBMP("images/selector.bmp");
	if (gSelector == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gLoading = SDL_LoadBMP("images/loading.bmp");
	if (gLoading == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gOptionsBG = SDL_LoadBMP("images/options.bmp");
	if (gOptionsBG == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gOptionsSelector = SDL_LoadBMP("images/selector.bmp");
	if (gOptionsSelector == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gNumbers = SDL_LoadBMP("images/timermap.bmp");
	if (gNumbers == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gSprites = SDL_LoadBMP("images/spritemap.bmp");
	if (gSprites == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gBGS1 = SDL_LoadBMP("images/stage1.bmp");
	if (gBGS1 == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gTime = SDL_LoadBMP("images/time.bmp");
	if (gTime == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gScore = SDL_LoadBMP("images/score.bmp");
	if (gScore == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gEndScreen = SDL_LoadBMP("images/endgame.bmp");
	if (gEndScreen == NULL) {
		printf("Impossivel carregar imagem! Erro %s\n", SDL_GetError());
		flag = 0;
	}

	gBeep = Mix_LoadWAV("songs/beep.wav");
	if (gBeep == NULL) {
		printf("Impossivel carregar som! Erro SDL_Mixer: %s\n", Mix_GetError());
		flag = 0;
	}

	gConfirm = Mix_LoadWAV("songs/confirm.wav");
	if (gBeep == NULL) {
		printf("Impossivel carregar som! Erro SDL_Mixer: %s\n", Mix_GetError());
		flag = 0;
	}

	gMusic = Mix_LoadMUS("songs/theme.wav");
	if (gMusic == NULL) {
		printf("Impossivel carregar musica tema! Erro SDL_Mixer: %s\n", Mix_GetError());
		flag = 0;
	}
	return flag;
}

void fecha() {

	SDL_FreeSurface(gHelloWorld);
	SDL_FreeSurface(gSelector);
	SDL_FreeSurface(gLoading);
	SDL_FreeSurface(gBGS1);
	SDL_FreeSurface(gNumbers);
	SDL_FreeSurface(gOptionsBG);
	SDL_FreeSurface(gOptionsSelector);
	SDL_FreeSurface(gScreenSurface);
	SDL_FreeSurface(gSprites);
	SDL_FreeSurface(gTime);
	SDL_FreeSurface(gScore);

	gHelloWorld = NULL;
	gSelector = NULL;
	gLoading = NULL;
	gBGS1 = NULL;
	gOptionsBG = NULL;
	gWindow = NULL;

	SDL_DestroyWindow(gWindow);

	Mix_FreeChunk(gBeep);
	Mix_FreeChunk(gConfirm);
	Mix_FreeMusic(gMusic);

	gMusic = NULL;
	gConfirm = NULL;
	gBeep = NULL;

	Mix_Quit();
	SDL_Quit();
}