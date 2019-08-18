#include<iostream>
#include "Game.h"
#include "Utilities.h"

Game::~Game()
{
	delete[] m_delayForBlocksFall;
	delete[] m_toBeDroppedDown;
	delete[] m_clearedLineTextPositions;
	delete[] m_nextPieces;
}

void Game::Init()
{
	m_currentFrame = 0;
	m_dt = 0;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	srand((int)time(NULL));
	m_window.create(sf::VideoMode(1600,900), "Tetris", sf::Style::Default, settings);
	m_view.setSize(sf::Vector2f(16.0f,9.0f));
	m_view.setCenter(sf::Vector2f(0.0f,0.0f));
	m_view.setRotation(0);
	m_window.setView(m_view);
	m_dpu = sf::Vector2f((float)m_window.getSize().x/m_view.getSize().x,(float)m_window.getSize().y/m_view.getSize().y);
	PopupText::m_dpu = m_dpu;
	
	m_bgTexture.loadFromFile("Resources/Graphics/bg2.jpg");
	m_bgSprite.setTexture(m_bgTexture);
	m_bgSprite.setScale(16.0f/m_bgSprite.getTexture()->getSize().x, 9.0f/m_bgSprite.getTexture()->getSize().y);
	m_bgSprite.setOrigin((sf::Vector2f(m_bgSprite.getTexture()->getSize())*0.5f));

	Piece::LoadFromFile();
	Piece::LoadTextures();
	LoadSoundBuffers();
	m_sound[PIECEROTATE].setVolume(25);
	m_currentState = GAME;
	memset(m_keyPressed, (int)false, sf::Keyboard::Key::KeyCount * sizeof(bool));
	memset(m_frameOnKeyPress, -1, sf::Keyboard::Key::KeyCount * sizeof(long long int));

	m_sideMoveAccumulator = 0.0f;
	m_timeBetweenSideMoves = 0.1f;
	m_delayFirstSideMove = 0.2f;//must not be 0

	m_maxTimeSeated = 0.75f;

	m_maxDelayForBlocksFall = 0.4f;

	m_linesCleared = 0;

	m_board.Init(sf::Vector2i(10,20),sf::Vector2f(-2.0f,-4.0f),sf::Vector2f(4.0f,8.0f));

	m_toBeDroppedDown = new bool[m_board.m_matrixSize.y];
	m_delayForBlocksFall = new float[m_board.m_matrixSize.y];
	memset(m_delayForBlocksFall,0,m_board.m_matrixSize.y*sizeof(float));
	memset(m_toBeDroppedDown, (int)false, m_board.m_matrixSize.y*sizeof(bool));

	m_clearedLineTextPositions = new bool[m_board.m_matrixSize.y];
	memset(m_clearedLineTextPositions,(int)false,4*sizeof(bool));

	ApplyBuffers();

	m_font.loadFromFile("Resources/arial.ttf");

	m_gameOver = false;
	SpawnPiece();
	m_nrNextPieces = 5;
	m_nextPieces = new Piece[m_nrNextPieces];
	for(int i = 0 ; i < m_nrNextPieces ; ++i)
	{
		m_nextPieces[i].Init(sf::Vector2i(m_board.m_matrixSize.x+4,3*i),(Piece::EType)(rand()%7),m_board.m_cellSize*0.8f,m_board.m_graphicPosition,false,true);
	}
	m_holdIsEmpty = true;
}

void Game::SpawnPiece()
{
	m_piece.Init(sf::Vector2i(m_board.m_matrixSize.x/2 - 2, -1),(Piece::EType)(rand()%7),m_board.m_cellSize,m_board.m_graphicPosition);
	m_pieceShadow.Init(m_piece.m_position,m_piece.m_type,m_board.m_cellSize,m_board.m_graphicPosition,true);
	DropDownPieceShadow();
	if(ComputeCollision() != Board::ECollisions::None)
		m_gameOver = true;
}

void Game::UpdateNextPieces()
{
	m_piece = m_nextPieces[0];
	m_piece.m_isStationary = false;
	m_piece.m_position = sf::Vector2i(m_board.m_matrixSize.x/2 - 2, -1);
	m_piece.m_cellGraphicalSize /= 0.8f;
	m_pieceShadow.Init(m_piece.m_position,m_piece.m_type,m_board.m_cellSize,m_board.m_graphicPosition,true);
	for(int i = 0 ; i < m_nrNextPieces-1 ; ++i)
	{
		m_nextPieces[i] = m_nextPieces[i+1];
		m_nextPieces[i].m_position = sf::Vector2i(m_board.m_matrixSize.x+4,3*i);
	}
	m_nextPieces[4].Init(sf::Vector2i(m_board.m_matrixSize.x+4,3*4),(Piece::EType)(rand()%7),m_board.m_cellSize*0.8f,m_board.m_graphicPosition,false,true);
	if(ComputeCollision() != Board::ECollisions::None)
		m_gameOver = true;
}

void Game::LoadSoundBuffers()
{
	char* fileNames[COUNT] = {"GameStart.ogg","GameOver.ogg","ClearLine.ogg",
								"PieceMove.ogg","PieceRotate.ogg","PieceSeated.ogg",
								"PieceImprint.ogg","PieceHardDrop.ogg",
								"ClearSingle.ogg","ClearDouble.ogg","ClearTriple.ogg","ClearQuad.ogg"};
	char fileLocation[] = "Resources/Sounds/";
	char fullFileLocation[200];
	for(int i = 0 ; i < COUNT ; ++i)
	{
		m_soundBuffers[i] = new sf::SoundBuffer();
		fullFileLocation[0]=0;
		strcat_s(fullFileLocation,fileLocation);
		strcat_s(fullFileLocation,fileNames[i]);
		m_soundBuffers[i]->loadFromFile(fullFileLocation);
	}
}

void Game::ApplyBuffers()
{
	for(int i = 0 ; i < ESoundBuffers::COUNT ; ++i)
		m_sound[i].setBuffer(*m_soundBuffers[i]);
}

bool Game::OnKeyPress(sf::Keyboard::Key key)
{
	if(m_frameOnKeyPress[key] == m_currentFrame)
		return true;
	if(!sf::Keyboard::isKeyPressed(key) && m_keyPressed[key])
	{
		m_keyPressed[key] = false;
	}
	if(sf::Keyboard::isKeyPressed(key) && !m_keyPressed[key])
	{
		m_frameOnKeyPress[key] = m_currentFrame;
		m_keyPressed[key] = true;
		return true;
	}
	return false;
}

bool Game::SideMoveOnKeyHold(sf::Keyboard::Key key, EMoveDirection direction)
{
	bool hasMoved = false;
		if(OnKeyPress(key))
	{
		m_sideMoveAccumulator = m_timeBetweenSideMoves;
		if(direction == EMoveDirection::LEFT)
			m_piece.m_position.x--;
		if(direction == EMoveDirection::RIGHT)
			m_piece.m_position.x++;
		if(direction == EMoveDirection::UP)
			m_piece.m_position.y--;
		if(direction == EMoveDirection::DOWN)
			m_piece.m_position.y++;
		hasMoved = true;
	}
	if(sf::Keyboard::isKeyPressed(key))
	{
		if(m_sideMoveAccumulator >= m_delayFirstSideMove + m_timeBetweenSideMoves)
		{
			hasMoved = true;
			if(direction == EMoveDirection::LEFT)
				m_piece.m_position.x--;
			if(direction == EMoveDirection::RIGHT)
				m_piece.m_position.x++;
			if(direction == EMoveDirection::UP)
				m_piece.m_position.y--;
			if(direction == EMoveDirection::DOWN)
				m_piece.m_position.y++;
			m_sideMoveAccumulator = m_delayFirstSideMove;
		}
		m_sideMoveAccumulator += m_dt;
	}
	return hasMoved;
}

bool Game::CanLineBeCleared(int line)
{
	bool isLineComplete = true;
	for(int j = 0 ; j < m_board.m_matrixSize.x ; ++ j)
	{
		if(m_board.At(sf::Vector2i(j,line)) == -1)
		{
			isLineComplete = false;
		}
	}
	return isLineComplete;
}

void Game::ClearLine(int line)
{
	for(int j = 0 ; j < m_board.m_matrixSize.x ; ++ j)
		m_board.At(sf::Vector2i(j,line)) = -1;
}

void Game::DropDownTheLinesAbove(int line)
{
	for(int j = 0 ; j < m_board.m_matrixSize.x ; ++ j)
	{
		for(int i = line ; i > 0 ; -- i)
		{
			m_board.At(sf::Vector2i(j,i)) = m_board.At(sf::Vector2i(j,i-1));
		}
	}
	for(int i = line ; i > 0 ; -- i)
	{
		m_delayForBlocksFall[i] = m_delayForBlocksFall[i-1];
		m_toBeDroppedDown[i] = m_toBeDroppedDown[i-1];
	}
	m_toBeDroppedDown[0] = false;
	m_delayForBlocksFall[0] = 0.0f;
}

void Game::DropDownPieceShadow()
{
	for(int i = 0 ; i < m_board.m_matrixSize.y ; ++ i)
	{
		m_pieceShadow.m_position.y++;
		if(m_board.ComputeCollision(m_pieceShadow) != Board::ECollisions::None)
		{
			m_pieceShadow.m_position.y--;
		}
	}
}

Board::ECollisions::Enum Game::ComputeCollision()
{
	return m_board.ComputeCollision(m_piece);
}

bool Game::IsPieceSeated()
{
	sf::Vector2i lastPosition = m_piece.m_position;
	m_piece.m_position.y++;
	bool isSeated = ComputeCollision() != Board::ECollisions::None;
	m_piece.m_position.y--;
	return isSeated;
}

void Game::Update(float dt)
{
	m_dt = dt;
	for(int key = sf::Keyboard::A ; key < sf::Keyboard::KeyCount ; ++key)
		OnKeyPress((sf::Keyboard::Key)key);
	switch(m_currentState)
	{
	case SPLASHSCREEN:
		{

		}

	case MENU:
		{

		}

	case GAME:
		{
			if(OnKeyPress(sf::Keyboard::Key::Space))
			{
				m_holdPiece.m_isStationary = false;
				m_holdPiece.m_position = m_piece.m_position;
				if(m_holdIsEmpty)
				{
					m_holdPiece = m_piece;
					UpdateNextPieces(); 
					m_holdIsEmpty = false;

				}
				else
				{
					Piece pieceAux = m_piece;
					m_piece = m_holdPiece;
					m_holdPiece = pieceAux;
					
				}
				m_holdPiece.m_isStationary = true;
				m_holdPiece.m_position = sf::Vector2i(-4,0);
				m_pieceShadow.Init(m_piece.m_position,m_piece.m_type,m_board.m_cellSize,m_board.m_graphicPosition,true);
			}
			
			sf::Vector2i lastPosition = m_piece.m_position;
			//Rotation
			bool clockwise = false;
			bool hasRotated = false;
			sf::Vector2f lastGeometricCenter = (sf::Vector2f)m_piece.m_position + m_piece.m_geometricCenter;
			if(OnKeyPress(sf::Keyboard::Key::Q))
			{
				m_piece.Rotate(false);
				clockwise = false;
				hasRotated = true;
			}
			if(OnKeyPress(sf::Keyboard::Key::E))
			{
				m_piece.Rotate(true);
				clockwise = true;
				hasRotated = true;
			}
			if(hasRotated)
			{
				bool foundGoodSpot = false;
				for(int r = 0 ; r <= 2 ; ++ r)
				{
					for(int offSetX = -r ; offSetX <= r ; ++ offSetX)
					{
						for(int offSetY = -r ; offSetY <= r; ++ offSetY)
						{
							m_piece.m_position = lastPosition + sf::Vector2i(offSetX,offSetY);
							//DropDownPieceShadow();
							sf::Vector2f geometricCenter = (sf::Vector2f)m_piece.m_position + m_piece.m_geometricCenter;
							if(Utilities::Distance(geometricCenter,lastGeometricCenter) < 1.6f)
							{
								if(ComputeCollision() == Board::ECollisions::None)
								{
									foundGoodSpot = true;
									break;
								}
							}
						}
						if(foundGoodSpot)
							break;
					}	
					if(foundGoodSpot)
						break;
				}
				if(!foundGoodSpot)
				{
					m_piece.m_position = lastPosition;
					DropDownPieceShadow();
					hasRotated = false;
					if(clockwise)
					{
						m_piece.Rotate(false);
					}
					else
					{
						m_piece.Rotate(true);
					}
				}
				if(hasRotated)
				{
					m_piece.AddTimeSeated(-0.05f);
					m_sound[PIECEROTATE].play();
				}
			}
			//Move Left-Right-Down
			lastPosition = m_piece.m_position;
			bool hasMoved = false;
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)^sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				hasMoved = hasMoved || SideMoveOnKeyHold(sf::Keyboard::Key::A,EMoveDirection::LEFT);
				hasMoved = hasMoved || SideMoveOnKeyHold(sf::Keyboard::Key::D,EMoveDirection::RIGHT);
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
			{
				hasMoved = hasMoved || SideMoveOnKeyHold(sf::Keyboard::Key::S,EMoveDirection::DOWN);
				m_piece.ResetFallTime();
			}

			//we check the value of hasMoved here before checking the collision so that the
			//PIECEMOVE sound plays at every key stroke.

			if(hasMoved)
			{
				m_sound[PIECEMOVE].play();
			}

			//hard-drop
			bool performedHardDrop = false;
			if(OnKeyPress(sf::Keyboard::Key::W))
			{
				for(int i = 0 ; i < m_board.m_matrixSize.y ; ++ i)
				{
					m_piece.m_position.y++;
					if(ComputeCollision() != Board::ECollisions::None)
					{
						m_piece.m_position.y--;
					}
				}
				performedHardDrop = true;
				m_sound[PIECEHARDDROP].play();
			}
			if(ComputeCollision() != Board::ECollisions::None)
			{
				m_piece.m_position = lastPosition;
			}
			lastPosition = m_piece.m_position;
			m_piece.Update(dt);

			if(ComputeCollision() != Board::ECollisions::None)
			{
				m_piece.m_position = lastPosition;
			}
			//Piece position is now final.
			m_pieceShadow.CopyTransform(m_piece);
			DropDownPieceShadow();
			if(!performedHardDrop)
			{
				if(IsPieceSeated())
				{
					m_piece.AddTimeSeated(dt);
					m_sound[PIECESEATED].setPitch(m_piece.GetTimeSeated()*(2.5f-1.0f)/m_maxTimeSeated+1.0f);
					if(m_sound[PIECESEATED].getStatus()!=sf::Sound::Status::Playing)
						m_sound[PIECESEATED].play();

					//std::cout<<m_piece.GetTimeSeated()<<std::endl;
				}
				else
				{
					if(m_piece.m_position.y - m_piece.m_lastSeatPosition.y >= 4)
					{
						m_piece.ResetTimeSeated();
						m_sound[PIECESEATED].setPitch(1.0f);
					}
				}
			}
			if(m_piece.GetTimeSeated() >= m_maxTimeSeated || performedHardDrop)
			{
				m_board.Imprint(m_piece);
				//m_sound[PIECEIMPRINT].play();
				if(ComputeCollision() == Board::ECollisions::Piece)
				{
					UpdateNextPieces();
					if(m_gameOver)
					{
						std::cout<<"gameOver"<<std::endl;
						m_currentState = EStates::GAMEEND;
						m_sound[ESoundBuffers::GAMEOVER].play();
					}
				}
			}
			int linesCleared = 0;
			for(int i = 0 ; i < m_board.m_matrixSize.y ; ++ i)
			{
				if(CanLineBeCleared(i))
				{
					ClearLine(i);
					m_clearedLineTextPositions[i] = true;
					linesCleared++;
									
					m_clearedLineText.Init(m_font, "Line!!!", m_board.m_graphicPosition + sf::Vector2f(m_board.m_graphicSize.x/2.0f, (0.5f + i) * m_board.m_cellSize.y), 1.0f, sf::Color::Yellow, m_board.m_cellSize.y);
					m_popups.push_back(m_clearedLineText);

					m_sound[CLEARLINE].play();
					m_toBeDroppedDown[i] = true;
				}
				if(m_toBeDroppedDown[i])
				{
					m_delayForBlocksFall[i]+= dt;
					if(m_delayForBlocksFall[i] >= m_maxDelayForBlocksFall)
					{
						DropDownTheLinesAbove(i);
					}
				}
			}

			for(unsigned int i = 0 ; i < m_popups.size() ; ++i )
			{
				if(linesCleared == 1)
				{
					m_popups[i].SetMessage("Line");
					m_popups[i].SetColor(sf::Color::Yellow);
				}
				if(linesCleared == 2)
				{
					m_popups[i].SetMessage("Double");
					m_popups[i].SetColor(sf::Color(100,200,200));
				}
				if(linesCleared == 3)
				{
					m_popups[i].SetMessage("Triple");
					m_popups[i].SetColor(sf::Color::Green);
				}
				if(linesCleared >= 4)
				{
					m_popups[i].SetMessage("TETRIS!!!");
					m_popups[i].SetColor(sf::Color::Red);
				}
			}

			for(unsigned int i = 0 ; i < m_popups.size() ; ++i)
			{
				m_popups[i].Update(dt);
			}
			int nrOfDeads = 0;
			for(unsigned int i = 0 ; i < m_popups.size() - nrOfDeads; ++i)
			{
				if(!m_popups[i].IsAlive())
				{
					nrOfDeads++;
					PopupText aux = m_popups[i];
					m_popups[i] = m_popups[m_popups.size() - nrOfDeads];
					m_popups[m_popups.size() - nrOfDeads] = aux;
					i--;
				}
			}
			m_popups.erase(m_popups.end() - nrOfDeads, m_popups.end());

			if(linesCleared == 1)
				m_sound[CLEARSINGLE].play();
			else if(linesCleared == 2)
				m_sound[CLEARDOUBLE].play();
			else if(linesCleared == 3)
				m_sound[CLEARTRIPLE].play();
			else if(linesCleared >= 4)
				m_sound[CLEARQUAD].play();
		}

	case GAMEEND:
		{

		}
	}
	m_currentFrame++;
}

void Game::Draw()
{
	switch(m_currentState)
	{
	case SPLASHSCREEN:
		{

		}
		
	case MENU:
		{

		}

	case GAME:
		{
			m_window.draw(m_bgSprite);
			m_board.Draw(m_window);
			m_piece.Draw(m_window);
			m_pieceShadow.Draw(m_window);
			m_holdPiece.Draw(m_window);
			for(int i = 0 ; i < m_nrNextPieces ; ++i)
			{
				m_nextPieces[i].Draw(m_window);
			}
			for(unsigned int i = 0 ; i < m_popups.size() ; ++i)
			{
				m_popups[i].Draw(m_window);
			}
		}

	case GAMEEND:
		{

		}
	}
}