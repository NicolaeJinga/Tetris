#ifndef __GAME_H__
#define __GAME_H__

#include<iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Piece.h"
#include "Board.h"
#include "PopupText.h"

class Game
{
public:

	sf::RenderWindow m_window;
	sf::View m_view;
	int m_currentState;

	float m_dt;

	sf::Texture m_bgTexture;
	sf::Sprite  m_bgSprite;

	Board m_board;
	Piece m_piece;
	Piece m_pieceShadow;
	Piece m_holdPiece;
	Piece *m_nextPieces;
	int m_nrNextPieces;
	bool m_holdIsEmpty;
	
	bool m_keyPressed[sf::Keyboard::Key::KeyCount];
	long long int m_frameOnKeyPress[sf::Keyboard::Key::KeyCount];
	long long int m_currentFrame;
	float m_sideMoveAccumulator;
	float m_timeBetweenSideMoves;
	float m_delayFirstSideMove;
	
	float m_maxTimeSeated;

	float* m_delayForBlocksFall;
	bool* m_toBeDroppedDown;
	float m_maxDelayForBlocksFall;


	int m_linesCleared;

	PopupText m_clearedLineText;
	bool* m_clearedLineTextPositions;

	enum ESoundBuffers
	{
		GAMESTART = 0,
		GAMEOVER,
		CLEARLINE,
		PIECEMOVE,
		PIECEROTATE,
		PIECESEATED,
		PIECEIMPRINT,
		PIECEHARDDROP,

		CLEARSINGLE,
		CLEARDOUBLE,
		CLEARTRIPLE,
		CLEARQUAD,

		COUNT
	};

	sf::SoundBuffer* m_soundBuffers[ESoundBuffers::COUNT];
	sf::Sound m_sound[ESoundBuffers::COUNT];

	enum EStates
	{
		SPLASHSCREEN = 0,
		MENU,
		GAME,
		GAMEEND
	};

	enum EMoveDirection
	{
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
	};

	sf::Vector2f m_dpu;
	bool m_gameOver;
	sf::Font m_font;
	std::vector<PopupText> m_popups;

	~Game();
	void Init();
	void SpawnPiece();
	void UpdateNextPieces();
	void LoadSoundBuffers();
	void ApplyBuffers();
	bool OnKeyPress(sf::Keyboard::Key key);
	bool SideMoveOnKeyHold(sf::Keyboard::Key key, EMoveDirection direction);
	bool CanLineBeCleared(int line);
	void ClearLine(int line);
	void DropDownTheLinesAbove(int line);
	void DropDownPieceShadow();
	Board::ECollisions::Enum ComputeCollision();
	bool IsPieceSeated();
	void Update(float dt);
	void Draw();
};

#endif // __GAME_H__