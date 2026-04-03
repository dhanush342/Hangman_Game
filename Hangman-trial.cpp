#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <vector>
#include <unordered_set>
#include <random>
#include <cctype>
#include <stdexcept>
#include <cstdlib>

#define setPosition(x, y) setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)))
#define setRotation(a) setRotation(static_cast<float>(a))
#define VideoMode(w, h) VideoMode(static_cast<unsigned int>(w), static_cast<unsigned int>(h))

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

struct WordEntry {
	string word;
	string hint;
};

const vector<WordEntry> dictionary = {
	{"PRIVACY", "keeping yourself and your personal information data safe when off and online"},
	{"INTEGER", "a data type in programming languages, representing a whole number positive or negative"},
	{"TERNARY", "numbers expressed in the base of three, instead of 10, using 0, 1 and 2 only"},
	{"SOFTWARE", "It is a collection of data or computer instructions that tell the computer how to work"},
	{"COMPILER", "It is a program that translates source code into object code"}
};

template <typename Str, typename CharT>
bool revealLetters(Str& guessed, const Str& correct, CharT letter) {
	bool found = false;
	for (size_t i = 0; i < correct.size(); ++i) {
		if (correct[i] == letter) {
			guessed[i] = correct[i];
			found = true;
		}
	}
	return found;
}

template <typename Resource, typename Loader>
void loadResourceOrThrow(Resource& resource, const string& path, Loader loader) {
	if (!loader(resource, path)) {
		throw runtime_error("Failed to load resource: " + path);
	}
}

const WordEntry& chooseRandomWord(mt19937& rng) {
	uniform_int_distribution<size_t> dist(0, dictionary.size() - 1);
	return dictionary[dist(rng)];
}

string buildHintText(const string& hint, size_t firstLineLength = 58) {
	if (hint.size() <= firstLineLength) {
		return "Hint: " + hint;
	}
	return "Hint: " + hint.substr(0, firstLineLength) + "\n" + hint.substr(firstLineLength);
}

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
		guessedString.assign(correctString.size(), '_');
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
		return revealLetters(guessedString, correctString, letter);
	}
};

int main()
{
	try {
		mt19937 rng(random_device{}());
		Hangman hg;
		const WordEntry& entry = chooseRandomWord(rng);
		hg.hint = entry.hint;
		hg.setCorrectString(entry.word);
		hg.setGuessedString();

		bool playing = false;
		unordered_set<char> usedLetters;

		array<sf::Vector2f, 14> positions = {
			sf::Vector2f(437.f, 270.f),
			sf::Vector2f(427.f, 295.f),
			sf::Vector2f(450.f, 297.f),
			sf::Vector2f(427.f, 365.f),
			sf::Vector2f(450.f, 367.f),
			sf::Vector2f(300.f, 340.f),
			sf::Vector2f(300.f, 210.f),
			sf::Vector2f(305.f, 278.f),
			sf::Vector2f(410.f, 200.f),
			sf::Vector2f(370.f, 500.f),
			sf::Vector2f(370.f, 600.f),
			sf::Vector2f(370.f, 700.f),
			sf::Vector2f(220.f, 700.f),
			sf::Vector2f(150.f, 600.f)
		};

		sf::RenderWindow window(sf::VideoMode(900, 900), "HANGMAN!");

		sf::Texture texture;
		loadResourceOrThrow(texture, "images\\squared-paper.jpg", [](sf::Texture& t, const string& p) { return t.loadFromFile(p); });
		sf::Sprite sprite(texture);

		sf::Font font;
		loadResourceOrThrow(font, "Fonts\\GreatVibes-Regular.otf", [](sf::Font& f, const string& p) { return f.loadFromFile(p); });

		sf::Font font1;
		loadResourceOrThrow(font1, "Fonts\\Montserrat-Light.otf", [](sf::Font& f, const string& p) { return f.loadFromFile(p); });

		sf::SoundBuffer buffer;
		loadResourceOrThrow(buffer, "Audio\\Wrong-answer-sound-effect.wav", [](sf::SoundBuffer& b, const string& p) { return b.loadFromFile(p); });
		sf::Sound sound(buffer);

		sf::SoundBuffer buffer1;
		loadResourceOrThrow(buffer1, "Audio\\Vip.wav", [](sf::SoundBuffer& b, const string& p) { return b.loadFromFile(p); });
		sf::Sound soundintro(buffer1);

		sf::SoundBuffer buffer2;
		loadResourceOrThrow(buffer2, "Audio\\wrong.wav", [](sf::SoundBuffer& b, const string& p) { return b.loadFromFile(p); });
		sf::Sound soundwrong(buffer2);

		sf::SoundBuffer buffer3;
		loadResourceOrThrow(buffer3, "Audio\\correct.wav", [](sf::SoundBuffer& b, const string& p) { return b.loadFromFile(p); });
		sf::Sound soundcorrect(buffer3);

		// Create text
		sf::Text text("Hangman", font);
		text.setCharacterSize(120);
		text.setStyle(sf::Text::Bold);
		text.setFillColor(sf::Color::Red);
		text.setPosition(200, 0);

		sf::Text text1("Play", font);
		text1.setCharacterSize(60);
		text1.setStyle(sf::Text::Underlined);
		text1.setStyle(sf::Text::Bold);
		text1.setFillColor(sf::Color::Red);
		LegacyRect r1{ 370, 500, 120, 60 };
		text1.setPosition(370, 500);

		sf::Text text2("Rules", font);
		text2.setCharacterSize(60);
		text2.setStyle(sf::Text::Underlined);
		text2.setStyle(sf::Text::Bold);
		text2.setFillColor(sf::Color::Red);
		LegacyRect r2{ 370, 600, 140, 60 };
		text2.setPosition(370, 600);

		sf::Text text3("Close", font);
		text3.setCharacterSize(60);
		text3.setStyle(sf::Text::Underlined);
		text3.setStyle(sf::Text::Bold);
		text3.setFillColor(sf::Color::Red);
		LegacyRect r3{ 370, 700, 120, 60 };
		text3.setPosition(370, 700);

		sf::Text text4(hg.getGuessedString(), font1);
		text4.setCharacterSize(80);
		text4.setStyle(sf::Text::Underlined);
		text4.setStyle(sf::Text::Bold);
		text4.setFillColor(sf::Color::Red);
		text4.setPosition(1000, 0);

		sf::Text text5("Guess the word", font1);
		text5.setCharacterSize(60);
		text5.setStyle(sf::Text::Underlined);
		text5.setStyle(sf::Text::Bold);
		text5.setFillColor(sf::Color::Red);
		text5.setPosition(1000, 0);

		sf::Text text6("Rules", font);
		text6.setCharacterSize(120);
		text6.setStyle(sf::Text::Underlined);
		text6.setStyle(sf::Text::Bold);
		text6.setFillColor(sf::Color::Red);
		text6.setPosition(1000, 0);

		string s = "Hints will be displayed for the word. Press the letter\n to complete the word to be guessed. If the letter\n you typed is right than it will appear in the string\n of word else the hangman will start to build . \nDifferent sounds of right and wrong typed letters will\n be played . If  the user guessed it right than he/she\n will win and loose if all the body parts of hangman\n are displayed and game continues till the word\n is guessed fully . Quit the game by clicking close\n button.";

		sf::Text text7(s, font1);
		text7.setCharacterSize(30);
		text7.setStyle(sf::Text::Underlined);
		text7.setStyle(sf::Text::Bold);
		text7.setFillColor(sf::Color::Red);
		text7.setPosition(1000, 0);

		sf::Text text8(buildHintText(hg.hint), font1);
		text8.setCharacterSize(22);
		text8.setStyle(sf::Text::Underlined);
		text8.setStyle(sf::Text::Bold);
		text8.setFillColor(sf::Color::Red);
		text8.setPosition(1000, 0);

		sf::Text text9("Wins: " + to_string(hg.wins) + "\nLosses: " + to_string(hg.losses), font1);
		text9.setCharacterSize(70);
		text9.setStyle(sf::Text::Underlined);
		text9.setStyle(sf::Text::Bold);
		text9.setFillColor(sf::Color::Green);
		text9.setPosition(1000, 0);

		sf::CircleShape circle(30);
		circle.setTexture(&texture);
		circle.setOutlineColor(sf::Color::Black);
		circle.setOutlineThickness(5);
		circle.setPosition(410, 200);

		array<sf::RectangleShape, 8> rectangle;
		rectangle[0].setSize(sf::Vector2f(5, 100));
		rectangle[0].setTexture(&texture);
		rectangle[0].setOutlineColor(sf::Color::Black);
		rectangle[0].setOutlineThickness(5);
		rectangle[0].setPosition(437, 270);

		rectangle[1].setSize(sf::Vector2f(5, 50));
		rectangle[1].setTexture(&texture);
		rectangle[1].setRotation(45);
		rectangle[1].setOutlineColor(sf::Color::Black);
		rectangle[1].setOutlineThickness(5);
		rectangle[1].setPosition(427, 295);

		rectangle[2].setSize(sf::Vector2f(5, 50));
		rectangle[2].setTexture(&texture);
		rectangle[2].setPosition(427, 295);
		rectangle[2].setRotation(315);
		rectangle[2].setOutlineColor(sf::Color::Black);
		rectangle[2].setOutlineThickness(5);
		rectangle[2].setPosition(450, 297);

		rectangle[3].setSize(sf::Vector2f(5, 75));
		rectangle[3].setTexture(&texture);
		rectangle[3].setRotation(15);
		rectangle[3].setOutlineColor(sf::Color::Black);
		rectangle[3].setOutlineThickness(5);
		rectangle[3].setPosition(427, 365);

		rectangle[4].setSize(sf::Vector2f(5, 75));
		rectangle[4].setTexture(&texture);
		rectangle[4].setRotation(345);
		rectangle[4].setOutlineColor(sf::Color::Black);
		rectangle[4].setOutlineThickness(5);
		rectangle[4].setPosition(450, 367);

		rectangle[5].setSize(sf::Vector2f(5, 125));
		rectangle[5].setTexture(&texture);
		rectangle[5].setOutlineColor(sf::Color::Black);
		rectangle[5].setOutlineThickness(5);
		rectangle[5].setPosition(300, 340);

		rectangle[6].setSize(sf::Vector2f(5, 125));
		rectangle[6].setTexture(&texture);
		rectangle[6].setOutlineColor(sf::Color::Black);
		rectangle[6].setOutlineThickness(5);
		rectangle[6].setPosition(300, 210);

		rectangle[7].setSize(sf::Vector2f(5, 125));
		rectangle[7].setTexture(&texture);
		rectangle[7].setRotation(270);
		rectangle[7].setOutlineColor(sf::Color::Black);
		rectangle[7].setOutlineThickness(5);
		rectangle[7].setPosition(305, 278);

		auto hideHangman = [&]() {
			for (auto& part : rectangle) {
				part.setPosition(1000, 0);
			}
			circle.setPosition(1000, 0);
		};

		auto startNewRound = [&]() {
			const WordEntry& newEntry = chooseRandomWord(rng);
			hg.hint = newEntry.hint;
			hg.setCorrectString(newEntry.word);
			hg.setGuessedString();
			hg.setTries(0);
			usedLetters.clear();
			text8.setString(buildHintText(hg.hint));
			text4.setString(hg.getGuessedString());
			hideHangman();
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
			text4.setPosition(positions[12].x, positions[12].y);
			text5.setPosition(positions[13].x, positions[13].y);
			text8.setPosition(50, 500);
			soundintro.pause();
			soundcorrect.play();
			playing = true;
		};

		soundintro.play();

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
				}

				if (event.type == sf::Event::MouseButtonPressed) {
					if (r1.contains(event.mouseButton.x, event.mouseButton.y)) {
						cout << "Play Button pressed\n";
						startNewRound();
					}
					if (r2.contains(event.mouseButton.x, event.mouseButton.y)) {
						cout << "Rules Button pressed\n";
						hideHangman();
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
					if (r3.contains(event.mouseButton.x, event.mouseButton.y)) {
						cout << "Exit Button pressed\n";
						sound.play();
						window.close();
					}
				}

				if (event.type == sf::Event::TextEntered)
				{
					if (playing && event.text.unicode < 128) {
						if (isalpha(static_cast<unsigned char>(event.text.unicode))) {
							const char typedCharacter = static_cast<char>(event.text.unicode);
							const char upperCharacter = static_cast<char>(toupper(static_cast<unsigned char>(typedCharacter)));

							cout << "ASCII character typed: " << typedCharacter << endl;
							cout << "ASCII character typed: " << static_cast<unsigned int>(event.text.unicode) << endl;

							if (!usedLetters.insert(upperCharacter).second) {
								cout << "Already guessed\n";
							}
							else {
								if (hg.guessWord(upperCharacter)) {
									cout << "Correct Choice\n";
									soundcorrect.play();
									text4.setString(hg.getGuessedString());
									if (hg.isWon()) {
										text5.setString("You Won!!!!");
										text4.setCharacterSize(50);
										text4.setPosition(120, positions[12].y);
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
									cout << "Wrong choice\n";
									hg.increaseTries();

									if (hg.getTries() == 1) {
										rectangle[5].setPosition(positions[5].x, positions[5].y);
									}
									if (hg.getTries() == 2) {
										rectangle[6].setPosition(positions[6].x, positions[6].y);
									}
									if (hg.getTries() == 3) {
										rectangle[7].setPosition(positions[7].x, positions[7].y);
									}
									if (hg.getTries() == 4) {
										circle.setPosition(positions[8].x, positions[8].y);
									}
									if (hg.getTries() == 5) {
										rectangle[0].setPosition(positions[0].x, positions[0].y);
									}
									if (hg.getTries() == 6) {
										rectangle[1].setPosition(positions[1].x, positions[1].y);
										rectangle[2].setPosition(positions[2].x, positions[2].y);
									}
									if (hg.getTries() == 7) {
										rectangle[3].setPosition(positions[3].x, positions[3].y);
										rectangle[4].setPosition(positions[4].x, positions[4].y);
									}
									if (hg.isLost()) {
										text5.setString("You Lost!!!!");
										text4.setCharacterSize(50);
										text4.setPosition(120, positions[12].y);
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

			for (auto& part : rectangle) {
				window.draw(part);
			}

			window.display();
		}

		return 0;
	}
	catch (const exception& ex) {
		cerr << "Error: " << ex.what() << endl;
		return EXIT_FAILURE;
	}
}
