#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <ctime>
#include <time.h>
#include <cctype>

#define setPosition(x, y) setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)))
#define setRotation(a) setRotation(sf::degrees(static_cast<float>(a)))
#define VideoMode(w, h) VideoMode(sf::Vector2u(static_cast<unsigned int>(w), static_cast<unsigned int>(h)))

using namespace std;

struct LegacyRect {
	int left;
	int top;
	int width;
	int height;

	bool contains(int x, int y) const {
		return x >= left && x <= (left + width) && y >= top && y <= (top + height);
	}
};

class Game {
protected:
	int tries;
public:
	static int max_tries;
	static int losses;
	static int wins;
	int isLost() {
		if (tries > max_tries) {
			return 1;
		}
		else {
			return 0;
		}
	}
};

int Game::max_tries = 6;
int Game::wins = 0;
int Game::losses = 0;

class Hangman :public Game {
private:
	string correctString;
	string guessedString;
public:
	string hint;
	Hangman() {
		tries = 0;
	}
	int getTries() {
		return tries;
	}
	void setTries(int number) {
		tries = number;
	}
	void increaseTries() {
		setTries(getTries() + 1);
	}
	string getCorrectString() {
		return correctString;
	}
	void setCorrectString(string word) {
		correctString = word;
	}
	string getGuessedString() {
		return guessedString;
	}
	void setGuessedString() {
		guessedString = "";
		for (int i = 0; i < correctString.length(); i++) {
			guessedString = guessedString + "_";
		}
	}
	void setGuessedString(string word) {
		guessedString = word;
	}

	bool isWon() {
		if (guessedString == correctString) {
			return true;
		}
		else {
			return false;
		}
	}
	bool guessWord(char letter) {
		bool correctness = false;
		for (int i = 0; i < correctString.length(); i++) {
			if (letter == correctString[i]) {
				int sizeOfString = correctString.length();
				guessedString = guessedString.substr(0, i) + correctString.substr(i, 1) + guessedString.substr(i + 1, sizeOfString - (i + 1));
				correctness = true;
			}
		}
		return correctness;
	}
};
int main()
{
	srand(time(0));
	string words[] = { "PRIVACY","INTEGER","TERNARY","SOFTWARE","COMPILER" };
	string hints[] = { "keeping yourself and your personal information data safe when off and online","a data type in programming languages, representing a whole number positive or negative","numbers expressed in the base of three, instead of 10, using 0, 1 and 2 only","It is a collection of data or computer instructions that tell the computer how to work","It is a program that translates source code into object code" };
	Hangman hg;
	int c = (rand() % 5);
	hg.hint = hints[c];
	hg.setCorrectString(words[c]);
	hg.setGuessedString();

	bool playing = false;

	int positions[20][2];
	bool choice[40];
	for (int i = 0; i < 40; i++) {
		choice[i] = false;
	}
	sf::RenderWindow window(sf::VideoMode(900, 900), "HANGMAN!");

	sf::Texture texture;
	if (!texture.loadFromFile("images\\squared-paper.jpg"))
		return EXIT_FAILURE;
	sf::Sprite sprite(texture);

	sf::Font font;
	font.openFromFile("Fonts\\GreatVibes-Regular.otf");

	sf::Font font1;
	font1.openFromFile("Fonts\\Montserrat-Light.otf");

	sf::SoundBuffer buffer;
	buffer.loadFromFile("Audio\\Wrong-answer-sound-effect.wav");
	sf::Sound sound(buffer);

	sf::SoundBuffer buffer1;
	if (!buffer1.loadFromFile("Audio\\Vip.wav"))
		return -1;
	sf::Sound soundintro(buffer1);

	sf::SoundBuffer buffer2;
	if (!buffer2.loadFromFile("Audio\\wrong.wav"))
		return -1;
	sf::Sound soundwrong(buffer2);

	sf::SoundBuffer buffer3;
	if (!buffer3.loadFromFile("Audio\\correct.wav"))
		return -1;
	sf::Sound soundcorrect(buffer3);



	// Create a text
	sf::Text text(font, "Hangman");
	text.setCharacterSize(120);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::Red);
	text.setPosition(200, 0);

	sf::Text text1(font, "Play");
	text1.setCharacterSize(60);
	text1.setStyle(sf::Text::Underlined);
	text1.setStyle(sf::Text::Bold);
	text1.setFillColor(sf::Color::Red);
	LegacyRect r1{ 370, 500, 120, 60 };
	text1.setPosition(370, 500);

	sf::Text text2(font, "Rules");
	text2.setCharacterSize(60);
	text2.setStyle(sf::Text::Underlined);
	text2.setStyle(sf::Text::Bold);
	text2.setFillColor(sf::Color::Red);
	LegacyRect r2{ 370, 600, 140, 60 };
	text2.setPosition(370, 600);

	sf::Text text3(font, "Close");
	text3.setCharacterSize(60);
	text3.setStyle(sf::Text::Underlined);
	text3.setStyle(sf::Text::Bold);
	text3.setFillColor(sf::Color::Red);
	LegacyRect r3{ 370, 700, 120, 60 };
	text3.setPosition(370, 700);

	sf::Text text4(font1, hg.getGuessedString());
	text4.setCharacterSize(80);
	text4.setStyle(sf::Text::Underlined);
	text4.setStyle(sf::Text::Bold);
	text4.setFillColor(sf::Color::Red);
	text4.setPosition(1000, 00);

	sf::Text text5(font1, "Guess the word");
	text5.setCharacterSize(60);
	text5.setStyle(sf::Text::Underlined);
	text5.setStyle(sf::Text::Bold);
	text5.setFillColor(sf::Color::Red);
	text5.setPosition(1000, 00);

	sf::Text text6(font, "Rules");
	text6.setCharacterSize(120);
	text6.setStyle(sf::Text::Underlined);
	text6.setStyle(sf::Text::Bold);
	text6.setFillColor(sf::Color::Red);
	text6.setPosition(1000, 00);

	string s = "Hints will be displayed for the word. Press the letter\n to complete the word to be guessed. If the letter\n you typed is right than it will appear in the string\n of word else the hangman will start to build . \nDifferent sounds of right and wrong typed letters will\n be played . If  the user guessed it right than he/she\n will win and loose if all the body parts of hangman\n are displayed and game continues till the word\n is guessed fully . Quit the game by clicking close\n button.";

	sf::Text text7(font1, s);
	text7.setCharacterSize(30);
	text7.setStyle(sf::Text::Underlined);
	text7.setStyle(sf::Text::Bold);
	text7.setFillColor(sf::Color::Red);
	text7.setPosition(1000, 00);

	sf::Text text8(font1, "Hint: " + hg.hint.substr(0, 58) + "\n" + hg.hint.substr(58, 58));
	text8.setCharacterSize(22);
	text8.setStyle(sf::Text::Underlined);
	text8.setStyle(sf::Text::Bold);
	text8.setFillColor(sf::Color::Red);
	text8.setPosition(1000, 00);

	sf::Text text9(font1, "Wins: " + to_string(hg.wins) + "\nLosses: " + to_string(hg.losses));
	text9.setCharacterSize(70);
	text9.setStyle(sf::Text::Underlined);
	text9.setStyle(sf::Text::Bold);
	text9.setFillColor(sf::Color::Green);
	text9.setPosition(1000, 00);

	sf::CircleShape circle(30);
	circle.setTexture(&texture);
	circle.setOutlineColor(sf::Color::Black);
	circle.setOutlineThickness(5);
	circle.setPosition(410, 200);

	sf::RectangleShape rectangle[20];
	rectangle[0].setSize(sf::Vector2f(5, 100));
	rectangle[0].setTexture(&texture);
	rectangle[0].setOutlineColor(sf::Color::Black);
	rectangle[0].setOutlineThickness(5);
	rectangle[0].setPosition(437, 270);//437,270

	rectangle[1].setSize(sf::Vector2f(5, 50));
	rectangle[1].setTexture(&texture);
	rectangle[1].setRotation(45);
	rectangle[1].setOutlineColor(sf::Color::Black);
	rectangle[1].setOutlineThickness(5);
	rectangle[1].setPosition(427, 295);//427,295

	rectangle[2].setSize(sf::Vector2f(5, 50));
	rectangle[2].setTexture(&texture);
	rectangle[2].setPosition(427, 295);
	rectangle[2].setRotation(315);
	rectangle[2].setOutlineColor(sf::Color::Black);
	rectangle[2].setOutlineThickness(5);
	rectangle[2].setPosition(450, 297);//450,297

	rectangle[3].setSize(sf::Vector2f(5, 75));
	rectangle[3].setTexture(&texture);
	rectangle[3].setRotation(15);
	rectangle[3].setOutlineColor(sf::Color::Black);
	rectangle[3].setOutlineThickness(5);
	rectangle[3].setPosition(427, 365);//427,365

	rectangle[4].setSize(sf::Vector2f(5, 75));
	rectangle[4].setTexture(&texture);
	rectangle[4].setRotation(345);
	rectangle[4].setOutlineColor(sf::Color::Black);
	rectangle[4].setOutlineThickness(5);
	rectangle[4].setPosition(450, 367);//450,367

	rectangle[5].setSize(sf::Vector2f(5, 125));
	rectangle[5].setTexture(&texture);
	rectangle[5].setOutlineColor(sf::Color::Black);
	rectangle[5].setOutlineThickness(5);
	rectangle[5].setPosition(300, 340);//300,340

	rectangle[6].setSize(sf::Vector2f(5, 125));
	rectangle[6].setTexture(&texture);
	rectangle[6].setOutlineColor(sf::Color::Black);
	rectangle[6].setOutlineThickness(5);
	rectangle[6].setPosition(300, 210);//300,210

	rectangle[7].setSize(sf::Vector2f(5, 125));
	rectangle[7].setTexture(&texture);
	rectangle[7].setRotation(270);
	rectangle[7].setOutlineColor(sf::Color::Black);
	rectangle[7].setOutlineThickness(5);
	rectangle[7].setPosition(305, 278);//300,277

	positions[0][0] = 437; positions[0][1] = 270;
	positions[1][0] = 427; positions[1][1] = 295;
	positions[2][0] = 450; positions[2][1] = 297;
	positions[3][0] = 427; positions[3][1] = 365;
	positions[4][0] = 450; positions[4][1] = 367;
	positions[5][0] = 300; positions[5][1] = 340;
	positions[6][0] = 300; positions[6][1] = 210;
	positions[7][0] = 305; positions[7][1] = 278;
	positions[8][0] = 410; positions[8][1] = 200;
	positions[9][0] = 370; positions[9][1] = 500;
	positions[10][0] = 370; positions[10][1] = 600;
	positions[11][0] = 370; positions[11][1] = 700;
	positions[12][0] = 220; positions[12][1] = 700;
	positions[13][0] = 150; positions[13][1] = 600;

	soundintro.play();


	while (window.isOpen())
	{

		while (const std::optional<sf::Event> event = window.pollEvent())
		{

			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}

			if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (r1.contains(mouseButton->position.x, mouseButton->position.y)) {
					std::cout << "Play Button pressed\n";
					c = (rand() % 5);
					hg.hint = hints[c];
					hg.setCorrectString(words[c]);
					hg.setGuessedString();
					hg.setTries(0);
					text8.setString("Hint: " + hg.hint.substr(0, 58) + "\n" + hg.hint.substr(58, 58));
					text4.setString(hg.getGuessedString());

					for (int i = 0; i < 40; i++) {
						choice[i] = false;
					}
					rectangle[0].setPosition(1000, 0);
					rectangle[1].setPosition(1000, 0);
					rectangle[2].setPosition(1000, 0);
					rectangle[3].setPosition(1000, 0);
					rectangle[4].setPosition(1000, 0);
					rectangle[5].setPosition(1000, 0);
					rectangle[6].setPosition(1000, 0);
					rectangle[7].setPosition(1000, 0);
					circle.setPosition(1000, 0);
					text1.setPosition(1000, 0);
					r1.left = 1000;
					text2.setPosition(1000, 0);
					r2.left = 1000;
					text3.setPosition(1000, 0);
					r3.left = 1000;
					text9.setPosition(1000, 0);
					text4.setCharacterSize(80);
					text5.setString("Guess the Word!");
					text6.setPosition(1000, 0);
					text7.setPosition(1000, 0);
					text4.setPosition(positions[12][0], positions[12][1]);
					text5.setPosition(positions[13][0], positions[13][1]);
					text8.setPosition(50, 500);
					soundintro.pause();
					soundcorrect.play();
					playing = true;

				}
				if (r2.contains(mouseButton->position.x, mouseButton->position.y)) {
					std::cout << "Rules Button pressed\n";
					rectangle[0].setPosition(1000, 0);
					rectangle[1].setPosition(1000, 0);
					rectangle[2].setPosition(1000, 0);
					rectangle[3].setPosition(1000, 0);
					rectangle[4].setPosition(1000, 0);
					rectangle[5].setPosition(1000, 0);
					rectangle[6].setPosition(1000, 0);
					rectangle[7].setPosition(1000, 0);
					circle.setPosition(1000, 0);
					text2.setPosition(1000, 0);
					r2.left = 1000;
					text3.setPosition(1000, 0);
					r3.left = 1000;
					text6.setPosition(310, 150);
					text7.setPosition(50, 300);
					text1.setPosition(370, 650);
					r1.top = 650;
					soundcorrect.play();

				}
				if (r3.contains(mouseButton->position.x, mouseButton->position.y)) {
					std::cout << "Exit Button pressed\n";
					sound.play();
					window.close();
				}

			}
			if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
			{
				if (playing) {
					if (textEntered->unicode < 128)
						if (isalpha(static_cast<unsigned char>(textEntered->unicode))) {
							const char typedCharacter = static_cast<char>(textEntered->unicode);
							const char upperCharacter = static_cast<char>(toupper(static_cast<unsigned char>(typedCharacter)));
							const int letterIndex = upperCharacter - 'A';

							std::cout << "ASCII character typed: " << typedCharacter << std::endl;
							std::cout << "ASCII character typed: " << static_cast<unsigned int>(textEntered->unicode) << std::endl;
							if (choice[letterIndex] == true) {
								std::cout << "Already guessed\n";
							}
							else {
								choice[letterIndex] = true;
								if (hg.guessWord(upperCharacter)) {
									std::cout << "Correct Choice\n";
									soundcorrect.play();
									text4.setString(hg.getGuessedString());
									if (hg.isWon()) {
										text5.setString("You Won!!!!");
										text4.setCharacterSize(50);
										text4.setPosition(120, positions[12][1]);
										text4.setString("You guessed \"" + hg.getCorrectString() + "\"");
										soundintro.play();
										playing = false;
										text1.setString("Play Again!");
										text1.setPosition(300, 800);
										r1.top = 800;
										r1.left = 300;
										hg.wins++;
										cout << "Losses=" << hg.losses << endl;
										cout << "Wins=" << hg.wins << endl;
										text9.setString("Wins: " + to_string(hg.wins) + "\nLosses: " + to_string(hg.losses));
										text9.setPosition(550, 200);
									}
								}
								else {
									soundwrong.play();
									std::cout << "Wrong choice\n";
									hg.increaseTries();
									if (hg.getTries() == 1) {
										rectangle[5].setPosition(positions[5][0], positions[5][1]);
									}
									if (hg.getTries() == 2) {
										rectangle[6].setPosition(positions[6][0], positions[6][1]);
									}
									if (hg.getTries() == 3) {
										rectangle[7].setPosition(positions[7][0], positions[7][1]);
									}
									if (hg.getTries() == 4) {
										circle.setPosition(positions[8][0], positions[8][1]);
									}
									if (hg.getTries() == 5) {
										rectangle[0].setPosition(positions[0][0], positions[0][1]);
									}
									if (hg.getTries() == 6) {
										rectangle[1].setPosition(positions[1][0], positions[1][1]);
										rectangle[2].setPosition(positions[2][0], positions[2][1]);
									}
									if (hg.getTries() == 7) {
										rectangle[3].setPosition(positions[3][0], positions[3][1]);
										rectangle[4].setPosition(positions[4][0], positions[4][1]);
									}
									if (hg.isLost()) {
										text5.setString("You Lost!!!!");
										text4.setCharacterSize(50);
										text4.setPosition(120, positions[12][1]);
										text4.setString("You didnt guess \"" + hg.getCorrectString() + "\"");
										playing = false;
										text1.setString("Play Again!");
										text1.setPosition(300, 800);
										soundintro.play();
										r1.top = 800;
										r1.left = 300;
										hg.losses++;
										cout << "Losses=" << hg.losses << endl;
										cout << "Wins=" << hg.wins << endl;
										text9.setString("Wins: " + to_string(hg.wins) + "\nLosses: " + to_string(hg.losses));
										text9.setPosition(550, 200);

									}


								}
							}
						}
				}
			}

		}

		window.clear();

		window.draw(sprite);

		window.draw(circle);

		window.draw(text);
		window.draw(text1);
		window.draw(text2);
		window.draw(text3);
		window.draw(text4);
		window.draw(text5);
		window.draw(text6);
		window.draw(text7);
		window.draw(text8);
		window.draw(text9);

		window.draw(rectangle[0]);
		window.draw(rectangle[1]);
		window.draw(rectangle[2]);
		window.draw(rectangle[3]);
		window.draw(rectangle[4]);
		window.draw(rectangle[5]);
		window.draw(rectangle[6]);
		window.draw(rectangle[7]);

		window.display();
	}

	return 0;
}
