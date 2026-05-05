#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

/* ====================================================================== */
/*                         GAME LOGIC FUNCTIONS                           */
/* ====================================================================== */

// updateDifficultyByLevel(): Cap nhat cau hinh do kho dua tren level hien tai.
void updateDifficultyByLevel(int level, GameConfig *cfg)
{
  /* So asteroid toi da tang 2 moi level, nhung khong qua MAX_ASTEROIDS */
  cfg->activeAsteroids = 8 + level * 2;
  if (cfg->activeAsteroids > MAX_ASTEROIDS)
    cfg->activeAsteroids = MAX_ASTEROIDS;
  /* Toc do roi tang nhe moi level */
  cfg->astSpeedMin = AST_SPD_MIN + level * 0.12f;
  cfg->astSpeedMax = AST_SPD_MAX + level * 0.22f;
  /* Goc roi chenh lech nhieu hon */
  cfg->angleVariety = 0.3f + level * 0.08f;
  if (cfg->angleVariety > 1.2f)
    cfg->angleVariety = 1.2f;
  /* Mau asteroid tang: level 1 = 1HP, level 2-3 = 2HP, v.v. */
  cfg->baseAstHp = 1 + level / 2;
}

void initShip(Ship *s)
{
  s->x = WIDTH / 2.0f;
  s->y = HEIGHT - 80.0f;
  s->radius = SHIP_RADIUS;
  s->speed = SHIP_SPEED;
  s->bulletDamage = 1;
  s->pelletsPerShot = 1;
  s->shootCooldown = DEFAULT_SHOOT_CD;
}

void initBullet(Bullet *b)
{
  b->x = b->y = b->dx = b->dy = 0;
  b->active = 0;
  b->radius = BULLET_RADIUS;
  b->damage = 1;
}

/*
 * initAsteroid(): Tao 1 thien thach moi o vi tri ngau nhien PHIA TREN man hinh.
 */
void initAsteroid(Asteroid *a, int level, GameConfig *cfg)
{
  float spd, mxA;

  a->radius = AST_MIN_R + rand() % (AST_MAX_R - AST_MIN_R + 1);
  a->x = (float)(a->radius + rand() % (WIDTH - 2 * a->radius));
  a->y = (float)(-(rand() % 400) - a->radius);

  spd = cfg->astSpeedMin +
        (rand() % 100) / 100.0f * (cfg->astSpeedMax - cfg->astSpeedMin);
  mxA = cfg->angleVariety;
  a->vy = spd;
  a->vx = ((rand() % 200 - 100) / 100.0f) * mxA * spd * 0.4f;

  a->seed = rand();
  a->rot = 0;
  a->rotSpeed = ((rand() % 100) - 50) / 1500.0f;
  a->maxHp = cfg->baseAstHp;
  a->hp = a->maxHp;
  a->active = 1;
}

// initAllAsteroids(): Khoi tao toan bo mang asteroid khi bat dau game.
void initAllAsteroids(Asteroid ast[], int n, int level, GameConfig *cfg)
{
  int i;
  for (i = 0; i < n; i++)
  {
    initAsteroid(&ast[i], level, cfg);
    if (i >= cfg->activeAsteroids)
      ast[i].active = 0;
  }
}

void initSkillSystem(SkillSystem *sk)
{
  /* Skill 1 – Bomb Burst: CD 8 giay, ban kinh 160px, dame 8 */
  sk->bombCD = 8000;
  sk->bombLast = 0;
  sk->bombActive = 0;
  sk->bombFrame = 0;
  sk->bombRadius = 160.0f;
  sk->bombDmg = 8;
  sk->bombDmgDone = 0;

  /* Skill 2 – Time Slow: CD 12 giay, ham cham 4 giay, he so 0.35 */
  sk->slowCD = 12000;
  sk->slowLast = 0;
  sk->slowDur = 4000;
  sk->slowActive = 0;
  sk->slowFactor = 0.35f;

  /* Skill 3 – Piercing Beam: CD 10 giay, hien tia 450ms, dame 12 */
  sk->beamCD = 10000;
  sk->beamLast = 0;
  sk->beamDur = 450;
  sk->beamActive = 0;
  sk->beamDmg = 12;
  sk->beamDmgDone = 0;
}

// initGame(): Khoi dong toan bo trang thai game ve gia tri ban dau.
void initGame(Ship *ship, Asteroid ast[], Bullet bul[], Item items[],
              Explosion exps[], SkillSystem *sk, int *score, int *hp,
              int *level)
{
  int i;

  srand((unsigned int)time(NULL));
  *level = 1;
  *score = 0;
  *hp = INITIAL_HP;
  updateDifficultyByLevel(*level, &gCfg);
  initShip(ship);
  initAllAsteroids(ast, MAX_ASTEROIDS, *level, &gCfg);
  for (i = 0; i < MAX_BULLETS; i++)
    initBullet(&bul[i]);
  for (i = 0; i < MAX_ITEMS; i++)
  {
    items[i].active = 0;
    items[i].radius = 12;
  }
  for (i = 0; i < MAX_EXPLOSIONS; i++)
    exps[i].active = 0;
  initSkillSystem(sk);
  if (!starsReady)
    initStars();
}

/* ====================================================================== */
/*                         INPUT HANDLING                                 */
/* ====================================================================== */

void handleKeyboardMove(Ship *s)
{
  if (GetAsyncKeyState(VK_LEFT) & 0x8000 || GetAsyncKeyState('A') & 0x8000)
    s->x -= s->speed;
  if (GetAsyncKeyState(VK_RIGHT) & 0x8000 || GetAsyncKeyState('D') & 0x8000)
    s->x += s->speed;
  if (GetAsyncKeyState(VK_UP) & 0x8000 || GetAsyncKeyState('W') & 0x8000)
    s->y -= s->speed;
  if (GetAsyncKeyState(VK_DOWN) & 0x8000 || GetAsyncKeyState('S') & 0x8000)
    s->y += s->speed;
}

// handleMouse(): Cap nhat toa do con tro chuot moi frame.
void handleMouse(int *mx, int *my)
{
  *mx = mousex();
  *my = mousey();
}

void handleSkillsInput(SkillSystem *sk, Ship *ship, int mx, int my,
                       unsigned long now)
{
  /* Skill 1 – Bomb Burst: phim '7', CD 8000ms */
  if (GetAsyncKeyState('7') & 0x8000)
  {
    if (now - sk->bombLast >= sk->bombCD)
    {
      sk->bombLast = now;
      sk->bombActive = 1;
      sk->bombFrame = 0;
      sk->bombX = ship->x;
      sk->bombY = ship->y;
      sk->bombDmgDone = 0;
    }
  }

  if (GetAsyncKeyState('8') & 0x8000)
  {
    if (now - sk->slowLast >= sk->slowCD)
    {
      sk->slowLast = now;
      sk->slowActive = 1;
    }
  }

  if (GetAsyncKeyState('9') & 0x8000)
  {
    if (now - sk->beamLast >= sk->beamCD)
    {
      float dx = mx - ship->x, dy = my - ship->y;
      float ln = sqrtf(dx * dx + dy * dy);
      if (ln < 1.0f)
        return;
      sk->beamLast = now;
      sk->beamActive = 1;
      sk->beamDmgDone = 0;
      sk->beamSX = ship->x;
      sk->beamSY = ship->y;
      sk->beamDX = dx / ln;
      sk->beamDY = dy / ln;
    }
  }
}

/* ====================================================================== */
/*                         UPDATE FUNCTIONS                               */
/* ====================================================================== */

void updateShip(Ship *s)
{
  if (s->x - s->radius < 0)
    s->x = (float)s->radius;
  if (s->x + s->radius > WIDTH)
    s->x = (float)(WIDTH - s->radius);
  if (s->y - s->radius < 0)
    s->y = (float)s->radius;
  if (s->y + s->radius > HEIGHT)
    s->y = (float)(HEIGHT - s->radius);
}

static int freeBullet(Bullet b[], int n)
{
  int i;
  for (i = 0; i < n; i++)
    if (!b[i].active)
      return i;
  return -1;
}

// shootPellets(): Tao N vien dan (N = pelletsPerShot) theo huong (dirX, dirY).
void shootPellets(Ship *ship, Bullet bul[], int n, float dirX, float dirY)
{
  int p, idx;
  float base = atan2f(dirY, dirX);
  int cnt = ship->pelletsPerShot;
  float totalSpread = (cnt > 1) ? (cnt - 1) * 0.08f : 0;
  if (totalSpread > 0.5f)
    totalSpread = 0.5f;

  for (p = 0; p < cnt; p++)
  {
    float off = 0;
    if (cnt > 1)
      off = -totalSpread / 2.0f + totalSpread * p / (cnt - 1);
    float a = base + off;

    idx = freeBullet(bul, n);
    if (idx < 0)
      return;
    bul[idx].x = ship->x;
    bul[idx].y = ship->y;
    bul[idx].dx = cosf(a) * BULLET_SPEED;
    bul[idx].dy = sinf(a) * BULLET_SPEED;
    bul[idx].active = 1;
    bul[idx].radius = BULLET_RADIUS;
    bul[idx].damage = ship->bulletDamage;
  }
}

// tryAutoShoot(): Tu dong ban dan moi khi het cooldown.
void tryAutoShoot(Ship *ship, Bullet bul[], int n, int mx, int my,
                  unsigned long *lastShot)
{
  unsigned long now = GetTickCount();
  if (now - *lastShot < (unsigned long)ship->shootCooldown)
    return;
  float dx = mx - ship->x, dy = my - ship->y;
  float ln = sqrtf(dx * dx + dy * dy);
  if (ln < 5.0f)
    return;
  shootPellets(ship, bul, n, dx / ln, dy / ln);
  *lastShot = now;
}

void updateBullets(Bullet b[], int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!b[i].active)
      continue;
    b[i].x += b[i].dx;
    b[i].y += b[i].dy;
    if (b[i].x < -20 || b[i].x > WIDTH + 20 || b[i].y < -20 ||
        b[i].y > HEIGHT + 20)
      b[i].active = 0;
  }
}

// updateAsteroids(): Di chuyen va xoay cac asteroid moi frame.
void updateAsteroids(Asteroid a[], int n, int level, float spdMul)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!a[i].active)
      continue;

    a[i].x += a[i].vx * spdMul;
    a[i].y += a[i].vy * spdMul;
    a[i].rot += a[i].rotSpeed;

    if (a[i].y - a[i].radius > HEIGHT || a[i].x + a[i].radius < -60 ||
        a[i].x - a[i].radius > WIDTH + 60)
    {
      initAsteroid(&a[i], level, &gCfg);
    }
  }

  for (i = 0; i < n && i < gCfg.activeAsteroids; i++)
    if (!a[i].active)
      initAsteroid(&a[i], level, &gCfg);
}

void updateItems(Item it[], int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!it[i].active)
      continue;
    it[i].y += it[i].vy;
    if (it[i].y > HEIGHT + 20)
      it[i].active = 0;
  }
}

void updateExplosions(Explosion e[], int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!e[i].active)
      continue;
    e[i].frame++;
    if (e[i].frame >= e[i].maxFrames)
      e[i].active = 0;
  }
}

void updateSkillEffects(SkillSystem *sk, unsigned long now)
{
  if (sk->bombActive)
  {
    sk->bombFrame++;
    if (sk->bombFrame >= 22)
      sk->bombActive = 0;
  }

  if (sk->slowActive)
  {
    if (now - sk->slowLast >= sk->slowDur)
      sk->slowActive = 0;
  }

  if (sk->beamActive)
  {
    if (now - sk->beamLast >= sk->beamDur)
      sk->beamActive = 0;
  }
}

/* ====================================================================== */
/*                    COLLISION + ITEM + SKILL DAMAGE                     */
/* ====================================================================== */

int checkCircleCollision(float x1, float y1, float r1, float x2, float y2,
                         float r2)
{
  float dx = x2 - x1, dy = y2 - y1;
  return (sqrtf(dx * dx + dy * dy) < r1 + r2) ? 1 : 0;
}

// spawnExplosion(): Tao hieu ung no tai vi tri (x, y) voi ban kinh toi da mr.
void spawnExplosion(Explosion e[], int n, float x, float y, float mr)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!e[i].active)
    {
      e[i].x = x;
      e[i].y = y;
      e[i].maxRadius = mr;
      e[i].frame = 0;
      e[i].maxFrames = 12;
      e[i].active = 1;
      return;
    }
  }
}

// spawnItem(): Tao tinh the roi tai vi tri (x, y) khi asteroid bi no.
void spawnItem(Item it[], int n, float x, float y)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!it[i].active)
    {
      it[i].x = x;
      it[i].y = y;
      it[i].vy = 1.5f;
      it[i].active = 1;
      it[i].radius = 13;
      it[i].sparkle = 0;
      return;
    }
  }
}

/* applyItemToShip(): Ap dung nang cap xoay vong khi nhat tinh the. */
void applyItemToShip(Ship *s)
{
  switch (gItemCycle % 3)
  {
  case 0:
    if (s->pelletsPerShot < MAX_PELLETS)
      s->pelletsPerShot++;
    break;
  case 1:
    if (s->bulletDamage < MAX_B_DAMAGE)
      s->bulletDamage++;
    break;
  case 2:
    s->shootCooldown -= 15;
    if (s->shootCooldown < MIN_SHOOT_CD)
      s->shootCooldown = MIN_SHOOT_CD;
    break;
  }
  gItemCycle++;
}

/*
 * handleBulletAsteroidCollisions(): Xu ly va cham DAN vs THIEN THACH.
 */
void handleBulletAsteroidCollisions(Bullet bul[], int nb, Asteroid ast[],
                                    int na, int *score, int level, Item items[],
                                    Explosion exps[])
{
  int i, j;
  for (i = 0; i < nb; i++)
  {
    if (!bul[i].active)
      continue;
    for (j = 0; j < na; j++)
    {
      if (!ast[j].active)
        continue;
      if (checkCircleCollision(bul[i].x, bul[i].y, (float)bul[i].radius,
                               ast[j].x, ast[j].y, (float)ast[j].radius))
      {
        ast[j].hp -= bul[i].damage;
        bul[i].active = 0;

        if (ast[j].hp <= 0)
        {
          int pts = 10;
          if (ast[j].radius <= 22)
            pts = 30;
          else if (ast[j].radius <= 35)
            pts = 20;
          *score += pts + ast[j].maxHp * 5;

          spawnExplosion(exps, MAX_EXPLOSIONS, ast[j].x, ast[j].y,
                         (float)ast[j].radius);
          if (rand() % 100 < ITEM_DROP_CHANCE)
            spawnItem(items, MAX_ITEMS, ast[j].x, ast[j].y);
          initAsteroid(&ast[j], level, &gCfg);
        }
        break;
      }
    }
  }
}

/*
 * handleShipAsteroidCollision(): Xu ly va cham PHI THUYEN vs THIEN THACH.
 */
void handleShipAsteroidCollision(Ship *s, Asteroid ast[], int na, int *hp,
                                 unsigned long *lastHit, Explosion exps[],
                                 int level)
{
  int j;
  unsigned long now = GetTickCount();
  for (j = 0; j < na; j++)
  {
    if (!ast[j].active)
      continue;
    if (checkCircleCollision(s->x, s->y, (float)s->radius, ast[j].x, ast[j].y,
                             (float)ast[j].radius))
    {
      if (now - *lastHit >= HIT_COOLDOWN)
      {
        *hp -= HIT_DAMAGE;
        if (*hp < 0)
          *hp = 0;
        *lastHit = now;
      }
      spawnExplosion(exps, MAX_EXPLOSIONS, ast[j].x, ast[j].y,
                     (float)ast[j].radius * 0.5f);
      initAsteroid(&ast[j], level, &gCfg);
    }
  }
}

/* handleItemPickup(): Kiem tra ship cham tinh the va nhat len. */
void handleItemPickup(Ship *s, Item it[], int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    if (!it[i].active)
      continue;
    if (checkCircleCollision(s->x, s->y, (float)s->radius, it[i].x, it[i].y,
                             (float)it[i].radius))
    {
      applyItemToShip(s);
      it[i].active = 0;
    }
  }
}

/*
 * applySkillDamage(): Ap dung sat thuong cua skill len asteroid.
 */
void applySkillDamage(SkillSystem *sk, Asteroid ast[], int na, int *score,
                      int level, Item items[], Explosion exps[])
{
  int j;
  /* Bomb Burst */
  if (sk->bombActive && !sk->bombDmgDone)
  {
    sk->bombDmgDone = 1;
    for (j = 0; j < na; j++)
    {
      if (!ast[j].active)
        continue;
      if (checkCircleCollision(sk->bombX, sk->bombY, sk->bombRadius, ast[j].x,
                               ast[j].y, (float)ast[j].radius))
      {
        ast[j].hp -= sk->bombDmg;
        if (ast[j].hp <= 0)
        {
          int pts = 10;
          if (ast[j].radius <= 22)
            pts = 30;
          else if (ast[j].radius <= 35)
            pts = 20;
          *score += pts + ast[j].maxHp * 5;
          spawnExplosion(exps, MAX_EXPLOSIONS, ast[j].x, ast[j].y,
                         (float)ast[j].radius);
          if (rand() % 100 < ITEM_DROP_CHANCE)
            spawnItem(items, MAX_ITEMS, ast[j].x, ast[j].y);
          initAsteroid(&ast[j], level, &gCfg);
        }
      }
    }
  }
  /* Piercing Beam */
  if (sk->beamActive && !sk->beamDmgDone)
  {
    sk->beamDmgDone = 1;
    for (j = 0; j < na; j++)
    {
      if (!ast[j].active)
        continue;
      if (beamHitsCircle(sk->beamSX, sk->beamSY, sk->beamDX, sk->beamDY,
                         ast[j].x, ast[j].y, (float)ast[j].radius))
      {
        ast[j].hp -= sk->beamDmg;
        if (ast[j].hp <= 0)
        {
          int pts = 10;
          if (ast[j].radius <= 22)
            pts = 30;
          else if (ast[j].radius <= 35)
            pts = 20;
          *score += pts + ast[j].maxHp * 5;
          spawnExplosion(exps, MAX_EXPLOSIONS, ast[j].x, ast[j].y,
                         (float)ast[j].radius);
          if (rand() % 100 < ITEM_DROP_CHANCE)
            spawnItem(items, MAX_ITEMS, ast[j].x, ast[j].y);
          initAsteroid(&ast[j], level, &gCfg);
        }
      }
    }
  }
}

/* checkLevelUp(): Kiem tra dieu kien len level. */
int checkLevelUp(int score, int level)
{
  return (score >= level * LEVEL_UP_SCORE) ? 1 : 0;
}

#endif
