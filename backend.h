void initPlayer(player_t * player);
void playerMove(int dire, player_t * player);
void playerShoot(bullet_t *playerBullet, player_t *player, bool *tryShoot);
void initAliensBlock(alienBlock_t * aliensBlock);
void initAliensArray(alien_t aliens[ALIEN_ROWS][ALIEN_COLS]);
void updateAliensBlock(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock);
