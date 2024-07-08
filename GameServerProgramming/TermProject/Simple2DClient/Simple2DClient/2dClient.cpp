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

sf::RenderWindow* g_window;
sf::Font g_font;

// Flags
bool is_attack = false;

class OBJECT {
private:
	bool m_showing;
	sf::Sprite m_sprite;
	sf::Sprite m_sprite_attack;
	//sf::Sprite m_sprite_damage;

	char m_mess[MAX_STR_LEN];	// for message
	high_resolution_clock::time_point m_time_out;
	sf::Text m_text;
	sf::Text m_name;

public:
	int m_x, m_y;
	char name[MAX_ID_LEN];
	short hp;
	short level;
	short exp;
	short id;

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

	void set_attack(sf::Texture& t, int x, int y, int x2, int y2)
	{
		m_sprite_attack.setTexture(t);
		m_sprite_attack.setTextureRect(sf::IntRect(x, y, x2, y2));
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

			m_sprite_attack.setPosition(rx, ry +TILE_WIDTH);
			g_window->draw(m_sprite_attack);

			m_sprite_attack.setPosition(rx, ry - TILE_WIDTH);
			g_window->draw(m_sprite_attack);
		}
	}
	void set_name(char str[]) {
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
};

OBJECT avatar;
unordered_map <int, OBJECT> npcs;

OBJECT map_tile;
OBJECT block_tile;
OBJECT monster_1;
OBJECT monster_2;
OBJECT boss_monster;
OBJECT quest_npc;
OBJECT attack;


sf::Texture* piece;
sf::Texture* maptile;
sf::Texture* blocktile;
sf::Texture* monster1;
sf::Texture* monster2;
sf::Texture* bossmonster;
sf::Texture* questnpc;
sf::Texture* attacktex;

void client_initialize()
{
	piece = new sf::Texture;
	maptile = new sf::Texture;
	blocktile = new sf::Texture;
	monster1 = new sf::Texture;
	monster2 = new sf::Texture;
	bossmonster = new sf::Texture;
	questnpc = new sf::Texture;
	attacktex = new sf::Texture;

	if (false == g_font.loadFromFile("cour.ttf")) {
		cout << "Font Loading Error!\n";
		while (true);
	}
	piece->loadFromFile("piece.png");
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
	
	avatar = OBJECT{ *piece, 0, 0, 64, 64 };

	avatar.set_attack(*attacktex, 0, 0, 64, 64);
	avatar.move(4, 4);
}

void client_finish()
{
	delete piece;
	delete maptile;
	delete blocktile;
	delete monster1;
	delete monster2;
	delete bossmonster;
	delete questnpc;
	delete attacktex;
}

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
		avatar.show();
	}
	break;

	case S2C_ENTER:
	{
		sc_packet_enter* my_packet = reinterpret_cast<sc_packet_enter*>(ptr);
		int id = my_packet->id;

		if (id == g_myid) {
			avatar.move(my_packet->x, my_packet->y);
			//g_left_x = my_packet->x - (SCREEN_WIDTH / 2);
			//g_top_y = my_packet->y - (SCREEN_HEIGHT / 2);
			avatar.show();
		}
		else {
			if (id < NPC_ID_START)
				npcs[id] = OBJECT{ *piece, 0, 0, 64, 64 };
			else if (id >= NPC_ID_START && id < NPC2_ID_START)
				npcs[id] = OBJECT{ *monster1, 0, 0, 64, 64 };
			else if (id >= NPC2_ID_START && id < NPC3_ID_START)
				npcs[id] = OBJECT{ *monster2, 0, 0, 64, 64 };
			else if (id >= NPC3_ID_START && id < NUM_NPC + MAX_USER)
				npcs[id] = OBJECT{ *bossmonster, 0, 0, 64, 64 };
			else
				npcs[id] = OBJECT{ *questnpc, 0, 0, 64, 64 };
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
			attack.move(my_packet->x, my_packet->y);
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
				npcs[other_id].hide();
		}
	}
	break;
	case S2C_CHAT:
	{
		sc_packet_chat *my_packet = reinterpret_cast<sc_packet_chat*>(ptr);
		int o_id = my_packet->id;
		if (0!=npcs.count(o_id))
		{
			npcs[o_id].add_chat(my_packet->mess);
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

	while (0 != io_byte) {
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
			
			if (g_Map[tile_x][tile_y] == eBLOCKED) {
				block_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
				block_tile.a_draw();
			}
			
			else if (g_Map[tile_x][tile_y]==eBLANK)
			{
				map_tile.a_move(TILE_WIDTH * i + 7, TILE_WIDTH * j + 7);
				map_tile.a_draw();
			}
		}
	}
	avatar.draw();
	//	for (auto &pl : players) pl.draw();
	for (auto& npc : npcs) npc.second.draw();
	sf::Text text;
	text.setFont(g_font);
	char buf[100];
	sprintf_s(buf, "(%d, %d)", avatar.m_x, avatar.m_y);
	text.setString(buf);
	g_window->draw(text);

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

	cs_packet_login l_packet;
	l_packet.size = sizeof(l_packet);
	l_packet.type = C2S_LOGIN;
	int t_id = GetCurrentProcessId();
	sprintf_s(l_packet.name, "P%03d", t_id % 1000);
	strcpy_s(avatar.name, l_packet.name);
	avatar.set_name(l_packet.name);
	send_packet(&l_packet);

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH , WINDOW_HEIGHT), "2D CLIENT");
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