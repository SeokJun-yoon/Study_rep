#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <windows.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>
using namespace std;
using namespace chrono;

#include "..\..\TermProjectServer\TermProjectServer\protocol.h"

sf::TcpSocket g_socket;
sf::Text userdata_text;

constexpr auto SCREEN_WIDTH = 20;
constexpr auto SCREEN_HEIGHT = 20;

constexpr auto TILE_WIDTH = 65;
constexpr auto WINDOW_WIDTH = TILE_WIDTH * SCREEN_WIDTH / 2 + 10;   // size of window
constexpr auto WINDOW_HEIGHT = TILE_WIDTH * SCREEN_WIDTH / 2 + 10;
constexpr auto BUF_SIZE = 200;
//constexpr auto MAX_USER = NPC_ID_START;

int g_left_x;
int g_top_y;
int g_myid;
float hp_rate;

sf::RenderWindow* g_window;
sf::Font g_font;


bool login_ok = false;
// Flags
bool is_attack = false;

sf::Text m_worldText[3];
high_resolution_clock::time_point m_worldtime_out[3];

sf::Text m_responText;
high_resolution_clock::time_point m_time_out_responText;

class OBJECT {
private:
	bool m_showing;
	sf::Sprite m_sprite;
	sf::Sprite m_sprite_attack;
	sf::Sprite m_sprite_damage;

	sf::RectangleShape Hp_greenbar;
	sf::RectangleShape Hp_redbar;
	sf::ConvexShape Level_UI;
	sf::CircleShape exp_UI;

	char m_mess[MAX_STR_LEN];	// for message
	high_resolution_clock::time_point m_time_out;
	sf::Text m_text;
	sf::Text m_name;

public:
	int m_x, m_y;
	char name[MAX_ID_LEN];
	int hp;
	int level;
	int exp;
	int id;
	int maxexp;
	int maxhp;
	int att;
	int attrange;

	//// Hp bar 관련 변수
	//int maxhp_rate = 0;

	OBJECT(sf::Texture& t, int x, int y, int x2, int y2) {
		m_showing = false;
		m_sprite.setTexture(t);
		m_sprite.setTextureRect(sf::IntRect(x, y, x2, y2));
		m_time_out = high_resolution_clock::now();
	}
	OBJECT() {
		m_showing = false;
		m_time_out = high_resolution_clock::now();
	}

	void set_move(sf::Texture& t, int x, int y, int x2, int y2)
	{
		m_sprite.setTexture(t);
		m_sprite.setTextureRect(sf::IntRect(x, y, x2, y2));
	}

	void set_attack(sf::Texture& t, int x, int y, int x2, int y2)
	{
		m_sprite_attack.setTexture(t);
		m_sprite_attack.setTextureRect(sf::IntRect(x, y, x2, y2));
	}

	void set_damage(sf::Texture& t, int x, int y, int x2, int y2)
	{
		m_sprite_damage.setTexture(t);
		m_sprite_damage.setTextureRect(sf::IntRect(x, y, x2, y2));
	}

	void show()
	{
		m_showing = true;
	}

	void hide()
	{
		m_showing = false;
	}

	void a_move(int x, int y) {
		m_sprite.setPosition((float)x, (float)y);
	}

	void a_draw() {
		g_window->draw(m_sprite);
	}

	void move(int x, int y) {
		m_x = x;
		m_y = y;
	}

	void draw() {
		if (false == m_showing) return;
		if (maxhp <= 0) return;

		float rx = (m_x - g_left_x) * 65.0f + 8;
		float ry = (m_y - g_top_y) * 65.0f + 8;
		m_sprite.setPosition(rx, ry);
		g_window->draw(m_sprite);
		m_name.setPosition(rx - 10, ry - 10);
		g_window->draw(m_name);
		if (high_resolution_clock::now() < m_time_out) {
			m_text.setPosition(rx - 10, ry + 15);
			g_window->draw(m_text);
		}
		if (is_attack == true)
		{
			m_sprite_attack.setPosition(rx + TILE_WIDTH, ry);
			g_window->draw(m_sprite_attack);

			m_sprite_attack.setPosition(rx - TILE_WIDTH, ry);
			g_window->draw(m_sprite_attack);

			m_sprite_attack.setPosition(rx, ry + TILE_WIDTH);
			g_window->draw(m_sprite_attack);

			m_sprite_attack.setPosition(rx, ry - TILE_WIDTH);
			g_window->draw(m_sprite_attack);
		}

		float hpValue = PLAYER_MAX_HP * ((float)hp / (float)maxhp);

		Hp_redbar.setPosition(sf::Vector2f(rx, ry - 50));
		Hp_redbar.setOutlineThickness(3.0f);
		Hp_redbar.setOutlineColor(sf::Color::Black);
		Hp_redbar.setFillColor(sf::Color(255, 0, 0, 100));
		Hp_redbar.setSize(sf::Vector2f(PLAYER_MAX_HP, 30));
		g_window->draw(Hp_redbar);

		Hp_greenbar.setPosition(sf::Vector2f(rx, ry - 50));
		Hp_greenbar.setOutlineThickness(3.0f);
		Hp_greenbar.setOutlineColor(sf::Color::Black);
		Hp_greenbar.setFillColor(sf::Color(0, 255, 0, 100));
		Hp_greenbar.setSize(sf::Vector2f(hpValue, 30));
		g_window->draw(Hp_greenbar);

		// maxhp = 50    | PLAYER_MAX_HP(100)  | hp = ?
		// maxhp = 100	 | PLAYER_MAX_HP(100)  | hp = ? 
		// maxhp = 150	 | PLAYER_MAX_HP(100)  | hp = ?


		//if (id < NPC_ID_START || (id>=NPC2_ID_START && id<NPC3_ID_START))
		//{
		//	Hp_redbar.setPosition(sf::Vector2f(rx, ry - 50));
		//	Hp_redbar.setOutlineThickness(3.0f);
		//	Hp_redbar.setOutlineColor(sf::Color::Black);
		//	Hp_redbar.setFillColor(sf::Color(255, 0, 0, 100));
		//	Hp_redbar.setSize(sf::Vector2f(PLAYER_MAX_HP, 30));
		//	g_window->draw(Hp_redbar);

		//	Hp_greenbar.setPosition(sf::Vector2f(rx, ry - 50));
		//	Hp_greenbar.setOutlineThickness(3.0f);
		//	Hp_greenbar.setOutlineColor(sf::Color::Black);
		//	Hp_greenbar.setFillColor(sf::Color(0, 255, 0, 100));
		//	Hp_greenbar.setSize(sf::Vector2f(a, 30));
		//	g_window->draw(Hp_greenbar);
		//}

		//if (id >= NPC_ID_START && id < NPC2_ID_START)
		//{
		//	Hp_redbar.setPosition(sf::Vector2f(rx, ry - 50));
		//	Hp_redbar.setOutlineThickness(3.0f);
		//	Hp_redbar.setOutlineColor(sf::Color::Black);
		//	Hp_redbar.setFillColor(sf::Color(255, 0, 0, 100));
		//	Hp_redbar.setSize(sf::Vector2f(PLAYER_MAX_HP, 30));
		//	g_window->draw(Hp_redbar);

		//	Hp_greenbar.setPosition(sf::Vector2f(rx, ry - 50));
		//	Hp_greenbar.setOutlineThickness(3.0f);
		//	Hp_greenbar.setOutlineColor(sf::Color::Black);
		//	Hp_greenbar.setFillColor(sf::Color(0, 255, 0, 100));
		//	Hp_greenbar.setSize(sf::Vector2f(a, 30));
		//	g_window->draw(Hp_greenbar);
		//}

		//if (id >= NPC3_ID_START && id < QUEST_NPC_NUMBER)
		//{
		//	Hp_redbar.setPosition(sf::Vector2f(rx, ry - 50));
		//	Hp_redbar.setOutlineThickness(3.0f);
		//	Hp_redbar.setOutlineColor(sf::Color::Black);
		//	Hp_redbar.setFillColor(sf::Color(255, 0, 0, 100));
		//	Hp_redbar.setSize(sf::Vector2f(PLAYER_MAX_HP, 30));
		//	g_window->draw(Hp_redbar);

		//	Hp_greenbar.setPosition(sf::Vector2f(rx, ry - 50));
		//	Hp_greenbar.setOutlineThickness(3.0f);
		//	Hp_greenbar.setOutlineColor(sf::Color::Black);
		//	Hp_greenbar.setFillColor(sf::Color(0, 255, 0, 100));
		//	Hp_greenbar.setSize(sf::Vector2f(a, 30));
		//	g_window->draw(Hp_greenbar);

		//}


		//Hp_redbar.setPosition(sf::Vector2f(rx, ry - 50));
		//Hp_redbar.setOutlineThickness(3.0f);
		//Hp_redbar.setOutlineColor(sf::Color::Black);
		//Hp_redbar.setFillColor(sf::Color(255, 0, 0, 100));
		//Hp_redbar.setSize(sf::Vector2f(PLAYER_MAX_HP, 30));
		//g_window->draw(Hp_redbar);

		//Hp_greenbar.setPosition(sf::Vector2f(rx, ry - 50));
		//Hp_greenbar.setOutlineThickness(3.0f);
		//Hp_greenbar.setOutlineColor(sf::Color::Black);
		//Hp_greenbar.setFillColor(sf::Color(0, 255, 0, 100));
		//Hp_greenbar.setSize(sf::Vector2f(hp, 30));
		//g_window->draw(Hp_greenbar);
	}
	void set_name(char str[]) {
		m_name.setFont(g_font);
		m_name.setString(str);
		m_name.setFillColor(sf::Color(255, 255, 255));
		m_name.setStyle(sf::Text::Bold);
		m_name.setCharacterSize(25);
	}

	void set_hp(char str[]) {
		m_name.setFont(g_font);
		m_name.setString(str);
		m_name.setFillColor(sf::Color(255, 255, 255));
		m_name.setStyle(sf::Text::Bold);
	}

	void set_level(char str[]) {
		m_name.setFont(g_font);
		m_name.setString(str);
		m_name.setFillColor(sf::Color(255, 255, 255));
		m_name.setStyle(sf::Text::Bold);
	}

	void set_exp(char str[]) {
		m_name.setFont(g_font);
		m_name.setString(str);
		m_name.setFillColor(sf::Color(255, 255, 0));
		m_name.setStyle(sf::Text::Bold);
	}

	void add_chat(char chat[]) {
		m_text.setFont(g_font);
		m_text.setString(chat);
		m_time_out = high_resolution_clock::now() + 1s;
	}

	void display_userdata()
	{
		char buf[100];
		userdata_text.setFont(g_font);
		sprintf_s(buf, "[ ID : %dP ]  [ HP : %d / %d ]  [ EXP : %d / %d ]  [ Level : %d ]", id, hp, maxhp, exp, maxexp, level);
		userdata_text.setString(buf);
		userdata_text.setPosition(30, 0);
		userdata_text.setCharacterSize(42);
		sf::Color color(255, 255, 255);
		userdata_text.setFillColor(color);
		userdata_text.setOutlineColor(sf::Color::Blue);
		userdata_text.setStyle(sf::Text::Underlined);
	}
};

OBJECT avatar;
unordered_map <int, OBJECT> npcs;

OBJECT map_tile;
OBJECT block_tile;
//OBJECT attack;
//OBJECT monster_1;
//OBJECT monster_2;
//OBJECT boss_monster;
//OBJECT quest_npc;
//OBJECT Player;


sf::Texture* player;
sf::Texture* maptile;
sf::Texture* blocktile;
sf::Texture* monster1;
sf::Texture* monster2;
sf::Texture* bossmonster;
sf::Texture* questnpc;
sf::Texture* attacktex;

void client_initialize()
{
	player = new sf::Texture;
	maptile = new sf::Texture;
	blocktile = new sf::Texture;
	monster1 = new sf::Texture;
	monster2 = new sf::Texture;
	bossmonster = new sf::Texture;
	questnpc = new sf::Texture;
	attacktex = new sf::Texture;

	if (false == g_font.loadFromFile("Galmuri7.ttf")) {
		cout << "Font Loading Error!\n";
		while (true);
	}

	player->loadFromFile("piece.png");
	maptile->loadFromFile("maptile.png");
	blocktile->loadFromFile("blocktile.png");
	monster1->loadFromFile("monster1.png");
	monster2->loadFromFile("monster2.png");
	bossmonster->loadFromFile("boss.png");
	questnpc->loadFromFile("questnpc.png");
	attacktex->loadFromFile("attack1.png");

	map_tile = OBJECT{ *maptile, 0, 0, TILE_WIDTH, TILE_WIDTH };
	block_tile = OBJECT{ *blocktile, 0, 0, TILE_WIDTH, TILE_WIDTH };

	for (int i = 0; i < WORLD_WIDTH; ++i) {
		for (int j = 0; j < WORLD_HEIGHT; ++j) {
			if (uid(dre) == 0)
				g_Map[i][j] = eBLOCKED;
			else
				g_Map[i][j] = eBLANK;
		}
	}

	avatar.set_move(*player, 0, 0, 64, 64);
	avatar.set_attack(*attacktex, 0, 0, 64, 64);
	avatar.move(4, 4);
}

void client_finish()
{
	delete player;
	delete maptile;
	delete blocktile;
	delete monster1;
	delete monster2;
	delete bossmonster;
	delete questnpc;
	delete attacktex;
}

//sf::Text userdata_text;
sf::Text messdata_text;
sf::Text enemymess_text;
sf::Text messdata2_text;

void ProcessPacket(char* ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case S2C_LOGIN_OK:
	{
		sc_packet_login_ok* my_packet = reinterpret_cast<sc_packet_login_ok*>(ptr);
		g_myid = my_packet->id;

		avatar.move(my_packet->x, my_packet->y);

		g_left_x = my_packet->x - (SCREEN_WIDTH / 2);
		g_top_y = my_packet->y - (SCREEN_HEIGHT / 2);
		avatar.id = my_packet->id;
		avatar.level = my_packet->level;
		avatar.exp = my_packet->exp;
		avatar.maxexp = my_packet->maxexp;
		avatar.hp = my_packet->hp;
		avatar.maxhp = my_packet->maxhp;
		
		avatar.display_userdata();
		//char buf[100];
		//userdata_text.setFont(g_font);
		//sprintf_s(buf, "[ ID : %dP ]  [ HP : %d / %d ]  [ EXP : %d / %d ]  [ Level : %d ]", avatar.id, avatar.hp, avatar.maxhp, avatar.exp, avatar.maxexp, avatar.level);

		//userdata_text.setString(buf);
		//userdata_text.setPosition(30, 0);
		//userdata_text.setCharacterSize(42);
		//sf::Color color(255, 255, 255);
		//userdata_text.setFillColor(color);
		//userdata_text.setOutlineColor(sf::Color::Blue);
		//userdata_text.setStyle(sf::Text::Underlined);

		avatar.show();

		login_ok = true;
	}
	break;

	case S2C_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;

		if (id == g_myid) {
			avatar.move(my_packet->x, my_packet->y);
			avatar.show();
		}
		else {
			if (id < NPC_ID_START)	// id값이 플레이어 일 경우
			{
				npcs[id] = OBJECT{ *player, 0, 0, 64, 64 };
				npcs[id].level = my_packet->level;
				npcs[id].hp = my_packet->hp;
				npcs[id].maxhp = my_packet->maxhp;
				npcs[id].exp = my_packet->exp;
				npcs[id].maxexp = my_packet->maxexp;
			}
			else if (id >= NPC_ID_START && id < NPC2_ID_START) // id값이 monster1일 경우
			{
				npcs[id] = OBJECT{ *monster1, 0, 0, 64, 64 };
				npcs[id].level = my_packet->level;
				npcs[id].hp = my_packet->hp;
				npcs[id].maxhp = my_packet->maxhp;
				npcs[id].exp = my_packet->exp;
				npcs[id].maxexp = my_packet->maxexp;
			}
			else if (id >= NPC2_ID_START && id < NPC3_ID_START) // id값이 monster2일 경우
			{
				npcs[id] = OBJECT{ *monster2, 0, 0, 64, 64 };
				npcs[id].level = my_packet->level;
				npcs[id].hp = my_packet->hp;
				npcs[id].maxhp = my_packet->maxhp;
				npcs[id].exp = my_packet->exp;
				npcs[id].maxexp = my_packet->maxexp;
			}
			else if (id >= NPC3_ID_START && id < NUM_NPC + MAX_USER) // id값이 monster3일 경우
			{
				npcs[id] = OBJECT{ *bossmonster, 0, 0, 64, 64 };
				npcs[id].level = my_packet->level;
				npcs[id].hp = my_packet->hp;
				npcs[id].maxhp = my_packet->maxhp;
				npcs[id].exp = my_packet->exp;
				npcs[id].maxexp = my_packet->maxexp;
			}
			else // id값이 questnpc일 경우
			{
				npcs[id] = OBJECT{ *questnpc, 0, 0, 64, 64 };
				npcs[id].level = my_packet->level;
				npcs[id].hp = my_packet->hp;
				npcs[id].maxhp = my_packet->maxhp;
				npcs[id].exp = my_packet->exp;
				npcs[id].maxexp = my_packet->maxexp;
			}
			strcpy_s(npcs[id].name, my_packet->name);
			npcs[id].set_name(my_packet->name);
			npcs[id].move(my_packet->x, my_packet->y);
			npcs[id].show();
		}
	}
	break;
	case S2C_MOVE:
	{
		sc_packet_move* my_packet = reinterpret_cast<sc_packet_move*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
			avatar.move(my_packet->x, my_packet->y);
			//attack.move(my_packet->x, my_packet->y);
			g_left_x = my_packet->x - (SCREEN_WIDTH / 2);
			g_top_y = my_packet->y - (SCREEN_HEIGHT / 2);
		}
		else {
			if (0 != npcs.count(other_id))
				npcs[other_id].move(my_packet->x, my_packet->y);
		}
	}
	break;

	case S2C_LEAVE:
	{
		sc_packet_leave* my_packet = reinterpret_cast<sc_packet_leave*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
			avatar.hide();
		}
		else {
			if (0 != npcs.count(other_id))
			{
				npcs[other_id].hide();
				npcs[other_id].hp = 0;
			}
		}
	}
	break;

	case S2C_CHAT:
	{
		sc_packet_chat *my_packet = reinterpret_cast<sc_packet_chat*>(ptr);
		int other_id = my_packet->id;
		int mess_type = my_packet->mess_type;
		char buf[100];
		messdata_text.setString(buf);
		messdata_text.setPosition(800, 1000);
		messdata_text.setCharacterSize(40);
		sf::Color color(255, 255, 255);
		messdata_text.setFillColor(color);
		messdata_text.setOutlineColor(sf::Color::Blue);
		messdata_text.setStyle(sf::Text::Underlined);

		if (mess_type == 0)	// exp, level, id 등의 player 정보 메시지
		{
			if (0 != npcs.count(other_id))
			{
				npcs[other_id].add_chat(my_packet->mess);
			}

			char buf[100];

			sprintf_s(buf, my_packet->mess);
			enemymess_text.setFont(g_font);
			enemymess_text.setString(buf);
			enemymess_text.setPosition(20, 400);
			enemymess_text.setCharacterSize(40);
			sf::Color color(255, 255, 255);
			enemymess_text.setFillColor(color);
			enemymess_text.setOutlineColor(sf::Color::Blue);
			enemymess_text.setStyle(sf::Text::Underlined);

		}

		else if (mess_type == 1)
		{
			npcs[other_id].add_chat(my_packet->mess);
			char buf[100];

			messdata2_text.setFont(g_font);
			messdata2_text.setString(buf);
			messdata2_text.setPosition(20, 0);
			messdata2_text.setCharacterSize(40);
			sf::Color color(0, 0, 0);
			messdata2_text.setFillColor(color);
			messdata2_text.setOutlineColor(sf::Color::Blue);
			messdata2_text.setStyle(sf::Text::Underlined);
		}
	}
	break;

	case S2C_LOGIN_FAIL:
	{
		sc_packet_login_fail* my_packet = reinterpret_cast<sc_packet_login_fail*>(ptr);
		int id = my_packet->id;
		if (id != g_myid) {
			cout << "Login Failed!!\n";
			cout << my_packet->message << endl;
		}
	}

	case S2C_CHANGE_STATS:
	{
		sc_packet_stat_change* my_packet = reinterpret_cast<sc_packet_stat_change*>(ptr);
		int id = my_packet->id;

		if (id == g_myid)
		{
			avatar.level = my_packet->level;
			avatar.hp = my_packet->hp;
			avatar.maxhp = my_packet->maxhp;
			avatar.exp = my_packet->exp;
			avatar.maxexp = my_packet->maxexp;
			avatar.att = my_packet->att;
			avatar.attrange = my_packet->attrange;

			avatar.display_userdata();
			avatar.show();
		}

		if (NPC_ID_START < id && id < QUEST_NPC_NUMBER)
		{
			npcs[id].level = my_packet->level;
			npcs[id].hp = my_packet->hp;
			npcs[id].maxhp = my_packet->maxhp;
			npcs[id].exp = my_packet->givenexp;
			npcs[id].att = my_packet->att;
		}
	}
	break;

	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);

	}
}

void process_data(char* net_buf, size_t io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[BUF_SIZE];

	while (0 != io_byte)
	{
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (io_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}
}

void client_main()
{
	char net_buf[BUF_SIZE];
	size_t	received;

	auto recv_result = g_socket.receive(net_buf, BUF_SIZE, received);
	if (recv_result == sf::Socket::Error)
	{
		wcout << L"Recv 에러!";
		while (true);
	}

	if (recv_result == sf::Socket::Disconnected)
	{
		wcout << L"서버 접속 종료.";
		g_window->close();
	}

	if (recv_result != sf::Socket::NotReady)
		if (received > 0) process_data(net_buf, received);

	if (login_ok == true)
	{
		for (int i = 0; i < SCREEN_WIDTH; ++i) {
			int tile_x = i + g_left_x;
			if (tile_x >= WORLD_WIDTH) break;
			if (tile_x < 0) continue;
			for (int j = 0; j < SCREEN_HEIGHT; ++j)
			{
				int tile_y = j + g_top_y;
				if (tile_y < 0) continue;
				if (tile_y >= WORLD_HEIGHT) break;
				// if (((tile_x + tile_y) % 2) == 0) {

				if (g_Map[tile_x][tile_y] == eBLOCKED)
				{
					block_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
					block_tile.a_draw();
				}

				else if (g_Map[tile_x][tile_y] == eBLANK)
				{
					map_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
					map_tile.a_draw();
				}
			}
		}
	}

	avatar.draw();

	for (auto& npc : npcs)
	{
		npc.second.draw();
		if (npc.second.hp <= 0)
			npc.second.hide();
		else
			npc.second.show();
	}

	sf::Text coordinate_text;
	coordinate_text.setFont(g_font);
	char buf[100];
	sprintf_s(buf, "Pos : (%d, %d)", avatar.m_x, avatar.m_y);
	coordinate_text.setString(buf);
	coordinate_text.setPosition(30, 70);
	coordinate_text.setCharacterSize(42);
	g_window->draw(coordinate_text);
	g_window->draw(userdata_text);
	g_window->draw(messdata_text);
	g_window->draw(messdata2_text);
}

void send_packet(void* packet)
{
	char* p = reinterpret_cast<char*>(packet);
	size_t sent;
	g_socket.send(p, p[0], sent);
}

void send_move_packet(unsigned char dir)
{
	cs_packet_move m_packet;
	m_packet.type = C2S_MOVE;
	m_packet.size = sizeof(m_packet);
	m_packet.direction = dir;
	send_packet(&m_packet);
}

void send_attack_packet()
{
	cs_packet_attack m_packet;
	m_packet.type = C2S_ATTACK;
	m_packet.size = sizeof(m_packet);
	send_packet(&m_packet);
}

void send_chat_packet(char* mess)
{
	cs_packet_chat m_packet;
	m_packet.type = C2S_CHAT;
	m_packet.size = sizeof(m_packet);
	strcpy_s(m_packet.message, mess);
	send_packet(&m_packet);
}

void send_logout_packet()
{
	cs_packet_logout m_packet;
	m_packet.type = C2S_LOGOUT;
	m_packet.size = sizeof(m_packet);
	send_packet(&m_packet);
}

int main()
{
	wcout.imbue(locale("korean"));
	sf::Socket::Status status = g_socket.connect("127.0.0.1", SERVER_PORT);
	g_socket.setBlocking(false);

	if (status != sf::Socket::Done) {
		wcout << L"서버와 연결할 수 없습니다.\n";
		while (true);
	}

	client_initialize();

	// Login
	cs_packet_login m_packet;
	m_packet.type = C2S_LOGIN;
	m_packet.size = sizeof(m_packet);
	int t_id = GetCurrentProcessId();
	char id[10];
	cout << "Input ID : ";
	cin >> id;
	strcpy_s(m_packet.name, id);
	sprintf_s(m_packet.name, "P%03d", t_id % 1000);
	avatar.set_name(m_packet.name);
	send_packet(&m_packet);
	//

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D CLIENT");
	g_window = &window;

	sf::View view = g_window->getView();
	view.zoom(2.0f);
	view.move(SCREEN_WIDTH * TILE_WIDTH / 4, SCREEN_HEIGHT * TILE_WIDTH / 4);
	g_window->setView(view);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {	// 키 누를 때
				int p_type = -1;
				switch (event.key.code) {
				case sf::Keyboard::Left:
					send_move_packet(D_LEFT);
					break;
				case sf::Keyboard::Right:
					send_move_packet(D_RIGHT);
					break;
				case sf::Keyboard::Up:
					send_move_packet(D_UP);
					break;
				case sf::Keyboard::Down:
					send_move_packet(D_DOWN);
					break;
				case sf::Keyboard::A:
					is_attack = true;
					send_attack_packet();
					break;
				case sf::Keyboard::Escape:
					window.close();

					break;
				}
			}
			if (event.type == sf::Event::KeyReleased)	// 키를 뗄 때
			{
				switch (event.key.code)
				{
				case sf::Keyboard::A:
					is_attack = false;
				}
			}
		}

		window.clear();
		client_main();
		window.display();
	}
	client_finish();

	return 0;
}