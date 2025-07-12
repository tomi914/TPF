#ifndef BACKEND_H
#define BACKEND_H
//FUNCIONES DE USO GENERAL
int getAlienWidthByRow(int row);
int getAlienHeightByRow(int row);
//FUNCIONES DE INICIALIZACION
void initPlayer(player_t * player);
void initAliensBlock(aliensBlock_t * aliensBlock);
void initAliensArray(alien_t aliens[ALIEN_ROWS][ALIEN_COLS]);
void initShieldsArray(shield_t shields[NUM_SHIELDS]);
void initGameStats(stats_t * gameStats);
void initOvni(ovni_t * ovni, clock_t currentTime, clock_t * LastOvniDespawnTime);
//FUNCIONES DE ACTUALIZACION
void updateAliensBlock(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock);
void shieldsUpdate(shield_t shields[NUM_SHIELDS]);
void newLevelCheck(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock, player_t * player, shield_t shields[NUM_SHIELDS], stats_t * gameStats);
void updateOvni (ovni_t * ovni, clock_t currentTime, clock_t * LastOvniDespawnTime, int random);
//FUNCIONES DE COLISIONES
void collisionDetect(bullet_t * bulletP, bullet_t * bulletA, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], ovni_t * ovni, shield_t shields[NUM_SHIELDS], aliensBlock_t * aliensBlock, player_t * player, stats_t * gameStats, uint8_t printedRow, double * lastOvniDespawnTime);
void collisionBA(bullet_t * bullet, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock, stats_t * gameStats, int printedRow);
void collisionBB(bullet_t * bulletP , bullet_t * bulletA);
void collisionAS (alien_t aliens[ALIEN_ROWS][ALIEN_COLS], shield_t shields[NUM_SHIELDS], aliensBlock_t * aliensBlock);
void collisionBP (bullet_t * bullet, player_t * player);
void collisionAP (player_t * player, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock);
void collisionBS(bullet_t * bulletP, bullet_t * bulletA, shield_t shields[NUM_SHIELDS]);
void collisionBO(bullet_t * bullet, ovni_t * ovni, double *lastOvniDespawnTime, stats_t * gameStats);
//FUNCIONES DE DISPAROS
alien_t * selectAlienShooter(alien_t alien[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock, player_t * player);
void alienShoot(bullet_t * bullet, alien_t * alien, int level, aliensBlock_t * aliensBlock, int lastRowToPrint, int alienRowIndex);
void playerShoot(bullet_t *playerBullet, player_t *player, bool *tryShoot);
//FUNCIONES DE MOVIMIENTO
void playerMove(int dire, player_t * player);
void blockMove(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock);


//FUNCIONES DE USO GENERAL --- HAY QUE DEJARLAS ACA?
//static int getAlienWidthByRow(int row);
//static int getAlienHeightByRow(int row); 
//char rectangleOverlap(uint16_t AX, uint16_t AW, uint16_t BX, uint16_t BW, uint16_t AY, uint16_t AH, uint16_t BY, uint16_t BH);

#endif // BACKEND_H