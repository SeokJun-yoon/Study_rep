#pragma once

enum class WORKER_OP { OP_RECV, OP_SEND, OP_ACCEPT, OP_RANDOM_MOVE, OP_PLAYER_MOVE, OP_CHECK_CONNECT_TIME, OP_DB_LOAD_CHARACTER_SUCCESS };

enum class ObjectType { OT_Player, OT_Monster1, OT_Monster2, OT_Monster3, OT_NPC };
enum class ObjectStatus { OS_Free, OS_Alloc, OS_Active, OS_Sleep };

enum class PlayerMaxExp { PME_Level1 = 20, PME_Level2 = 40, PME_Level3 = 60 };
enum class PlayerAttackData { PAD_Level1 = 10, PAD_Level2 = 15, PAD_Level3 = 20 };

enum class MonsterMaxHp { MMH_Level1 = 50, MMH_Level2 = 100, MMH_Level3 = 150 };
enum class MonsterGivenExp { MGE_Level1 = 5, MGE_Level2 = 8, MGE_Level3 = 10 };
enum class MonsterAttackData { MAD_Level1 = 5, MAD_Level2 = 10, MAD_Level3 = 15 };
